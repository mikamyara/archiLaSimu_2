#include "APropos.h"
#include "ArchiTheme.h"
#include <iostream>
APropos::APropos() {
    mShowMe=false;

    LoadImage("imgs/logos.png",&logo_eea_tid,&logo_eea_w,&logo_eea_h);
    std::cout << logo_eea_tid << "\n";


}


    
       

void  APropos::drawMe(ImDrawList* dl, ImVec2 window_pos){

   // ImGui::SetCursorPos (mTablePos);

    if(mShowMe == false) return;


    ImGui::Begin("A Propos de ArchiLaSimu - version Web",&mShowMe,ImGuiWindowFlags_NoResize);



    static bool firstOpen = true;

    if(firstOpen == true) {
        ImGui::SetWindowSize(ImVec2(800,490));
        ImGui::SetWindowPos(ImVec2(300,50));             
        firstOpen = false;
    }

    ImGui::SetCursorPos(ImVec2{170,30});
    ImGui::PushFont(gArchiTheme.mRobotoBoldFont);

    ImGui::Text("Archi la simu - version web");
    ImGui::PopFont();
    ImGui::PushFont(gArchiTheme.mRobotoFont);
    ImGui::SetCursorPos(ImVec2{100,80});
    ImGui::Text("Logiciel à vocation pédagogique pour la simulation d'architecture de microprocesseurs");
    ImGui::SetCursorPos(ImVec2{300,110});
    ImGui::Text("Version 1.0 - Avril 2024");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));


    ImGui::Text("Concept pédagogique : Arnaud VIRAZEL");
    ImGui::Text("Développement : Mikhaël MYARA");
    ImGui::Text("D'après la version 1.0, développée par: ENJALBERT, HABEILLON & MALHOMME (étudiants UM)");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Logiciel développé en C++ avec Emscripten (gcc/clang pour Webassembly) ");
    ImGui::Text("Bibliothèques graphiques : Dear ImGui 1.90.5 - GLFW - OpenGL 3 - stb_image");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::PopFont();


    ImGui::Image((void*)(intptr_t)logo_eea_tid, ImVec2(logo_eea_w, logo_eea_h));
    
    ImGui::End();
}
