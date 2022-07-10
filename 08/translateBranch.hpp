#ifndef __TRANSLATER__HPP
#define __TRANSLATER__HPP

using namespace std;
class TranslateBranch{
public:
    TranslateBranch(){}
    ~TranslateBranch(){}
    std::string translateLabel(std::string label){
        return string("(")+label+")\n";
    }
    std::string translateGoto(std::string label){
        std::string res=string("@")+label+"\n";
        res+=string("0;JMP")+"\n";
        return res;
    }
    std::string translateIsGoto(std::string label){
        std::string res=string("@SP\n")+"AM=M-1\n"+"D=M\n"+"@"+label+"\n";
        res+=string("D;JNE")+"\n";
        return res;
    }
    std::string translate_Branch(std::string command,std::string label){
        if(command=="label") return translateLabel(label);
        else if(command=="goto") return translateGoto(label);
        else return translateIsGoto(label);
    }

};

#endif // __TRANSLATER__HPP