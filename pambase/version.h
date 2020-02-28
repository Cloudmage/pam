#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.02";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 291;
	static const long REVISION  = 1616;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 313;
	#define RC_FILEVERSION 1,0,291,1616
	#define RC_FILEVERSION_STRING "1, 0, 291, 1616\0"
	static const char FULLVERSION_STRING [] = "1.0.291.1616";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
