/*
 * ArchiLaSimu - A Web navigator based Von-Neuman Architecture simulator 
 * Copyright (C) 2025 Mikhaël Myara, Arnaud Virazel - Université de Montpellier
 *
 * Ce programme est un logiciel libre : vous pouvez le redistribuer et/ou
 * le modifier selon les termes de la Licence Publique Générale GNU publiée
 * par la Free Software Foundation, soit la version 3 de la licence, soit
 * (à votre choix) toute version ultérieure.
 *
 * Ce programme est distribué dans l’espoir qu’il sera utile,
 * mais SANS AUCUNE GARANTIE, même implicite.
 * Voir la Licence Publique Générale GNU pour plus de détails.
 *
 * Vous devriez avoir reçu une copie de la Licence Publique Générale GNU
 * avec ce programme. Si ce n’est pas le cas, consultez <https://www.gnu.org/licenses/>.
 */

#pragma once



//#include "ArchiDescriptor.h"
#include "CircuitElements.h"
#include <GLFW/glfw3.h>
#include "imGuiApp.h"



class ArchiTheme {
public:
    ArchiTheme() ;
    ImU32 mBus1Color,mBus2Color,mBus3Color,mBoxWireColor,mRegisterOutputCircleColor,mMuxColor,mMicrocodeBusColor;
    ImU32 mAdressBusColor, mDataBusColor ;

    ImU32 mSelectedColor,mErrorColor;
    float mBusThickness;


    ImFont *mRobotoFont=NULL,*mRobotoBoldFont=NULL;   
    void LoadFonts( ImGuiIO *in_io);

};


enum eHTextAlign {eTextLeft,eTextCenter,eTextRight};

void addAlignedText(ImDrawList* dl,ImVec2 pos,eHTextAlign align,  std::string str,ImU32 inColor,ImFont* font,float font_size);
void drawOrientedTriangle(ImDrawList* dl, ImVec2 pos, float l, float a, ImU32 color) ;
void DrawArrow(ImDrawList* drawList, float t, float h, float l,ImU32 fillColor,ImU32 contourColor, ImVec2 position,float angleRotation);
void TranslatePoints(ImVec2* points, int numPoints, ImVec2 translation);
void RotatePoints(ImVec2* points, int numPoints, ImVec2 center, float angle);
ImU32 ArchiBusColor(const ImU32 inNormalColor,eSignalStatus inSignal);


void LoadImage(const char* filename, GLuint* textureID, int* width, int* height) ;


extern ArchiTheme gArchiTheme;
/*


NVGcolor ArchiBusColor(const NVGcolor& inNormalColor,eSignalStatus inSignal);
void drawTriangle(NVGcontext* ctx,Vector2i pos,NVGcolor inColor,double angle);

*/


