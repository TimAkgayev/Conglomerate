#pragma once
#include "EngineObject.h"

namespace ObjectReader
{

	//extract an object only if the exact name matches
	EngineObject ExtractByName(std::wstring filename, std::string objName);
	//extracts an object only if the file contains one single object
	EngineObject ExtractSingle(std::wstring filename);
	//return all objects that have the tag word in their name
	std::vector<EngineObject> ExtractByTag(std::wstring filename, std::string tag);

};