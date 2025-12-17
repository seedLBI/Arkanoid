#include "Engine.h"

namespace engine {

    namespace update {
        namespace shader{

            void Shader(){
                using namespace engine::core::vars;
                MainBlock_Update();
                ubo_mainBlock->Update(mainBlock);
            }

			void MainBlock_Update(){
                using namespace engine::core::vars;

                MainBlock& block = mainBlock;

                glm::mat4 view = glm::mat4(1.f);
                view = glm::rotate(view, view_rotate_angle, glm::vec3(0.f, 0.f, 1.f));
                view = glm::translate(view, glm::vec3(view_translate, 0.f));
                view = glm::scale(view, glm::vec3(view_scale, 1.f));




                glm::mat4 proj = glm::ortho(0.f, block.Resolution.x, block.Resolution.y, 0.f, -1.f, 1.f);

                block.deltaTime = fps_limiter->GetDeltaTime();
                block.timeProgramm = glfwGetTime();
                block.Resolution = engine::window::GetFramebufferSize();
                block.projectionView = view * proj;

                currentCursor = engine::window::Cursor::Arrow;

            }
        }
    }

    namespace shader{
        void BindUBO_MainBlock(const Shader& shader){
            engine::core::vars::ubo_mainBlock->BindProgramm(shader);
        }
    }

}
