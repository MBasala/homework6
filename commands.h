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
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);
        char *cryptBuffer = (char*)malloc(sizeof(*cryptBuffer)*this->keyBufSize);
        size_t fileSize, currentPos, tail;
        ui rep;

        inputFile.seekg(0, std::ios::end);
        fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        outputFile.seekp(0, std::ios::beg);

        rep = (this->keyBufSize < fileSize)  ? fileSize/this->keyBufSize : this->keyBufSize/fileSize;
        tail = (rep*fileSize)%this->keyBufSize;

        if(keyBufSize <= fileSize) {
            for (si i = 0; i < rep && inputFile.good(); i++) {
                inputFile.read(buffer, this->keyBufSize);
                cryptBuffer = this->char_prt_xor_256_bytes(buffer, keyBuffer, this->keyBufSize);
                outputFile.write(cryptBuffer, this->keyBufSize);
            }
        }

        if(tail != 0) {
            inputFile.read(buffer, tail);
            std::cout << buffer;
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
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);
        size_t fileSize, currentPos, tail;
        ui rep;

        inputFile.seekg(0, std::ios::end);
        fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);
        outputFile.seekp(0, std::ios::beg);

        rep = (this->keyBufSize < fileSize)  ? fileSize/this->keyBufSize : this->keyBufSize/fileSize;
        tail = (rep*fileSize)%this->keyBufSize;


        if(keyBufSize < fileSize) {
            for (si i = 0; i < rep && inputFile.good(); i++) {
                inputFile.read(buffer, this->keyBufSize);
                outputFile.write(this->char_prt_xor_256_bytes(keyBuffer, buffer, this->keyBufSize), this->keyBufSize);
            }
        }

        //ToDo need to input part of the XOR
        if(tail != 0) {
            inputFile.read(buffer, tail);
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

        if(seedSize > this->keyBufSize){
            std::cout << "password needs to be shorter then 256 bytes\n";
            return "";
        }

        ui rep = this->keyBufSize/seedSize;
        size_t tail = this->keyBufSize-seedSize*rep;

        for(si i=0; i < rep; i++) {
            std::memcpy(key+(seedSize*i), keySeed_c_str, seedSize);
        }
        if(tail != 0){
            std::memcpy(key+(rep*seedSize), keySeed_c_str, tail);
        }

        std::string stringKey(key);
        delete[] key;

        return stringKey;
    }

    //Can't use int for this. The size of a int changes from 64bit to 32bit systems. 32bits to 16bits respectively.
    char *char_prt_xor_256_bytes(char *lhs, char *rhs, size_t charSize){
        u64 lhsu64 = 0, rhsu64 = 0, outu64=0;
        char *outCharPoint = (char*)malloc(256);
        for(int i=0;i<(256/sizeof(u64)); i++){
            outu64 = *reinterpret_cast<unsigned long long int *>(rhs+i*sizeof(u64)) ^ *reinterpret_cast<unsigned long long int *>(lhs+i*sizeof(u64));
            memcpy(outCharPoint+sizeof(u64)*i, (reinterpret_cast<char *>(&outu64)), sizeof(u64));
            //std::cout <<  "Full buff: " << outCharPoint << "\n" <<
            //         "location: " << outCharPoint+(sizeof(u64)*i) << "\n";
        }

        //std::cout << "End xorbuffer\n";
            //std::string charPointWrap(outCharPoint);
            //std::cout << "output stream: " << charPointWrap << "\n";
            /*if(charSize < 256) {
                char * truncReturn = (char *)malloc(charSize);
                memcpy(truncReturn, outCharPoint, charSize);
                delete[] outCharPoint;
                return truncReturn;
            }*/
        char* outPut = (char *)malloc(charSize);
        memcpy(outPut, outCharPoint, charSize);
        delete[] outCharPoint;
        return outPut;
    }

    si vaildater(std::filesystem::path og, std::filesystem::path dr);

    si cat(std::filesystem::path readPath){
        std::ifstream outStream(readPath.string(), std::ios::out);

        if(outStream.fail() || !outStream.is_open()){
            std::cout << "Failed opening file for reading :" << readPath << "\n";
        }

        std::string readBuffer;

        while(outStream.good()){
            std::getline(outStream, readBuffer);
            std::cout << readBuffer;
        }

        return 1;
    }


};
#endif //HOMEWORK6_COMMANDS_H
