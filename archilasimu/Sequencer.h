#pragma once

#include "CircuitElements.h"
#include "MicrocodeTable.h"
#include <string>
#include <vector>

class CPU;


class Sequencer {
public :
    Sequencer(CPU* inCPU);
    virtual  void    Rebuild();
    virtual  void    draw(ImDrawList* dl, ImVec2 window_pos);
    virtual  void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);
    void             reskinIOBox(IOBox* outBox);
    virtual void calcBus();

    //virtual void     calcBus();
    virtual ~Sequencer();
    void displayBusses(std::string str);


    BasicRegister *SeIMS,*Cond,*Fin;
    IOBox *Plus1;
    BasicRegister *Fetch,*OpCode,*Microcode;
    MicrocodeRegister* MicrocodeReg;
    std::vector<IOBox*> allBoxes;
    ImVec2 mPos;

    std::vector<Bus*> mBusses;
    MicrocodeTable* mCodeTable;
    CPU* mCPU;
    std::vector<std::string> mMuxesNames;
};