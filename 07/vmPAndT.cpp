#include<iostream>

#include "parser.hpp"

#include "VMCode.hpp"
#include "translater.hpp"
using namespace std;
void transLateAndWrit(string filename,parser& parser,Translater& translater){
    int j=filename.find('.');
    string name=filename.substr(0,j);
    name+=".asm";
    fstream file=fstream(name,ios::out);
    if(!file.is_open()){
        cout<<"open file error"<<endl;
        return;
    }
    parserElement* current=parser.mcurrent;
    while(current){
        VMCode code(current->mcommand);
        if(code.isPushOrPop()){
            file<<translater.tarnstlatePushOrPop(code.getCommand(),code.getSegmet(),code.getINdex());
        }
        else{
            file<<translater.tarnstlateArithmetic(code.getCommand());
        }
        current=current->mnext;
    }
    file.close();
}

int main(int argc,char* argv[]){
    if(argc !=2){
        cout<<"please input file name"<<endl;
        exit(1);
    }
    string fileName=argv[1];
    // string fileName="MemoryAccess/StaticTest/StaticTest.vm";
    // string fileName="MemoryAccess/BasicTest/BasicTest.vm";
    // string fileName="StackArithmetic/StackTest/StackTest.vm";
    parser pr(fileName);
    Translater translater(fileName);
    transLateAndWrit(fileName,pr,translater);

    return 0;
}