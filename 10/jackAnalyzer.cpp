// -------------------------------------------
// Author: xiandu568
// Date: 2022/07/22
// 
// 
// 

#include<iostream>

#include "compilationEngine.hpp"    
#include "JackTokenizer.hpp"
#include<sys/stat.h>
#include<fstream>
#include<io.h> // 
#include<vector>
using namespace std;
vector<string> getFiles(string path){
    // get all files in the path and return a vector of file names
    struct  stat st;
    vector<string> files;
    if(stat(path.c_str(),&st)==-1){
        // path is not exist
        cout<<"path is not exist"<<endl;
        return files;
    }else{
        if(st.st_mode & S_IFDIR){
            // path is a directory
            intptr_t hFile=0;
            _finddata_t fileInfo;
            string filePath;
            if((hFile=_findfirst(filePath.assign(path).append("/*").c_str(), &fileInfo))!=-1){
                do{
                    if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
			        {
                        string  temp=string(fileInfo.name);
                        int j=temp.rfind('.');
                        //jack 文件将其加入vector
				        if(j!=-1 && temp.substr(j)==".jack") files.push_back(filePath.assign(path).append("/").append(fileInfo.name));
			        }
		            } while (_findnext(hFile, &fileInfo) == 0);
                _findclose(hFile);
            }

        }else if(st.st_mode & S_IFREG){
            // path is a file
            files.push_back(path);
        }else{
            // path is neither a file nor a directory
            cout<<"path is neither a file nor a directory"<<endl;
        }
    }
    return files;
}

void translateAndWrite(vector<string>& path){
    for(string& file:path){
        int j=file.rfind('.');
        string outPutFile=file.substr(0,j)+"S.xml";
        cout<<"translate "<<file<<" to "<<outPutFile<<endl;
        try{
            JackTokenizer* tokenizer=new JackTokenizer(file);
            CompilationEngine compilationEngine(tokenizer);
            compilationEngine.compileClass();
            fstream mfile=fstream(outPutFile, ios::out);
            mfile<<compilationEngine.getOutPut();
            mfile.close();
            cout<<"translate success!"<<endl;
        }
        catch(string msg){
            cout<<msg<<endl; //错误提示信息
            exit(1);
        }
    }

}


int main(int argc, char* argv[]){
    if(argc!=2){
        cout<<"Usage: "<<argv[0]<<" <path>"<<endl;
        exit(1);
    }
    string fileName=argv[1];
    vector<string>&& sourcePath=getFiles(fileName);
    translateAndWrite(sourcePath);
   
    return 0;
}