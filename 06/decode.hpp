#ifndef __DECODE_HPP__
#define __DECODE_HPP__
#include<string>
#include<unordered_map>
using namespace std;

#define CINSTUCTION_START "111"

class decodeASS{
public:
    decodeASS(){

    }    
    std::string demicalToBinStr(int num){
        std:string ans;
        for(int i=0;i<16;++i){
            int d=num&1;
            num>>=1;
            ans.push_back(d+'0');
        }
        for(int i=0,j=15;i<j;++i,--j){
            char tmp=ans[i];
            ans[i]=ans[j];
            ans[j]=tmp;
        }
        return ans;
    }
public: 
    std::unordered_map<std::string,std::string>DEST_TO_BITS={
                                                   {"M","001"},
                                                   {"D","010"},
                                                   {"MD","011"},
                                                   {"A","100"},
                                                   {"AM","101"},
                                                   {"AD","110"},
                                                   {"AMD","111"}};  
    std::unordered_map<std::string,std::string>COMP_TO_BITS={{"0","0101010"},
                                                             {"1","0111111"},
                                                             {"-1","0111010"},
                                                             {"D","0001100"},
                                                             {"A","0110000"},
                                                             {"M","1110000"},
                                                             {"!D","0001101"},
                                                             {"!A","0110001"},
                                                             {"!M","1110001"},
                                                             {"-D","0001111"},
                                                             {"-!","0110011"},
                                                             {"-M","1110011"},
                                                             {"D+1","0011111"},
                                                             {"A+1","0110111"},
                                                             {"M+1","1110111"},
                                                             {"D-1","0001110"},
                                                             {"A-1","0110010"},
                                                             {"M-1","1110010"},
                                                             {"D+A","0000010"},
                                                             {"D+M","1000010"},
                                                             {"D-A","0010011"},
                                                             {"D-M","1010011"},
                                                             {"A-D","0000111"},
                                                             {"M-D","1000111"},
                                                             {"D&A","0000000"},
                                                             {"D&M","1000000"},
                                                             {"D|A","0010101"},
                                                             {"D|M","1010101"}};
    std::unordered_map<std::string,std::string>JUMP_TO_BITS={
                                                             {"JGT","001"},
                                                             {"JEQ","010"},
                                                             {"JLT","100"},
                                                             {"JGE","011"},
                                                             {"JLE","110"},
                                                             {"JNE","101"},
                                                             {"JMP","111"}};

};




#endif // __DECODE_HPP__