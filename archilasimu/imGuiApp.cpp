#include "imGuiApp.h"
#include <iostream>

int imGuiApp::sFrameRate=0;
int imGuiApp::shidpiScale=1;

#define EMSCRIPTEN_MAINLOOP_END_MM     ;     emscripten_set_main_loop(MainLoopForEmscripten, imGuiApp::sFrameRate, true);


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


imGuiApp::imGuiApp() {
    m_io = nullptr;
    m_window=nullptr;
    mWindowTitle = "My Basic App";

}




ImVec2  toHD(ImVec2 inPt){
    return ImVec2(inPt.x*imGuiApp::shidpiScale,inPt.y*imGuiApp::shidpiScale);
}
int toHD(int inV) {
    return inV*imGuiApp::shidpiScale;
}


ImVec2  toLD(ImVec2 inPt){
    return ImVec2(inPt.x/imGuiApp::shidpiScale,inPt.y/imGuiApp::shidpiScale);
}
int toLD(int inV) {
    return inV/imGuiApp::shidpiScale;
}


EM_JS(void, set_canvas_size, (int width, int height), {
    var canvas = document.getElementById("canvas");
    canvas.style.width = width + "px";
    canvas.style.height = height + "px";
});


int
imGuiApp::Initialize() {

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    m_glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    m_glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    m_glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(1280, 720, mWindowTitle.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
        return 1;
    glfwMakeContextCurrent(m_window);
    // below : unuseful in the context of emscripten
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void)*m_io;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    // try hidpi : does not work.
    /*std::cout << "FONT : " << m_io->FontGlobalScale<< "\n";
    std::cout << m_io->DisplaySize.x << " "  << m_io->DisplaySize.y<<"\n" ;
    std::cout << m_io->DisplayFramebufferScale.x << " " <<m_io->DisplayFramebufferScale.y << "\n";
    m_io->DisplayFramebufferScale.x = 2;
    m_io->DisplayFramebufferScale.y = 2;
    m_io->DisplaySize.x = 800;
    m_io->DisplaySize.y = 600;
    m_io->FontGlobalScale = 0.5;
    */

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(m_glsl_version.c_str());


    int windowWidth, windowHeight;
    int framebufferWidth, framebufferHeight;
    glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
    std::cout << windowWidth<< " "<<windowHeight<<"\n";
    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);  
    std::cout << windowWidth<< " "<<windowHeight<< " " << framebufferWidth << " " <<framebufferHeight << "\n";
/*
    glfwSetWindowSize(
        m_window, 
        windowWidth * imGuiApp::shidpiScale, 
        windowHeight * imGuiApp::shidpiScale
    );
*/
 /*       m_io->DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        m_io->DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);

    glViewport(
        0, 
        0, 
        framebufferWidth * imGuiApp::shidpiScale, 
        framebufferHeight * imGuiApp::shidpiScale
    );
*/
    set_canvas_size(windowWidth,windowHeight);
  /*  glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);  */
   // std::cout << windowWidth<< " "<<windowHeight<< " " << framebufferWidth << " " <<framebufferHeight << "\n";
   // set_canvas_size(   windowWidth/2,     windowHeight/2);

   /// glfwSetFramebufferSize(m_window, framebufferWidth*2, framebufferHeight*2);  

    //float pixelRatio = static_cast<float>(framebufferWidth) / static_cast<float>(windowWidth);

    // Configurer l'échelle de Dear ImGui pour HiDPI
    //ImGui::GetIO().FontGlobalScale = pixelRatio;









    return 0;
}


int
imGuiApp::customInit()  {
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    return 0;

}


int
imGuiApp::Run()  {
    int ret = Initialize();
    if(ret != 0) return -1;
    ret = customInit();
    if(ret != 0) return -1;



    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    m_io->IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(m_window))
#endif
    {
        mainLoop();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END_MM
    //EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    return 0;

}



void
imGuiApp::mainLoop () {}