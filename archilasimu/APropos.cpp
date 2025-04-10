#include "APropos.h"
#include "ArchiTheme.h"
#include <iostream>
APropos::APropos() {
    mShowMe=false;

    LoadImage("imgs/logos.png",&logo_eea_tid,&logo_eea_w,&logo_eea_h);


}

void  APropos::drawMe(ImDrawList* dl, ImVec2 window_pos){

   // ImGui::SetCursorPos (mTablePos);

    if(mShowMe == false) return;

    ImGui::Begin("A Propos de ArchiLaSimu - version Web",&mShowMe,ImGuiWindowFlags_NoResize);

    static bool firstOpen = true;

    if(firstOpen == true) {
        ImGui::SetWindowSize(toHD(ImVec2(800,650)));
        ImGui::SetWindowPos(toHD(ImVec2(300,10)));             
        firstOpen = false;
    }

    ImGui::SetCursorPos(toHD(ImVec2{170,30}));
    ImGui::PushFont(gArchiTheme.mRobotoBoldFont);

    ImGui::Text("Archi la simu - version web");
    ImGui::PopFont();
    ImGui::PushFont(gArchiTheme.mRobotoFont);
    ImGui::SetCursorPos(toHD(ImVec2{100,80}));
    ImGui::Text("Logiciel à vocation pédagogique pour la simulation d'architecture de microprocesseurs");
    ImGui::SetCursorPos(toHD(ImVec2{300,110}));
    ImGui::Text("Version 1.9 - Avril 2025");
    ImGui::Dummy(toHD(ImVec2(0.0f, 20.0f)));



    ImGui::Text("Concept pédagogique : Arnaud VIRAZEL");
    ImGui::Text("Développement : Mikhaël MYARA");
    ImGui::Text("D'après la version Visual Basic développée par: ENJALBERT, HABEILLON & MALHOMME (étudiants UM)");
    ImGui::Dummy(toHD(ImVec2(0.0f, 10.0f)));
    ImGui::Text("Logiciel écrit en C++ avec Emscripten (gcc/clang pour Webassembly) ");
    ImGui::Text("Bibliothèques graphiques : Dear ImGui 1.90.5 - GLFW - OpenGL 3 - stb_image");
    ImGui::Dummy(toHD(ImVec2(0.0f, 20.0f)));
    ImGui::Image((void*)(intptr_t)logo_eea_tid, toHD(ImVec2(logo_eea_w, logo_eea_h)));
    ImGui::Dummy(toHD(ImVec2(0.0f, 20.0f)));    
    std::string text = "Que faut-il faire ?\n\n";
    text+="Dans la sous-partie 'Séquenceur' se trouve la table des microcodes.Dans cette table, ";
    text+="il faut indiquer les séquences de signaux à appliquer, phase par phase, pour chaque instruction assembleur reconnue"; 
    text+="par le processeur.\n\n";
    text+="Un exemple de séquence de signaux est rempli par défaut dans la table de microcode, au niveau des lignes 253 à 255.";
    text+="Cette partie montre comment se déroule le cycle 'Fetch'. ";
    text+="Vous pouvez observer cette table de 2 façons : soit en regardant les signaux qui sont ";
    text+="cochés dans la vue 'Vue principale', soit en regardant juste les noms ";
    text+="des signaux utiles dans la 'Vue Synthétique'.\n\n";
    text+="Chaque instruction assembleur correspond à un 'OpCode'. La valeur de l'Opcode est simplement la ligne de la table de microcode où l'instruction débute. ";
    text+="On peut connaitre cet 'OpCode' en consultant la 'Table des Opcodes et Mnémoniques' intégrée au logiciel. ";
    text+="Par exemple, on peut y lire que LOAD_B immédiat correspond à l'OpCode 75, et donc la séquence des signaux qui permettent de réaliser ";
    text+="cette instruction débutera à la ligne 75 dans la table des microcodes. Si l'instruction a besoin de plusieurs lignes, on pourra ";
    text+="continuer l'instruction après la ligne 155 de la table des microcodes. En effet, c'est à partir de la ligne 155 qu'un espace ";
    text+="libre conséquent est disponible.\n\n";
    text+="Les signaux à piloter sont indiqués dans la partie 'Architecture' et dans la partie 'Séquenceur'\n";
    text+="Une fois qu'une instruction est définie, on peut l'utiliser plaçant son OpCode dans la RAM. On peut utiliser pour cela l'assistant proposé, activable dans l'interface principale,";
    text+="ou remplir soi même le code opération complet si on sait l'établir.\n\n";
    text+="Une fois que l'on a fait tout cela, on peut tester ce que cela donne en exécutant phase par phase les microcodes pour voir l'évolution des signaux ";
    text+="pilotés par la table de microcode, et aussi vérifier que l'action voulue est bien réalisée pour les instructions qui ont été ";
    text+="décrites dans la table de microcode.\n\n";
    text+="Enfin, si on décrit les instructions utiles dans la table de microcode, on peut exécuter un programme en assembleur en pas à pas.\n\n\n\n";
    
     


    ImGui::TextWrapped("%s",text.c_str());



   // ImGui::Text("Chaque instruction a un 'OpCode' qui correspond à la ligne à laquelle elle débute.");
    
   



    ImGui::PopFont();


    
    ImGui::End();
}
