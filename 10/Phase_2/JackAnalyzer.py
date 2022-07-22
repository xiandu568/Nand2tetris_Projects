


import os.path
from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine




def compileFile(inPath):
    outPath = inPath.replace('.jack', 'S.xml')
    fileIn = open(inPath, 'r')
    fileOut = open(outPath, 'w')
    jackTokenizer = JackTokenizer(fileIn)
    compilationEngine = CompilationEngine(jackTokenizer.getTokens(), fileOut)
    fileIn.close()
    fileOut.close()
def main(path):
    if os.path.isdir(path):
        for root, dirs, files in os.walk(path):
            for file in files:
                if os.path.splitext(file)[1] == '.jack':
                    compileFile(os.path.join(root , file))
    if os.path.isfile(path):
        compileFile(path)




main('ArrayTest/Main.jack')

