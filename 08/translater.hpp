#ifndef __Translater__HPP
#define __TRANSLATER__HPP
#include<unordered_map>
#include<string>
using namespace std;
// 翻译虚拟内存操作 和算数逻辑操作
class Translater{
public:
          //    构造函数
    Translater(std::string FileName){
        int j=FileName.rfind('/');
        string fileName=FileName.substr(j+1);
        for(char& ch:fileName){
            if(ch=='/') ch='.';
        }
        mFileName=fileName;
    }


    string loadValueToDInstruction(string& value){
        return string("@")+value+"\n"+"D=A\n";
    }

    string loadDToMInstruction(string segment,string index){
        if(segment=="constant") return string("@SP\n")+"A=M\n"+"M=D\n";
        else if(segment=="local" ||segment=="argument" || segment=="this" || segment=="that"){
            // 首先取得段 目标地值
            std::string res="@"+VIRTUAL_MEMORY_TABLE[segment]+"\n";
            // 短地址值->A
            res+="A=M\n";
            // 目标地址= 段地址+索引值
            res+="A=D+A\n";
            // 目标地址值 ——> D 寄存器
            res+="D=M\n";
            // 取栈指针
            res+="@SP\n";
            // 取栈指针的值
            res+="A=M\n";
            // 入栈
            res+="M=D\n";
            return res;
        }else if(segment=="pointer" || segment=="temp"){
            return "@"+VIRTUAL_MEMORY_TABLE_2[segment]+"\n"+"A=D+A\n"+"D=M\n"+"@SP\n"+"A=M\n"+"M=D\n";
        }else{ //static
            return "@"+mFileName+"."+index+"\n"+"D=M\n"+"@SP\n"+"A=M\n"+"M=D\n";
        }
    }
    string stackPointerAddInstruction(){
        // 栈指针加一
        return string("@SP\n")+"M=M+1\n";
    }
    string stackPointerSubInstruction(){
        //  栈指针-1
        return string("@SP\n")+"M=M-1\n";
    }
    string loadStackPointerToDInstruction_saveToR13(){
        //  将栈顶的值加载到D中  
        // 实现了栈指针减一的操作
        return string("@SP\n")+"AM=M-1\n"+"D=M\n"+"@R13\n"+"M=D\n";
    }
    string loadR13ToMInstruction(string& segment,string& index){
        //  将R13的值加载到M中
        if(segment=="local" ||segment=="argument" || segment=="this" || segment=="that"){
            // 首先取得目标地值
            std::string res="@"+VIRTUAL_MEMORY_TABLE[segment]+"\n";
            res+="D=D+M\n";
            // 借用VM 通用寄存器来实验V R14
            res+="@R14\n";
            res+="M=D\n";
            // 再取得目标地值的偏移量
            res+="@R13\n";
            res+="D=M\n";
            res+="@R14\n";
            res+="A=M\n";
            res+="M=D\n";
            return res;
        }else if(segment=="pointer"||segment=="temp")
            return "@"+VIRTUAL_MEMORY_TABLE_2[segment]+"\n"+"D=D+A\n"+"@R14\n"+"M=D\n"+"@R13\n"+"D=M\n"+"@R14\n"+"A=M\n"+"M=D\n";
        else{
            // static
            return "@"+mFileName+"."+index+"\n"+"D=A\n"+"@R14\n"+"M=D\n" +"@R13\n"+"D=M\n"+"@R14\n"+"A=M\n"+"M=D\n";
        }
    }
    // 解析 入栈 和出栈操作
    string tarnstlatePushOrPop(string&& command,string&& segment,string&& index){
        if(command=="push"){
            return loadValueToDInstruction(index)+loadDToMInstruction(segment,index)+stackPointerAddInstruction();
        }else{
            return loadStackPointerToDInstruction_saveToR13()+loadValueToDInstruction(index) +loadR13ToMInstruction(segment,index);
        }
    }
    // 解析算术操作
    string tarnstlateArithmetic(string command){
        string res;
        if(command=="add" || command=="sub"  || command=="and" || command=="or"){
            // 取栈指针
            res="@SP\n";
            // 栈指针减一
            res+="AM=M-1\n";
            // 取栈顶的值
            res+="D=M\n";
            // 取栈指针
            res+="@SP\n";
            // 栈指针减一
            res+="AM=M-1\n";
            //  算数操作
            res+=OPEARTION_TABLE[command]+"\n";  
            // 取栈指针
            res+="@SP\n";
            // 栈指针加一
            res+="M=M+1\n";
        }else if(command=="neg" || command=="not"){
            // 取栈指针
            res="@SP\n";
            // 栈指针减一
            res+="AM=M-1\n";
            // 算数操作
            res+=OPEARTION_TABLE[command]+"\n";
            // 取栈指针
            res+="@SP\n";
            // 栈指针加一
            res+="M=M+1\n";
            
        }else{
            //  EQ LT GT
            // 取栈指针
            res="@SP\n";
            // 栈指针减一
            res=res+"AM=M-1\n"+"D=M\n";
            // 取栈指针
            res+="@SP\n";
            // 栈指针减一
            res+="AM=M-1\n";
            //  算数操作
            res+="D=M-D\n";
            // 取分支标签
            res+="@"+COMP_COMMAND_TABLE[command]+"_"+to_string(COMP_TIMES[command])+"\n";
            // 比较跳转
            res+="D;"+COMP_COMMAND_TABLE[command]+"\n";
            // 加载栈指针
            res+="@SP\n";
            // 加入栈指针值
            res+="A=M\nM=0\n";
            // load inc stack pointer
            res+="@INC_STACK_POINTER_"+COMP_COMMAND_TABLE[command]+"_"+to_string(COMP_TIMES[command])+"\n";
            // 跳转到栈顶的地址
            res+="0;JMP\n";
            // 
            res=res+"("+COMP_COMMAND_TABLE[command]+"_"+to_string(COMP_TIMES[command])+")\n";
            // 加载栈指针
            res+="@SP\n";
            // 成立的情况下
            res+="A=M\nM=-1\n";
            // load inc stack pointer
            res=res+"("+"INC_STACK_POINTER_"+COMP_COMMAND_TABLE[command]+"_"+to_string(COMP_TIMES[command])+")\n";
            // 栈指针加一
            res=res+"@SP\n"+"M=M+1\n";
            ++COMP_TIMES[command];
        }
        return res;

    }




private:
    std::unordered_map<std::string,std::string> VIRTUAL_MEMORY_TABLE={{"local","LCL"},{"argument","ARG"},{"this","THIS"},{"that","THAT"}};
    std::unordered_map<std::string,std::string> VIRTUAL_MEMORY_TABLE_2={{"pointer","3"},{"temp","5"}};

    std::string mFileName;
    std::unordered_map<std::string,std::string> OPEARTION_TABLE={{"add","M=D+M"},
                                                                 {"sub","M=M-D"},
                                                                 {"neg","M=-M"},
                                                                 {"or","M=D|M"},
                                                                 {"and","M=D&M"},
                                                                 {"not","M=!M"}};
    // std::unordered_map<std::string,std::string> COMP_OPRATION_TABLE={{"EQ",""}};
    std::unordered_map<std::string,int> COMP_TIMES={{"eq",0},{"gt",0},{"lt",0}};

    std::unordered_map<std::string,std::string>COMP_COMMAND_TABLE={{"eq","JEQ"},{"gt","JGT"},{"lt","JLT"}};
};








#endif // __TRANSLATER__HPP