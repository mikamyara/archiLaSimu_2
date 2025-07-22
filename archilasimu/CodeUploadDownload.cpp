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

#include "CodeUploadDownload.h"
#include "emscripten_browser_file.h"
#include "RAM.h"
#include <iostream>
#include <string>
#include <sstream>



void handle_upload_mic_file(std::string const &filename, std::string const &mime_type, std::string_view chaine, void* obj) {
  MicrocodeFiles* MF= static_cast<MicrocodeFiles*>(obj);

 size_t debutLigne = 0;
    size_t finLigne = chaine.find_first_of("\n");

    //int numeroLigne = 1;

    while (finLigne != std::string_view::npos) {
        std::string_view ligne = chaine.substr(debutLigne, finLigne - debutLigne);
        // Utilisez la variable "ligne" ici pour chaque ligne
        //std::cout << "Ligne " << numeroLigne << " : " << ligne << std::endl;
        MF->mTable->insertByExpression(std::string(ligne));

        debutLigne = finLigne + 1;
        finLigne = chaine.find_first_of("\n", debutLigne);

      //  numeroLigne++;
    }

    // Gérer la dernière ligne
    if (debutLigne < chaine.size()) {
        std::string_view derniereLigne = chaine.substr(debutLigne);
        MF->mTable->insertByExpression(std::string(derniereLigne));

        //std::cout << "Ligne " << numeroLigne << " : " << derniereLigne << std::endl;
    }

}



MicrocodeFiles::MicrocodeFiles() {}

void
MicrocodeFiles::Download() {
    std::string table = "";
    for(int k=0;k<mTable->mRows;k++) {
       table+= mTable->exportExpression(k) + "\n";
    }

  std::string filename{"archiLaSimu.mic"};
  std::string mime_type{"application/text/plain"};
  emscripten_browser_file::download(filename, mime_type, table);
}

void
MicrocodeFiles::Upload() {
  emscripten_browser_file::upload(".mic", handle_upload_mic_file,this);

}

///----------------------------------------------------------------------------------------------------------

void handle_upload_ram_file(std::string const &filename, std::string const &mime_type, std::string_view chaine, void* obj) {
  RamFiles* RF= static_cast<RamFiles*>(obj);

 size_t debutLigne = 0;
    size_t finLigne = chaine.find_first_of("\n");

    //int numeroLigne = 1;

    while (finLigne != std::string_view::npos) {
        std::string_view ligne = chaine.substr(debutLigne, finLigne - debutLigne);
        // Utilisez la variable "ligne" ici pour chaque ligne
        //std::cout << "Ligne " << numeroLigne << " : " << ligne << std::endl;
        RF->mRAM->insertByExpression(std::string(ligne));

        debutLigne = finLigne + 1;
        finLigne = chaine.find_first_of("\n", debutLigne);

      //  numeroLigne++;
    }

    // Gérer la dernière ligne
    if (debutLigne < chaine.size()) {
        std::string_view derniereLigne = chaine.substr(debutLigne);
        RF->mRAM->insertByExpression(std::string(derniereLigne));

        //std::cout << "Ligne " << numeroLigne << " : " << derniereLigne << std::endl;
    }

}



RamFiles::RamFiles() {

}

void 
RamFiles::Download() {
  std::string table = "";
  for(int k=0;k<mRAM->mRows;k++) {
      table+= std::to_string(k)+":"+std::string(mRAM->mData[k]) +":"+mRAM->mHelpers[k]+ "\n";
  }

  std::string filename{"archiLaSimu.ram"};
  std::string mime_type{"application/text/plain"};
  emscripten_browser_file::download(filename, mime_type, table);

}

void
RamFiles::Upload(){
  emscripten_browser_file::upload(".ram", handle_upload_ram_file,this);

}
