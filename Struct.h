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
#ifndef AssCompressor_Struct_h
#define AssCompressor_Struct_h

#include <string>

using std::string;

struct AssConfig{//The Configure Struct
    string XMLContent,XMLUrl,ContentType,ExportLocation;
    bool ACCEnable,TOPEnable,BOTEnable,ROLEnable,TRKEnable;
    int AlphaTOP,AlphaBOT,AlphaROL,AlphaACC,AlphaTRK;
    long int layerACC,layerTOP,layerBOT,layerROL,layerTRK;
    long int DisplayX,DisplayY;
    long int iframeX,iframeY;
    string FontNameHei,FontNameYouYuan,FontNameSong,
           FontNameKai,FontNameMSHei;
    bool MultilineComment,MultilineCompare;
    bool FollowACCFontName;
    bool Bold,ACCAutoAdjust,ACCFixRotate;
    double MovingVelocity,DynamicLengthRatio;
    long int DefaultFontSize;
    string DefaultFontName;
    string VideoRatio;
    bool AsyncSpeed;          //true?different speed:same speed
    long int SpeedFactor;     //Async?pixel per second:time to cross the whole screen
    //all time related value are millisecond aka 0.001 seconds
    bool CmtReWrap;           //Re-wrap TOP/BOTTOM comments if they have lines
                              //longer than screen width
};
enum CommentType{
    Rolling,        //A comment rolls from right to left
    ReverseRolling, //A comment rolls from left to right
    Top,Bottom,     //Comments fixed in the center of the top/bottom of the screen
    Accurate,       //Comments with lots of features
    Track,          //Moving along a track
    Code            //Comments with Javascript, will *NEVER* support
}
enum PoolType{
    NormalPool,
    SubtitlePool
}
struct CommentExtra{//Extra Infomation for Accurate Comment
                     //Seperated in order to save memory
    unsigned char alphaOut;          //Use together with fontColor.A
    long int xRotate,yRotate,zRotate;//
    bool acceleration;               //true?accelerated:uniform
    long int pauseTime,movingTime;
    string archer,supplement;
}
struct ColorStruct{
    unsigned char R,G,B,A; //0~255
}
struct Comment{
    CommentType type;
    string content,;
    long int time,duration;          //offset to the start of the video
    long int lineNumber,lineLength;
    long int xStart,yStart,xEnd,yEnd;
    long int charCount;

    string fontName;
    long int fontSize;
    ColorStruct fontColor,borderColor;

    long int sentTime;               //Unix timestamp?
    PoolType pool

    CommentExtra *accExtra;
    long int rightIn,rightOut,leftIn,leftOut; //Extra info for rolling comments(Time)
};

#endif
