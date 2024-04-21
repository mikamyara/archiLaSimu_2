#include "ArchiTheme.h"
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>

ArchiTheme gArchiTheme;
int ArchiTheme::shidpiScale=1;


ArchiTheme::ArchiTheme() {
    mBus1Color     = IM_COL32(255, 255, 0,255);
    mBus3Color     = IM_COL32(255, 180, 0,255);
    mBus2Color     = IM_COL32(255, 255, 255,255);
    mDataBusColor   = IM_COL32(30,75,00,255);
    mAdressBusColor     = IM_COL32(10,45,00,255);

    mBoxWireColor  = IM_COL32(255,255,255,255);
    mSelectedColor = IM_COL32(75,255,75,255);
    mErrorColor    = IM_COL32(188,133,0,120);
    mRegisterOutputCircleColor =  IM_COL32(00,80,140,255);
    mMuxColor = IM_COL32(75,50,30,255);
    mMicrocodeBusColor = IM_COL32(00,200,255,255);

    mBusThickness  = 4.0f*ArchiTheme::shidpiScale;

    mRobotoFont=NULL;
    mRobotoBoldFont=NULL;

}


void
ArchiTheme::LoadFonts( ImGuiIO *in_io) {
    in_io->Fonts->AddFontDefault ();
    mRobotoFont =
        in_io->Fonts->AddFontFromFileTTF ("/fonts/Roboto-Regular.ttf", shidpiScale*20.0f);
    mRobotoBoldFont =
        in_io->Fonts->AddFontFromFileTTF ("/fonts/Roboto-Bold.ttf", shidpiScale*50.0f);

}


void addAlignedText(ImDrawList* dl,ImVec2 pos,eHTextAlign align,  std::string str,ImU32 inColor,ImFont* font,float font_size) {
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




void TranslatePoints(ImVec2* points, int numPoints, ImVec2 translation)
{
    for (int i = 0; i < numPoints; ++i)
    {
        points[i].x += translation.x;
        points[i].y += translation.y;
    }
}


void RotatePoints(ImVec2* points, int numPoints, ImVec2 center, float angle)
{
    // Calcul des cosinus et sinus de l'angle
    angle = angle/180*M_PI;
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    // Rotation de chaque point autour du centre
    for (int i = 0; i < numPoints; ++i)
    {
        // Translater le point pour le mettre au point d'origine
        ImVec2 translatedPoint = ImVec2(points[i].x - center.x, points[i].y - center.y);

        // Appliquer la rotation
        float rotatedX = translatedPoint.x * cosAngle - translatedPoint.y * sinAngle;
        float rotatedY = translatedPoint.x * sinAngle + translatedPoint.y * cosAngle;

        // Remettre le point à sa position d'origine
        points[i].x = rotatedX + center.x;
        points[i].y = rotatedY + center.y;
    }
}


void DrawArrow(ImDrawList* drawList, float t, float h, float l,ImU32 fillColor,ImU32 contourColor, ImVec2 position,float angleRotation)
{

    ImVec2 arrowPoints[7];
     // Ajouter les points du rectangle
    arrowPoints[0] = ImVec2(-h / 2.0f, -l/2);   
    arrowPoints[1] = ImVec2(h / 2.0f, -l/2);    
    arrowPoints[2] = ImVec2(h / 2.0f, l/2);     
    // Ajouter les points du triangle
    arrowPoints[3] = ImVec2(t/2, l/2);                    
    arrowPoints[4] = ImVec2(0, l/2 + t);       
    arrowPoints[5] = ImVec2(-t/2, l/2);    
    arrowPoints[6] = ImVec2(-h / 2.0f, l/2);   

    RotatePoints(arrowPoints,7,ImVec2(0,0),angleRotation);
    TranslatePoints(arrowPoints,7,position);

    drawList->AddConcavePolyFilled(arrowPoints, 7, fillColor);
    drawList->AddPolyline(arrowPoints, 7, contourColor,ImDrawFlags_Closed,1);

}

ImU32 ArchiBusColor(const ImU32 inNormalColor,eSignalStatus inSignal){
   if( inSignal == selected ) {
            return gArchiTheme.mSelectedColor;
    }
   if(inSignal == error) {
        return gArchiTheme.mErrorColor;

   }
   return inNormalColor;
}





void LoadImage(const char* filename, GLuint* textureID, int* width, int* height) {
    int channels;
    unsigned char* image = stbi_load(filename, width, height, &channels, STBI_rgb_alpha);
    if (!image) {
        printf("Impossible de charger l'image : %s\n", filename);
        return;
    }

    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
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
*/