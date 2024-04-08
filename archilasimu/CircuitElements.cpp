#include <cstddef>
#include "CircuitElements.h"
#include <iostream>
#include <cmath>
#include "ArchiTheme.h"
#include <imgui.h>
#include "ArchiLaSimuApp.h"
////---------------Wire----------------

Wire::Wire(bool inArrowed) {mStatus = normal; mTargetNode = nullptr;mArrowed = inArrowed;}

Node* 
Wire::setTarget(Node* inTarget) {
mTargetNode = inTarget;
return mTargetNode;
}

Node* 
Wire::getTarget(){
return mTargetNode;
}



////---------------Node----------------

Node::Node(std::string inName,ImVec2 inPos) {
    mPos = inPos;
    mName = inName;
    mStatus = normal;
    mLocalPos = mPos;
}

Wire* 
Node::addNewWire(bool inArrowed)
{
Wire* theWire = new Wire(inArrowed);
mWires.push_back(theWire);
return theWire;
}

Node* 
Node::FindFollowingNode(const std::string inName){
    if(mName == inName) return this;
    else {
        int k;
        for(k=0;k<mWires.size();k++) {
            if(mWires[k] != nullptr && mWires[k]->mTargetNode != nullptr)
            {
                Node* found  = mWires[k]->mTargetNode->FindFollowingNode(inName);
                if (found != nullptr) return found;
            }
        }
        return nullptr;
    }
} 
void 
Node::drawWires(ImDrawList* dl,ImU32 inBusColor,float inThickness,ImVec2 inAbsPos){

    for(int k=0;k<mWires.size();k++) {
        if(mWires[k] != nullptr && mWires[k]->mTargetNode != nullptr) {
            Node* destNode = mWires[k]->mTargetNode;

            ImVec2 P1,P2;
            P1 =  ImVec2( (int)(inAbsPos.x + mLocalPos.x)                ,(int)(inAbsPos.y + mLocalPos.y) );
            P2 =  ImVec2( (int)(inAbsPos.x + destNode->mLocalPos.x)      ,(int)(inAbsPos.y + destNode->mLocalPos.y));
            dl->AddLine(P1, P2, inBusColor, inThickness);
            
            dl->AddCircleFilled(P2,((int)inThickness)/2,inBusColor);


            //if(mWires[k]->mArrowed) {
            //    ImVec2 Point = (P1+P2)/2;
            //    float angle = std::atan2(P2.y()-P1.y(),P2.x()-P1.x());
            //    drawTriangle(ctx,Point,inBusColor,angle);
            //}
            destNode->drawWires(dl,inBusColor,inThickness,inAbsPos);
        }
    }

}



bool 
Node::changeStatus(Node* target,eSignalStatus newStatus){
    if(this == target) {
        mStatus = newStatus;
        return true;
    }
    else {
        for(int k=0;k<mWires.size();k++) {
            if(mWires[k]!= nullptr && mWires[k]->mTargetNode != nullptr) {
                if(mWires[k]->mTargetNode->changeStatus(target,newStatus)==true){ 
                    //mStatus = newStatus;
                    mWires[k]->mStatus = newStatus;
                    return true;}
            }
        }
    return false;
    }
}
/////---------------IOBox---------------
IOBoxNodes::IOBoxNodes():vector<Node*>() {
    mPosMode = e_Left;
}

ImVec2
IOBoxNodes::getNodePos(int inNum) const {
    return (*this)[inNum]->mPos;
}

ImVec2
IOBoxNodes::calcWireOrigin(int inNum, int inWireLen) const {
    return calcWireOrigin((*this)[inNum],inWireLen);
}

ImVec2
IOBoxNodes::calcWireOrigin(Node* inNode,int inWireLen) const {
    ImVec2 wireOrigin = inNode->mPos;
        if(mPosMode == e_Left) {
            wireOrigin.x+=inWireLen;
        }
        if(mPosMode == e_Right) {
            wireOrigin.x-=inWireLen;
        }
        if(mPosMode == e_Top) {
            wireOrigin.y+=inWireLen;
        }
        if(mPosMode == e_Bottom) {
            wireOrigin.y-=inWireLen;
        }    
    return wireOrigin;
}

IOBox::IOBox(){
    mName = "";
    mPos.x = 500;
    mPos.y = 500;
}
IOBox::IOBox(std::string inName,ImU32 inColor,ImVec2 inPos){
    mName = inName; mColor = inColor;
    mBorderColor = gArchiTheme.mBoxWireColor;
    mNameColor = gArchiTheme.mBoxWireColor;


    mRectSize = {60,80};
    mWireLen = 40; 
    mInputs.mPosMode = e_Left;
    mOutputs.mPosMode = e_Right;
    mPos = inPos;
}

void 
IOBox::DefineStructure()
{

}


void 
IOBox::RebuildMainRectAndPos(std::vector<eNodePos> inPosModeList){
    std::vector<eNodePos> finalPosList;
    // find unique position modes in given list, and fill
    // finalPosList with them.
    int k;
    for(k=0;k<inPosModeList.size();k++) {
        bool found = false;
        int m;
        for(m=0;m<finalPosList.size();m++) {
            if(inPosModeList[k] == finalPosList[m]){
                found=true;
            }
        }
        if(found==false) {
            finalPosList.push_back(inPosModeList[k]);
        }
    }
    // now build bounding rect and BOX position with these informations
    mBoundingRectSize = mRectSize;
    mRectPos={0,0};
    for(k=0;k<finalPosList.size();k++) {
        if(finalPosList[k] == e_Left) {
            mBoundingRectSize.x+=mWireLen;
            mRectPos.x+=mWireLen;
        }
        if(finalPosList[k] == e_Right) {
            mBoundingRectSize.x+=mWireLen;
        }
        if(finalPosList[k] == e_Top) {
            mBoundingRectSize.y+=mWireLen;
            mRectPos.y+=mWireLen;
        }
        if(finalPosList[k] == e_Bottom) {
            mBoundingRectSize.y+=mWireLen;
        }
    }
}

void IOBox::RebuildNodesCoords(IOBoxNodes& ioNodesList){
    int k;
    int betweenY = mRectSize.y/(ioNodesList.size()+1);
    int betweenX = mRectSize.x/(ioNodesList.size()+1);

    for(k=0;k<ioNodesList.size();k++) {
        if(ioNodesList.mPosMode == e_Left)
        {
            ioNodesList[k]->mPos = ImVec2(0                                   ,mRectPos.y+betweenY*(k+1));
        }
        if(ioNodesList.mPosMode == e_Right)
        {
            ioNodesList[k]->mPos = ImVec2(mRectPos.x+mRectSize.x+mWireLen  ,mRectPos.y+betweenY*(k+1));
        }    
        if(ioNodesList.mPosMode == e_Top)
        {
            ioNodesList[k]->mPos = ImVec2(mRectPos.x+ betweenX*(k+1)         , 0);
        }               
        if(ioNodesList.mPosMode == e_Bottom)
        {
            ioNodesList[k]->mPos = ImVec2(mRectPos.x+ betweenX*(k+1)          ,mRectPos.y+mRectSize.y+mWireLen);
        }    
    }

}

void
IOBox::Rebuild() {
    DefineStructure();    
    RebuildMainRectAndPos({mInputs.mPosMode,mOutputs.mPosMode});
    RebuildNodesCoords(mInputs);
    RebuildNodesCoords(mOutputs);
    RebuildLocalCoords();
}



void
IOBox::RebuildLocalCoords() {
     int k;
    for(k=0;k<mOutputs.size();k++) {
        mOutputs[k]->mLocalPos = ImVec2( mOutputs[k]->mPos.x + mPos.x,mOutputs[k]->mPos.y + mPos.y);
    }

    for(k=0;k<mInputs.size();k++) {
        mInputs[k]->mLocalPos =  ImVec2( mInputs[k]->mPos.x + mPos.x,mInputs[k]->mPos.y + mPos.y);
    }
 
}




void
IOBox::draw(ImDrawList* dl,ImVec2 window_pos) {

    drawBox(dl,window_pos);
    drawName(dl,window_pos);    
    drawInputNodes(dl,window_pos);
    drawOutputNodes(dl,window_pos);
}



void
IOBox::drawInputNodes(ImDrawList* dl,ImVec2 window_pos) {
    drawNodes(dl,window_pos,mInputs,gArchiTheme.mBoxWireColor);
}
void
IOBox::drawOutputNodes(ImDrawList* dl,ImVec2 window_pos) {
    drawNodes(dl,window_pos,mOutputs,gArchiTheme.mBoxWireColor);

}
  //  IMGUI_API void  AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 0);


void 
IOBox::calcWirePosition(ImVec2 window_pos,const IOBoxNodes& theNodes,int num,ImVec2& P1, ImVec2& P2)   {
    P1 = ImVec2(theNodes[num]->mPos.x +mPos.x+window_pos.x,theNodes[num]->mPos.y +mPos.y+window_pos.y);
    P2 = theNodes.calcWireOrigin(num,mWireLen);
    P2 = ImVec2(P2.x + mPos.x + window_pos.x , P2.y + mPos.y + window_pos.y) ;
  
}

void 
IOBox::drawNode(ImDrawList* dl,ImVec2 window_pos,const IOBoxNodes& theNodes,int num,ImU32 inBusColor){

    ImVec2 P1,P2;
    calcWirePosition(window_pos,theNodes,num,P1,P2);
    dl->AddLine(P1, P2, inBusColor, gArchiTheme.mBusThickness);
}

void
IOBox::drawNodes(ImDrawList* dl,ImVec2 window_pos, const IOBoxNodes& theNodes,ImU32 inBusColor) {
    int k;
    for(k=0;k<theNodes.size();k++) {
        drawNode(dl,window_pos,theNodes,k,inBusColor);
     }
}


void
IOBox::drawBox(ImDrawList* dl,ImVec2 window_pos) {
    ImVec2 thePosMin,thePosMax;

    thePosMin = ImVec2(window_pos.x + mPos.x + mRectPos.x , window_pos.y + mPos.y + mRectPos.y );
    thePosMax = ImVec2 (thePosMin.x + mRectSize.x, thePosMin.y + mRectSize.y);
    dl->AddRectFilled (thePosMin, thePosMax, mColor);  
    dl->AddRect(thePosMin, thePosMax, mBorderColor, 0.0f, ImDrawFlags_None, 6.0f);
}



void    
IOBox::drawName(ImDrawList* dl,ImVec2 window_pos) {
    ImVec2 pos;
    pos = ImVec2(mPos.x+window_pos.x+mRectPos.x+mRectSize.x/2,mPos.y+window_pos.y+mRectPos.y+6);

    addAlignedText(dl,pos,eTextCenter,mName.c_str(),mNameColor,gArchiTheme.mRobotoBoldFont,24);


    //dl->AddText(pos,mNameColor,mName.c_str());
    //std::cout << pos.x << " " << pos.y << " " << mName << " " << mNameColor << " " <<mRectPos.x << "\n";
}









////---------------RegisterBus123----------------
RegisterBus123::RegisterBus123():IOBox("",0,{0,0}) {}

RegisterBus123::RegisterBus123(std::string inName,ImU32 inColor,ImVec2 inPos):IOBox(inName,inColor,inPos)
{
    mValue = 0;
    mRectSize = ImVec2(90,72);
    mWireLen = 55.0f;
    mBorderColor = inColor;
    

    mOutputs.resize(0); // makes memory leak, should be improved
    mInputs.resize(0);
    // add nodes
    mOutputs.push_back(mBus2Node=new Node());
    mOutputs.push_back(mBus1Node=new Node());
    mInputs.push_back(mBus3Node=new Node());

    // set nodes names
    if(mName.size() != 0) {
        mBus1Node->mName = mName+"B1";
        mBus2Node->mName = mName+"B2";
        mBus3Node->mName = "e"+mName;
    }

    for(int k=0;k<5;k++) { buf[k] ='0';}
    buf[5]=0;

    mInputTextLabel = "##"+mName;
    
    mInputTextWidth = 60;
}

 
void
RegisterBus123::drawOutputNodes(ImDrawList* dl,ImVec2 window_pos) {
    int k;
    ImU32 busColor ;
    const IOBoxNodes& theNodes = mOutputs;
    for(k=0;k<theNodes.size();k++) {
        if(theNodes[k] == mBus1Node )  {busColor = gArchiTheme.mBus1Color;}
        else {busColor = gArchiTheme.mBus2Color;}
        drawNode(dl,window_pos,theNodes,k,busColor);
     }
}
void
RegisterBus123::drawInputNodes(ImDrawList* dl,ImVec2 window_pos) {
    //std::cout << "YOUHOU\n";
    drawNodes(dl,window_pos,mInputs,gArchiTheme.mBus3Color);

}



void 
RegisterBus123::draw(ImDrawList* dl, ImVec2 window_pos){

IOBox::draw(dl,window_pos);
drawLabels(dl,window_pos);

drawInputText(dl,window_pos);
}


void     
RegisterBus123::drawInputText(ImDrawList* dl,ImVec2 window_pos){
    ImVec2 labelPos = ImVec2(mPos.x + window_pos.x + mRectPos.x + (mRectSize.x - mInputTextWidth)/2,mPos.y + window_pos.y + mRectPos.y + 35);   
    ImGui::SetCursorPos (labelPos);
    ImGui::PushItemWidth(mInputTextWidth);
    ImGui::InputText(mInputTextLabel.c_str(), buf, 6,ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();
}


void 
RegisterBus123::drawLabels(ImDrawList* dl,ImVec2 window_pos){
//if(mName == "RAM") {std::cout << mBus1Node <<" " << mBus2Node << "\n";}
    ImVec2 P1,P2,C;
    eHTextAlign mode = eTextLeft;
    // Bus 3
    if(mBus3Node != nullptr){
        calcWirePosition(window_pos,mInputs,0,P1,P2);
        C.x = (P1.x + P2.x)/2 ; C.y = (P1.y + P2.y)/2;
        dl->AddCircleFilled(C,5,gArchiTheme.mBus3Color);
        if(mInputs.mPosMode == e_Left){ C.y-=25; mode =eTextCenter; }
        else if(mInputs.mPosMode == e_Bottom) {C.x+=10; C.y-=10; mode=eTextLeft;}
        addAlignedText(dl,C,mode,mInputs[0]->mName.c_str(),gArchiTheme.mBus3Color,gArchiTheme.mRobotoBoldFont,20);
    }


    // Bus 1
    if(mBus1Node != nullptr){
        calcWirePosition(window_pos,mOutputs,1,P1,P2);
        C.x = (P1.x + P2.x)/2 ; C.y = (P1.y + P2.y)/2;
        dl->AddCircleFilled(C,8,gArchiTheme.mRegisterOutputCircleColor);
        dl->AddCircle(C,8,gArchiTheme.mBus1Color,12,2);
        C.y+=8;
        addAlignedText(dl,C,eTextCenter,mOutputs[1]->mName.c_str(),gArchiTheme.mBus1Color,gArchiTheme.mRobotoBoldFont,20);
    }

    // Bus 2
    if(mBus2Node != nullptr){
        calcWirePosition(window_pos,mOutputs,0,P1,P2);
        C.x = (P1.x + P2.x)/2 ; C.y = (P1.y + P2.y)/2;
        dl->AddCircleFilled(C,8,gArchiTheme.mRegisterOutputCircleColor);
        dl->AddCircle(C,8,gArchiTheme.mBus2Color,12,2);
        C.y-=27;
        addAlignedText(dl,C,eTextCenter,mOutputs[0]->mName.c_str(),gArchiTheme.mBus2Color,gArchiTheme.mRobotoBoldFont,20);
    }

}

void 
RegisterBus123::DefineStructure()
{

}


//------------------------- CombinatorialOperator ----------------------

CombinatorialOperator::CombinatorialOperator(std::string inName,ImU32 inColor,ImVec2 inPos):RegisterBus123(inName,inColor,inPos)
{
mOutputs.mPosMode=e_Top; 
mInputs.mPosMode=e_Bottom;
mRectSize = {120,130};
Node* swap = mBus1Node;
mBus1Node = mBus2Node;
mBus2Node = swap;
}

void 
CombinatorialOperator::draw(ImDrawList* dl, ImVec2 window_pos){

    IOBox::draw(dl,window_pos);

    ImVec2 labelPos = ImVec2(mPos.x + window_pos.x + mRectPos.x + (mRectSize.x - mInputTextWidth)/2,mPos.y + window_pos.y + mRectPos.y + 35);
    ImGui::SetCursorPos (labelPos);
    ImGui::PushItemWidth(mInputTextWidth);
    ImGui::InputText(mInputTextLabel.c_str(), buf, 6);
    ImGui::PopItemWidth();

}

void    
CombinatorialOperator::drawName(ImDrawList* dl,ImVec2 window_pos) {
    ImVec2 pos;
    pos = ImVec2(mPos.x + window_pos.x + mRectPos.x + mBus1Node->mPos.x,mPos.y + window_pos.y + mRectPos.y + 5);
    addAlignedText(dl,pos,eTextCenter,"X",mNameColor,gArchiTheme.mRobotoBoldFont,24);
    pos.x = mPos.x + window_pos.x + mRectPos.x + mBus2Node->mPos.x;
    addAlignedText(dl,pos,eTextCenter,"Y",mNameColor,gArchiTheme.mRobotoBoldFont,24);

    
    
    pos = ImVec2(mPos.x+window_pos.x+mRectPos.x+mRectSize.x/2,mPos.y+window_pos.y+mRectPos.y+mRectSize.y-50);
    addAlignedText(dl,pos,eTextCenter,"Opérateur",mNameColor,gArchiTheme.mRobotoBoldFont,21);
    pos.y+=23;
    addAlignedText(dl,pos,eTextCenter,"Combinatoire",mNameColor,gArchiTheme.mRobotoBoldFont,21);

}

  


//------------------------- InstructionRegisterWidget ----------------------
InstructionRegister::InstructionRegister( ) {

}
InstructionRegister::InstructionRegister(std::string inName,ImU32 inColor,ImVec2 inPos):RegisterBus123(inName,inColor,inPos) {
    //mRectSize = {190,90};
    mRectSize = {90,144};
    mFormaterInputTextLabel = mInputTextLabel + "F";
}
void
InstructionRegister::drawName(ImDrawList* dl,ImVec2 window_pos) {

    ImVec2 thePosMin = ImVec2(window_pos.x + mPos.x + mRectPos.x , window_pos.y + mPos.y + mRectPos.y+90);
    ImVec2 thePosMax = ImVec2 (thePosMin.x + mRectSize.x, thePosMin.y + mRectSize.y-90);    
    dl->AddRectFilled (thePosMin, thePosMax, IM_COL32(0,0,0,80));    

    RegisterBus123::drawName(dl,window_pos);
    ImVec2 pos = ImVec2(mPos.x + window_pos.x + mRectPos.x + mRectSize.x/2 ,mPos.y + window_pos.y + mRectPos.y + 35);
    addAlignedText(dl,pos,eTextCenter,"OPCode",mNameColor,gArchiTheme.mRobotoFont,20);
    pos = ImVec2(mPos.x + window_pos.x + mRectPos.x + mRectSize.x/2,mPos.y + window_pos.y + mRectPos.y + 95);
    addAlignedText(dl,pos,eTextCenter,"Formatteur",mNameColor,gArchiTheme.mRobotoFont,20);
  


}

void     
InstructionRegister::drawInputText(ImDrawList* dl,ImVec2 window_pos){
    ImVec2 pos = ImVec2(mPos.x + window_pos.x + mRectPos.x + 15 ,mPos.y + window_pos.y + mRectPos.y + 55);
    ImGui::SetCursorPos (pos);
    ImGui::PushItemWidth(mInputTextWidth);
    ImGui::InputText(mInputTextLabel.c_str(), buf, 6,ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();
    pos = ImVec2(mPos.x + window_pos.x + mRectPos.x + 15,mPos.y + window_pos.y + mRectPos.y + 115);
    ImGui::SetCursorPos (pos);
    ImGui::PushItemWidth(mInputTextWidth);
    ImGui::InputText(mFormaterInputTextLabel.c_str(), buf, 6,ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();    

}


void
InstructionRegister::Rebuild() {
    DefineStructure();    
    RebuildMainRectAndPos({mInputs.mPosMode,mOutputs.mPosMode});
    RebuildNodesCoords(mInputs);
//    RebuildNodesCoords(mOutputs);

    int k;
    int mOffset = 80;
    int betweenY = (mRectSize.y-mOffset)/(mOutputs.size()+1);

    for(k=0;k<mOutputs.size();k++) {
            mOutputs[k]->mPos = ImVec2(mRectPos.x+mRectSize.x+mWireLen  , mOffset + mRectPos.y+betweenY*(k+1));
    }
    RebuildLocalCoords();
}




////----------------Bus-----------------------
Bus::Bus(Node* inStart ,std::string inName,ImU32 inColor,float inThickness) {mStart = inStart;mName = inName; mColor = inColor;mThickness = inThickness;}

Bus::~Bus() {}

Node* 
Bus::FindNode(const std::string inName) {
    if(mStart != nullptr) {
        return mStart->FindFollowingNode(inName);
    }
    else return nullptr;
}

void 
Bus::changePortionStatus(std::string targetNodeName,eSignalStatus newStatus) {
Node* targetNode = FindNode(targetNodeName);
mStart->mStatus = newStatus;
mStart->changeStatus(targetNode,newStatus);
}



void 
Bus::draw(ImDrawList* dl,ImVec2 window_pos){
   // m_size = preferred_size(ctx);
    //Vector2i the_size = preferred_size(ctx);
 
    //nvgLineCap(ctx, NVG_ROUND);
    //std::cout <<"BUS\n";
mStart->drawWires(dl,mColor,mThickness,window_pos);

}



////-----------MUX-------------------
MUX::MUX(std::string inName,int inInputs):IOBox(inName,0,{0,0}) {
mWireLen = 20; 
mColor= IM_COL32(75,50,30,255);
mRectSize.y=20*(inInputs+1);
mRectSize.x = 90;

int k;
for(k=0  ; k<inInputs;k++) {
    mInputs.push_back(new Node());
}
mOutputs.push_back(new Node());

}
 

///// ---------- Utilities ----------------------
ImVec2  HVPos(ImVec2 HNode, ImVec2 VNode)
{ return ImVec2(HNode.x,VNode.y);
}

ImVec2 HExt(ImVec2 Node, int ext)
{  return ImVec2(Node.x+ ext,Node.y);
}

ImVec2 VExt(ImVec2 Node, int ext)
{   return ImVec2(Node.x,Node.y + ext);
}


