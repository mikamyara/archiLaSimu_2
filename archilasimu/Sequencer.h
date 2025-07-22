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
