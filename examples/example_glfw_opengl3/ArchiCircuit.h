#pragma once
#include <vector>
#include "CircuitElements.h"


class ArchiCircuit {
    public :
        ArchiCircuit();

        virtual  void    Rebuild();
        virtual  void    draw(ImDrawList* dl, ImVec2 window_pos);

        RegisterBus123  *RA,*RB,*RC,*RD,*OP,*CO,*RI,*RX,*RE,*RAM;
        std::vector<RegisterBus123*> allRegisters;

        ImVec2 mPos;

};

