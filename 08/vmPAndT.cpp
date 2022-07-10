#include<iostream>
#include<sys/stat.h>
#include<io.h>
#include "parser.hpp"
#include "translateBranch.hpp"
#include "VMCode.hpp"
#include "translater.hpp"
#include "translateFunction.hpp"
using namespace std;

void getFiles(std::string path, std::vector<std::string>& files)
{
    // 该函数主要获取文件夹下面所有文件
	intptr_t   hFile = 0;//intptr_t和uintptr_t的类型:typedef long int； typedef unsigned long int
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)//assign方法：先将原字符串清空，然后赋予新的值作替换。
	{
		do
		{
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void transLateAndWrit(string filename,Translater& translater,TranslateBranch& TranslateBranch,TranslateFunction& translateFunction){
    parser parser(filename);
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
        else if(code.isBranch()){

            file<<TranslateBranch.translate_Branch(code.getCommand(),code.getLabel());

        }else if(code.isFunction()){

            if(code.isReturn()) file<<translateFunction.translateFun_Return();
            else if(code.isCall()) file<<translateFunction.translateFun_Call(code.getFunName(),code.getINdex());
            else file<<translateFunction.translateFun_Defination(code.getFunName(),code.getINdex());

        }
        else{
            file<<translater.tarnstlateArithmetic(code.getCommand());
        }
        current=current->mnext;
    }
    file.close();
}
void transLateDirAndW(vector<string>& paths,string fileName ,Translater& translater,TranslateBranch& TranslateBranch,TranslateFunction& translateFunction){
    int j=fileName.rfind('/');
    string name =fileName+"/"+fileName.substr(j+1)+".asm";

    // 这里主要是清空文件，如果不清空，会有重复内容
    fstream file=fstream(name,ios::out);
    if(!file.is_open()){
        cout<<"open file error"<<endl;
        return;
    }
    file.close();
    fstream file2=fstream(name,ios::app | ios::out);
    if(!file2.is_open()){
        cout<<"open file error"<<endl;
        return;
    }

    file2<<translateFunction.sysInit();
    for(string path:paths){
        int j=path.rfind('.');
        if(path.substr(j+1)!="vm") continue;
        parser pr(path);
        parserElement* current=pr.mcurrent;
        while(current){
            VMCode code(current->mcommand);
            if(code.isPushOrPop()){
                file2<<translater.tarnstlatePushOrPop(code.getCommand(),code.getSegmet(),code.getINdex());
            }
            else if(code.isBranch()){

                file2<<TranslateBranch.translate_Branch(code.getCommand(),code.getLabel());

            }else if(code.isFunction()){

                if(code.isReturn()) file2<<translateFunction.translateFun_Return();
                else if(code.isCall()) file2<<translateFunction.translateFun_Call(code.getFunName(),code.getINdex());
                else file2<<translateFunction.translateFun_Defination(code.getFunName(),code.getINdex());

            }
            else{
                file2<<translater.tarnstlateArithmetic(code.getCommand());
            }
            current=current->mnext;
        }
        
    }
    file2.close();
   
}


int main(int argc,char* argv[]){
    if(argc !=2){
        cout<<"please input file name"<<endl;
        exit(1);
    }
    string fileName=argv[1];
    // string fileName="FunctionCalls/FibonacciElement";
    // string fileName="FunctionCalls/NestedCall/NestedCall.vm";
    // string fileName="ProgramFlow/BasicLoop/BasicLoop.vm";
    // 调用器 和翻译器
    vector<string> files;
    struct stat st;
    // 初始化3个解释器
    Translater translater(fileName);
    TranslateBranch translaterBranch;
    TranslateFunction translateFunction;
    if(stat(fileName.c_str(),&st)==-1){
        cout<<"file not exist"<<endl;
        exit(1);
    }else{
       if(st.st_mode & S_IFDIR){
            getFiles(fileName,files);
            transLateDirAndW(files,fileName,translater,translaterBranch,translateFunction);
            
       }
       else if(st.st_mode & S_IFREG){
            transLateAndWrit(fileName,translater,translaterBranch,translateFunction);
       } 
       else cout<<"is other"<<endl;
    }
    
    return 0;
}