#pragma once
#include <vector>
#include <string>
#include "CircuitElements.h"


class ArchiCircuit {
public :
    ArchiCircuit();

    virtual  void    Rebuild();
    virtual  void    draw(ImDrawList* dl, ImVec2 window_pos);
    virtual  void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);
    //virtual  void    calcSignalsList(std::vector<std::string>>& outList);
    virtual void     calcBus();
    void             getMainNodes(std::vector<std::string>& outB1,std::vector<std::string>& outB2,std::vector<std::string>& outB3);


    Bus* getBusBySignalName(std::string inStr);
    
    virtual ~ArchiCircuit();



    RegisterBus123  *RA,*RB,*RC,*RD,*CO,*RX,*RE,*RAM;
    IOBox *sensEch;

    CombinatorialOperator* OP;
    InstructionRegister* RI;
    std::vector<IOBox*> allRegisters;

    Bus *mBus1,*mBus2,*mBus3;
    std::vector<Bus*> mBuses;
    std::vector<std::string> mB1signals,mB2signals,mB3signals,mOtherSignals;
    std::map < std::string, Bus* > sigToBus;


    ImVec2 mPos;


};

