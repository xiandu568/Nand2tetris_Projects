#include<iostream>
#include<fstream>
#include "symbolTable.hpp"
#include "parser.hpp"
#include "decode.hpp"

using namespace std;
bool isNum(string& str){
    for(char ch:str){
        if(ch<'0'||ch>'9') return false;
    }
    return true;
}
void parseForSymbol(Parser &parser, symbolTable &symTable){
    parser_element* cur=parser.curINstruction;
    while(cur){
        if(cur->mtype==Label){
            symTable.AddEntry(cur->mcur);
        }
        cur=cur->mnext;
    }
}

void translateCode(Parser& parser,symbolTable& symTable, decodeASS &decoder,string filename){
    // 重置curINstruction 
    parser.reset();
    string outputFile =filename.substr(0,filename.find_last_of('.'))+".hack";
    fstream output(outputFile,ios::out);
    parser_element* cur=parser.curINstruction;
    while(cur){
        string machineCode;
        if(cur->mtype==No_INStruction){
            
        }
        else if(cur->mtype==Address){
            string curinst=cur->mcur.substr(1);
            if(isNum(curinst)){
                machineCode=decoder.demicalToBinStr(stoi(curinst));
            }
            else{
                if(symTable.isExist(curinst)){
                    machineCode=decoder.demicalToBinStr(symTable.getAddress(curinst));
                }
                else{
                    symTable.AddEntry(curinst);
                    machineCode=decoder.demicalToBinStr(symTable.getAddress(curinst));
                }
            }
        }
        else if(cur->mtype==Calaulate){
            vector<string> tokens=parser.destAddrCompJump(cur->mcur);
            machineCode=string("111");
            machineCode+=decoder.COMP_TO_BITS[tokens[1]];
            if(tokens[0]=="") machineCode+="000";
            else machineCode+=decoder.DEST_TO_BITS[tokens[0]];

            if(tokens[2]=="") machineCode+="000";
            else machineCode+=decoder.JUMP_TO_BITS[tokens[2]]; 
        }
        if(machineCode.size()>0)    output<<machineCode<<endl;
        cur=cur->mnext;
    }
}

int main(int argv, char** argc){
    if(argv!=2){
        cout<<"usage: ./asm filename"<<endl;
        exit(1);
    }

    string filename=argc[1];
    Parser parser(filename);
    symbolTable symTable;
    decodeASS decoder;
    parseForSymbol(parser,symTable);
    translateCode(parser,symTable,decoder,filename);
    return 0;
}