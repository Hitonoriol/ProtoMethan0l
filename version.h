#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 2;
	static const long BUILD  = 7;
	static const long REVISION  = 39;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 57;
	#define RC_FILEVERSION 0,2,7,39
	#define RC_FILEVERSION_STRING "0, 2, 7, 39\0"
	static const char FULLVERSION_STRING [] = "0.2.7.39";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 7;
	

}
#endif //VERSION_H
