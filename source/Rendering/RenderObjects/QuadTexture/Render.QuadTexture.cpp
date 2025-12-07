#include "Render.QuadTexture.h"

#include "Core/Engine/Engine.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

QuadTexture::QuadTexture() {
    Init();
}
QuadTexture::~QuadTexture() {

}

void QuadTexture::DrawFullScreenByAspectRatio(Texture& texture) {
    texture.SetActive(TextureActiveNumber::TEXTURE_0);
    texture.Bind();

    glm::vec2 size = engine::window::GetFramebufferSize();
    //float min_size = std::min(size.x, size.y);


    const float current_ratio = (float)texture.GetWidth()/(float)texture.GetHeight();
    //const float correct_ratio =  16.f / 9.f;

    float screen_correct_width = current_ratio * size.y;

    shader->use();


    shader->setMat4("u_model",
                    glm::scale(
                        glm::translate(
                            glm::mat4(1.f),
                            glm::vec3(size.x/2.f, size.y/2.f ,0.f)
                        ),
                        glm::vec3(screen_correct_width/2.f, -size.y/2.f,1.f)
                        )
                    );



    quad.Draw();
}

void QuadTexture::Init() {

    shader = new Shader(
        "QuadTexture_Shader",
        (std::string)"resources/shaders/QuadTexture/QuadTexture.vert",
        (std::string)"resources/shaders/QuadTexture/QuadTexture.frag");


    shader->use();
    shader->setMat4("u_model", glm::mat4(1.f));
    shader->setInt ("u_texture", 0);

    engine::shader::BindUBO_MainBlock(*shader);
}
