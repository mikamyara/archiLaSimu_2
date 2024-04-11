#include "ArchiTheme.h"
#include <math.h>



ArchiTheme gArchiTheme;


ArchiTheme::ArchiTheme() {
mBus1Color     = IM_COL32(255, 255, 0,255);
mBus3Color     = IM_COL32(255, 180, 0,255);
mBus2Color     = IM_COL32(255, 255, 255,255);
mBoxWireColor  = IM_COL32(255,255,255,255);
mSelectedColor = IM_COL32(00,200,255,255);
mErrorColor    = IM_COL32(188,133,0,255);
mRegisterOutputCircleColor =  IM_COL32(00,80,140,255);
mMuxColor = IM_COL32(75,50,30,255);
mMicrocodeBusColor = IM_COL32(100,255,100,255); 

mBusThickness  = 4.0f;

mRobotoFont=NULL;
mRobotoBoldFont=NULL;

}


void
ArchiTheme::LoadFonts( ImGuiIO *in_io) {
  in_io->Fonts->AddFontDefault ();
  mRobotoFont =
    in_io->Fonts->AddFontFromFileTTF ("/fonts/Roboto-Regular.ttf", 20.0f);
  mRobotoBoldFont =
    in_io->Fonts->AddFontFromFileTTF ("/fonts/Roboto-Bold.ttf", 50.0f);

}

 
void addAlignedText(ImDrawList* dl,ImVec2 pos,eHTextAlign align,  std::string str,ImU32 inColor,ImFont* font,float font_size){
    if(align==eTextCenter) {
        ImVec2 textSize = ImGui::CalcTextSize(str.c_str());
        pos.x = pos.x - textSize.x/2 - (font_size-2)/10  ;
    }
    if(align==eTextRight) {
        ImVec2 textSize = ImGui::CalcTextSize(str.c_str());
        pos.x = pos.x - textSize.x;
    }
    dl-> AddText( font, font_size,pos, inColor,str.c_str());

//    dl->AddText(pos,inColor,str.c_str());
}



void drawOrientedTriangle(ImDrawList* dl, ImVec2 pos, float l, float a, ImU32 color) {
    float halfL = l / 2.0f;
    float radians = a * 3.14159265359f / 180.0f; // Convertir l'angle en radians

    ImVec2 p1(pos.x + halfL * cos(radians), pos.y + halfL * sin(radians)); // Point 1
    ImVec2 p2(pos.x + halfL * cos(radians + 2.094f), pos.y + halfL * sin(radians + 2.094f)); // Point 2 (2.094 radians = 120 degrés)
    ImVec2 p3(pos.x + halfL * cos(radians - 2.094f), pos.y + halfL * sin(radians - 2.094f)); // Point 3

    dl->AddTriangleFilled(p1, p2, p3, color);
}



 /*

NVGcolor ArchiBusColor(const NVGcolor& inNormalColor,eSignalStatus inSignal){
    static double last_time = glfwGetTime();
    static bool last_status = true;
    if( (inSignal == selected) || (inSignal == error)) {
        double new_time = glfwGetTime() ;
        if( (new_time - last_time) > 1)
        {
            last_status = !last_status;
            last_time = new_time;
        }
    }
    if(last_status == false) return inNormalColor;
    if(inSignal == selected)  return gArchiTheme.mSelectedColor;
    else if(inSignal == error) return  gArchiTheme.mErrorColor;
    else return inNormalColor;
} 

void drawTriangle(NVGcontext* ctx,Vector2i pos,NVGcolor inColor,double angle){
    int delta = 15;

    nvgFillColor(ctx, inColor);    
    //nvgResetTransform(ctx);


    nvgSave(ctx); 
    nvgTranslate(ctx, pos.x(),pos.y());
    nvgRotate(ctx,angle);
    nvgTranslate(ctx, -pos.x(),-pos.y());       
    nvgBeginPath(ctx); 
    nvgMoveTo(ctx,pos.x()+delta/2,pos.y());            
    nvgLineTo(ctx,pos.x()-delta/2,pos.y()+delta/2);
    nvgLineTo(ctx,pos.x()-delta/2,pos.y()-delta/2);
    nvgLineTo(ctx,pos.x()+delta/2,pos.y());            
    nvgClosePath(ctx);    
    nvgFill(ctx);
    nvgRestore(ctx);


}*/