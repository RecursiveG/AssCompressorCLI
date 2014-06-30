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
 along with this program; if not, write to the Free Sddddsdaoftware
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __AssCompressor_H__
#define __AssCompressor_H__

#include "BilibiliCommentManager.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


using namespace std;
#define AssCompressorVersion (throw runtime_error("CHANGE THE VERSION STRING!!!"))
class AssCompressor{
public:
    AssCompressor(AssConfig&);
    string ErrorMsg,ASS;
#ifdef DEBUG
    void PrintConfig();
#endif
private:
    AssConfig &config;
    vector<Comment> cmtVector;

    void doConvert_Bilibili_XML();
    void InjectHeader();


    void sortVector();
    void yAxisRelocate();
    void ReduceThickness();
    void FurtherCalc();
    static long int _countUTF8Char(const string&,const long int=0);
    void _crlfReplace(Comment &);
    void _rewrapper(Comment &);
    static void __split(const string&,const char*,vector<string>&);
    void _moveACCpos(Comment &);

    static string Comment2ASS(const Comment&);
    static string _track2ASS(const Comment&)
    static string _effectPos(const Comment&);
    static string _effectRoll(const Comment&);
    static string _effectAcc(const Comment&);

    static string Time2Str(long);   //Convert millisecond-based time to ASS format
};

class PixelChannelManager{
public:
    PixelChannelManager(long int);//size
    long FindPlace(long,long);//length,value
private:
    //TODO: Implement Required
    //TODO: Using segment tree?
}

class UTF8Parser(){
public:
    UTF8Parser(const string&);
    static long Length();
    static void Split(long,vector<string>&);
    static getLength(const string&);
private:
    vector<long long> str;
    void toString(long,long,string&);

}
#endif//__AssCompressor_H__
