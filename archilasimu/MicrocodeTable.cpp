#include "MicrocodeTable.h"
#include <vector>
#include <string>
#include "CPU.h"
#include <iostream>
#include <regex>
#include <vector>
#include "stringUtils.h"
#include "ArchiTheme.h"
MicrocodeTable::MicrocodeTable (CPU * inCPU)
{

    mColNames = nullptr;
    mSeIMSMenu = nullptr;
    mShortViewStrings = nullptr;
    mShortViewCols = 0;
    mCPU = inCPU;
    mCols = 0;
    mRows = 0;

}

void
MicrocodeTable::Rebuild ()
{

    mRows = 501;
//## Step A : check column names
    std::vector < std::string > theColsList;
// build a list of all signals to consider in the MicrocodeTable
    theColsList.push_back ("uCode");
    theColsList.push_back ("suiv");
    int k;
    for (k = 0; k < mCPU->mSequencer->mMuxesNames.size (); k++)
    {
        theColsList.push_back (mCPU->mSequencer->mMuxesNames[k]);
    }

    for (k = 0; k < mCPU->mArchiCircuit->mB1signals.size (); k++)
    {
        theColsList.push_back (mCPU->mArchiCircuit->mB1signals[k]);
    }
    for (k = 0; k < mCPU->mArchiCircuit->mB2signals.size (); k++)
    {
        theColsList.push_back (mCPU->mArchiCircuit->mB2signals[k]);
    }
    for (k = 0; k < mCPU->mArchiCircuit->mOtherSignals.size (); k++)
    {
        theColsList.push_back (mCPU->mArchiCircuit->mOtherSignals[k]);
    }


    std::map < std::string, CombinatorialOperator::FunctionPtr > &ops =
        mCPU->mArchiCircuit->OP->mOperations;
    for (std::map < std::string,
            CombinatorialOperator::FunctionPtr >::iterator it = ops.begin ();
            it != ops.end (); ++it)
    {
        theColsList.push_back (it->first);
    }

    for (k = 0; k < mCPU->mArchiCircuit->mB3signals.size (); k++)
    {
        theColsList.push_back (mCPU->mArchiCircuit->mB3signals[k]);
    }

// then convert it in more standard C structure in order to give it to imGui
    mColNames = new char *[theColsList.size ()];
    for (k = 0; k < theColsList.size (); k++)
    {
        mColNames[k] = new char[theColsList[k].size () + 1];
        strcpy (mColNames[k], theColsList[k].c_str ());
    }

    mCols = theColsList.size ();

// build Map
    std::string uped;
    for (k = 0; k < theColsList.size (); k++)
    {   uped = toUpper(theColsList[k]);
       // std::cout << uped <<" "<< k << "\n";
        sigToCol[uped] = k;

        if(!(uped =="UCODE" || uped =="SUIV" || uped =="SEIMS" || uped =="COND" || uped =="FIN")) { 
            sigToColNoMuxes[uped] = k;
            }
    }


//## Step B : build next address column (data)
    mAdrSuiv.resize (mRows);
    for (int l = 0; l < mRows; l++)
    {
        mAdrSuiv[l] = new char[4];
        strcpy (mAdrSuiv[l], "000");
    }

//## Step C : build SeIMS and Cond columns (data)
    mSeIMS.resize (mRows);
    for (int l = 0; l < mSeIMS.size (); l++)
    {
        mSeIMS[l] = 0;
    }
    mCond.resize (mRows);
    for (int l = 0; l < mCond.size (); l++)
    {
        mCond[l] = 0;
    }

//## Step D : build array for all other bools
    mSignals = new bool *[mRows];
    for (int l = 0; l < mRows; l++)
    {
        mSignals[l] = new bool[mCols];
        for (int m = 4; m < mCols; m++)
        {
            mSignals[l][m] = false;
        }
    }




//## Step D : build SeIMS & Cond menus
    mSeIMSMenu = new char *[4];
    for (int l = 0; l < 4; l++)
    {
        mSeIMSMenu[l] = new char[2];
        char str[] = "0";
        str[0] += l;
        strcpy (mSeIMSMenu[l], str);
    }
    mCondMenu = new char *[7];
    for (int l = 0; l < 7; l++)
    {
        mCondMenu[l] = new char[2];
        char str[] = "0";
        str[0] += l;
        strcpy (mCondMenu[l], str);
    }




// ### short view management
    mShortViewColNames = new char *[mRows];
    mShortViewStrings = new char *[mRows];
    for (k = 0; k < mRows; k++)
    {
        mShortViewColNames[k] = new char[30];
        mShortViewStrings[k] = new char[100];
        mShortViewColNames[k][0] = 0;
        mShortViewStrings[k][0] = 0;
    }
    mShortViewCols = 6;
    for (k = 0; k < 5; k++)
    {
        strcpy (mShortViewColNames[k], theColsList[k].c_str ());
    }

    mShortViewOrdresCol = 5;    
    strcpy (mShortViewColNames[mShortViewOrdresCol], "Ordres");
    
    mShortViewErrors.resize(mRows);
    for(k=0;k<mShortViewErrors.size();k++) {
        mShortViewErrors[k]=0;
    }




/// precalc some callbacks
    mShortModeCallbacks.resize(mRows);
    for(k=0;k<mShortModeCallbacks.size();k++) {
        mShortModeCallbacks[k].row = k;
        mShortModeCallbacks[k].source = this;
    }



}




void
MicrocodeTable::drawWidgets (ImDrawList * dl, ImVec2 window_pos)
{
    ImGui::SetCursorPos (window_pos);
    ImGui::PushStyleVar (ImGuiStyleVar_ChildBorderSize, 0);	// Ajustez la taille de la bordure ici
    ImGui::BeginChild ("Mon enfant", ImVec2 (560, 325), true);
    ImGui::PopStyleVar ();

    // Crée un onglet (tab) dans l'enfant
    if (ImGui::BeginTabBar ("Mon onglet"))

        if (ImGui::BeginTabItem ("Vue principale"))
        {
            // Contenu de l'onglet 1
            //ImGui::Text("Contenu de l'onglet 1");
            ImVec2 thePos = window_pos;
            thePos.y += 10;
            MainMicrocodeTableWidget (dl, thePos);
            ImGui::EndTabItem ();
        }
    {

        if (ImGui::BeginTabItem ("Vue synthétisée"))
        {
            ImVec2 thePos = window_pos;
            thePos.y += 10;
            // Contenu de l'onglet 2
            ShortMicroCodeTableWidget (dl, thePos);
            ImGui::EndTabItem ();
        }

        ImGui::EndTabBar ();
    }
    ImGui::EndChild ();

    // MainMicrocodeTableWidget(dl,window_pos);
}



void
MicrocodeTable::ShortMicroCodeTableWidget (ImDrawList * dl, ImVec2 window_pos)
{

    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;	// ImGuiTableFlags_Resizable
    static ImGuiTableColumnFlags column_flags =
        ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;

    static int frozen_cols = 1;
    static int frozen_rows = 2;
    if (ImGui::BeginTable ("table_angled_headers", mShortViewCols, table_flags,
                           ImVec2 (550, 275)))
    {
        //ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = 0; n < mShortViewCols; n++)
            ImGui::TableSetupColumn (mShortViewColNames[n], column_flags);
        ImGui::TableSetupScrollFreeze (frozen_cols, frozen_rows);

        ImGui::TableAngledHeadersRow ();	// Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow ();	// Draw remaining headers and allow access to context-menu and other functions.
        for (int row = 0; row < mRows; row++)
        {
            ImGui::PushID (row);
            ImGui::TableNextRow ();
            ImGui::TableSetColumnIndex (0);
            ImGui::AlignTextToFramePadding ();
            ImGui::Text ("%03d", row);
            for (int column = 1; column < mShortViewCols; column++)
                if (ImGui::TableSetColumnIndex (column))
                {
                    ImGui::PushID (column);
                    if (column == 1)
                    {
                        ImGui::PushItemWidth (11 * 3);
                        ImGui::InputText ("", mAdrSuiv[row], 4,
                                          ImGuiInputTextFlags_CharsDecimal);
                        ImGui::PopItemWidth ();
                    }
                    else if (column == 2)
                    {
                        ImGui::PushItemWidth (50);
                        ImGui::Combo ("", &mSeIMS[row], mSeIMSMenu, 4);
                        ImGui::PopItemWidth ();
                    }
                    else if(column == 3) {
                        ImGui::PushItemWidth (50);
                        ImGui::Combo ("", &mCond[row], mCondMenu, 7);
                        ImGui::PopItemWidth ();
                    }
                    else if (column == 5)
                    {    
                        if(mShortViewErrors[row]!=0) { 
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, gArchiTheme.mErrorColor); 
                            ImGui::PushItemWidth (300);
                            ImGui::InputText ("", mShortViewStrings[row], 100, ImGuiInputTextFlags_CallbackEdit, MicrocodeTable::shortTextSignalCallback,(void*)&(mShortModeCallbacks[row]));
                            ImGui::PopItemWidth ();
                            ImGui::PopStyleColor();
                        }
                        else {
                            ImGui::PushItemWidth (300);
                            ImGui::InputText ("", mShortViewStrings[row], 100, ImGuiInputTextFlags_CallbackEdit, MicrocodeTable::shortTextSignalCallback,(void*)&(mShortModeCallbacks[row]));
                            ImGui::PopItemWidth ();                           
                        }
                    }
                    else
                    {
                        ImGui::Checkbox ("", &(mSignals[row][column]));
                    }
                    ImGui::PopID ();
                }
            ImGui::PopID ();
        }
        ImGui::EndTable ();
    }
}


void
MicrocodeTable::MainMicrocodeTableWidget (ImDrawList * dl, ImVec2 window_pos)
{
    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;	// ImGuiTableFlags_Resizable
    static ImGuiTableColumnFlags column_flags =
        ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;

    static int frozen_cols = 1;
    static int frozen_rows = 2;
    if (ImGui::BeginTable ("table_angled_headers", mCols, table_flags,
                           ImVec2 (550, 275)))
    {
        //ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = 0; n < mCols; n++)
            ImGui::TableSetupColumn (mColNames[n], column_flags);
        ImGui::TableSetupScrollFreeze (frozen_cols, frozen_rows);

        ImGui::TableAngledHeadersRow ();	// Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow ();	// Draw remaining headers and allow access to context-menu and other functions.
        for (int row = 0; row < mRows; row++)
        {
            ImGui::PushID (row);
            ImGui::TableNextRow ();
            ImGui::TableSetColumnIndex (0);
            ImGui::AlignTextToFramePadding ();
            ImGui::Text ("%03d", row);
            for (int column = 1; column < mCols; column++)
                if (ImGui::TableSetColumnIndex (column))
                {
                    ImGui::PushID (column);
                    if (column == 1)
                    {
                        ImGui::PushItemWidth (11 * 3);
                        ImGui::InputText ("", mAdrSuiv[row], 4,
                                          ImGuiInputTextFlags_CharsDecimal);
                        ImGui::PopItemWidth ();
                    }
                    else if (column == 2)
                    {
                        ImGui::PushItemWidth (50);
                        ImGui::Combo ("", &mSeIMS[row], mSeIMSMenu, 4);
                        ImGui::PopItemWidth ();

                    }
                    else if(column == 3) {
                        ImGui::PushItemWidth (50);
                        ImGui::Combo ("", &mCond[row], mCondMenu, 7);
                        ImGui::PopItemWidth ();
                    }
                    else
                    {
                        if(ImGui::Checkbox ("", &(mSignals[row][column]))) {
                            signalsCallback(row,column,mSignals[row][column],this);
                        }
                    }
                    ImGui::PopID ();
                }
            ImGui::PopID ();
        }
        ImGui::EndTable ();
    }

}



 

std::string
MicrocodeTable::signalsToString(int row) {
    std::string s = "";
    for (int m = 5; m < mCols; m++)
        { if(mSignals[row][m]) 
            {s+= mColNames[m];
            s+=" ";
            }
        }
    return trim(s);
}

bool
MicrocodeTable::isOrderValid(std::string inOrder,bool includeMuxes) {

    std::map < std::string, int >* bdd = &sigToCol;
    if(!includeMuxes)  bdd = &sigToColNoMuxes;

    if (bdd->find (toUpper (inOrder)) == bdd->end ())
    {
        return false;
    }
    return true;
}



bool
MicrocodeTable::insertByExpression (std::string expr,bool updateShortView)
{

    int code, suiv, SeIMS,Cond;
    bool Fin;
    std::vector < std::string > orders;

    bool ret =
        matchMicrocodeExpression (expr, code, suiv, SeIMS, Cond, Fin, orders);
    if (ret == false || code > 500 || suiv > 500 || SeIMS > 3 || Cond >7)
        return false;

    for (int k = 0; k < orders.size (); k++)
    {
        if( isOrderValid(orders[k])==false) return false;
    }

    sprintf (mAdrSuiv[code], "%03d", suiv);
    mSeIMS[code] = SeIMS;
    mCond[code] = Cond;
    mSignals[code][sigToCol[toUpper ("Fin")]] = Fin;

    for (int m = 5; m < mCols; m++)
    {
        mSignals[code][m] = false;
    }
    
    for (int k = 0; k < orders.size (); k++)
    {
        mSignals[code][sigToCol[toUpper (orders[k])]] = true;
    }


    if(updateShortView==true) {

        strcpy(mShortViewStrings[code],signalsToString(code).c_str());
    }

    return true;

}






bool
MicrocodeTable::matchMicrocodeExpression (std::string s,
        int &code, int &suiv, int &SeIMS,
        int &Cond, bool &Fin,
        std::vector < std::string > &orders)
{
    std::vector < std::string > r;

    if (matchCommand (s, r))
    {

        code =  std::stoi (r[0]);
        suiv =  std::stoi (r[1]);
        SeIMS = std::stoi (r[2]);
        Cond =  std::stoi (r[3]) ;
        Fin =   std::stoi (r[4]) == 1 ? true : false;
        matchSignals (r[5], orders);
        return true;
    }
    return false;
}


bool
MicrocodeTable::matchCommand (std::string s, std::vector < std::string > &v)
{
    std::regex
    rgx
    ("^\\s*([0-9]+)\\s*[:/\\|]\\s*([0-9]+)\\s*[:/\\|]\\s*([0-9]+)\\s*[:/\\|]\\s*([0-9]+)\\s*[:/\\|]\\s*([0-9]+)\\s*[:/\\|]?\\s*(.*)\\s*");
    std::smatch match;

    if (std::regex_search (s.cbegin (), s.cend (), match, rgx))
    {
        for (int k = 1; k <= 6; k++)
        {
            v.push_back (match[k]);
        }
        return true;
    }
    return false;
}

bool
MicrocodeTable::matchSignals (const std::string input, std::vector < std::string > &result)
{
 return explode(input,result);
}

void 
MicrocodeTable::signalsCallback(int row,int column,bool p_open,MicrocodeTable* p){
strcpy(p->mShortViewStrings[row],p->signalsToString(row).c_str());

}

int 
MicrocodeTable::shortTextSignalCallback(ImGuiInputTextCallbackData *p){
    // get usefull data from callback
    shortMicrocodeTextCallbackStruct* myData = (shortMicrocodeTextCallbackStruct*) p->UserData;
    MicrocodeTable* M = myData->source;
    std::vector<std::string> orders, validOrders;

    // find space-separated strings 
    M->matchSignals( p->Buf ,orders);
 
    // match with orders keywords
    bool allFound = true;
    for (int k = 0; k < orders.size (); k++)
    {
        if(M->isOrderValid(orders[k],false)) {
            validOrders.push_back(orders[k]);
        }    
        else {
            allFound = false;
        }
    }
   if(allFound == true) {M->mShortViewErrors[myData->row] = 0;} else {M->mShortViewErrors[myData->row]=1;}
    

    // erase previously connected keywords
    for (int m = 5; m < M->mCols; m++)
    {
        M->mSignals[myData->row][m] = false;
    }
    
    // rewrite keywords
    for (int k = 0; k < validOrders.size (); k++)
    {
        M->mSignals[myData->row][M->sigToCol[toUpper (orders[k])]] = true;
    }

    return 0;

} 


shortMicrocodeTextCallbackStruct::shortMicrocodeTextCallbackStruct() {}