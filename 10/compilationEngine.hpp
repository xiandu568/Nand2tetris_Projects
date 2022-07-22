#ifndef __CompilationEngine_HPP__
#define __CompilationEngine_HPP__
#include "JackTokenizer.hpp"  
#include <unordered_map>
using std::unordered_map;


class CompilationEngine {
private:
    JackTokenizer* tokenizer;
    string output;
    unordered_map<TOKENTYPE,string> KEYWORD_MAP = {
        {MKEYWORD,"<keyword>"},
        {MSYMBOL,"<symbol>"},
        {MIDENTIFER,"<identifier>"},
        {MINT_CONSTANT,"<integerConstant>"},
        {MSTRING_CONSTANT,"<stringConstant>"},
        {MEOF,"<eof>"}
    };
    unordered_map<TOKENTYPE,string> KEYWORD_MAP_END = {
        {MKEYWORD,"</keyword>"},
        {MSYMBOL,"</symbol>"},
        {MIDENTIFER,"</identifier>"},
        {MINT_CONSTANT,"</integerConstant>"},
        {MSTRING_CONSTANT,"</stringConstant>"},
        {MEOF,"</eof>"}
    };
    unordered_set<string>keywordConstant={
        "true",
        "false",
        "null",
        "this"
    };
    //jack 语法中用 = 表示判断 和赋值
    unordered_set<string>doubleOpeator={
        "+",
        "-",
        "*",
        "/",
        "&",
        "|",
        "<",
        ">",
        "="
    };

public:
    string getOutPut(){
        return output;
    }
    CompilationEngine(JackTokenizer* tokenizer){
        this->tokenizer = tokenizer;
    }
    ~CompilationEngine(){
        delete tokenizer;
        tokenizer = nullptr;
    }
    // 编译主程序
    void compileClass(){
        tokenizer->getNextTokens();
        if(tokenizer->curTyepe==MKEYWORD && tokenizer->curToken=="class"){
            output += "<class>\n ";
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";

            // 接下来是类名
            tokenizer->getNextTokens();
            if(tokenizer->curTyepe==MIDENTIFER){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                // 下一个token 必须是左花括号
                tokenizer->getNextTokens();
                //类的结束
                if(tokenizer->curTyepe==MSYMBOL && tokenizer->curToken=="{"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    // 接下来是类的内容
                    tokenizer->getNextTokens();
                    while(tokenizer->hasNextTokens() && tokenizer->curTyepe!=MEOF){
                        // 判断下一个token 
                        if(tokenizer->curTyepe==MSYMBOL && tokenizer->curToken=="}"){
                            // 类的结束
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            // 判断是否有下一个token
                            // 如果有则错误
                            tokenizer->getNextTokens();
                            if(tokenizer->hasNextTokens()){

                                string errorMsg=string("Unexpected token: ")+tokenizer->curToken+string(" at line: ")+tokenizer->getStrline();
                                throw errorMsg;
                            }
                        }
                        // 如果token 为
                        else if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="static" || tokenizer->curToken=="field") || tokenizer->curTyepe==MIDENTIFER){
                            compileClassVarDec();
                        }
                        // 属性方法
                        else if(tokenizer->curTyepe==MKEYWORD &&(tokenizer->curToken=="constructor" || tokenizer->curToken=="method" || tokenizer->curToken=="function")  || tokenizer->curTyepe==MIDENTIFER){
                            compileSubroutineDec();
                        }
                        else{
                            string errorMsg=string("Unexpected token: ")+tokenizer->curToken+string(" at line: ")+tokenizer->getStrline();
                            throw errorMsg;
                        } 
                        tokenizer->getNextTokens();     
                    }
                }
                else{
                    //  找不到左括号
                    string errorMsg=string("compileClass error: class name must be followed by {")+tokenizer->getStrline();
                    throw errorMsg;
                }
            }
            else{
                // 类名错误
                string errormsg=string("class name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
            
        }
        else{
            // 关键字错误
            string errormsg =string("error: class is not keyword,in ")+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</class>\n";
    }
    void compileClassVarDec(){
        // static || field
        output+="<classVarDec>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="int" || tokenizer->curToken=="char" || tokenizer->curToken=="boolean") || tokenizer->curTyepe==MIDENTIFER ){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            if(tokenizer->curTyepe==MIDENTIFER){
                // 变量名
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                while(tokenizer->curToken!=";"){
                    // 连续变量申明 int a,b,c;
                    if(tokenizer->curToken==","){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                        if(tokenizer->curTyepe==MIDENTIFER){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                        }
                        else{
                            // 变量名错误
                            string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                            throw errormsg;
                        }
                    }
                    else{
                        // 符号错误
                        string errormsg=string(" comma expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errormsg;
                    }
                }
                // 结束符号 ;
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                // 由class 程序 判断下一步行动
            }
            else{
                string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
        }else{
            // 类型错误
            string errormsg=string("type expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</classVarDec>\n";      
    }
    // subroutineDec: (constructor | function | method) subroutineName '(' parameterList ')' subroutineBody 
    // 子程序
    void compileSubroutineDec(){
        //constructor || method || function
        output+="<subroutineDec>\n";
        // 关键字 constructor || method || function
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        // 返回值类型
        if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="void" || tokenizer->curToken=="int" || tokenizer->curToken=="char" || tokenizer->curToken=="boolean") || tokenizer->curTyepe==MIDENTIFER){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // 函数名
            if(tokenizer->curTyepe==MIDENTIFER){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 左括号
                if(tokenizer->curToken=="("){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // 参数列表编译
                    compileParameterList(); //已经讲右括号加入到output中了
            
                    tokenizer->getNextTokens(); 
                    // 函数体编译
                    compileSubroutineBody();
                }
                else{
                    // 左括号错误
                    string errormsg=string("left parenthesis expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }
            }
            else{
                // 函数名错误
                string errormsg=string("function name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }

        }
        else{
            // 类型错误
            string errormsg=string("type expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</subroutineDec>\n";
    }

    void compileParameterList(){
        // (type varName) int a,int c,char b and so on
        output+="<parameterList>\n";
        // ) 结束符号
        if(tokenizer->curToken!=")"){
            // 类型 boolean || int || char || void || className
            if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="int" || tokenizer->curToken=="char" || tokenizer->curToken=="boolean") || tokenizer->curTyepe==MIDENTIFER){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 变量名
                if(tokenizer->curTyepe==MIDENTIFER){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // 
                    while(tokenizer->curToken!=")"){
                        // , 符号 
                        if(tokenizer->curToken==","){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                            // 类型 boolean || int || char || void || className
                            if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="int" || tokenizer->curToken=="char" || tokenizer->curToken=="boolean") || tokenizer->curTyepe==MIDENTIFER){
                                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                                tokenizer->getNextTokens();
                                // 变量名
                                if(tokenizer->curTyepe==MIDENTIFER){
                                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                                    tokenizer->getNextTokens();
                                }
                                else{
                                    // 变量名错误
                                    string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                                    throw errormsg;
                                }
                            }
                            else{
                                // 类型错误
                                string errormsg=string("type expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                                throw errormsg;
                            }
                        }
                        else{
                            // 符号错误
                            string errormsg=string(" comma expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                            throw errormsg;
                        }
                    }
                    // )
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    // 如果没有右括号 会引发其他类型的报警错误
                    // 在这里不对该错误类型提示
                }else{
                    // 变量名错误
                    string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }
            }
        }
        output+="</parameterList>\n";
    }

    void compileSubroutineBody(){
        output+="<subroutineBody>\n";
        // {
        if(tokenizer->curToken=="{"){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            while(tokenizer->curToken=="var" && tokenizer->curTyepe==MKEYWORD){
                compileVarDec();
                tokenizer->getNextTokens();
            }
            
            // statements
            compileStatements();
            // }
            if(tokenizer->curToken=="}"){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                // 不跳入下一个token
            }
            else{
                // }错误
                string errormsg=string("right parenthesis expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
        }
        else{
            // {错误
            string errormsg=string("left parenthesis expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</subroutineBody>\n";

           
    }
    /*
    * compileVarDec()
    * 对函数体中的局部变量声明进行编译

    */
    void compileVarDec(){
        output+="<varDec>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        // 类型名
        if(tokenizer->curTyepe==MKEYWORD && (tokenizer->curToken=="int" || tokenizer->curToken=="char" || tokenizer->curToken=="boolean") || tokenizer->curTyepe==MIDENTIFER){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // 变量名
            if(tokenizer->curTyepe==MIDENTIFER){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 变量名之后的符号
                // 下面的情况针对 var i,j,k;这种情况
                while(tokenizer->curToken!=";"){
                    if(tokenizer->curToken==","){
                        // , 符号
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                        // 变量名
                        if(tokenizer->curTyepe==MIDENTIFER){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                        }
                        else{
                            // 变量名错误
                            string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                            throw errormsg;
                        }
                    }
                    else{
                        // 符号错误
                        string errormsg=string(" comma expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errormsg;
                    }
                }
                // ;
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                //  如果缺乏分号 会引发其他类型的报警错误
                // 在这里不对该错误类型提示
            }else{
                // 变量名错误
                string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
        }
        else{
            // 类型错误
            string errormsg=string("type expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</varDec>\n";
    }
    /*
    * compileStatements()
    * 对函数体中的语句进行编译
    
    */
    void compileStatements(){
        output+="<statements>\n";
        while(tokenizer->curToken=="let" || tokenizer->curToken=="if" || tokenizer->curToken=="while" || tokenizer->curToken=="do" || tokenizer->curToken=="return"){
            if(tokenizer->curToken=="let"){
                //赋值语句
                compileLet();
            }
            else if(tokenizer->curToken=="if"){
                // 判断语句
                compileIf();
            }
            else if(tokenizer->curToken=="while"){
                compileWhile();
            }
            else if(tokenizer->curToken=="do"){
                compileDo();
            }
            else if(tokenizer->curToken=="return"){
                compileReturn();
            }
        }
        output+="</statements>\n";
    }
    /*
    * compileLet()
    * 对let语句进行编译
    
    */
    void compileLet(){
        output+="<letStatement>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        // 变量名
        if(tokenizer->curTyepe==MIDENTIFER){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // =
            //[
            // ] 有可能为数组
            if(tokenizer->curToken=="["){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                //表达式 forexample A[i+1];
                compileExpression(); 
                
                // ]
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
            }
            
            if(tokenizer->curToken=="="){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 表达式
                compileExpression();
            }
            else{
                // =错误
                string errormsg=string("= expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
        }
        else{
            // 变量名错误
            string errormsg=string("variable name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        // ;
        if(tokenizer->curToken==";"){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
        }
        else{
            // ;错误
            string errormsg=string("; expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</letStatement>\n";
    }
    void compileExpression(){
        //表达式有 
        output+="<expression>\n";
        compileTerm();
        while(doubleOpeator.find(tokenizer->curToken)!=doubleOpeator.end()){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            compileTerm();
        }
        output+="</expression>\n";
    }

    // 判断语句
    void compileIf(){
        output+="<ifStatement>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        //(  if 语句下一个token 必须为左括号
        if(tokenizer->curToken=="("){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // 判断条件
            compileExpression();
            // )
            if(tokenizer->curToken==")"){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // {
                // if 条件语句的下一句必须是 { 否则报错
                if(tokenizer->curToken=="{"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // 判断语句体
                    compileStatements();
                    // }
                    if(tokenizer->curToken=="}"){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();

                        // 有可能存在 else 单元
                        if(tokenizer->curToken=="else"){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                            // {
                            if(tokenizer->curToken=="{"){
                                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                                tokenizer->getNextTokens();
                                // 判断语句体
                                compileStatements();
                                // }
                                if(tokenizer->curToken=="}"){
                                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                                    tokenizer->getNextTokens();
                                }
                                else{
                                    // }错误
                                    string errormsg=string("} expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                                    throw errormsg;
                                }
                            }
                            else{
                                // {错误
                                string errormsg=string("{ expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                                throw errormsg;
                            }
                        }




                    }else{
                        // }错误
                        string errormsg=string("} expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errormsg;
                    }

                }
                else{
                    string errormsg=string("{ expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }

            }else{
                // )错误
                string errormsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }

        }else{
            string errorMsg=string("( expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errorMsg;
        }
        output+="</ifStatement>\n";
    }
    void compileWhile(){
        output+="<whileStatement>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        //(  if 语句下一个token 必须为左括号
        if(tokenizer->curToken=="("){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // 判断条件
            compileExpression();
            // )
            if(tokenizer->curToken==")"){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // {
                // if 条件语句的下一句必须是 { 否则报错
                if(tokenizer->curToken=="{"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // 判断语句体
                    compileStatements();
                    // }
                    if(tokenizer->curToken=="}"){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                    }else{
                        // }错误
                        string errormsg=string("} expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errormsg;
                    }

                }
                else{
                    string errormsg=string("{ expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }

            }
            else{
                string errorMsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errorMsg;
            }
        }
        else{
            string errorMsg=string("( expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline(); 
            throw errorMsg;
        }
        output+="</whileStatement>\n";

    }
    void compileDo(){
        //标签
        output+="<doStatement>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();
        
        if(tokenizer->curTyepe==MIDENTIFER){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            // 由于jack 语法没有继承语句
            //  则表明不会出现 xxx.xxx.xxx.xxx 的情况
            if(tokenizer->curToken=="("){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 参数列表
                compileExpressionList();
                // )
                if(tokenizer->curToken==")"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // ;
                    if(tokenizer->curToken==";"){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                    }else{
                        string errorMsg=string("; expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errorMsg;
                    }
                }
                else{
                    string errorMsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errorMsg;
                }
            }
            else if(tokenizer->curToken=="."){
                // 表明调用某个类的 method
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // method name
                if(tokenizer->curTyepe==MIDENTIFER){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // (
                    if(tokenizer->curToken=="("){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                        // 参数列表
                        compileExpressionList();
                        // )
                        if(tokenizer->curToken==")"){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                            // ;
                            if(tokenizer->curToken==";"){
                                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                                tokenizer->getNextTokens();
                            }else{
                                string errorMsg=string("; expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                                throw errorMsg;
                            }
                        }
                        else{
                            string errorMsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                            throw errorMsg;
                        }
                    }
                    else{
                        string errorMsg=string("( expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errorMsg;
                    }

                }else{
                    string errorMsg=string("class Method expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errorMsg;
                }
            }

        }
        else{
            string errorMsg=string("Function expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errorMsg;
        }
        output+="</doStatement>\n";
    }
    void compileReturn(){
        //format: return [expression];
        output+="<returnStatement>\n";
        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
        tokenizer->getNextTokens();

        if(tokenizer->curToken==";"){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
        }
        else{
            compileExpression();
            if(tokenizer->curToken==";"){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
            }
            else{
                string errorMsg=string("; expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errorMsg;
            }
        }
        output+="</returnStatement>\n";
    }
    /* 
        编译一个term 
        存在多种选择 
        可以通过提前判定符号来决定
    */
    void compileTerm(){
        output+="<term>\n";
        if(tokenizer->curTyepe==MINT_CONSTANT || tokenizer->curTyepe==MSTRING_CONSTANT || keywordConstant.find(tokenizer->curToken)!=keywordConstant.end()){
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
        }
        else if(tokenizer->curTyepe==MIDENTIFER){
            // 变量名
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            //[
            if(tokenizer->curToken=="["){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                //表达式 forexample A[i+1];
                compileExpression();
                //]
                if(tokenizer->curToken=="]"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                }
                else{
                    // ]错误
                    string errormsg=string("] expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }
            }
            else if(tokenizer->curToken=="("){
                //程序调用类型的表达式
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();

                // 函数调用中 可能存在参数 或者无参数
                compileExpressionList();
                if(tokenizer->curToken==")"){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                }
                else{
                    // )错误
                    string errormsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }
            }
            // 还有可能碰到子函数调用
            // . 号
            else if(tokenizer->curToken=="."){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                // 函数名
                if(tokenizer->curTyepe==MIDENTIFER){
                    output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                    tokenizer->getNextTokens();
                    // (
                    if(tokenizer->curToken=="("){
                        output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                        tokenizer->getNextTokens();
                        // 函数调用中 可能存在参数 或者无参数
                        compileExpressionList();
                        if(tokenizer->curToken==")"){
                            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                            tokenizer->getNextTokens();
                        }
                        else{
                            // )错误
                            string errormsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                            throw errormsg;
                        }
                    }
                    else{
                        // (错误
                        string errormsg=string("( expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                        throw errormsg;
                    }
                }
                else{
                    // 函数名错误
                    string errormsg=string("function name expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                    throw errormsg;
                }
            }
        }
        else if(tokenizer->curToken=="("){
            //这里出现括号的原因是
            //jack 语言没有优先级
            //括号内的表达式会被当做一个整体来处理
            //由此来确保优先级
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();
            compileExpression();
            if(tokenizer->curToken==")"){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
            }
            else{
                // )错误
                string errormsg=string(") expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
                throw errormsg;
            }
        }
        else if(tokenizer->curToken=="~" || tokenizer->curToken=="-"){
            //一元表达式
            output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
            tokenizer->getNextTokens();

            // 递归编译下一个term
            compileTerm();
        }
        else{
            //错误
            string errormsg=string("term expected, but ")+tokenizer->curToken+"be found in"+tokenizer->getStrline();
            throw errormsg;
        }
        output+="</term>\n";       
    }
    /* 
    *  针对表达式中调用某个函数 
    *  其中函数的参数列表
    
     */
    void compileExpressionList(){
        output+="<expressionList>\n";
        if(tokenizer->curToken!=")"){
            //有参数
            compileExpression();
            while(tokenizer->curToken==","){
                output+=string(KEYWORD_MAP[tokenizer->curTyepe])+tokenizer->curToken+KEYWORD_MAP_END[tokenizer->curTyepe]+"\n";
                tokenizer->getNextTokens();
                compileExpression();
            }
        }
        output+="</expressionList>\n";
    }
};




#endif // __CompilationEngine_HPP__
