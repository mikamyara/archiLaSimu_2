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



    RegisterBus123  *RA,*RB,*RX,*CO,*SP,*RE,*RAM;
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

