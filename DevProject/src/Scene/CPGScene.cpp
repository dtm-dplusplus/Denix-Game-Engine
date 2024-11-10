#include "CPGScene.h"

#include "imgui.h"
#include "CPG/Ray.h"

#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <omp.h>

struct vec3
{
    float x = 0, y = 0, z = 0;
    float& operator[](const int i) { return i == 0 ? x : (1 == i ? y : z); }
    const float& operator[](const int i) const { return i == 0 ? x : (1 == i ? y : z); }
    vec3 operator*(const float v) const { return {x * v, y * v, z * v}; }
    float operator*(const vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    vec3 operator+(const vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    vec3 operator-(const vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    vec3 operator-() const { return {-x, -y, -z}; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    vec3 normalized() const { return (*this) * (1.f / norm()); }
};

vec3 cross(const vec3 v1, const vec3 v2)
{
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

struct TRMaterial
{
    float refractive_index = 1;
    float albedo[4] = {2, 0, 0, 0};
    vec3 diffuse_color = {0, 0, 0};
    float specular_exponent = 0;
};

struct TRSphere
{
    vec3 center;
    float radius;
    TRMaterial TRMaterial;
};

constexpr TRMaterial ivory = {1.0, {0.9, 0.5, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.};
constexpr TRMaterial glass = {1.5, {0.0, 0.9, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.};
constexpr TRMaterial red_rubber = {1.0, {1.4, 0.3, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.};
constexpr TRMaterial mirror = {1.0, {0.0, 16.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

constexpr TRSphere spheres[] = {
    {{-3, 0, -16}, 2, ivory},
    {{-1.0, -1.5, -12}, 2, glass},
    {{1.5, -0.5, -18}, 3, red_rubber},
    {{7, 5, -18}, 4, mirror}
};

constexpr vec3 lights[] = {
    {-20, 20, 20},
    {30, 50, -25},
    {30, 20, 30}
};

vec3 reflect(const vec3& I, const vec3& N)
{
    return I - N * 2.f * (I * N);
}

vec3 refract(const vec3& I, const vec3& N, const float eta_t, const float eta_i = 1.f)
{
    // Snell's law
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    if (cosi < 0) return refract(I, -N, eta_i, eta_t);
    // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3{1, 0, 0} : I * eta + N * (eta * cosi - std::sqrt(k));
    // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

std::tuple<bool, float> ray_sphere_intersect(const vec3& orig, const vec3& dir, const TRSphere& s)
{
    // ret value is a pair [intersection found, distance]
    vec3 L = s.center - orig;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > s.radius * s.radius) return {false, 0};
    float thc = std::sqrt(s.radius * s.radius - d2);
    float t0 = tca - thc, t1 = tca + thc;
    if (t0 > .001) return {true, t0}; // offset the original point by .001 to avoid occlusion by the object itself
    if (t1 > .001) return {true, t1};
    return {false, 0};
}

std::tuple<bool, vec3, vec3, TRMaterial> scene_intersect(const vec3& orig, const vec3& dir)
{
    vec3 pt, N;
    TRMaterial TRMaterial;

    float nearest_dist = 1e10;
    if (std::abs(dir.y) > .001)
    {
        // intersect the ray with the checkerboard, avoid division by zero
        float d = -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
        vec3 p = orig + dir * d;
        if (d > .001 && d < nearest_dist && std::abs(p.x) < 10 && p.z < -10 && p.z > -30)
        {
            nearest_dist = d;
            pt = p;
            N = {0, 1, 0};
            TRMaterial.diffuse_color = (int(.5 * pt.x + 1000) + int(.5 * pt.z)) & 1
                                           ? vec3{.3, .3, .3}
                                           : vec3{.3, .2, .1};
        }
    }

    for (const TRSphere& s : spheres)
    {
        // intersect the ray with all spheres
        auto [intersection, d] = ray_sphere_intersect(orig, dir, s);
        if (!intersection || d > nearest_dist) continue;
        nearest_dist = d;
        pt = orig + dir * nearest_dist;
        N = (pt - s.center).normalized();
        TRMaterial = s.TRMaterial;
    }
    return {nearest_dist < 1000, pt, N, TRMaterial};
}

vec3 cast_ray(const vec3& orig, const vec3& dir, const int depth = 0)
{
    auto [hit, point, N, TRMaterial] = scene_intersect(orig, dir);
    if (depth > 4 || !hit)
        return {0.2, 0.7, 0.8}; // background color

    vec3 reflect_dir = reflect(dir, N).normalized();
    vec3 refract_dir = refract(dir, N, TRMaterial.refractive_index).normalized();
    vec3 reflect_color = cast_ray(point, reflect_dir, depth + 1);
    vec3 refract_color = cast_ray(point, refract_dir, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (const vec3& light : lights)
    {
        // checking if the point lies in the shadow of the light
        vec3 light_dir = (light - point).normalized();
        auto [hit, shadow_pt, trashnrm, trashmat] = scene_intersect(point, light_dir);
        if (hit && (shadow_pt - point).norm() < (light - point).norm()) continue;
        diffuse_light_intensity += std::max(0.f, light_dir * N);
        specular_light_intensity += std::pow(std::max(0.f, -reflect(-light_dir, N) * dir),
                                             TRMaterial.specular_exponent);
    }
    return TRMaterial.diffuse_color * diffuse_light_intensity * TRMaterial.albedo[0] + vec3{1., 1., 1.} *
        specular_light_intensity * TRMaterial.albedo[1] + reflect_color * TRMaterial.albedo[2] + refract_color *
        TRMaterial.albedo[3];
}

using namespace Denix;

CPGScene::CPGScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
}

void CPGScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void CPGScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    constexpr int width = 1024;
    constexpr int height = 768;
    constexpr float fov = 1.05; // 60 degrees field of view in radians
    static unsigned char* pixel_data = new unsigned char[width * height * 3];
    static const std::string rayImage = FileSubsystem::GetContentRoot() + "ray.png";
    static Ref<FrameBuffer> rayFB = MakeRef<FrameBuffer>(width, height);
    static int interval = 500;
    static int intCount = 0;
    if (ImGui::Begin(m_SceneName.c_str()))
    {
        if (ImGui::CollapsingHeader("Ray Tracing", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const glm::vec3& camPos = m_ActiveCamera->GetTransformComponent()->GetPosition();
            const glm::vec3& camForward = m_ActiveCamera->GetCameraFront();
            const glm::vec3& camRight = m_ActiveCamera->m_CameraRight;
            glm::vec3 rot = m_ActiveCamera->GetTransformComponent()->GetRotation();
            
            ImGui::SeparatorText("Ray Tracing");
            if (ImGui::Button("Spawn Ray"))
            {
                
                rot = {rot.x, -rot.y, 90.0f};
                m_Ray = SpawnGameObject<Ray>(camPos, rot);
            }
    ImGui::DragInt("interval", &interval);
            ImGui::Text("int count %d", intCount);
            if (ImGui::Button("Run Ray Tracer"))
            {
                
            }
            
            if(intCount++ >= interval)
            {
                intCount = 0;
                
                #pragma omp parallel for
                for (int pix = 0; pix < width * height; pix++)
                {
                    // actual rendering loop
                    float dir_x = (pix % width + 0.5) - width / 2.;
                    float dir_y = -(pix / width + 0.5) + height / 2.; // this flips the image at the same time
                    float dir_z = -height / (2. * tan(fov / 2.));
                    const vec3& color = cast_ray({camPos.x, camPos.y, camPos.z}, vec3{dir_x, dir_y, dir_z}.normalized());
                    float max = std::max(1.f, std::max(color[0], std::max(color[1], color[2])));
                    pixel_data[pix * 3] = static_cast<unsigned char>(255 * color[0] / max);
                    pixel_data[pix * 3 + 1] = static_cast<unsigned char>(255 * color[1] / max);
                    pixel_data[pix * 3 + 2] = static_cast<unsigned char>(255 * color[2] / max);
                }

                glBindTexture(GL_TEXTURE_2D, rayFB->GetTexureID());
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            if(rayFB->GetTexureID()) ImGui::Image((void*)rayFB->GetTexureID(), ImVec2(width, height));
            ImGui::End();
        }
    }
}
