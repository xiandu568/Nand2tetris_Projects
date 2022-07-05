#ifndef _SYMBOL_TABLE_HPP_
#define _SYMBOL_TABLE_HPP_
#include<string>
#include<unordered_map>
#define START_SYMBOL_ADD 16

class symbolTable{
private:
    std::unordered_map<std::string,int> symTable;
    int _nextAddress;
public:
    symbolTable(){
        symTable["SP"]=0;
        symTable["LCL"]=1;
        symTable["ARG"]=2;
        symTable["THIS"]=3;
        symTable["THAT"]=4;
        symTable["R0"]=0;
        symTable["R1"]=1;
        symTable["R2"]=2;
        symTable["R3"]=3;
        symTable["R4"]=4;
        symTable["R5"]=5;
        symTable["R6"]=6;
        symTable["R7"]=7;
        symTable["R8"]=8;
        symTable["R9"]=9;
        symTable["R10"]=10;
        symTable["R11"]=11;
        symTable["R12"]=12;
        symTable["R13"]=13;
        symTable["R14"]=14;
        symTable["R15"]=15;
        symTable["SCREEN"]=16384;
        symTable["KBD"]=24576;
        _nextAddress=START_SYMBOL_ADD;
    }
    ~symbolTable(){
        symTable.clear();
    }


    bool isExist(std::string name){
        return symTable.find(name)!=symTable.end();
    }

    int getAddress(std::string name){
        return symTable[name];
    }
    void AddEntry(std::string name){
        if(isExist(name)){
            exit(1);
        }
        symTable[name]=_nextAddress++;
    }
   
};

#endif // _SYMBOL_TABLE_HPP_