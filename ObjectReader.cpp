#include "ObjectReader.h"
#include "tiny_obj_loader.h"
#include "Utility.h"

namespace ObjectReader
{

	struct _object
	{
		tinyobj::attrib_t m_attributes;
		std::vector<tinyobj::shape_t> m_shapes;
		std::vector<tinyobj::material_t> m_materials;
	};


	class index_comparator
	{
	public:
		bool operator()(const tinyobj::index_t& lhv, const tinyobj::index_t& rhv) const
		{
			return std::tie(lhv.vertex_index, lhv.normal_index, lhv.texcoord_index) < std::tie(rhv.vertex_index, rhv.normal_index, rhv.texcoord_index);
		}
	};

	EngineObject ExtractByName(std::wstring filename, std::string objName)
	{
		EngineObject worldObject;
		_object obj;
		std::wstring warn;
		std::string err;
		std::wstring base_dir = Utility::GetBaseDir(filename);
		tinyobj::LoadObj(& obj.m_attributes, & obj.m_shapes, & obj.m_materials, & warn, & err, filename.c_str(), base_dir.c_str());

		// Loop over shapes and extract each object
		for (unsigned int s = 0; s < obj.m_shapes.size(); s++)
		{
			if (obj.m_shapes[s].name.compare(objName) != 0)
			{
				std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

				//go through each index and find unique entries
				for (tinyobj::index_t i : obj.m_shapes[s].mesh.indices)
					uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

				Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

				for (auto& keyval : uniqueVertexMap)
				{
					tinyobj::real_t vx = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 0];
					tinyobj::real_t vy = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 1];
					tinyobj::real_t vz = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 2];

					tinyobj::real_t nx = obj.m_attributes.normals[3 * keyval.first.normal_index + 0];
					tinyobj::real_t ny = obj.m_attributes.normals[3 * keyval.first.normal_index + 1];
					tinyobj::real_t nz = obj.m_attributes.normals[3 * keyval.first.normal_index + 2];

					tinyobj::real_t tx = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 0];
					tinyobj::real_t ty = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 1];

					// Optional: vertex colors
					tinyobj::real_t red = obj.m_attributes.colors[3 * keyval.first.vertex_index + 0];
					tinyobj::real_t green = obj.m_attributes.colors[3 * keyval.first.vertex_index + 1];
					tinyobj::real_t blue = obj.m_attributes.colors[3 * keyval.first.vertex_index + 2];

					//
					// per-face material
					//shapes[s].mesh.material_ids[f];

					Vertex vertex;

					vertex.pos.x = vx;
					vertex.pos.y = vy;
					vertex.pos.z = vz;

					vertex.norm.x = nx;
					vertex.norm.y = ny;
					vertex.norm.z = nz;

					vertex.uv.x = tx;
					vertex.uv.y = ty;

					vertex.color.x = red;
					vertex.color.y = green;
					vertex.color.z = blue;

					vlist_temp[keyval.second] = vertex;
				}


				//copy over the vertex list
				worldObject.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

				//now re-index the old index list
				for (tinyobj::index_t i : obj.m_shapes[s].mesh.indices)
					worldObject.index_list.push_back(uniqueVertexMap[i]);

				//save the texture path
				if (obj.m_materials.size())
					worldObject.texture_path = Utility::StringToWString(obj.m_materials[obj.m_shapes[s].mesh.material_ids[0]].diffuse_texname);
				else
					worldObject.texture_path = L"standin_texture.jpg";


				delete[] vlist_temp;

				return worldObject;
			}

		}

		return worldObject;

	}

	EngineObject ExtractSingle(std::wstring filename)
	{
		EngineObject engineObject;
		_object obj;
		std::wstring warn;
		std::string err;
		std::wstring base_dir = Utility::GetBaseDir(filename);
		tinyobj::LoadObj(& obj.m_attributes, & obj.m_shapes, & obj.m_materials, & warn, & err, filename.c_str(), base_dir.c_str());

		// extract only if there is exactly one object in the file
		if (obj.m_shapes.size() == 1)
		{

			std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

			//go through each index and find unique entries
			for (tinyobj::index_t i : obj.m_shapes[0].mesh.indices)
				uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

			Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

			for (auto& keyval : uniqueVertexMap)
			{
				tinyobj::real_t vx = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t vy = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t vz = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 2];

				tinyobj::real_t nx = obj.m_attributes.normals[3 * keyval.first.normal_index + 0];
				tinyobj::real_t ny = obj.m_attributes.normals[3 * keyval.first.normal_index + 1];
				tinyobj::real_t nz = obj.m_attributes.normals[3 * keyval.first.normal_index + 2];

				tinyobj::real_t tx = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 0];
				tinyobj::real_t ty = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 1];

				// Optional: vertex colors
				tinyobj::real_t red = obj.m_attributes.colors[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t green = obj.m_attributes.colors[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t blue = obj.m_attributes.colors[3 * keyval.first.vertex_index + 2];

				//
				// per-face material
				//shapes[s].mesh.material_ids[f];

				Vertex vertex;

				vertex.pos.x = vx;
				vertex.pos.y = vy;
				vertex.pos.z = vz;

				vertex.norm.x = nx;
				vertex.norm.y = ny;
				vertex.norm.z = nz;

				vertex.uv.x = tx;
				vertex.uv.y = ty;

				vertex.color.x = red;
				vertex.color.y = green;
				vertex.color.z = blue;

				vlist_temp[keyval.second] = vertex;
			}

			//copy over the vertex list
			engineObject.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

			//now re-index the old index list
			for (tinyobj::index_t i : obj.m_shapes[0].mesh.indices)
				engineObject.index_list.push_back(uniqueVertexMap[i]);

			//save the texture path
			if (obj.m_materials.size())
				engineObject.texture_path = Utility::StringToWString(obj.m_materials[obj.m_shapes[0].mesh.material_ids[0]].diffuse_texname);
			else
				engineObject.texture_path = L"standin_texture.jpg";

			//remove the old pointer
			delete[] vlist_temp;

			return engineObject;


		}

		return engineObject;
	}

	std::vector<EngineObject> ExtractByTag(std::wstring filename, std::string tag)
	{
		std::vector<EngineObject> o_list;
		_object obj;
		std::wstring warn;
		std::string err;
		std::wstring base_dir = Utility::GetBaseDir(filename);
		tinyobj::LoadObj(&obj.m_attributes, &obj.m_shapes, &obj.m_materials, &warn, &err, filename.c_str(), base_dir.c_str());

		// Loop over shapes and extract each object
		for (unsigned int s = 0; s < obj.m_shapes.size(); s++)
		{
			if (obj.m_shapes[s].name.find(tag) != std::string::npos)
			{
				std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

				//go through each index and find unique entries
				for (tinyobj::index_t i : obj.m_shapes[s].mesh.indices)
					uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

				Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

				for (auto& keyval : uniqueVertexMap)
				{
					tinyobj::real_t vx = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 0];
					tinyobj::real_t vy = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 1];
					tinyobj::real_t vz = obj.m_attributes.vertices[3 * keyval.first.vertex_index + 2];

					tinyobj::real_t nx = obj.m_attributes.normals[3 * keyval.first.normal_index + 0];
					tinyobj::real_t ny = obj.m_attributes.normals[3 * keyval.first.normal_index + 1];
					tinyobj::real_t nz = obj.m_attributes.normals[3 * keyval.first.normal_index + 2];

					tinyobj::real_t tx = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 0];
					tinyobj::real_t ty = obj.m_attributes.texcoords[2 * keyval.first.texcoord_index + 1];

					// Optional: vertex colors
					tinyobj::real_t red = obj.m_attributes.colors[3 * keyval.first.vertex_index + 0];
					tinyobj::real_t green = obj.m_attributes.colors[3 * keyval.first.vertex_index + 1];
					tinyobj::real_t blue = obj.m_attributes.colors[3 * keyval.first.vertex_index + 2];

					//
					// per-face material
					//shapes[s].mesh.material_ids[f];

					Vertex vertex;

					vertex.pos.x = vx;
					vertex.pos.y = vy;
					vertex.pos.z = vz;

					vertex.norm.x = nx;
					vertex.norm.y = ny;
					vertex.norm.z = nz;

					vertex.uv.x = tx;
					vertex.uv.y = ty;

					vertex.color.x = red;
					vertex.color.y = green;
					vertex.color.z = blue;

					vlist_temp[keyval.second] = vertex;
				}

				EngineObject worldObject; 

				worldObject.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

				//now re-index the old index list
				for (tinyobj::index_t i : obj.m_shapes[s].mesh.indices)
					worldObject.index_list.push_back(uniqueVertexMap[i]);

				//save the texture path
				if (obj.m_materials.size())
					worldObject.texture_path = Utility::StringToWString(obj.m_materials[obj.m_shapes[s].mesh.material_ids[0]].diffuse_texname);
				else
					worldObject.texture_path = L"standin_texture.jpg";

				o_list.push_back(worldObject);

				//delete the temporary vertex list;
				delete[] vlist_temp;
			}

		}

		return o_list;
	}




};