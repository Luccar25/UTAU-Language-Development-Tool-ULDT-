/*
 * Copyright © 2016-2017 Lucien Cartier-Tilet
 *
 * This file is part of the UTAU Language Development Tool (ULDT).
 *
 * The ULDT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The ULDT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.hpp"

using namespace std;

void openfolder(const char* folder){
    char *command;

#if (defined (__WIN32__) || defined (_WIN32)) && !defined (__MINGW32__)
    command = (char *) "explorer.exe ";
#else
    command = (char *) "xdg-open";
#endif

    char cmdcommand[100];
    strcpy(cmdcommand, command);
    strcpy(cmdcommand, folder);
    std::system(cmdcommand);
    return;
}

void load_phonemes(ifstream& in_consonants, ifstream& in_vowels){
    string line;
    while (getline(in_consonants, line)){
        if(line[0] != ' ' && line[0] != '#' && line != ""){
            consonants.push_back(line);
        }
    }

    while (getline(in_vowels, line)){
        if(line[0] != ' ' && line[0] != '#' && line != ""){
            vowels.push_back(line);
        }
    }

    in_consonants.close();
    in_vowels.close();
}

std::string get_preset_oto(const OtoType preset) noexcept {
    switch(preset){
        case OtoType::VV:
            return ",1000,200,1000,100,60";
        case OtoType::V:
            return ",100,200,100,60,60";
        case OtoType::CV:
            return ",1000,200,1000,30,60";
        case OtoType::VC:
            return ",1000,90,1000,60,60";
        case OtoType::CC:
            return ",1000,40,1000,20,10";
        case OtoType::CCV:
            return ",1000,200,1000,20,60";
        case OtoType::VCC:
            return ",1000,90,1000,15,30";
        case OtoType::STAT:
            return ",1000,60,1000,60,60";
    }
}

std::ofstream generateUST(const RecType recordingType, int& filecounter, int& notecounter) noexcept {
    string recType;
    switch(recordingType){
        case RecType::VV:
            recType = "VV";
            break;
        case RecType::V:
            recType = "V";
            break;
        case RecType::CV:
            recType = "CV";
            break;
        case RecType::CVVC:
            recType = "CVVC";
            break;
        case RecType::VC:
            recType = "VC";
            break;
        case RecType::VCV:
            recType = "VCV";
            break;
        case RecType::VCCV:
            recType = "VCCV";
            break;
        case RecType::STAT:
            recType = "Stat";
            break;
    }
    const string filename = recType + "_" + to_string(++filecounter) + "_debug.ust";
    notecounter = 3;
    ofstream file(filename);

    file << UST_HEADER[0] << recType << " debug" << UST_HEADER[1] << filename << UST_HEADER[2];

    return file;
}

std::ofstream generateUST(const RecType rectype, int& filecounter, int& notecounter, std::ofstream& oldfile) noexcept {
    closeUST(oldfile);
    return generateUST(rectype, filecounter, notecounter);
}

void closeUST(std::ofstream &file) noexcept {
    file << UST_FOOTER;
    file.close();
}

template<typename T>
string num_to_string(T&& num){
    if (num < 10){
        return "000" + to_string(num);
    } else if (num < 100) {
        return "00" + to_string(num);
    } else if (num < 1000) {
        return "0" + to_string(num);
    } else {
        return to_string(num);
    }
}

void writeNote(std::ofstream &ustFile, int notecounter, int &filecounter, const RecType rectype, const NoteLength length, const std::string lyrics = "R"){
    string notecount;
    if(notecounter > 950){
        ustFile = generateUST(rectype, filecounter, notecounter, ustFile);
    }

    notecount = num_to_string(notecounter);

    ustFile << NOTE_BODY[0] << notecount << NOTE_BODY[1] << length << NOTE_BODY[2] << lyrics;
    if(lyrics == "R"){
        ustFile << NOTE_BODY[4];
    } else {
        ustFile << NOTE_BODY[3];
    }
}

void writeAlias(std::ofstream& otoini, const std::string& filename, const std::string &alias, const OtoType otoType) noexcept {
    otoini << filename + OTO_BODY + alias + get_preset_oto(otoType) + "\n";
    return;
}

void writeRecOto(std::ofstream &reclist, std::ofstream &otoini, std::ofstream &ust, int &notecounter, int &filecounter, RecType recType, std::string &filename, std::vector<std::string> &aliases, OtoType otoType) noexcept {
    reclist << filename + "\n";

    for(string& alias : aliases){
        writeAlias(otoini, filename, alias, otoType);
        writeNote(ust, notecounter, filecounter, recType, NoteLength::full, alias);
    }



}
















