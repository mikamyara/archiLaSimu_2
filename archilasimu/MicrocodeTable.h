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
#include <vector>
#include <string>
#include <map>

class CPU;
class MicrocodeTable;


class shortMicrocodeTextCallbackStruct {
    public:
        shortMicrocodeTextCallbackStruct();
        MicrocodeTable* source;
        int row;
};

class MicrocodeTable
{
public:
    MicrocodeTable (CPU * inCPU);

    void ShortMicroCodeTableWidget (ImDrawList * dl, ImVec2 window_pos);
    void MainMicrocodeTableWidget (ImDrawList * dl, ImVec2 window_pos);
    void drawWidgets (ImDrawList * dl, ImVec2 window_pos);
    void Rebuild ();
    bool insertByExpression (std::string expr,bool updateShortView=true);
    bool matchMicrocodeExpression (std::string s,
                                   int &code, int &suiv, int &SeIMS,
                                   int &Cond, bool &Fin,
                                   std::vector < std::string > &orders);

    bool matchCommand (std::string s, std::vector < std::string > &v);

    bool matchSignals (const std::string input,
                       std::vector < std::string > &result);
   // int signalToCol (std::string signal);
    std::string signalsToString(int row);
    std::string exportExpression(int row);
    bool isOrderValid(std::string inOrder,bool includeMuxes=true);

    CPU *mCPU;
    char **mColNames;
    int mCols, mRows;
    std::vector < int >mSeIMS;
    std::vector < int >mCond;
    std::vector < char *>mAdrSuiv;
    char **mSeIMSMenu;
    char **mCondMenu;
    bool **mSignals;
    std::map < std::string, int > sigToCol;
    std::map < std::string, int > sigToColNoMuxes;

    int mShortViewCols;
    char **mShortViewColNames;
    char **mShortViewStrings;
    int mShortViewOrdresCol;
    std::vector<int> mShortViewErrors;
    std::vector<shortMicrocodeTextCallbackStruct> mShortModeCallbacks;

    static void signalsCallback(int row,int column,bool p_open,MicrocodeTable* p);
    static int shortTextSignalCallback(ImGuiInputTextCallbackData *p);


};
