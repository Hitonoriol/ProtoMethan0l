#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 394;
	static const long REVISION  = 2195;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 631;
	#define RC_FILEVERSION 1,6,394,2195
	#define RC_FILEVERSION_STRING "1, 6, 394, 2195\0"
	static const char FULLVERSION_STRING [] = "1.6.394.2195";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 19;
	

}
#endif //VERSION_H
