#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.08";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 376;
	static const long REVISION  = 2064;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 425;
	#define RC_FILEVERSION 1,0,376,2064
	#define RC_FILEVERSION_STRING "1, 0, 376, 2064\0"
	static const char FULLVERSION_STRING [] = "1.0.376.2064";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
