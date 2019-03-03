#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 10;
	static const long BUILD  = 214;
	static const long REVISION  = 1192;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 349;
	#define RC_FILEVERSION 0,10,214,1192
	#define RC_FILEVERSION_STRING "0, 10, 214, 1192\0"
	static const char FULLVERSION_STRING [] = "0.10.214.1192";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
