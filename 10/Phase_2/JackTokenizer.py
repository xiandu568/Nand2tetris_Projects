


import re
import uuid




class JackTokenizer:
    keywordType = ['class', 'constructor', 'function', 'method', 'field', 'static', 'var', 'int', 'char', 'boolean',
                   'void', 'true', 'false', 'null', 'this', 'let', 'do', 'if', 'else', 'while', 'return']
    symbolType = ['{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~',
                  '&lt;', '&gt;', '&amp;']
    
    def __init__(self, file):
        self.data = [] # 源文件
        self.tokens = [] # 处理后的字元和所在行
        self.length = 0 # tokens的长度
        self.pc = -1 # 当前处理的字元下标
        self.Type = '' # 当前处理的字元的类型
        
        # 读入文件内容并删除注释
        self.data = self.delNotes(file).split('\n')
        # 分割字元
        self.tokens = self.divideTokens(self.data)
        self.length = len(self.tokens)
    
    def delNotes(self, file):
        _map = { }
        outstring = ''

        line = file.readline()
        while line:
            while True:
                m = re.compile('\".*?\"')
                # 每次只查找一个并替换
                _str = m.search( line )
                # 如果没匹配成功，就合并，然后下一行
                if None == _str:
                    outstring += line
                    break
                # 如果匹配成功了，将字符串替换为uuid
                key = str( uuid.uuid1() ) # 通用唯一识别码
                m = re.compile('\".*?\"')
                # 将line中的串模式m替换为key
                outtmp = re.sub(m, key, line)
                line = outtmp
                # 将 uuid:字符串 放入字典
                _map[ key ] = _str.group() # group() -- 匹配的整个表达式的字符串
            line = file.readline()

        # 删除单行注释
        m = re.compile(r'//.*')
        outtmp = re.sub(m, '', outstring)
        outstring = outtmp

        # 删除多行注释
        m = re.compile(r'/\*.*?\*/', re.S)
        outtmp = re.sub(m, '', outstring)
        outstring = outtmp

        # 将字符串恢复
        for key in _map.keys():
            outstring = outstring.replace(key, _map[key])

        return outstring
    
    def divideTokens(self, data):
        tokens = []
        lineIndex = 0
        # 按行进行处理
        for line in data:
            lineIndex += 1 # 行数加1
            inStr = False
            token = ''
            for char in line:
                if inStr == False: # 如果没遇到第一个"
                    if re.match('\w|_', char) != None: # 如果是字符或下划线
                        token += char # 放入字元
                    if char == ' ' and token != '': # 如果遇到空格而且字元不为空
                        tokens.append([token, lineIndex]) # 把字元放入字元列表
                        token = '' # 清空字元
                    if char in self.symbolType: # 如果遇到符号
                        if char == '>': # 转换三种符号
                            char = '&gt;'
                        if char == '<':
                            char = '&lt;'
                        if char == '&':
                            char = '&amp;'
                        if token != '':
                            tokens.append([token, lineIndex])
                            token = ''
                        tokens.append([char, lineIndex])
                    if char == '"':
                        inStr = True
                        token += char
                else: # 如果当前是字符串，所有的字符都将放入字元，直至遇到下一个"
                    token += char
                    if char == '"':
                        inStr = False
                        tokens.append([token, lineIndex])
                        token = ''
            if token != '':
                tokens.append([token, lineIndex])
        return tokens
    
    def hasMoreTokens(self):
        return self.pc < self.length - 1
    
    def advance(self):
        self.pc = self.pc + 1
    
    def tokenType(self):
        self.Type = ''
        token = self.tokens[self.pc][0]
        lineIndex = self.tokens[self.pc][1]
        
        if token in self.keywordType: # 关键字识别一定要在标识符之前
            self.Type = 'KEYWORD'
            return 'KEYWORD'
        if token in self.symbolType:
            self.Type = 'SYMBOL'
            return 'SYMBOL'
        if re.match(r'^[_a-zA-Z][_a-zA-Z0-9]*$', token):
            self.Type = 'IDENTIFIER'
            return 'IDENTIFIER'
        if re.match(r'^\d+$', token) and 0 <= int(token) <= 32767:
            self.Type = 'INT_CONST'
            return 'INT_CONST'
        if re.match(r'^".*"$', token):
            self.Type = 'STRING_CONST'
            return 'STRING_CONST' 
        raise Exception("\n--> " + str(lineIndex-1) + ' ' + self.data[lineIndex-2] + 
                        "\n--> " + str(lineIndex) + ' ' + self.data[lineIndex-1] + '    <-- Syntax Error:' + token + 
                        "\n--> " + str(lineIndex+1) + ' ' + self.data[lineIndex])
    
    def keyword(self):
        if self.Type == 'KEYWORD':
            return self.tokens[self.pc] # 返回关键字及所在行
        raise Exception("KEYWORD_类型不符")
    
    def symbol(self):
        if self.Type == 'SYMBOL':
            return self.tokens[self.pc]
        raise Exception("SYMBOL_类型不符")
    
    def identifier(self):
        if self.Type == 'IDENTIFIER':
            return self.tokens[self.pc]
        raise Exception("INT_CONST_类型不符")
    
    def intVal(self):
        if self.Type == 'INT_CONST':
            return self.tokens[self.pc]
        raise Exception("STRING_CONST_类型不符")
    
    def stringVal(self):
        if self.Type == 'STRING_CONST':
            return [self.tokens[self.pc][0][1:-1], self.tokens[self.pc][1]]
        raise Exception("IDENTIFIER_类型不符")
        
    def getTokens(self):
        tags_tokens = [] # 标签、字元、所在行
        while(self.hasMoreTokens()):
            self.advance()
            if self.tokenType() == 'KEYWORD':
                tags_tokens.append(['keyword', self.keyword()[0], str(self.keyword()[1])])
            if self.tokenType() == 'SYMBOL':
                tags_tokens.append(['symbol', self.symbol()[0], str(self.symbol()[1])])
            if self.tokenType() == 'IDENTIFIER':
                tags_tokens.append(['identifier', self.identifier()[0], str(self.identifier()[1])])
            if self.tokenType() == 'INT_CONST':
                tags_tokens.append(['integerConstant', self.intVal()[0], str(self.intVal()[1])])
            if self.tokenType() == 'STRING_CONST':
                tags_tokens.append(['stringConstant', self.stringVal()[0], str(self.stringVal()[1])])
        
        return tags_tokens

