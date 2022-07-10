#ifndef ___VMCODE__HPP
#define ___VMCODE__HPP
#include "string"
#include "vector"

class VMCode{
public:
    VMCode(std::string rawText){
        auto split = [](std::string s,char c){
            s.push_back(c);
            std::vector<std::string> v;
            std::string tmp;
            for(int i=0;i<s.size();i++){
                if(s[i]==c){
                    if(tmp!="") v.push_back(tmp);
                    tmp="";
                }
                else{
                    tmp.push_back(s[i]);
                }
            }
            return v;
        };
        mcode=split(rawText,' ');
    }
    // 析构函数
    ~VMCode(){}
    // 
    // 函数指令


    // 程序控制流 指令
    bool isLabel(){
        return mcode[0]=="label";
    }
    bool isGoto(){
        return mcode[0]=="goto" || mcode[0]=="if-goto";
    }
    bool isBranch(){
        return isGoto() || isLabel();
    }   // 是否是跳转指令
    bool isFunction(){
        return mcode[0]=="function" || mcode[0]=="call" || mcode[0]=="return";
    }
    bool isReturn(){
        return mcode[0]=="return";
    }
    bool isCall(){
        return mcode[0]=="call";
    }
    bool isPushOrPop(){
        return mcode[0]=="push" || mcode[0]=="pop";
    }
    std::string getCommand(){
        return mcode[0];
    }
    std::string getSegmet(){
        if(mcode.size()!=3) return "";
        return mcode[1];
    }
    std::string getFunName(){
        if(mcode.size()!=3) return "";
        return mcode[1];
    }
    std:: string getINdex(){
        if(mcode.size()!=3) return "";
        return mcode[2];
    }
    std::string getLabel(){
        if(mcode.size()!=2) return "";
        return mcode[1];
    }



public:
    std::vector<std::string> mcode;
};



#endif // ___VMCODE__HPP