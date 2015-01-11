#ifndef TMXSCENEEENCODER_H_
#define TMXSCENEEENCODER_H_

#include <tinyxml2.h>

#include "Base.h"
#include "StringUtil.h"

/**
 * Class for encoding an TMX file.
 */
class TMXSceneEncoder
{
public:
	/**
	* Constructor.
	*/
	TMXSceneEncoder();

	/**
	* Destructor.
	*/
	~TMXSceneEncoder();

	/**
	* Writes out encoded FBX file.
	*/
	void write(const std::string& filepath);
};

#endif
