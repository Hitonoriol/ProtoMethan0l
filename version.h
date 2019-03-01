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
	static const long MINOR  = 7;
	static const long BUILD  = 149;
	static const long REVISION  = 819;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 241;
	#define RC_FILEVERSION 0,7,149,819
	#define RC_FILEVERSION_STRING "0, 7, 149, 819\0"
	static const char FULLVERSION_STRING [] = "0.7.149.819";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;
	

}
#endif //VERSION_H
