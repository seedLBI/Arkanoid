#include "Engine.h"


namespace engine {

    namespace core {
        namespace vars {
            GLFWwindow* handle_window = nullptr;
            FPS_Timer* fps_limiter = nullptr;

            glm::vec2 view_translate = {0.f,0.f};
            glm::vec2 view_scale = {1.f,1.f};
            float view_rotate_angle = 0.f;

            MainBlock mainBlock;
            UBO<MainBlock>* ubo_mainBlock = nullptr;

            std::vector<GLFWcursor*> cursors;
            int currentCursor = engine::window::Cursor::Hand;
            
            glm::vec2 sizeFramebuffer = { 0,0 };
            float minSizeFramebuffer = 0.f;
            glm::ivec2 last_pos_window = {100.f,100.f};
            glm::ivec2 last_size_window = {1280.f,720.f};
            int window_state = window::WindowState::WINDOWED;

            bool VSync_state = false;
        }

        void InitGLFW() {
            printf("void InitGLFW()\n");
            glfwSetErrorCallback(engine::core::Error_callback);

            if (!glfwInit()) {
                std::cout << "Failed to initialize GLFW" << std::endl;
                exit(-11111);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 8);
        }

        void InitNFD() {
            printf("void InitNFD()\n");

            nfdresult_t nfdInitResult = NFD_Init();
            if (nfdInitResult != NFD_OKAY)
                printf("NFD_Init failed: %s\n", NFD_GetError());
            else if (nfdInitResult == NFD_OKAY)
                printf("NFD_Init: NFD_OKAY\n");
        }

        void InitWindow() {
            printf("void InitWindow()\n");
            using namespace vars;
            handle_window = glfwCreateWindow(1280, 720, "Arkanoid", NULL, NULL);
            if (handle_window == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(-1);
            }
            glfwMakeContextCurrent(handle_window);

            window::Centerize();

            last_pos_window = window::GetPosition();
            last_size_window = window::GetSize();

            if (glfwRawMouseMotionSupported()) {
                std::cout << "RAW MOUSE SUPPORTED!!!\n";
                glfwSetInputMode(handle_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }
        }

        void Error_callback(int error, const char* description) {
            std::printf("GLFW Error %d: %s\n", error, description);
        }

        void InitGLEW() {
            printf("void InitGLEW()\n");
            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                std::cout << "Failed to initialize GLEW" << std::endl;
                exit(-2);
            }

            glEnable(GL_MULTISAMPLE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            
        }
        void InitImGui() {
            printf("void InitImGui()\n");
            using namespace vars;
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            ImGui::StyleColorsDark();


            ImGui_ImplGlfw_InitForOpenGL(handle_window, true);
            ImGui_ImplOpenGL3_Init("#version 330");


            auto& s = ImGui::GetStyle();
            s.ImageBorderSize = 1.f;
            s.SeparatorTextAlign.x = 0.5f;
            s.SeparatorTextAlign.y = 0.5f;
            s.SeparatorTextBorderSize = 9;
            s.FrameRounding = 6.f;
            s.WindowBorderSize = 1.f;
            s.FrameBorderSize = 1.f;
            s.PopupBorderSize = 1.f;
            s.WindowRounding = 12.f;
            s.ChildRounding = 12.f;

            ImGui::GetIO().IniFilename = NULL;
            ImGui::GetIO().LogFilename = NULL;
        }
        void InitCallbacks() {
            printf("void InitCallbacks()\n");
            using namespace vars;


            glfwSetKeyCallback(handle_window, 
                engine::update::keyboard::Keyboard_Buttons_Callback
            );

            glfwSetMouseButtonCallback(
                handle_window, 
                engine::update::mouse::Mouse_Buttons_Callback
            );

            glfwSetScrollCallback(
                handle_window, 
                engine::update::mouse::Mouse_Scroll_Callback
            );

            glfwSetCursorEnterCallback(
                handle_window, 
                engine::update::window::Cursor_Enter_Callback
            );

            glfwSetWindowSizeCallback(
                handle_window, 
                engine::update::window::Window_Size_Callback
            );

            glfwSetFramebufferSizeCallback(
                handle_window, 
                engine::update::window::Framebuffer_Size_Callback
            );
        }
        void InitCursors() {
            printf("void InitCursors()\n");
            using namespace vars;

            cursors.resize(4);

            cursors[engine::window::Cursor::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            cursors[engine::window::Cursor::Drag]  = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
            cursors[engine::window::Cursor::Hand]  = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
            cursors[engine::window::Cursor::Text]  = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        }

        void Terminate() {
            printf("void Terminate()\n");
            using namespace vars;

            bool static WasTerminated = false;
            if (WasTerminated) {
                std::cout << "WAS TERMINATED!!!!\n";
                exit(-111101);
            }
            WasTerminated = true;



            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            NFD_Quit();
            glfwDestroyWindow(handle_window);
            glfwTerminate();
            
        }
        void Initialize() {
            printf("void Initialize()\n");
            bool static WasInited = false;
            if (WasInited) {
                std::cout << "WAS INITED!!!!\n";
                exit(-111101);
            }
            WasInited = true;


            InitGLFW();
            InitNFD();
            InitWindow();
            InitGLEW();
            InitCallbacks();
            InitImGui();
            InitCursors();

            engine::core::vars::fps_limiter = new FPS_Timer();
            engine::core::vars::ubo_mainBlock = new UBO<MainBlock>("MainBlock");
            engine::core::vars::ubo_mainBlock->BindUBO(0);

            if(vars::sizeFramebuffer.x == 0){
                int width,height;
                glfwGetFramebufferSize(engine::core::vars::handle_window, &width, &height);
                vars::sizeFramebuffer = {width,height};
                vars::minSizeFramebuffer = (std::min)(width, height);
            }

        }

        void UpdateFrame() {
            engine::update::keyboard::Keyboard();
            engine::update::mouse::Mouse();
            engine::update::window::Window();
            engine::update::shader::Shader();
        }

    }

}
