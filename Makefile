CPP=AssCompressor.cpp AssCompressorCLI.cpp BilibiliCommentManager.cpp
HXX=AssCompressor.h BilibiliCommentManager.h Struct.h RapidXML.h

all:$(CPP) $(HXX)
	g++ -Wall -Wextra -ggdb -pg -O0 $(CPP) -lcurl -o AssCompressorCLI
