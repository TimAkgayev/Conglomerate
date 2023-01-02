#pragma once
#include "Vertex.h"
#include <string>
#include <vector>


class EngineObject
{
public:
	std::vector<Vertex> vertex_list;
	std::vector<UINT> index_list;
	std::wstring texture_path;
	unsigned int effect_id;
	unsigned int object_id;
};
