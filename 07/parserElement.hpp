#ifndef __PARSER_ELEMENT__HPP
#define __PARSER_ELEMENT__HPP
#include <string>
#include <vector>

enum commandType{NOINSTRUCTION,PUSH,POP,ADD,SUB,EQ,GT,LT,AND,OR,NOT};
struct parserElement
{
    std::string mcommand;
    parserElement* mnext;
    
    parserElement(std::string command, parserElement* next)
    {
        this->mcommand = command;
        this->mnext = next;
    }
    parserElement(std::string command)
    {
        this->mcommand = command;
        this->mnext = nullptr;
    }
};



#endif  // __PARSER_ELEMENT__HPP