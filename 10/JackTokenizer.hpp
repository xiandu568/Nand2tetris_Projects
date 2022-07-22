// -------------------------------------------
// Author: xiandu568
// Date: 2022/07/22
// 
// 
// 




#ifndef __JACKTOKENIZER_HPP__
#define __JACKTOKENIZER_HPP__

#include <string>

#include <fstream>
#include<unordered_set>
using std::string;
using std::fstream;
using std::ios;
using std::unordered_set;


enum TOKENTYPE{MKEYWORD,MSYMBOL,MIDENTIFER,MINT_CONSTANT,MSTRING_CONSTANT,MEOF};

class JackTokenizer {
private:
    string inputSrc;
    int commandIndex;
    
    unordered_set<string>KEYWORD={
        "class",
        "constructor",
        "function",
        "method",
        "field",
        "static",
        "var",
        "int",
        "char",
        "boolean",
        "void",
        "true",
        "false",
        "null",
        "this",
        "let",
        "do",
        "else",
        "while",
        "return"
    };
    unordered_set<char>SYMBOL={
        '{',
        '}',
        '(',
        ')',
        '[',
        ']',
        '.',
        ',',
        ';',
        '+',
        '-',
        '*',
        '/',
        '&',
        '|',
        '<',
        '>',
        '=',
        '~'
    };

public:
    string curToken;
    bool hasMorToken;
    int lineNO;
    TOKENTYPE curTyepe;

public:
    
    JackTokenizer(string fileName){
        fstream file=fstream(fileName, ios::in);
        if(!file.is_open()){
            throw "File not found";
            exit(1);
        }
        
        string line;
        while(getline(file,line)){
            this->inputSrc+=line+"\n";
        }
        lineNO=1;      //行号  编译出错时 提醒
        commandIndex=0;  // 文件指针为0
        curToken="";
        curTyepe=MEOF;
        hasMorToken=true;
    }
    void printThis(){
        // 测试
        std::cout<<inputSrc;
    }
    void skipBlanks(){
        while(inputSrc[commandIndex]==' ' || inputSrc[commandIndex]=='\t') ++commandIndex;
    }
    void skipAline(){
        while(inputSrc[commandIndex]!='\n') ++commandIndex;
        ++commandIndex;
        ++lineNO;
    }

    void skipServalComment(){
        //多行注释
        while(commandIndex<inputSrc.size()-1){
            if(inputSrc[commandIndex]=='*' && inputSrc[commandIndex+1]=='/') break;
            if(inputSrc[commandIndex]=='\n') ++lineNO;
            ++commandIndex;
        }
        commandIndex+=2;
    }
    void handleConStr(){
        // 处理字符串的函数
      
        while(commandIndex<inputSrc.size()){
            if(inputSrc[commandIndex]=='\"') break;
            ++commandIndex;
        }
    }
    bool isAlpha(char& ch){
        return (ch<='z' && ch>='a') ||(ch>='A' && ch<='Z');
    }
    bool isNum(char& ch){
        return ch>='0' && ch<='9';
    }
    
    string getStrline(){
        string res;
        int index=lineNO;
        while(index>0){
            int d=index%10;
            res+=(char)(d+'0');
            index/=10;
        }
        for(int i=0;i<res.size()/2;++i){
            char temp=res[i];
            res[i]=res[res.size()-1-i];
            res[res.size()-1-i]=temp;
        }
        return res;
    }

    bool hasNextTokens(){
        return this->commandIndex<inputSrc.size();
    }

    void getNextTokens(){
        if(!hasNextTokens()){
            curToken="";
            curTyepe=MEOF;
            hasMorToken=false;
            return;
        }
        while(commandIndex<inputSrc.size()){
            // 跳过空格
            skipBlanks();
            char ch=inputSrc[commandIndex];
            if(ch=='\n'){
                ++lineNO;
                ++commandIndex;
                continue;
            }else if(ch=='/'){
                //3 种情况
                if(inputSrc[commandIndex+1]=='/'){
                    // 单行注释
                    skipAline();
                    continue;
                }else if(inputSrc[commandIndex+1]=='*' && inputSrc[commandIndex+2]=='*'){
                    // 多行注释
                    commandIndex+=3;
                    skipServalComment();
                    if(commandIndex==inputSrc.size()-1){
                        throw "comment need */  to end!";
                    }
                    continue;
                }else{
                    this->curToken="/";
                    this->curTyepe=MSYMBOL;
                    ++commandIndex;
                    break;
                }
            }else if(ch=='\"'){
                // 处理字符串
                commandIndex++;
                int start=commandIndex;
                handleConStr();
                // 没有找到字符串 报错
                if(commandIndex==inputSrc.size()) throw "string need singal of \" ";
                
                curToken=inputSrc.substr(start,commandIndex-start);
                curTyepe=MSTRING_CONSTANT;
                // 跳过后面的引号
                ++commandIndex;
                break;
            }
            else if(isalpha(ch) || ch=='_'){    
                //首字母是下划线 或者字母 肯定为变量名或者关键字
                int start=commandIndex;
                ++commandIndex;
                while(inputSrc[commandIndex]=='_' || isAlpha(inputSrc[commandIndex]) || isNum(inputSrc[commandIndex])){
                    ++commandIndex;
                }
                string id=inputSrc.substr(start,commandIndex-start);
                if(this->KEYWORD.find(id)!=KEYWORD.end()){
                    this->curToken=id;
                    this->curTyepe=MKEYWORD;
                }else{
                    this->curToken=id;
                    this->curTyepe=MIDENTIFER;
                }
                break;
            }else if(ch>='0' && ch<='9'){
                // 首字符数字
                int start=commandIndex;
                ++commandIndex;
                while(isNum(inputSrc[commandIndex])){
                    ++commandIndex;
                }
                string id=inputSrc.substr(start,commandIndex-start);
                int num=stoi(id);
                if(num<=32767){
                    this->curToken=id;
                    this->curTyepe=MINT_CONSTANT;
                }else{
                    this->curToken="";
                    this->curTyepe=MEOF; 
                }
                break;
            }else if(SYMBOL.find(ch)!=SYMBOL.end()){
                this->curToken=string("")+ch;
                this->curTyepe=MSYMBOL;
                ++commandIndex;
                break;
            }
            else{
                string errormsg="error at line "+getStrline()+" : "+ch;
                throw errormsg;
            }
        }
    }
};




#endif // __JACKTOKENIZER_HPP__