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
#include <imgui.h>
#include <string>
#include "CodeUploadDownload.h"

class Assembleur;
class RAM {
public:
    RAM();
    void    draw(ImDrawList* dl, ImVec2 window_pos);
    void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);
    int     getValue(int address);
    void    setValue(int address,int data);
    void    setRemark(int address, std::string remark);
    void    insertByExpression(std::string inStr);


    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ImVec2 mPos;
    ImVec2 mRectPos,mRectSize;
    char** mColNames;
    int mCols,mRows;
    char** mData;
    char** mHelpers;
    RamFiles* mRamFiles;

    Assembleur* mASM;

    // options
    int disableLoad;
    int disableSave;
};
