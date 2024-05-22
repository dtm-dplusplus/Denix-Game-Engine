#include "GraphicsGame.h"

#include "imgui.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Input/InputSubsystem.h"

#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Core/Math.h"
#include "Graphics/JumpPawn.h"
#include "Graphics/Pipe.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Denix
{

	void GraphicsGame::InitPipes()
	{
		Ref<Model> pipeModel = ResourceSubsystem::GetModel("SM_Pipe");
		for (int i = 0; i < 10; i++)
		{
			Ref<PipePair> pipePair = MakeRef<PipePair>(PipePairCount, ObjectInitializer("PipePair " + std::to_string(PipePairCount)));
			pipePair->GetTransformComponent()->SetPosition({ PipeInitOffset + i * PipeDistance, 0.0f, 0.0f });
			pipePair->InitPipePair();
			pipePair->PipeTop->GetMeshComponent()->SetModel(pipeModel);
			pipePair->PipeBottom->GetMeshComponent()->SetModel(pipeModel);
			m_SceneObjects.push_back(pipePair);
			m_SceneObjects.push_back(pipePair->PipeTop);
			m_SceneObjects.push_back(pipePair->PipeBottom);
			PipePairCount++;
		}
	}

	bool GraphicsGame::Load()
	{
		Scene::Load();

		Player = MakeRef<JumpPawn>();
		Player->GetMeshComponent()->SetModel(ResourceSubsystem::GetModel("SM_Bird"));
		

		m_SceneObjects.push_back(Player);
		m_SceneObjects.push_back(Player->FollowCamera);

		InitPipes();

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}


	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	std::map<char, Character> Characters;

	void GraphicsGame::BeginScene()
	{
		Scene::BeginScene();

		// Set initial viewport cam
		glm::vec3& campos = m_ActiveCamera->GetTransformComponent()->GetPosition();
		const glm::vec3 camOffset = Player->CameraOffset;
		campos = Player->FollowCamera->GetTransformComponent()->GetPosition();
		campos = { camOffset.x, camOffset.y,camOffset.z };
		m_ActiveCamera->GetTransformComponent()->SetRotation({ 0.0f, -90.0f, 0.0f });

		// Text
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}

		FT_Face face;
		if (FT_New_Face(ft, (FileSubsystem::GetUserContentRoot() + R"(fonts/Fluffy.ttf)").c_str(), 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void GraphicsGame::BeginPlay()
	{
		Scene::BeginPlay();

		GameOver = false;
		GameOverTime = 0.0f;
		GameStart = false;
	}

	void GraphicsGame::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50), ImGuiCond_Appearing);

		ImGui::Begin("Graphics Game");
		ImGui::Checkbox("Game Over", &GameOver);
		if (ImGui::Button("Force Restart Game"))
		{
			m_RequestStop = true;
			//return;
		}
		if (GameOver)
		{
			if (ImGui::Button("Play Again"))
			{
                m_RequestStop = true;
                //return;
            }
		}
		ImGui::DragFloat("Pipe Distance", &PipeDistance, 0.1f);
		ImGui::DragFloat("Pipe Speed", &PipePair::MoveSpeed, 0.1f);
		ImGui::DragFloat("Pipe Destruction Distance", &PipePair::DestructionDistance, 0.1f);
		ImGui::SeparatorText("Character Camera");
		ImGui::DragFloat("Jump Force", &Player->JumpForce, 0.1f);
		ImGui::DragFloat3("Camera Offset", &Player->CameraOffset[0], 0.1f);
		ImGui::End();
	}

	void GraphicsGame::GameUpdate(float _deltaTime)
	{
		Scene::GameUpdate(_deltaTime);

		if (Player)
		{
			if (Player->HitPipe)
			{
				GameOver = true;
			}

			if (InputSubsystem::IsKeyDown(SDL_SCANCODE_SPACE))
			{
				if (!GameStart)
				{
					GameStart = true;
					Player->GetPhysicsComponent()->SetSimulatePhysics(true);
					PipePair::CanMove = true;
				}

				// Player hits space after game over to play again
				if (GameOver && GameOverTime >= GameOverWaitTime) m_RequestStop = true;
			}
		}

		if (GameOver)
		{
			GameOverTime += _deltaTime;

			PipePair::CanMove = false;
		}
	}
}