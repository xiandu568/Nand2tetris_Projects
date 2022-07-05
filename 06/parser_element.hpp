#ifndef __PARSER_ELEMENT_HPP
#define __PARSER_ELEMENT_HPP
#include<string>

enum CommonType{No_INStruction, Address, Label, Calaulate}; 
struct parser_element{
    std::string mcur;
    CommonType mtype;
    parser_element* mnext;
    parser_element(std::string cur, CommonType type, parser_element* next){
        this->mcur=cur;
        this->mtype=type;
        this->mnext=next;
    }
     parser_element(std::string cur, CommonType type){
        this->mcur=cur;
        this->mtype=type;
        this->mnext=nullptr;
    }
    parser_element(std::string cur):mcur(cur),mtype(No_INStruction),mnext(nullptr){}
};

#endif