#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <imgui.h>


enum eSignalStatus { normal,selected,error};


class Node;
////---------------Wire----------------

class Wire {
    public:
    Wire(bool arrowed = false);
    Node* setTarget(Node* inTarget);
    Node* getTarget();

    Node* mTargetNode;
    eSignalStatus mStatus;
    bool mArrowed;

};
////---------------Node----------------

class Node {
    public:
    Node(std::string inName="",ImVec2 inPos={0,0});
    Wire* addNewWire(bool inArrowed = false);
    Node* FindFollowingNode(const std::string inName);
    //void drawWires(NVGcontext* ctx,NVGcolor inBusColor,float inThickness,Vector2i inAbsPos);
    bool changeStatus(Node* target,eSignalStatus newStatus);

    std::vector<Wire*> mWires; 
    ImVec2 mPos; // is relative to box
    ImVec2 mLocalPos; // is relative to ArchiWidget     
    std::string mName;
    eSignalStatus mStatus;
};

///----------------IOBox---------------
enum eNodePos{e_Left,e_Right,e_Top,e_Bottom};

class IOBoxNodes : public std::vector<Node*> {
    public:
        IOBoxNodes();
        eNodePos mPosMode;
        ImVec2 calcWireOrigin(int inNum,int inWireLen) const;
        ImVec2 calcWireOrigin(Node* inNode,int inWireLen) const;
        ImVec2 getNodePos(int inNum) const ;     
};


class IOBox {
    public:
        IOBox();
        IOBox(std::string inName,ImU32 inColor,ImVec2 inPos);
        virtual void RebuildMainRectAndPos(std::vector<eNodePos> inPosModeList);
        virtual void RebuildNodesCoords(IOBoxNodes& ioNodesList);
        virtual void DefineStructure();
        virtual void Rebuild();

        // drawing related methods
        virtual  void    RebuildLocalCoords();
        virtual  void    draw(ImDrawList* dl, ImVec2 window_pos);
        virtual  void    drawBox(ImDrawList* dl,ImVec2 window_pos);    
       // void    drawName(ImDrawList* dl,ImVec2 window_pos);            
        virtual  void    drawNode(ImDrawList* dl,ImVec2 window_pos,const IOBoxNodes& theNodes,int num,ImU32 inBusColor);
        virtual  void    drawNodes(ImDrawList* dl,ImVec2 window_pos, const IOBoxNodes& theNodes,ImU32 inBusColor);
        virtual  void    drawName(ImDrawList* dl,ImVec2 window_pos);
        virtual  void    drawInputNodes(ImDrawList* dl,ImVec2 window_pos);
        virtual  void    drawOutputNodes(ImDrawList* dl,ImVec2 window_pos);    
        virtual  void    calcWirePosition(ImVec2 window_pos,const IOBoxNodes& theNodes,int num,ImVec2& P1, ImVec2& P2)    ;
        ImU32 mColor,mBorderColor;
        ImU32 mNameColor;
        std::string mName;
        IOBoxNodes mInputs,mOutputs;

        int mWireLen;
        ImVec2 mRectSize,mRectPos;
        ImVec2 mBoundingRectSize;
        ImVec2 mPos;
};
////---------------RegisterBus123----------------

class RegisterBus123:public IOBox{
    public:
    RegisterBus123();
    RegisterBus123(std::string inName,ImU32 inColor,ImVec2 inPos);
    virtual void DefineStructure();
    virtual void draw(ImDrawList* dl, ImVec2 window_pos);
    virtual void drawLabels(ImDrawList* dl,ImVec2 window_pos);
    virtual  void    drawInputNodes(ImDrawList* dl,ImVec2 window_pos);
    virtual  void    drawOutputNodes(ImDrawList* dl,ImVec2 window_pos);    
    virtual void     drawInputText(ImDrawList* dl,ImVec2 window_pos);


    // general data
    int mValue;
    char buf[6];
    std::string mInputTextLabel,mFormaterInputTextLabel;
    Node *mBus1Node,*mBus2Node,*mBus3Node;
    int mInputTextWidth;

};


////----------------CombinatorialOperator-----------------------


class CombinatorialOperator:public RegisterBus123 {
    public:
        CombinatorialOperator();
        CombinatorialOperator(std::string inName,ImU32 inColor,ImVec2 inPos);
        
        virtual void draw(ImDrawList* dl, ImVec2 window_pos);
        virtual void drawName(ImDrawList* dl,ImVec2 window_pos);
   
};
////----------------CombinatorialOperator-----------------------

class InstructionRegister:public RegisterBus123 {
    public:
    InstructionRegister();
    InstructionRegister(std::string inName,ImU32 inColor,ImVec2 inPos);
   // virtual void draw(ImDrawList* dl, ImVec2 window_pos);
    virtual void drawName(ImDrawList* dl,ImVec2 window_pos);
    virtual void drawInputText(ImDrawList* dl,ImVec2 window_pos);

};

////----------------Bus-----------------------

class Bus {
    public:
    Bus(Node* inStart,std::string inName,ImU32 inColor,float inThickness); //////// !!! auto init  ImU32 ?
    Node* FindNode(const std::string inName);
    void changePortionStatus(std::string inTargetNodeName,eSignalStatus newStatus);

    Node* mStart;
    std::string mName;
    ImU32 mColor;
    float mThickness;
};
////----------------MUX--------------------
class MUX:public IOBox {
    public:
    MUX(std::string inName="",int inInputs=0);
    int mCurrentValue;

};




//------------------Utilities-------------------
ImVec2 HVPos(ImVec2 HNode, ImVec2 VNode);
ImVec2 HExt(ImVec2 Node, int ext);
ImVec2 VExt(ImVec2 Node, int ext);

