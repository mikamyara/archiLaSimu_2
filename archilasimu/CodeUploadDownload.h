#pragma once
#include "MicrocodeTable.h"



#include <string>

class MicrocodeFiles {
public :
    MicrocodeFiles();
    void Download();
    void Upload();

    MicrocodeTable* mTable;

};



class RAM;

class RamFiles {
public :
    RamFiles();
    void Download();
    void Upload();

    RAM* mRAM;
};