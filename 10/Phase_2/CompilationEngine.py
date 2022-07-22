#!/usr/bin/env python
# coding: utf-8



class CompilationEngine:
    op = ['+', '-', '*', '/', '&', '|', '<', '>', '=', '&lt;', '&gt;', '&amp;']
    unaryOp = ['-', '~']
    keywordConstant = ['true', 'false', 'null', 'this']
    
    def __init__(self, tags_tokens, file):
        self.tags_tokens = tags_tokens
        self.length = len(tags_tokens)
        self.pc = -1
        self.tags_code = ''
        self.complieClass()
        file.write(self.tags_code)
        
    def getNextTagToken(self):
        self.pc += 1
        return self.tags_tokens[self.pc]
        
    def complieClass(self):
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        
        if self.val == 'class':
            self.tags_code += '<class>\n'
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            
            if self.key == 'identifier':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                
                if self.val == '{':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                    
                    while self.pc < self.length:
                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        
                        if self.val == '}':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                            if self.pc != self.length - 1:
                                print(self.tags_code)
                                raise Exception("\n--> " + self.line + ' ' + self.val + " before code")
                            break
                            
                        if self.val in ['static', 'field']:
                            self.compileClassVarDec()
                            if self.pc == self.length - 1:
                                print(self.tags_code)
                                raise Exception("\n--> " + self.line + ' ' + self.val + " need a }")
                        elif self.val in ['constructor', 'function', 'method']:
                            self.compileSubroutineDec()
                            if self.pc == self.length - 1:
                                print(self.tags_code)
                                raise Exception("\n--> " + self.line + ' ' + self.val + " need a }")
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a static|field|constructor|function|method")
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a {")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a class")
        
        self.tags_code += '</class>\n'
        
    def compileClassVarDec(self):
        self.tags_code += '<classVarDec>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val in ['int', 'char', 'boolean'] or self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.key == 'identifier':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                while self.val != ';':
                    if self.val == ',':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                        
                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        if self.key == 'identifier':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                            
                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ,")
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a type")
        
        self.tags_code += '</classVarDec>\n'
        return
    
    def compileSubroutineDec(self):
        self.tags_code += '<subroutineDec>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val in ['void', 'int', 'char', 'boolean'] or self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.key == 'identifier':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.val == '(':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key) # 把（写入
            
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    self.compileParameterList()
                    
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key) # 把）写入
                    
                    self.compileSubroutineBody()
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a (")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a void or type")

        self.tags_code += '</subroutineDec>\n'
        return
    
    def compileParameterList(self):
        self.tags_code += '<parameterList>\n'
        
        if self.val != ')':
            if self.val in ['int', 'char', 'boolean'] or self.key == 'identifier':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.key == 'identifier':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    while self.val != ')':
                        if self.val == ',':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                            if self.val in ['int', 'char', 'boolean'] or self.key == 'identifier':
                                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                                temp = self.getNextTagToken()
                                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                                if self.key == 'identifier':
                                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                                    temp = self.getNextTagToken()
                                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                                else:
                                    print(self.tags_code)
                                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
                            else:
                                print(self.tags_code)
                                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a type")
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ,")
                    self.tags_code += '</parameterList>\n'
                    return
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ) or type")
        self.tags_code += '</parameterList>\n'
        
    def compileSubroutineBody(self):
        self.tags_code += '<subroutineBody>\n'
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val == '{':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            while self.val == 'var':
                self.compileVarDec()

                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileStatements()
            if self.val == '}':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key) # 写入}
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a var|let|if|while|do|return or }")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a {")
            
        self.tags_code += '</subroutineBody>\n'
        return
        
    def compileVarDec(self):
        self.tags_code += '<varDec>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val in ['int', 'char', 'boolean'] or self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.key == 'identifier':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                while self.val != ';':
                    if self.val == ',':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        if self.key == 'identifier':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a , or ;")
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifier")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a type")
        
        self.tags_code += '</varDec>\n'
        return
    
    def compileStatements(self):
        self.tags_code += '<statements>\n'
        
        while self.val in ['let', 'if', 'while', 'do', 'return']:
            if self.val == 'let':
                self.compileLet()
            elif self.val == 'if':
                self.compileIf()
            elif self.val == 'while':
                self.compileWhile()
            elif self.val == 'do':
                self.compileDo()
            elif self.val == 'return':
                self.compileReturn()
        
        self.tags_code += '</statements>\n'
        return
    
    def compileLet(self):
        self.tags_code += '<letStatement>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.val == '[':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key) # 写入[
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpression()
                if self.val == ']':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ]")
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.val == '=':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key) # 写入=
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpression()
                if self.val == ';':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                    
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ;")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a =")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifer")
            
        self.tags_code += '</letStatement>\n'
        return
    
    def compileIf(self):
        self.tags_code += '<ifStatement>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val == '(':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileExpression()
            if self.val == ')':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.val == '{':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    self.compileStatements()
                    if self.val == '}':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        if self.val == 'else':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                            if self.val == '{':
                                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                                temp = self.getNextTagToken()
                                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                                self.compileStatements()
                                if self.val == '}':
                                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                                    
                                    temp = self.getNextTagToken()
                                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                                else:
                                    print(self.tags_code)
                                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a }")
                            else:
                                print(self.tags_code)
                                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a {")
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a }")
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a {")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a (")
            
        self.tags_code += '</ifStatement>\n'
        return
    
    def compileWhile(self):
        self.tags_code += '<whileStatement>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val == '(':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
            
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileExpression()
            if self.val == ')':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.val == '{':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    self.compileStatements()
                    if self.val == '}':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a }")
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a {")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a (")
        
        self.tags_code += '</whileStatement>\n'
        return
    
    def compileDo(self):
        self.tags_code += '<doStatement>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.val == '(':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpressionList()
                
                if self.val == ')':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
            elif self.val == '.':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.key == 'identifier':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    if self.val == '(':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        self.compileExpressionList()

                        if self.val == ')':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a (")
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifer")
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ( or .")
            if self.val == ';':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ;")
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifer")
        
        self.tags_code += '</doStatement>\n'
        return
    
    def compileReturn(self):
        self.tags_code += '<returnStatement>\n'
        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
        temp = self.getNextTagToken()
        self.key, self.val, self.line = temp[0], temp[1], temp[2]
        if self.val != ';':
            self.compileExpression()
        if self.val == ';':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a expression or ;")
        
        self.tags_code += '</returnStatement>\n'
        return
    
    def compileExpression(self):
        self.tags_code += '<expression>\n'
        self.compileTerm()
        while self.val in self.op:
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileTerm()
        self.tags_code += '</expression>\n'
        return
    
    def compileTerm(self):
        self.tags_code += '<term>\n'
        if self.key in ['integerConstant', 'stringConstant'] or self.val in self.keywordConstant:
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
        elif self.key == 'identifier':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            if self.val == '[':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpression()
                
                if self.val == ']':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a ]")
            elif self.val == '(':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpressionList()
                
                if self.val == ')':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
        
                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
            elif self.val == '.':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                if self.key == 'identifier':
                    self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                    temp = self.getNextTagToken()
                    self.key, self.val, self.line = temp[0], temp[1], temp[2]
                    if self.val == '(':
                        self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                        temp = self.getNextTagToken()
                        self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        self.compileExpressionList()

                        if self.val == ')':
                            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                            temp = self.getNextTagToken()
                            self.key, self.val, self.line = temp[0], temp[1], temp[2]
                        else:
                            print(self.tags_code)
                            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
                    else:
                        print(self.tags_code)
                        raise Exception("\n--> " + self.line + ' ' + self.val + " should be a (")
                else:
                    print(self.tags_code)
                    raise Exception("\n--> " + self.line + ' ' + self.val + " should be a identifer")
        elif self.val == '(':
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileExpression()
            
            if self.val == ')':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)

                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
            else:
                print(self.tags_code)
                raise Exception("\n--> " + self.line + ' ' + self.val + " should be a )")
        elif self.val in self.unaryOp:
            self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
            temp = self.getNextTagToken()
            self.key, self.val, self.line = temp[0], temp[1], temp[2]
            self.compileTerm()
        else:
            print(self.tags_code)
            raise Exception("\n--> " + self.line + ' ' + self.val + " should be a term")
        
        self.tags_code += '</term>\n'
        return
    
    def compileExpressionList(self):
        self.tags_code += '<expressionList>\n'
        if self.val != ')':
            self.compileExpression()
            while self.val == ',':
                self.tags_code += '<{}> {} </{}>\n'.format(self.key, self.val, self.key)
                
                temp = self.getNextTagToken()
                self.key, self.val, self.line = temp[0], temp[1], temp[2]
                self.compileExpression()
            
        self.tags_code += '</expressionList>\n'
        return

