#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math.h"
#include "GameObject.h"
#include "Denix/Video/GL/Viewport.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Denix
{
	class Camera : public GameObject
	{
	public:
		Camera(const ObjectInitializer& _objInit = {"Camera"}) : GameObject(ObjectInitializer(_objInit))
		{
			m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, 5));
			m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
			m_RenderComponent->SetIsVisible(false);
		}

		~Camera() override = default;

		void Update(float _deltaTime) override
		{
			// m_Projection matrix
			if (m_IsPerspective)
			{
				m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect.x / m_Aspect.y, m_NearPlane, m_FarPlane);
			}
			else
			{
				m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
			}

			// m_View Matrix
			glm::vec3 direction;
			direction.x = cos(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			direction.y = sin(glm::radians(m_TransformComponent->GetRotation().x));
			direction.z = sin(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			m_CameraFront = glm::normalize(direction);

			m_View = glm::lookAt(m_TransformComponent->GetPosition(), m_TransformComponent->GetPosition() + m_CameraFront, m_CameraUp);
		}

		bool IsPerspective() const
		{
			return m_IsPerspective;
		}
		bool& IsPerspective()
		{
			return m_IsPerspective;
		}
		void SetIsPerspective(const bool _isPerspective)
		{
			m_IsPerspective = _isPerspective;
		}

		float GetFov() const
		{
			return m_Fov;
		}
		float& GetFov()
		{
			return m_Fov;
		}
		void SetFov(const float _fov)
		{
			m_Fov = _fov;
		}

		float GetNearPlane() const
		{
			return m_NearPlane;
		}
		float& GetNearPlane()
		{
			return m_NearPlane;
		}
		void SetNearPlane(const float _nearPlane)
		{
			m_NearPlane = _nearPlane;
		}

		float GetFarPlane() const
		{
			return m_FarPlane;
		}
		float& GetFarPlane()
		{
			return m_FarPlane;
		}
		void SetFarPlane(const float _farPlane)
		{
			m_FarPlane = _farPlane;
		}

		glm::vec2 GetPitchConstraints() const
		{
			return m_PitchConstraints;
		}
		glm::vec2& GetPitchConstraints()
		{
			return m_PitchConstraints;
		}
		void SetPitchConstraints(const glm::vec2 _pitchConstraints)
		{
			m_PitchConstraints = _pitchConstraints;
		}

		glm::vec2 GetAspect() const
		{
			return m_Aspect;
		}
		glm::vec2& GetAspect()
		{
			return m_Aspect;
		}
		void SetAspect(const glm::vec2 _aspect)
		{
			m_Aspect = _aspect;
		}

		float GetRotationFactor() const
		{
			return m_RotationFactor;
		}
		float& GetRotationFactor()
		{
			return m_RotationFactor;
		}
		void SetRotationFactor(const float _rotationFactor)
		{
			m_RotationFactor = _rotationFactor;
		}

		float GetPitchRotationRate() const
		{
			return m_PitchRotationRate;
		}
		float& GetPitchRotationRate()
		{
			return m_PitchRotationRate;
		}
		void SetPitchRotationRate(const float _pitchRotationRate)
		{
			m_PitchRotationRate = _pitchRotationRate;
		}

		float GetYawRotationRate() const
		{
			return m_YawRotationRate;
		}
		float& GetYawRotationRate()
		{
			return m_YawRotationRate;
		}
		void SetYawRotationRate(const float _yawRotationRate)
		{
			m_YawRotationRate = _yawRotationRate;
		}

		glm::mat4 GetProjectionMatrix() const
		{
			return m_Projection;
		}
		glm::mat4& GetProjectionMatrix()
		{
			return m_Projection;
		}
		void SetProjectionMatrix(const glm::mat4& _projection)
		{
			m_Projection = _projection;
		}

		glm::mat4 GetViewMatrix() const
		{
			return m_View;
		}
		glm::mat4& GetViewMatrix()
		{
			return m_View;
		}
		void SetViewMatrix(const glm::mat4& _view)
		{
			m_View = _view;
		}

		glm::vec3 GetCameraFront() const
		{
			return m_CameraFront;
		}
		glm::vec3& GetCameraFront()
		{
			return m_CameraFront;
		}
		void SetCameraFront(const glm::vec3& _cameraFront)
		{
			m_CameraFront = _cameraFront;
		}

		glm::vec3 GetCameraUp() const
		{
			return m_CameraUp;
		}
		glm::vec3& GetCameraUp()
		{
			return m_CameraUp;
		}
		void SetCameraUp(const glm::vec3& _cameraUp)
		{
			m_CameraUp = _cameraUp;
		}

		// Ref<FrameBuffer> m_FrameBuffer;
	protected:
		bool m_IsPerspective = true;
		float m_Fov = 45.f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 10000.f;
		glm::vec2 m_PitchConstraints = glm::vec2(-89.f, 89.f);
		glm::vec2 m_Aspect = glm::vec2(800.f, 600.f);

		// Factor used to represent the speed of the rotation within respectable limits
		float m_RotationFactor = 100.0f;
		float m_PitchRotationRate = 1.0f;
		float m_YawRotationRate = 1.0f;


		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	struct TextCharacter {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	struct TextUI
	{
		std::string Text;
		glm::vec2 Position;
		glm::vec3 Color;
		float Scale;
	};

	class UICamera : public Camera
	{
	public:
		UICamera() : Camera({ "UI Camera" })
		{
			m_Layer = LayerType::UI;
            m_RenderComponent->SetIsVisible(false);

			// Text
			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			}

			FT_Face face;
			if (FT_New_Face(ft, (FileSubsystem::GetUserContentRoot() + R"(Fonts/Fluffy.ttf)").c_str(), 0, &face))
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
				
				unsigned int tex;
				glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
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
				TextCharacter character = {
					tex,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				Characters.insert(std::pair<char, TextCharacter>(c, character));
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			FT_Done_Face(face);
			FT_Done_FreeType(ft);

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

        }

		void Update(float _deltaTime) override
		{
            m_Projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, m_NearPlane, m_FarPlane);
		}

		void RenderText(Ref<Shader> s, std::string text, float x, float y, float scale, glm::vec3 color)
		{
			// activate corresponding render state	
			s->Bind();
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glUniformMatrix4fv(s->GetUniform("u_Projection"), 1,
				GL_FALSE, glm::value_ptr(m_Projection));

			glUniform3f(s->GetUniform("u_TextColor"), color.x, color.y, color.z);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(VAO);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				TextCharacter ch = Characters[*c];

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				// update VBO for each character
				float vertices[6][4] = {
					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos,     ypos,       0.0f, 1.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },

					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },
					{ xpos + w, ypos + h,   1.0f, 0.0f }
				};
				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		unsigned int VAO, VBO;
		unsigned int texture;
		std::vector<TextUI> m_Texts;
		std::map<char, TextCharacter> Characters;

	};


	class ViewportCamera: public Camera
	{
	public:
		ViewportCamera(): Camera({"Viewport Camera"})
		{
			
		}

		float GetMoveSpeed() const
		{
			return m_MoveSpeed;
		}
		float& GetMoveSpeed()
		{
			return m_MoveSpeed;
		}
		void SetMoveSpeed(const float _moveSpeed)
		{
			m_MoveSpeed = _moveSpeed;
		}

		float GetMouseScrollSpeed() const
		{
            return m_MouseScrollSpeed;
        }
		float& GetMouseScrollSpeed()
		{
            return m_MouseScrollSpeed;
        }
		void Update(float _deltaTime) override;

	protected:
		float m_MoveSpeed = 10.0f;
		float m_MouseScrollSpeed = 0.5f;
	};
}
