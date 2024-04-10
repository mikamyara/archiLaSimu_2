#pragma once

 

//#include "ArchiDescriptor.h"
#include "CircuitElements.h"



class ArchiTheme {
public:
    ArchiTheme() ;
    ImU32 mBus1Color,mBus2Color,mBus3Color,mBoxWireColor,mRegisterOutputCircleColor,mMuxColor;
    ImU32 mSelectedColor,mErrorColor;
    float mBusThickness;
    
    
    ImFont *mRobotoFont=NULL,*mRobotoBoldFont=NULL;
    void LoadFonts( ImGuiIO *in_io);
};


enum eHTextAlign {eTextLeft,eTextCenter,eTextRight};

void addAlignedText(ImDrawList* dl,ImVec2 pos,eHTextAlign align,  std::string str,ImU32 inColor,ImFont* font,float font_size);




extern ArchiTheme gArchiTheme;
/*


NVGcolor ArchiBusColor(const NVGcolor& inNormalColor,eSignalStatus inSignal);
void drawTriangle(NVGcontext* ctx,Vector2i pos,NVGcolor inColor,double angle);

*/


