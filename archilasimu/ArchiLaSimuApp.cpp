#include "ArchiLaSimuApp.h"
#include "ArchiTheme.h"
#include "CPU.h"
#include "Assembleur.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLES2/gl2.h>
#include <GL/glew.h>
#include <stdio.h>
#include "APropos.h"
#include <iostream>



ArchiLaSimuApp::ArchiLaSimuApp ():imGuiApp ()
{
    m_customFont = nullptr;
    m_show_demo_window = false;
    m_show_another_window = false;
    m_clear_color = ImVec4 (0.45f, 0.55f, 0.60f, 1.00f);

    imGuiApp::sFrameRate = 15;
    mWindowTitle = "Archi La Simu - version Web";

}

int
ArchiLaSimuApp::customInit ()
{
    // Setup Dear ImGui style
    ImGui::StyleColorsDark ();
    //ImGui::StyleColorsLight ();


    gArchiTheme.LoadFonts(m_io);
    m_customFont = gArchiTheme.mRobotoFont;


    mASM = new Assembleur();
    mCPU = new CPU();
    mRAM = new RAM();
    mExternalBus = new ExtBus();
    
    mRAM->mASM = mASM;
    mASM->mRAM = mRAM;

    mCPU->mExternalBus = mExternalBus;
    mCPU->mRAM = mRAM;
    mCPU->mASM = mASM;
    mCPU->Rebuild();
    mCPU->Reset();    

    mAPropos = new APropos();
std::cout << emscripten_get_device_pixel_ratio();

    return 0;
}











void
ArchiLaSimuApp::drawMainWindow() {
    int windowFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |ImGuiWindowFlags_AlwaysHorizontalScrollbar |ImGuiWindowFlags_AlwaysVerticalScrollbar;

    static float f = 0.0f;
    static int counter = 0;

    ImGui::SetNextWindowContentSize(ImVec2(1832,0.0f));
    ImGui::PushFont (m_customFont);
    ImGui::Begin ("Hello, world!", NULL, windowFlags);	// Create a window called "Hello, world!" and append into it.
    ImDrawList *draw_list = ImGui::GetWindowDrawList ();
    ImVec2 window_pos = ImGui::GetWindowPos ();
    ImVec2 scroll_pos = window_pos;
    scroll_pos.x -= ImGui::GetScrollX();
    scroll_pos.y -= ImGui::GetScrollY();

    mCPU->draw(draw_list,scroll_pos);
    mCPU->drawWidgets(draw_list,window_pos);

    mRAM->draw(draw_list,scroll_pos);
    mRAM->drawWidgets(draw_list,window_pos);
    mExternalBus->draw(draw_list,scroll_pos);

    ImVec2 titlePos = scroll_pos;
    titlePos.x +=800;
    titlePos.y +=750;
    addAlignedText(draw_list,titlePos,eTextCenter, "Archi la simu - version Web",IM_COL32(255,255,255,120),gArchiTheme.mRobotoBoldFont,50) ;
    ImVec2 aboutPos = window_pos;
    aboutPos.x +=800;
    aboutPos.y +=800;
    ImGui::SetCursorPos(aboutPos);
    ImGui::Checkbox ("A Propos de Archi la simu ...", &mAPropos->mShowMe);	// Edit bools storing our window open/close state
    mAPropos->drawMe(draw_list,window_pos);
    ImGui::SetCursorPos(ImVec2(0,0));
    //ImGui::Image((void*)(intptr_t)logo_eea_tid, ImVec2(logo_eea_w, logo_eea_h));



   // ImGui::Text ("This is some useful text.");	// Display some text (you can use a format strings too)
  //  ImGui::Checkbox ("Demo Window", &m_show_demo_window);	// Edit bools storing our window open/close state
    //ImGui::Checkbox ("Another Window", &m_show_another_window);
/*
    ImGui::SliderFloat ("float", &f, 0.0f, 1.0f);	// Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3 ("clear color", (float *) &m_clear_color);	// Edit 3 floats representing a color

    if (ImGui::Button ("Button"))	// Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine ();
    ImGui::Text ("counter = %d", counter);
    ImGui::SetCursorPos (ImVec2 (500, 500));*/
   /* ImGui::Text ("Application average %.3f ms/frame (%.1f FPS)",
     1000.0f / m_io->Framerate, m_io->Framerate); */
    ImGui::End ();
    ImGui::PopFont ();
}



void
ArchiLaSimuApp::mainLoop ()
{

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents ();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame ();
    ImGui_ImplGlfw_NewFrame ();
    ImGui::NewFrame ();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_show_demo_window)
        ImGui::ShowDemoWindow (&m_show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {

        // force main window to be fullscreen and always backgrounded.
        int screenWidth, screenHeight;
        glfwGetFramebufferSize (m_window, &screenWidth, &screenHeight);
        ImGui::SetNextWindowSize (ImVec2 (screenWidth, screenHeight));
        ImGui::SetNextWindowPos (ImVec2 (0, 0));

        drawMainWindow();

    }

    drawOtherWindow();

    // Rendering
    ImGui::Render ();
    int display_w, display_h;
    glfwGetFramebufferSize (m_window, &display_w, &display_h);
    glViewport (0, 0, display_w, display_h);
    glClearColor (m_clear_color.x * m_clear_color.w,
                  m_clear_color.y * m_clear_color.w,
                  m_clear_color.z * m_clear_color.w, m_clear_color.w);
    glClear (GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());

    glfwSwapBuffers (m_window);
}



void
ArchiLaSimuApp::drawOtherWindow() {
    // 3. Show another simple window.
    if (m_show_another_window)
    {
        ImGui::Begin ("Another Window", &m_show_another_window);	// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text ("Hello from another window!");
        if (ImGui::Button ("Close Me"))
            m_show_another_window = false;
        ImGui::End ();
    }

}