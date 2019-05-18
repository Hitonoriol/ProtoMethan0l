#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 9;
	static const long BUILD  = 460;
	static const long REVISION  = 2547;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 725;
	#define RC_FILEVERSION 1,9,460,2547
	#define RC_FILEVERSION_STRING "1, 9, 460, 2547\0"
	static const char FULLVERSION_STRING [] = "1.9.460.2547";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 10;
	

}
#endif //VERSION_H
