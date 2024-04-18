#pragma once
#include <imgui.h>
#include "ArchiTheme.h"

class APropos {
    public:
    APropos();
    void drawMe(ImDrawList* dl, ImVec2 window_pos);


    GLuint logo_eea_tid;
    int logo_eea_w,logo_eea_h;    
    bool mShowMe;
};