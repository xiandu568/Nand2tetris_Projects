#ifndef __PARSER_HPP__
#define __PARSER_HPP__
#include "parser_element.hpp"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;


class Parser {
public:
    Parser(std::string fileName){
        this->Header=new parser_element(string("elemt"));
        this->curINstruction=Header;
        std::fstream file=std::fstream(fileName,std::ios::in);
        if(!file.is_open()){
            std::cout<<"file open error"<<std::endl;
            exit(1);
        }
        // cout<<"file open success"<<endl;
        std::string line;
        while(getline(file,line)){
            if(line.empty() || line[0]=='/'&&line[1]=='/'){
                continue;
            }
            string str=clearSpace(line);
            if(str.empty()){
                continue;
            }
            CommonType type=getTheInstuctionType(str);
            curINstruction->mnext=new parser_element(str,type);
            curINstruction=curINstruction->mnext;
        }
        file.close();
        curINstruction=Header->mnext;
    }
    ~Parser(){
        curINstruction=nullptr;
        parser_element* tmp=nullptr;
        while(Header){
           tmp=Header->mnext;
           delete Header;
           Header=tmp;
        } 

    }

    string clearSpace(string& str){
        int i=0;
        while(str[i]==' '||str[i]=='\t'){
            i++;
        }
        if(str[i]=='\n' || str[i]=='/'){
            return "";
        }
        int j=i;
        for(;j<str.size();j++){
            if(str[j]==' '||str[j]=='\t'||str[j]=='\n'||str[j]=='/'){
                break;
            }
        }
        return str.substr(i,j-i);
    }
    string advance(){
        if(curINstruction==nullptr){
            return "";
        }
        curINstruction=curINstruction->mnext;
        return curINstruction->mcur;
    }
    void reset(){
        curINstruction=Header->mnext;
    }
    bool hasMore(){
        return curINstruction!=nullptr;
    }

    CommonType getTheInstuctionType(string& str){
        if(str=="") return No_INStruction;
        else if(str.find('@')!=string::npos) return Address;
        else if(str.find('(')!=string::npos) return Label;
        else return Calaulate;
    }
    string symbol(string& str){
        string ans;
        char target[5]={'@','(',')','/','\0'};
        for(char &ch:str){
            bool isOK=true;
            for(int i=0;i<4;++i){
                if(ch==target[i]){
                    isOK=false;
                    break;
                }
            }
            if(isOK)  ans.push_back(ch);
        }
        return ans;
    }
    vector<string> destAddrCompJump(string& str){
        vector<string> ans(3);
        int i=str.find('=');
        int j=str.find(';');
        if(i!=-1) ans[0]=str.substr(0,i);
        if(j!=-1) ans[2]=str.substr(j+1);
        //  dest jump 都存在
        if(i!=-1&&j!=-1) ans[2]=str.substr(i+1,j-i-1);
        // dest 不存在 jump 存在
        else if(i==-1 && j!=-1){
            ans[1]=str.substr(0,j);
          
        }
        // dest 存在 jump 不存在
        else if(i!=-1 && j==-1) ans[1]=str.substr(i+1);
        // dest 和 jump 都不存在
        else ans[1]=str;
        return ans;
    }
    
private:
    parser_element* Header;
public:   
    parser_element* curINstruction;    
};

#endif // __PARSER_HPP__