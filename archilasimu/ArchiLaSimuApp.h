#include "imGuiApp.h"
#include "CircuitElements.h"
#include "ArchiCircuit.h"
#include "CPU.h"
#include "RAM.h"


class ArchiLaSimuApp : public imGuiApp {
public :
    ArchiLaSimuApp();
    virtual int customInit();
    virtual void mainLoop();
    void drawRect();
    GLuint CreateFramebuffer(int width,int height );
    void DrawTriangleInFramebuffer();
    void DrawFramebufferInImGui( );


    void drawMainWindow();
    void drawOtherWindow();

    ImFont *m_customFont;
    bool m_show_demo_window ;
    bool m_show_another_window;
    ImVec4 m_clear_color ;
    CPU* mCPU;
    RAM* mRAM;
    ExtBus* mExternalBus;
};

