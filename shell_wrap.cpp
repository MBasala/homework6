//
// Created by fly0ut on 6/22/21.
//

#include <stdlib.h>
#include <iostream>
#include "commands.h"
#include "typeAlias.h"


class shellWrap{
private:

    std::string args, args2, password;
    si intArgs = -1;
    shell shellFuncs;
public:
    shellWrap(){
        this->mainLoop();
    }

private:
    si mainLoop(){
        while(intArgs != 0){
            std::cout << "0-Exit\n1-Select directory\n2-List directory content (first level)\n"
                         << "3-List directory content (all levels)\n4-Delete file\n5-Display file (hexadecimal view)\n"
                         << "6-Encrypt file (XOR with password)\n7-Decrypt file (XOR with password)\n: ";
            std::cin >> intArgs;
            switch (intArgs) {
                case 0:
                        std::cout << "Bye, bye!\n";
                        break;
                case 1:
                    std::cout << "Give director to select\n";
                    std::cin >> args;
                    this->shellFuncs.cd(args);
                    break;
                case 2:
                    this->shellFuncs.ls();
                    break;
                case 3:
                    this->shellFuncs.ls_r();
                    break;
                case 4:
                        std::cout << "Give file to remove\n";
                        std::cin >> args;
                        std::cout << "\n";
                        this->shellFuncs.rm(args);
                    break;
                case 5:
                    std::cout << "Give file to hexdump\n";
                    std::cin >> args;
                    std::cout << "\n";
                    this->shellFuncs.hexdump(args);
                    break;
                case 6:
                    std::cout << "Give file to encrypt.\n";
                    //std::cin >> this->args;
                    std::cout << "\n";
                    std::cout << "Give file to output to.\n";
                    //std::cin >> this->args2;
                    std::cout << "\n";
                    std::cout << "Enter password.\n";
                    //std::cin >> this->password;
                    std::cout << "\n";
                    args = "/home/fly0ut/Desktop/testingBig";
                    args2 = "/home/fly0ut/Desktop/testingBig2";
                    password = "d2d032-i90efw09ufa";
                    this->shellFuncs.xorCrypt(args, args2, password);
                    break;
                case 7:
                    std::cout << "Give file to encrypt.\n";
                    //std::cin >> this->args;
                    std::cout << "\n";
                    std::cout << "Give file to output to.\n";
                    //std::cin >> this->args2;
                    std::cout << "\n";
                    std::cout << "Enter password.\n";
                    //std::cin >> this->password;
                    std::cout << "\n";
                    args = "/home/fly0ut/Desktop/testingBig2";
                    args2 = "/home/fly0ut/Desktop/testingBigDe";
                    password = "d2d032-i90efw09ufa";
                    this->shellFuncs.xorDecrypt(args, args2, password);
                    break;
                case 1337:
                    std::cout << "Running demo\n";
                    this->demoHomework6();
                    std::cout << "Demo done\n";
                default:
                    std::cout << "Error, not a valid command\n";
            }
            args.empty();
            args2.empty();
            password.empty();
        }
        return 0;
    }

    si demoHomework6(){

        args = "/home/fly0ut/Desktop/testing";
        args2 = "/home/fly0ut/Desktop/testing2";
        password = "4v5525";
        this->shellFuncs.xorCrypt(args, args2, password);

        args = "/home/fly0ut/Desktop/testing2";
        args2 = "/home/fly0ut/Desktop/testingDe";
        password = "4v5525";
        this->shellFuncs.xorDecrypt(args, args2, password);

        args = "/home/fly0ut/Desktop/testingBig";
        args2 = "/home/fly0ut/Desktop/testingBig2";
        password = "d2d032-i90efw09ufa";
        this->shellFuncs.xorCrypt(args, args2, password);

        args = "/home/fly0ut/Desktop/testingBig2";
        args2 = "/home/fly0ut/Desktop/testingBigDe";
        password = "d2d032-i90efw09ufa";
        this->shellFuncs.xorDecrypt(args, args2, password);
        return 0;
    }

};