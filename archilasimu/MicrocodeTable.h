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