#include "MM_imgui.h"
#include <string>


class imGuiApp {
public:
    imGuiApp();
    virtual int Initialize();
    virtual int customInit ();
    virtual int Run();
    virtual void mainLoop();

    ImGuiIO* m_io;
    std::string m_glsl_version;
    GLFWwindow* m_window;

    std::string mWindowTitle;

    static int sFrameRate;

};
