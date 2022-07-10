#ifndef __TRANSLATEFUNCTION__HPP
#define __TRANSLATEFUNCTION__HPP
#include<string>
#include "VMCode.hpp"

using  std::string;
class TranslateFunction{
public:
    TranslateFunction(){
        functionCallCount=0;
        functionDefineCount=0;
    }
    ~TranslateFunction(){}
    string sysInit(){
        string res=string("@256\n")+"D=A\n"+"@SP\n"+"M=D\n";  // SP=256
        VMCode tmp("call Sys.init 0");

        res+=translateFun_Call(tmp.getFunName(),tmp.getINdex());
        return res;
    }

    std::string translateFun_Defination(std::string functionName,std::string index ){
        std::string res=string("(")+functionName+")\n";
        // 获取函数的argument size
        res+="@"+index+"\n";
        // 赋值给D
        res+="D=A\n";
        // 循环标签
        res+="(";
        res+="LOOP.ADD.LOCALS_";
        res+=to_string(functionDefineCount)+")\n";
        // 结束赋值标签
        res+="@NO_INDEX_"+to_string(functionDefineCount)+"\n";
        // 判断是否结束
        res+="D;JEQ\n";
        // 取栈指针
        res+="@SP\n";
        // 获取栈指针指向的位置
        res+="A=M\n";
        // 讲其赋值给0
        res+="M=0\n";
        // index=index-1;
        // SP=SP+1;
        res+="@SP\n";
        res+="M=M+1\n";

        res+="D=D-1\n";

        // 进入循环
        res+="@LOOP.ADD.LOCALS_"+to_string(functionDefineCount)+"\n";
        // 判断是否结束
        res+="D;JNE\n";
        // 跳转到结束标签
        res+="(";
        res+="NO_INDEX_"+to_string(functionDefineCount);
        res+=")\n";     
        ++functionDefineCount; 
        return res;
    }
    string translateFun_Call(string functionName,string index){
        // 标记返回地址
        string res=string("@RETURN_ADDRESS_")+to_string(functionCallCount)+"\n";
        res+="D=A\n";
        // 将标记保存到栈顶
        res+="@SP\n";

        res+="A=M\n";

        res+="M=D\n";
        // 增加栈指针
        res+="@SP\n";
        res+="M=M+1\n";


        // 转入LCL地址指针
        res+="@LCL\n";
        res+="D=M\n";
        // 将LCL地址指针保存到栈顶
        res+="@SP\n";
        res+="A=M\n";
        res+="M=D\n";
        // 增加栈指针
        res+="@SP\n";
        res+="M=M+1\n";

        // 转入ARG地址指针
        res+="@ARG\n";
        res+="D=M\n";
        // 将ARG地址指针保存到栈顶
        res+="@SP\n";
        res+="A=M\n";
        res+="M=D\n";
        // 增加栈指针
        res+="@SP\n";
        res+="M=M+1\n";

        // 转入THIS地址指针
        res+="@THIS\n";
        res+="D=M\n";
        // 将THIS地址指针保存到栈顶
        res+="@SP\n";
        res+="A=M\n";
        res+="M=D\n";
        // 增加栈指针
        res+="@SP\n";
        res+="M=M+1\n";

        // 转入THAT地址指针
        res+="@THAT\n";
        res+="D=M\n";
        // 将THAT地址指针保存到栈顶
        res+="@SP\n";
        res+="A=M\n";
        res+="M=D\n";
        // 增加栈指针
        res+="@SP\n";
        res+="M=M+1\n";

        // 取栈指针
        res+="@SP\n";
        res+="D=M\n";

        res+="@"+to_string(NUM_2_COPIED_ARGUMENTS)+"\n";    
        res+="D=D-A\n";

        res+="@"+index+"\n";
        res+="D=D-A\n";

        // 设置ARG
        res+="@ARG\n";
        res+="M=D\n";
        
        // 设置LCL
        res+="@SP\n";
        res+="D=M\n";
        res+="@LCL\n";
        res+="M=D\n";
        // 跳转到函数体
        res+="@"+functionName+"\n";
        res+="0;JMP\n";
        // 设置返回地址
        res+="(";
        res+="RETURN_ADDRESS_"+to_string(functionCallCount);
        res+=")\n";
        ++functionCallCount;
        return res;
    }
    string translateFun_Return(){
        // Frame=LCL
        string res="@LCL\n";
        res+="D=M\n";
        // 栈顶保存Frame
        res+="@R13\n";
        res+="M=D\n";
        //@5
        res+="@5\n";
        res+="D=A\n";
        
        // @R13
        res+="@R13\n";
        res+="A=M-D\n";
        // 得到返回地址
        res+="D=M\n";
        // 栈顶保存LCL
        // 将返回地址存放在R14中 temp reg
        res+="@R14\n";
        res+="M=D\n";


        // 得到返回值
        //取栈指针  栈指针减一
        res+="@SP\n";
        res+="AM=M-1\n";
        // 得到返回值
        res+="D=M\n";
        // @ARG
        res+="@ARG\n";
        res+="A=M\n";
        // 将返回值存放在ARG中
        res+="M=D\n";
        
        // 重设栈指针  sp=arg+1
        res+="@ARG\n";
        res+="D=M+1\n";
    
        res+="@SP\n";
        res+="M=D\n";
        // 恢复THAT指针
        // that=*(frame-1)
        res+="@1\n";
        res+="D=A\n";
        // @R13
        res+="@R13\n";
        res+="A=M-D\n";
        res+="D=M\n";
        // 得到THAT指针
        res+="@THAT\n";
        res+="M=D\n";
        

        // @this
        res+="@2\n";
        res+="D=A\n";
        // @R13
        res+="@R13\n";
        // R13存放的是address
        res+="A=M-D\n";
        res+="D=M\n";
        // 得到THIS指针
        res+="@THIS\n";
        res+="M=D\n";

        //arg
        res+="@3\n";
        res+="D=A\n";
        // @R13
        res+="@R13\n";
        res+="A=M-D\n";
        res+="D=M\n";
        // 得到ARG指针
        res+="@ARG\n";
        res+="M=D\n";
        // LCL
        res+="@4\n";
        res+="D=A\n";
        // @R13
        res+="@R13\n";
        res+="A=M-D\n";
        res+="D=M\n";   /// 这里出现一个bug  忘了添加这句话
        // 得到LCL指针
        res+="@LCL\n";
        res+="M=D\n";


        // 回到返回地址
        res+="@R14\n";
        res+="A=M\n";
        res+="0;JMP\n";
        return res;
    }




private:
    int functionCallCount;
    int functionDefineCount;
    int NUM_2_COPIED_ARGUMENTS=5;
};





#endif // __TRANSLATEFUNCTION__HPP