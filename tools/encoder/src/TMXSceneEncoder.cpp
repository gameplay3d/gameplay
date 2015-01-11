#include <string>

#include "TMXSceneEncoder.h"

using namespace gameplay;
using namespace tinyxml2;
using std::string;

TMXSceneEncoder::TMXSceneEncoder()
{
}

TMXSceneEncoder::~TMXSceneEncoder()
{
}

void TMXSceneEncoder::write(const string& filepath)
{
	XMLDocument xmlDoc;
	XMLError err;
	if ((err = xmlDoc.LoadFile(filepath.c_str())) != XML_NO_ERROR)
	{
		LOG(1, "Call to XMLDocument::LoadFile() failed.\n");
		LOG(1, "Error returned: %d\n\n", err);
		exit(-1);
	}

	//TODO
}
