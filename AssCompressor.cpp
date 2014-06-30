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
using namespace std;

#ifdef DEBUG
#define BOOL2STR(b) ((b)?"True":"False")
void AssCompressor::PrintConfig() {
    cout<<"XMLUrl:             "<<config.XMLUrl<<endl;
    cout<<"ACCEnable:          "<<BOOL2STR(config.ACCEnable)<<endl;
    cout<<"TOPEnable:          "<<BOOL2STR(config.TOPEnable)<<endl;
    cout<<"BOTEnable:          "<<BOOL2STR(config.BOTEnable)<<endl;
    cout<<"ROLEnable:          "<<BOOL2STR(config.ROLEnable)<<endl;
    cout<<"DisplayX:           "<<config.DisplayX<<endl;
    cout<<"DisplayY:           "<<config.DisplayY<<endl;
    cout<<"iframeX:            "<<config.iframeX<<endl;
    cout<<"iframeY:            "<<config.iframeY<<endl;
    cout<<"AlphaTOP:           "<<config.AlphaTOP<<endl;
    cout<<"AlphaBOT:           "<<config.AlphaBOT<<endl;
    cout<<"AlphaROL:           "<<config.AlphaROL<<endl;
    cout<<"AlphaACC:           "<<config.AlphaACC<<endl;
    cout<<"Default_Front_Name: "<<config.DefaultFontName<<endl;
    cout<<"Default_font_size:  "<<config.DefaultFontSize<<endl;
    cout<<"FollowACCfontname:  "<<BOOL2STR(config.FollowACCFontName)<<endl;
    cout<<"Moving Velocity:    "<<config.MovingVelocity<<endl;
    cout<<"Video Ratio:        "<<config.VideoRatio<<endl;
}
#endif

AssCompressor::AssCompressor(AssConfig &conf):config(conf){//done
    ErrorMsg="";
    if (config.ContentType=="Bilibili_XML")
        doConvert_Bilibili_XML();
}

string byte2hex(int x){  //done
    int L=x%16;
    int H=(x>>4)%16;
    char s[3]={0,0,0};
    s[1]=L<10?L+48:L+55;
    s[0]=H<10?H+48:H+55;
    return string(s);
}
void AssCompressor::InjectHeader() {  //Done
    ASS="[Script Info]\n";
    ASS+=";Build By BiliBuffer_AssCompressor\n";
    ASS+=";From Comment XML URL:"+config.XMLUrl+"\n";
    ASS+=";AssCompressor -- Open Source XML2ASS\n";
    ASS+=";GitHub:https://github.com/CHOSX/AssCompressor.git  [Git]\n";
    ASS+=";GitHub:https://github.com/CHOSX/AssCompressor      [Subversion]\n";
    ASS+="Title: "+config.Title+"\n";
    ASS+="Original Script: AssCompressor\n"
    ASS+="ScriptType: v4.00+\n";
    ASS+="Collisions: Normal\n";
    ASS+="PlayResX: "+INT2STR(config.DisplayX)+"\n";
    ASS+="PlayResY: "+INT2STR(config.DisplayY)+"\n\n";
    ASS+="WrapStyle: 2\n"
    ASS+="[V4+ Styles]\n";
    ASS+="Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ";
    ASS+="ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";
    ASS+="Style: Default, "+config.DefaultFontName+","+config.DefaultFontSize+",";
    ASS+="&H"+byte2hex(config.AlphaROL)+"FFFFFF,&H"+byte2hex(config.AlphaROL)+"FFFFFF,&H"+byte2hex(config.AlphaROL)+"000000,&H"+byte2hex(config.AlphaROL)+"000000,"
    ASS+=(config.Bold?"-1":"0")+",0,0,0,100,100,0,0,1,1,0,7,0,0,0,0\n";
    ASS+="Style: Top "+config.DefaultFontName+","+config.DefaultFontSize+",";
    ASS+="&H"+byte2hex(config.AlphaTOP)+"FFFFFF,&H"+byte2hex(config.AlphaTOP)+"FFFFFF,&H"+byte2hex(config.AlphaTOP)+"000000,&H"+byte2hex(config.AlphaTOP)+"000000,"
    ASS+=(config.Bold?"-1":"0")+",0,0,0,100,100,0,0,1,1,0,8,0,0,0,0\n\n";
    ASS+="Style: Bottom "+config.DefaultFontName+","+config.DefaultFontSize+",";
    ASS+="&H"+byte2hex(config.AlphaBOT)+"FFFFFF,&H"+byte2hex(config.AlphaBOT)+"FFFFFF,&H"+byte2hex(config.AlphaBOT)+"000000,&H"+byte2hex(config.AlphaBOT)+"000000,"
    ASS+=(config.Bold?"-1":"0")+",0,0,0,100,100,0,0,1,1,0,2,0,0,0,0\n\n";
    ASS+="[Events]\nFormat: Start, End, Layer, Style, Effect, Text\n";
}
void AssCompressor::doConvert_Bilibili_XML(){  //Done
    InjectHeader();
    BilibiliCommentManager worker(config);
    worker.Fill(cmtVector);

    sortVector();
    FurtherCalc();
    ReduceThickness();
    yAxisRelocate();

    long int len=cmtVector.size();
    for(int i=0;i<len;i++)
        ASS+=Comment2ASS(cmtVector[i])+"\n";

    /* TODO:Rewrite below */
    ASS+="\n;Total Comment Analysis: "+INT2STR(ASS_OBJ.size());
	  if (BilibiliObject.Warning_Message.size()>0)
        ASS+="\n;Warning Message:\n";
    for (unsigned int i=0; i<BilibiliObject.Warning_Message.size(); i++) {
        ASS+=";"+BilibiliObject.Warning_Message[i]+"\n";
    }
}
static string AssCompressor::Comment2ASS(const Comment &cmt){//done
    vector<string> args;
    args.push_back(Time2Str(cmt.time));
    args.push_back(Time2Str(cmt.time+cmt.duration)));
    if(cmt.type==Rolling||cmt.type==ReverseRolling){
        if(!config.ROLEnable)return "";
        args.push_back(ToString(config.layerROL));
        args.push_back("Default");
        args.push_back(_effectRoll(cmt));
    }else if(cmt.type==Top||cmt.type==Bottom){
        if(cmt.type==Top){
            if(!config.TOPEnable)return "";
            args.push_back(ToString(config.layerTOP));
            args.push_back("Top");
        }else{
            if(!config.BOTEnable)return "";
            args.push_back(ToString(config.layerBOT));
            args.push_back("Bottom");
        }
        args.push_back(_effectPos(cmt));
    }else if(cmt.type==Accurate){
        if(!config.ACCEnable)return "";
        args.push_back(ToString(config.layerACC));
        args.push_back("Default");
        args.push_back(_effectAcc(cmt));
    }else if(cmt.type==Track){
        if(config.TRKEnable)
            return _track2ASS(cmt);
        else
            return "";
    }else{              //Code comment will never be supported
        return "";
    }
    args.push_back(cmt.content);
    string ret="Dialogue: "+args[0];
    for(int i=1;i<args.size();i++)
        ret+=","+args[i];
    return ret;
}
static string AssCompressor::_effectRoll(const Comment &cmt){}
static string AssCompressor::_effectPos(const Comment &cmt){}
static string AssCompressor::_effectAcc(const Comment &cmt){}
static string AssCompressor::_track2ASS(const Comment &cmt){}


inline bool cmtTimeCompareFunc(Comment &a,Comment &b){
    return a.time<b.time; }
void AssCompressor::sortVector(){
    sort(this->cmtVector.begin(),this->cmtVector.end(),cmtTimeCompareFunc);
}
void AssCompressor::ReduceThickness(){}
void AssCompressor::yAxisRelocate(){}
void AssCompressor::FurtherCalc(){
    int len=cmtVector.size();
    for (int i=0;i<len;i++){
        long int len=_countUTF8Char(cmtVector[i].content);
        cmtVector[i].charCount=len;
        if (config.AsyncSpeed){
            long int dur=
            cmtVector.duration=dur;
        }
    }
}
int AssCompressor::_countUTF8Char(const string &str,const long int length){ //done
    int i=0,len,ans=0;
    if (length==0)
        len=str.size();
    else
        len=length;
    while(i<len){
        unsigned char ch=str[i];
        if (ch<0x80){i++;ans++;}
        else if ((ch>>4)==14){i+=3;ans++;}
        else if ((ch>>5)==6){i+=2;ans++;}
        else if ((ch>>3)==30){i+=4;ans++;}
        else i++;
    }
    return ans;
}
void AssCompressor::_crlfReplace(Comment &cmt){}
void AssCompressor::_rewrap(Comment &cmt){
    if(cmt.type!=Top&&cmt.type!=Bottom)return;
    vector<string> lines;
    bool need_rewrap=false;
    __split(cmt.content,"\\N",lines);
    for(int i=0;i<lines.size();i++)
        if(_countUTF8Char(lines[i])*cmt.fontSize>config.DisplayY){
            need_rewrap=true;break;}
    if(!need_rewrap)return;
    long int max_per_line=floor(config.DisplayX/double(cmt.fontSize));
    vector<string> new_lines;


}
static void AssCompressor::__split(const string &str,const char *pattern,vector<string> &v){//done
    v.clean();
    long int last_pos=0,now_pos=string::npos;
    long int pattern_len=strlen(pattern);
    now_pos=str.find(last_pos,pattern);
    string tmp="";
    while(now_pos!=string::npos){
        //last_pos ~ now_pos-1
        tmp="";
        tmp.append(&(str.c_str()[last_pos]),now_pos-last_pos);
        v.append(tmp);
        last_pos=now_pos+pattern_len;
        now_pos=str.find(last_pos,pattern);
    }
    tmp="";
    if(last_pos<str.size())
        tmp.append(&(str.c_str()[last_pos]));
    v.append(tmp);
}
void AssCompress::_moveACCpos(Comment &cmt){}

UTF8Parser::UTF8Parser(const string &s){
    str.clear();
    long long lch=0;
    int i=0,len,ans=0;
    len=s.size();
    while(i<len){
        unsigned char ch=s[i];
        unsigned int l;
        if (ch<0x80)l=1;
        else if ((ch>>4)==14)l=3;
        else if ((ch>>5)==6)l=2
        else if ((ch>>3)==30)l=4;
        else {i++;continue;};
        for(int j=0;j<l;j++)
            lch+=((unsigned char)s[i+j])<<(8*j);
        i+=l;
        str.push_back(lch);
    }
}
static long UTF8Parser::Length(){
    return str.size();
}
static void Split(long max_width,vector<string>& target){
    target.clear();
    long c=1;
    while(Length()/(double)c>=max_width)c++;
}
    long w=
void UTF8Parser::toString(long l,long r,string& target){
    //target="";
    for(long i=l;i<r;i++){
        int len=0;
        if(str[i]&0x80==0)len=1;
        else if(str[i]&0xF0==0xE0)len=3;
        else if(str[i]&0xE0==0xC0)len=2;
        else if(str[i]&0xF8==0xF0)len=4;
        else len=0;
        for(int j=0;j<len)
          target+=char((str[i]>>(8*j)&0xFF);
    }
}
