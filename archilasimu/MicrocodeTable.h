#pragma once
#include <imgui.h>
#include <vector>
#include <string>
#include <map>

class CPU;

class MicrocodeTable {
public :
MicrocodeTable(CPU* inCPU);
void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);
void    Rebuild();
bool    insertByExpression(std::string expr);
bool matchMicrocodeExpression (std::string s,
                               int &code, int &suiv, int &SeIMS,
                               bool &Cond, bool &Fin,
                               std::vector < std::string > &orders);

bool matchCommand (std::string s, std::vector < std::string > &v);

bool matchSignals (const std::string input,
                   std::vector < std::string > &result);
int signalToCol(std::string signal);
std::string toUpper(std::string inStr);


CPU* mCPU;
char** mColNames;
int mCols,mRows;
//char** mAdrSuiv;
std::vector<int> mSeIMS;
std::vector<char*> mAdrSuiv;
//std::vector<std::vector<bool>> mSignals;
//int* mSeIMS;
char** mSeIMSMenu;
bool** mSignals;
std::map<std::string, int>  sigToCol;

};


