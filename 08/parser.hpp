#ifndef __PARSER__HPP
#define __PARSER__HPP
#include "parserElement.hpp"
#include<fstream>
#include<iostream>
class parser{
public:
    parser(std::string FileName){
        mhead =new parserElement("");
        mcurrent=mhead;
        // 打开指令文件 读取代码 存入链表
        std::fstream file=std::fstream(FileName,std::ios::in);
        if(!file.is_open()){
            std::cout<<"open file error"<<std::endl;
            return;
        }
        std::string line;
        while(getline(file,line)){
            hadleNotes(line);
            if(line=="" || checkIsAllSpace(line)){
                continue;
            }
            // std::cout<<line<<std::endl;
            strip(line);
            mcurrent->mnext=new parserElement(line);
            mcurrent=mcurrent->mnext;
        }
        file.close();
        mcurrent=mhead->mnext;
    }
    ~parser(){
        mcurrent=mhead->mnext;
        while(mcurrent){
            delete mhead;
            mhead=mcurrent;
            mcurrent=mcurrent->mnext;
        }
        delete mhead;
        mcurrent=nullptr;
        mhead=nullptr;
    }
    void hadleNotes(std::string& s){
        int j=s.find('/');
        if(j!=-1) s=s.substr(0,j);
    }
    bool checkIsAllSpace(std::string& s){
        for(int i=0;i<s.size();i++){
            if(s[i]!=' ') return false;
        }
        return true;
    }
    void strip(std::string& s){
        int i=0,j=s.size()-1;
        while(s[i]==' '){++i;}
        while(s[j]==' '){--j;}
        s=s.substr(i,j-i+1);
    }

private:
    parserElement* mhead;
public:
    parserElement* mcurrent;

};





#endif // __PARSER__HPP