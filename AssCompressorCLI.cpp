/*
 AssCompressor - AssCompressor Designed For Project BiliBuffer
 Copyright (C) 2014  CHOSX RecursiveG

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "AssCompressor.h"
#include <curl/curl.h>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#ifndef _WIN32
    #define BilibiliFont_Name_HEI     "黑体-简 细体"
    #define BilibiliFont_Name_YouYuan "圆体-简 细体"
    #define BilibiliFont_Name_Song    "宋体-简 常规体"
    #define BilibiliFont_Name_Kai     "楷体-简 常规体"
    #define BilibiliFont_Name_MS_HEI  "兰亭黑-简 纤黑"
#else
    #define BilibiliFont_Name_HEI      "黑体"
    #define BilibiliFont_Name_YouYuan  "幼圆"
    #define BilibiliFont_Name_Song     "宋体"
    #define BilibiliFont_Name_Kai      "楷体"
    #define BilibiliFont_Name_MS_HEI   "微软雅黑"
#endif

using namespace std;
using namespace rapidxml;

void print_copyright(){
    cout<<"AssCompressor Ver."<<AssCompressorVersion<<endl;
    cout<<"Copyright (C) 2014 CHOSX_K9 RecursiveG"<<endl;
    cout<<endl;
    cout<<" This program is free software; you can redistribute it and/or"<<endl;
    cout<<"modify it under the terms of the GNU General Public License"<<endl;
    cout<<"as published by the Free Software Foundation; either version 2"<<endl;
    cout<<"of the License, or (at your option) any later version."<<endl;
    cout<<endl;
    cout<<"This program is distributed in the hope that it will be useful,"<<endl;
    cout<<"but WITHOUT ANY WARRANTY; without even the implied warranty of"<<endl;
    cout<<"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"<<endl;
    cout<<"GNU General Public License for more details."<<endl;
    cout<<endl;
    cout<<"You should have received a copy of the GNU General Public License"<<endl;
    cout<<"along with this program; if not, write to the Free Software"<<endl;
    cout<<"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA."<<endl;
}
void print_help(string argv0){
    cout<<"AssCompressor Ver."<<AssCompressorVersion<<endl;
    cout<<"Copyright (C) 2014 CHOSX_K9 RecursiveG"<<endl<<endl;
    cout<<"Usage: "<<argv0<<" [Configure XML Path]"<<endl;
    cout<<"       "<<argv0<<" --version"<<endl;
}
static size_t _onCurlWrite(void* buffer, size_t size, size_t nmemb, string &data)
{
    if(NULL==buffer){return -1;}
    data.append((char*)buffer,size*nmemb);
    return size*nmemb;
}


#define ToBool(str) ((str)[4]=='\0') //Assume a four-char-long string as 'true'
                                     //otherwise 'false'

void FillConfigure(AssConfig &conf,string cfgPath){
    ifstream in(cfgPath.c_str());
    if (!in)throw runtime_error("Corrupted Config: Can't open file: "+cfgPath);
    string xml="";char ch;
    while (!in.eof()){
        in.get(ch);
        xml+=ch;
    }
    char str[xml.size()+10];
    strcpy(str,xml.c_str());
    xml_document<> doc;
    doc.parse<0>(str);
    xml_node<> *root=doc.first_node();
    if (strcmp(root->first_attribute("version")->value(),"CLIv1"))
        throw runtime_error("Unsupported Configure version");

    conf.ACCEnable=ToBool(root->first_node("ACCEnable")->value());
    conf.TOPEnable=ToBool(root->first_node("TOPEnable")->value());
    conf.BOTEnable=ToBool(root->first_node("BOTEnable")->value());
    conf.ROLEnable=ToBool(root->first_node("ROLEnable")->value());
    conf.FollowACCFontName=ToBool(root->first_node("FollowACCFontName")->value());
    conf.Bold=ToBool(root->first_node("Bold")->value());
    conf.ACCAutoAdjust=ToBool(root->first_node("ACCAutoAdjust")->value());
    conf.ACCFixRotate=ToBool(root->first_node("ACCFixRotate")->value());
    conf.MultilineComment=ToBool(root->first_node("MultilineComment")->value());
    conf.MultilineCompare=conf.MultilineComment&&ToBool(root->first_node("MultilineCompare")->value());
    conf.DisplayX=atol(root->first_node("DisplayX")->value());
    conf.DisplayY=atol(root->first_node("DisplayY")->value());
    conf.iframeX=atol(root->first_node("iframeX")->value());
    conf.iframeY=atol(root->first_node("iframeY")->value());
    conf.DefaultFontSize=atol(root->first_node("DefaultFontSize")->value());
    conf.layerACC=atol(root->first_node("LayerACC")->value());
    conf.layerTOP=atol(root->first_node("LayerTOP")->value());
    conf.layerBOT=atol(root->first_node("LayerBOT")->value());
    conf.layerROL=atol(root->first_node("LayerROL")->value());
    conf.DefaultFontName=string(root->first_node("DefaultFontName")->value());
    conf.VideoRatio=string(root->first_node("VideoRatio")->value());
    string XmlPath=string(root->first_node("XMLUri")->value());
    conf.ExportLocation=string(root->first_node("ExportLocation")->value());
    conf.AlphaTOP=atof(root->first_node("AlphaTOP")->value());
    conf.AlphaBOT=atof(root->first_node("AlphaBOT")->value());
    conf.AlphaROL=atof(root->first_node("AlphaROL")->value());
    conf.AlphaACC=atof(root->first_node("AlphaACC")->value());
    conf.MovingVelocity=atof(root->first_node("MovingVelocity")->value());
    conf.DynamicLengthRatio=atof(root->first_node("DynamicCompare")->value());
    conf.FontNameHei=BilibiliFont_Name_HEI;
    conf.FontNameYouYuan=BilibiliFont_Name_YouYuan;
    conf.FontNameSong=BilibiliFont_Name_Song;
    conf.FontNameKai=BilibiliFont_Name_Kai;
    conf.FontNameMSHei=BilibiliFont_Name_MS_HEI;
    conf.ContentType="Bilibili_XML";
    conf.XMLUrl=XmlPath;
    doc.clear();
    const string pfx1("comment.bilibili.tv/");
    const string pfx2("http://comment.bilibili.tv/");
    string content;
    if (XmlPath.find(pfx1)==0||XmlPath.find(pfx2)==0){//Get though Internet
        content="";
        char errorBuffer[CURL_ERROR_SIZE];
        CURL *h=curl_easy_init();
        curl_easy_setopt(h,CURLOPT_READFUNCTION,NULL);
        curl_easy_setopt(h,CURLOPT_HEADERFUNCTION,NULL);
        curl_easy_setopt(h,CURLOPT_WRITEFUNCTION,_onCurlWrite);
        curl_easy_setopt(h,CURLOPT_WRITEDATA,&content);
        curl_easy_setopt(h,CURLOPT_ERRORBUFFER,errorBuffer);

        curl_easy_setopt(h,CURLOPT_URL,XmlPath.c_str());
        curl_easy_setopt(h,CURLOPT_FOLLOWLOCATION,1);
        curl_easy_setopt(h,CURLOPT_NOSIGNAL,1);
        curl_easy_setopt(h,CURLOPT_CONNECTTIMEOUT,3);
        curl_easy_setopt(h,CURLOPT_TIMEOUT,7);
        curl_easy_setopt(h,CURLOPT_ENCODING,"");
        CURLcode code=curl_easy_perform(h);
        curl_easy_cleanup(h);
        if (code!=CURLE_OK)
            throw runtime_error("Fail to get XML: "+string(errorBuffer));
        if (content=="")
            throw runtime_error("Corrupted XML: empty file");
    }else{//Get from local file
        content="";char ch;
        ifstream in(XmlPath.c_str());
        if (!in)
            throw runtime_error("Corrupted XML: Can't open file: "+XmlPath);
        while (!in.eof()){
            in.get(ch);
            content+=ch;
        }
        in.close();
    }
    conf.XMLContent=content;
}
int main(int argc, const char * argv[])
{
    vector<string> argv_str;
    for (int i=0; i<argc; i++) {
        argv_str.push_back(argv[i]);
    }
    if (argc<=1) {
        print_help(argv_str[0]);
        return 0;
    }
    if (argv_str[1]=="--version") {
        print_copyright();
        return 0;
    }
    try{
        AssConfig conf;
        FillConfigure(conf,argv_str[1]);
        AssCompressor worker(conf);//Auto-Start work
        string ass=worker.ASS;
        if (worker.ErrorMsg!=""){
            std::cerr<<"An Error occured. Please verify your input and try again later."<<std::endl;
            std::cerr<<"[ERROR]"<<worker.ErrorMsg<<std::endl;
            return -1;
        }
        if (conf.ExportLocation==""){
            cout<<ass<<endl<<";";
        }else{
            ofstream Ass_File(config.ExportLocation.c_str());
            Ass_File<<ASS;
            Ass_File.close();
        }
        cout<<"Work for config \""<<argv_str[1]<<"\" finished successfully."<<endl;
    }catch(runtime_error &e){
        std::cerr<<"An Error occured! Please verify your input BEFORE reporting the bug."<<std::endl;
        std::cerr<<"[ERROR]"<<e.what()<<std::endl;
        return -2;
    }
    return 0;
}
