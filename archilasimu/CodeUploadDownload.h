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