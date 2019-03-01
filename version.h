#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 8;
	static const long BUILD  = 153;
	static const long REVISION  = 838;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 246;
	#define RC_FILEVERSION 0,8,153,838
	#define RC_FILEVERSION_STRING "0, 8, 153, 838\0"
	static const char FULLVERSION_STRING [] = "0.8.153.838";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 3;
	

}
#endif //VERSION_H
