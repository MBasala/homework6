//
// Created by fly0ut on 6/22/21.
//

#ifndef HOMEWORK6_COMMANDS_H
#define HOMEWORK6_COMMANDS_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstring>
#include "typeAlias.h"

class shell {

private:
    const static size_t keyBufSize = 256;

    std::filesystem::path wd;

public:

    shell()= default;;

    si cd(std::string newPath) {
        std::filesystem::path dirPath(newPath);

        if (!std::filesystem::is_directory(dirPath)) {
            std::cout << "Not a valid path: " << newPath << "\n";
            return 1;
        }

        this->wd = dirPath;

        return 0;
    }

    int ls() {
        for(auto& e : std::filesystem::directory_iterator(wd)) {
            std::cout << e.path() << "\n";
        }

        return 0;
    }

    int ls_r() {
        for(auto& e : std::filesystem::recursive_directory_iterator(wd)) {
            std::cout << e.path() << "\n";
        }
        return 0;
    }

    int rm(std::string fileToRemove){
        std::filesystem::path file(fileToRemove);

        if(!is_regular_file(file)){
            std::cout << "Error finding file: " << file << "\n";
            return 1;
        }
        std::filesystem::remove(file);

        return 1;
    }

    int hexdump(std::filesystem::path inputPath){
        std::ifstream inputStream(inputPath.string(), std::ios::in | std::ios::binary);
        if(!inputStream || !inputStream.is_open()){
            std::cout << "Error opening input file: " << inputPath;
            return 1;
        }

        std::cout << "Hexdump of file " << inputPath << "\n";

        std::string buffer;

        while(inputStream.good()){
            std::getline(inputStream, buffer);
            std::cout << std::hex << buffer;
        }

        std::cout << "End of hexdump\n";

        return 0;
    };

    int xorCrypt(std::filesystem::path inputPath, std::filesystem::path outputPath, std::string keySeed){
        std::ifstream inputFile(inputPath.string(), std::ios::in | std::ios::binary);
        std::ofstream outputFile(outputPath.string(), std::ios::out | std::ios::binary | std::ios::trunc);

        if(!inputFile && !inputFile.is_open()) {
            std::cout << "Error opening input file: " << inputPath << "\n";
            return 1;
        }

        if(!outputFile && !outputFile.is_open()) {
            std::cout << "Error opening output file: " << outputPath << "\n";
            return 1;
        }

        std::string  keyString = (this->genKey(keySeed));
        //std::cout << "keyString: "<< keyString << "\n" << "keyString size: " << keyString.size() << "\n" << "keyString c string: " << keyString.c_str() << "\n";
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        //std::cout << "keyBuffer: " << keyBuffer << "\n";
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);
        //char *tailBuff;
        //char *tailCryptBuff;
        char *cryptBuffer = (char*)malloc(sizeof(*cryptBuffer)*this->keyBufSize);
        size_t fileSize, currentPos, tail;
        ui rep;

        inputFile.seekg(0, std::ios::end);
        fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        outputFile.seekp(0, std::ios::beg);

        rep = (this->keyBufSize < fileSize)  ? fileSize/this->keyBufSize : this->keyBufSize/fileSize;

        tail = (this->keyBufSize < fileSize) ? fileSize%rep*this->keyBufSize :  this->keyBufSize%rep*fileSize;


        if(keyBufSize < fileSize) {
            for (si i = 0; i < rep && !inputFile.eof() && currentPos < fileSize; i++) {
                //currentPos = inputFile.tellg();
                inputFile.read(buffer, this->keyBufSize);
                //cryptBuffer = this->char_prt_xor_256_bytes(buffer, keyBuffer, this->keyBufSize);
                //cryptBuffer = keyBuffer;
                //outputFile.seekp(currentPos, std::ios::beg);
                outputFile.write(this->char_prt_xor_256_bytes(buffer, keyBuffer, this->keyBufSize), this->keyBufSize);
                //inputFile.seekg(currentPos+this->keyBufSize, std::ios::beg);
            }
        }

        //ToDo need to input part of the XOR
        if(tail != 0) {
            inputFile.read(buffer, tail);
            //cryptBuffer = this->char_prt_xor_256_bytes(buffer, keyBuffer, tail);
            std::cout << "crypt buff: " << cryptBuffer << "\n";
            outputFile.write(this->char_prt_xor_256_bytes(buffer, keyBuffer, tail), tail);
        }

        inputFile.close();
        outputFile.close();

        delete[] keyBuffer;
        delete[] buffer;

        return 0;
    };

    int xorDecrypt(std::filesystem::path inputPath, std::filesystem::path outputPath, std::string keySeed) {
        std::ifstream inputFile(inputPath.string(), std::ios::in | std::ios::binary);
        std::ofstream outputFile(outputPath.string(), std::ios::out | std::ios::binary | std::ios::trunc);

        if(!inputFile && !inputFile.is_open()) {
            std::cout << "Error opening input file: " << inputPath << "\n";
            return 1;
        }

        if(!outputFile && !outputFile.is_open()) {
            std::cout << "Error opening output file: " << outputPath << "\n";
            return 1;
        }

        std::string  keyString = (this->genKey(keySeed));
        //std::cout << "keyString: "<< keyString << "\n" << "keyString size: " << keyString.size() << "\n" << "keyString c string: " << keyString.c_str() << "\n";
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        //std::cout << "keyBuffer: " << keyBuffer << "\n";
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);
        //char *tailBuff;
        //char *tailCryptBuff;
        char *cryptBuffer = (char*)malloc(sizeof(*cryptBuffer)*this->keyBufSize);
        size_t fileSize, currentPos, tail;
        ui rep;

        inputFile.seekg(0, std::ios::end);
        fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        outputFile.seekp(0, std::ios::beg);

        rep = (this->keyBufSize < fileSize)  ? fileSize/this->keyBufSize : this->keyBufSize/fileSize;

        tail = (this->keyBufSize < fileSize) ? fileSize%rep*this->keyBufSize :  this->keyBufSize%rep*fileSize;


        if(keyBufSize < fileSize) {
            for (si i = 0; i < rep && !inputFile.eof() && currentPos < fileSize; i++) {
                //currentPos = inputFile.tellg();
                inputFile.read(buffer, this->keyBufSize);
                //cryptBuffer = this->char_prt_xor_256_bytes(buffer, keyBuffer, this->keyBufSize);
                //cryptBuffer = keyBuffer;
                //outputFile.seekp(currentPos, std::ios::beg);
                outputFile.write(this->char_prt_xor_256_bytes(keyBuffer, buffer, this->keyBufSize), this->keyBufSize);
                //inputFile.seekg(currentPos+this->keyBufSize, std::ios::beg);
            }
        }

        //ToDo need to input part of the XOR
        if(tail != 0) {
            inputFile.read(buffer, tail);
            //cryptBuffer = this->char_prt_xor_256_bytes(buffer, keyBuffer, tail);
            std::cout << "crypt buff: " << cryptBuffer << "\n";
            outputFile.write(this->char_prt_xor_256_bytes(keyBuffer, buffer, tail), tail);
        }

        inputFile.close();
        outputFile.close();

        delete[] keyBuffer;
        delete[] buffer;

        return 0;
    };

    std::string genKey(std::string keySeed) {
        const char *keySeed_c_str=keySeed.c_str();
        char *key = (char*)malloc(sizeof(*key)*this->keyBufSize);

        size_t seedSize= keySeed.size();

        ui rep = this->keyBufSize / seedSize;
        size_t tail = this->keyBufSize%(seedSize*rep);

        for(si i=0; i < rep; i++) {
            std::memcpy(key+(seedSize*i), keySeed_c_str, seedSize);
        }

        if(tail != 0){
            std::memcpy(key+(rep*seedSize), keySeed_c_str, tail);
        }

        //std::cout << "Key seed: " <<keySeed << ", key c string : " << keySeed_c_str << ", key : " << key << "\n";

        std::string stringKey(key);

        delete[] key;

        return stringKey;
    }

    //Can't use int for this. The size of a int changes from 64bit to 32bit systems. 32bits to 16bits respectively.
    char *char_prt_xor_256_bytes(char *lhs, char *rhs, size_t charSize){
        u64 lhsu64 = 0, rhsu64 = 0, outu64=0;
        //Char is 1 byte so no converstion is needed for sizeof.
        char *outCharPoint = (char*)malloc(256);
        char *xorCharPoint = (char*)malloc(256);
        char *rhTempDeBug = (char*)malloc(sizeof (u64));
        char *lhTempDeBug = (char*)malloc(sizeof(u64));
        char *xorTempDeBug = (char *)malloc(sizeof(u64));
        //Logic for the 32 is that a long long is 8 bytes and 256/4 is 32.
        std::cout << "output xorbuffer: \n";
        for(int i=0;i<(256/sizeof(u64)); i++){
            memcpy(&lhsu64, lhs+i*sizeof(u64), sizeof(u64));
            memcpy(lhTempDeBug, lhs+i*sizeof(u64), sizeof(u64));
            //lhsu64 = *reinterpret_cast<unsigned long long int *>(xorCharPoint);
            memcpy(&rhsu64, rhs+i*sizeof(u64), sizeof(u64));
            memcpy(rhTempDeBug, rhs+i*sizeof(u64), sizeof(u64));
            //rhsu64 = *reinterpret_cast<unsigned long long int *>(xorCharPoint);
            outu64 = lhsu64 ^ rhsu64;
            xorCharPoint = (reinterpret_cast<char *>(&outu64));
            memcpy(outCharPoint+sizeof(u64)*i, (reinterpret_cast<char *>(&outu64)), sizeof(u64));
            //memcpy(xorTempDeBug, &outu64, sizeof(u64));
            memcpy(outCharPoint+sizeof(u64)*i, xorCharPoint, sizeof(u64));
            std::cout << "temp buff: " << xorCharPoint << "\n"  << "temp long: " << xorTempDeBug << "\n" << "Full buff: " << outCharPoint << "\n" <<
                      "location: " << outCharPoint+(sizeof(u64)*i) << "\n";
        }

        std::cout << "End xorbuffer\n";
            std::string charPointWrap(outCharPoint);
            std::cout << "output stream: " << charPointWrap << "\n";

            std::cout << "full: " << outCharPoint << "\n";
            if(charSize < 256) {
                char * truncReturn = (char *)malloc(256);
                memcpy(truncReturn, outCharPoint, charSize);
                std::cout << "truc: " << truncReturn << "\n";
                delete[] outCharPoint;
                //delete[] truncReturn;
                //delete[] xorCharPoint;
                return truncReturn;
            }
            char* outPut = strdup(outCharPoint);
        delete[] outCharPoint;
        delete[] xorCharPoint;
        return outPut;
    }

};
#endif //HOMEWORK6_COMMANDS_H
