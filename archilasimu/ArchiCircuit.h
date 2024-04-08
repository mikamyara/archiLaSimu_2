#pragma once
#include <vector>
#include "CircuitElements.h"


class ArchiCircuit {
    public :
        ArchiCircuit();

        virtual  void    Rebuild();
        virtual  void    draw(ImDrawList* dl, ImVec2 window_pos);
        virtual void     calcBus();
        virtual ~ArchiCircuit();


        RegisterBus123  *RA,*RB,*RC,*RD,*OP,*CO,*RI,*RX,*RE,*RAM;
        std::vector<RegisterBus123*> allRegisters;
        
        Bus *mBus1,*mBus2,*mBus3;
        std::vector<Bus*> mBuses;

        ImVec2 mPos;

};

