/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Obj_Loader.hpp"
#include "tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;

namespace Project1_3DA
{
	/**
	* El constructor del modelo se encarga de leer los datos desde el archivo del modelo, y volcar esa información en contenedores que después se manipularán en la escena
	* los principales elementos extraídos del archivo son: Shapes, Vértices, Normales y VertexColors
	*/
	Model::Model(const string & obj_filePath)
	{
		//Ruta del archivo del modelo
		std::string inputfile = obj_filePath;

		//Valores del modelo
		attrib_t           model_attributes;
		vector<shape_t>    model_shapes;
		vector<material_t> model_materials;

		//Cargamos el modelo
		if (!tinyobj::LoadObj(&model_attributes, &model_shapes, &model_materials, &error, inputfile.c_str())) { return; }

		//Validamos el modelo
		if (model_shapes.size() == 0)              { error = string("NO SHAPES IN MODEL"  ) + obj_filePath; return; }
		if (model_attributes.vertices.size() == 0) { error = string("NO VERTICES IN MODEL") + obj_filePath; return; }
		if (model_attributes.normals.size() == 0)  { error = string("NO SHAPES IN MODEL"  ) + obj_filePath; return; }

		//Reajustamos el tamaño de los vectors para Vertices, Normales, Triangulos y VertexColor del modelo
		objVertex     .resize(model_attributes.vertices.size());	
		objNormals    .resize(model_attributes.vertices.size());
		objTriangles  .resize(model_shapes[0].mesh.num_face_vertices.size());
		objVertexColor.resize(model_attributes.vertices.size());

		//Recorremos las formas del modelo (Shapes)
		for (size_t shape = 0; shape < model_shapes.size(); shape++)
		{
			size_t index_offset = 0;
			size_t face_index = 0;

			//Recorremos las caras (Faces)
			for (size_t face = 0; face < model_shapes[shape].mesh.num_face_vertices.size(); face++)
			{
				int faceVertex = model_shapes[shape].mesh.num_face_vertices[face];

				objTriangles[face_index] = Point3i({ 
					model_shapes[shape].mesh.indices[index_offset + 0].vertex_index,
					model_shapes[shape].mesh.indices[index_offset + 1].vertex_index,
					model_shapes[shape].mesh.indices[index_offset + 2].vertex_index });

				//Recorremos vértices por cara (Vertex)
				for(size_t vertex = 0; vertex < faceVertex; vertex++)
				{
					tinyobj::index_t idx = model_shapes[shape].mesh.indices[index_offset + vertex];

					//Valores per vertex
					float vx = model_attributes.vertices[3 * idx.vertex_index + 0];
					float vy = model_attributes.vertices[3 * idx.vertex_index + 1];
					float vz = model_attributes.vertices[3 * idx.vertex_index + 2];

					objVertex[idx.vertex_index] = Point4f({ vx, vy, vz, 1 });

					//color por vertice
					float vr = model_attributes.colors[3 * idx.vertex_index + 0];
					float vg = model_attributes.colors[3 * idx.vertex_index + 1];
					float vb = model_attributes.colors[3 * idx.vertex_index + 2];

					objVertexColor[idx.vertex_index] = Point3f({ vr, vg, vb });

					//Valores de normales
					tinyobj::real_t nx = model_attributes.normals [3 * idx.normal_index + 0];
					tinyobj::real_t ny = model_attributes.normals [3 * idx.normal_index + 1];
					tinyobj::real_t nz = model_attributes.normals [3 * idx.normal_index + 2];

					objNormals[idx.vertex_index] = Point4f({ nx, ny, nz, 0});

					//valores de coordenada de textura
					//tinyobj::real_t tx = model_attributes.texcoords[2 * idx.texcoord_index + 0];
					//tinyobj::real_t ty = model_attributes.texcoords[2 * idx.texcoord_index + 1];

				}
				index_offset += faceVertex;
				face_index++;

				//material por cara
				model_shapes[shape].mesh.material_ids[face];
			}
		}
	}
}