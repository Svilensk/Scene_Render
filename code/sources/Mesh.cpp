/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <cmath>
#include <cassert>
#include "Mesh.hpp"
#include <Vector.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>
#include <Point.hpp>

#include <algorithm>    // std::min

using namespace toolkit;
using toolkit::Point3i;
using toolkit::Point3f;

namespace Project1_3DA
{
	/**
	* El constructor de mesh obtiene valores de un modelo, los guarda y prepara nuevos contenedores y variables para la modificación y dibujado de estos
	*/
	Mesh::Mesh(std::string obj_filePath) : newModel(obj_filePath)
	{
		//Se cargan en un búffer los datos de los vertices del modelo

		//Actualizamos el numero de vertices del mesh segun los vertices del modelo
		size_t number_of_vertices = newModel.objVertex.size();

		//Cargamos los vertices del modelo en la mesh
		original_vertices.resize(number_of_vertices);
		original_vertices = newModel.objVertex;

		//Cargamos las normales del modelo
		original_normals.resize(number_of_vertices);
		original_normals = newModel.objNormals;

		//Ajustamos los contenedores de normales y vertices al tamaño de vertices del modelo
		transformed_normals.resize (number_of_vertices);
		transformed_vertices.resize(number_of_vertices);
		display_vertices.resize    (number_of_vertices);

		//Definimos el numero de colores segun el numero de vertices y ajustamos el tamaño del contenedor de vertices originales y transformados
		size_t number_of_colors = newModel.objVertex.size();
		original_colors.resize(number_of_colors);
		new_colors.resize(number_of_colors);

		//Ajustamos el tamaño de informacion de intensidad por vertice
		intensity_per_vertex.resize(number_of_colors);

		//Pintamos los vertices segun los vertexColors cargados desde el modelo
		for (size_t index = 0; index < number_of_colors; ++index)
		{
			original_colors[index].set(int(newModel.objVertexColor[index][0]*255), int(newModel.objVertexColor[index][1]*255), int(newModel.objVertexColor[index][2]*255));
		}

		//Se generan los índices de los triángulos:
		size_t number_of_triangles = newModel.objTriangles.size();
		original_indices.resize(number_of_triangles * 3);

		//Inicializamos el iterador de indices
		Index_Buffer::iterator indices_iterator = original_indices.begin();

		//Recorremos los triangulos
		for (size_t triangle_index = 0; triangle_index < number_of_triangles; triangle_index++)
		{
			*indices_iterator++ = newModel.objTriangles[triangle_index][0];
			*indices_iterator++ = newModel.objTriangles[triangle_index][1];
			*indices_iterator++ = newModel.objTriangles[triangle_index][2];
		}
	}
	/**
	* Update es el metodo encargado de actualizar la posición de vertices, su iluminación, de manera directa o jerárquicamente
	*/
	void Mesh::update(Point3f lightVector, size_t width, size_t height)
	{
		//Podemos actualizar la matriz de transformación en cada llamada al update
		Projection3f  projection(5, 15, 20, float(width) / float(height));

		// Establecemos escala y transformación por llamada al update
		scaling = Scaling3f(scale);
		translation = Translation3f(translationX, translationY, translationZ);

		//Establecemos la rotación por llamada al update
		rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(angleY);
		rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angleX);

		//Si la mesh tiene padre, calculamos la matriz resultante de dicha union
		if (parent) 
		{
			transformation = translation * rotation_x * rotation_y * scaling;
			ptransformation = parent->translation * parent->rotation_x * parent->rotation_y * parent->scaling;

			transformation = projection * (ptransformation * transformation);
		}

		//Si el modelo no tiene padre calculamos su matriz de transformacion de forma normal
		else transformation = projection * translation * rotation_x * rotation_y * scaling;

		// Se transforman todos los vértices usando la matriz de transformación resultante:
		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
		{			
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:
			Vertex & vertex = transformed_vertices[index] = Matrix44f(transformation) * Matrix41f(original_vertices[index]);
			Vertex & normals = transformed_normals[index] = Matrix44f(transformation) * Matrix41f(original_normals[index]);

			//Calculamos las normales actualizadas y las normalizamos
			transformed_normals[index][0] /= sqrtf(transformed_normals[index][0] * transformed_normals[index][0] + transformed_normals[index][1] * transformed_normals[index][1] + transformed_normals[index][2] * transformed_normals[index][2]);
			transformed_normals[index][1] /= sqrtf(transformed_normals[index][0] * transformed_normals[index][0] + transformed_normals[index][1] * transformed_normals[index][1] + transformed_normals[index][2] * transformed_normals[index][2]);
			transformed_normals[index][2] /= sqrtf(transformed_normals[index][0] * transformed_normals[index][0] + transformed_normals[index][1] * transformed_normals[index][1] + transformed_normals[index][2] * transformed_normals[index][2]);

			//Calculamos la intensidad luminica de cada vértice según el vector de iluminación
			intensity_per_vertex[index] = (transformed_normals[index][0] * lightVector[0]) + (transformed_normals[index][1] * lightVector[1]) + (transformed_normals[index][2] * lightVector[2]);

			//Ajustamos el valor maximo y minimo que puede poseer la iluminación de un vértice
			intensity_per_vertex[index] = std::max(0.3f, std::min(intensity_per_vertex[index], 1.0f));

			//Guardamos los colores actualizados por vétice
			new_colors[index].data.component.r = int(original_colors[index].data.component.r * intensity_per_vertex[index]);
			new_colors[index].data.component.g = int(original_colors[index].data.component.g * intensity_per_vertex[index]);
			new_colors[index].data.component.b = int(original_colors[index].data.component.b * intensity_per_vertex[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:
			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}
	}
	/**
	* Metodo de dibujado del mesh, convirtiendo coordenadas a coordenadas de pantalla (Z se escala a un valor suficientemente grande para el z_buffer)
	*/
	void Mesh::paint(Rasterizer< Color_Buffer >* rasterizer, size_t width, size_t height)
	{

		Scaling3f        scaling = Scaling3f(float(width / 2), float(height / 2), 100000000.f);
		Translation3f    translation = Translation3f(float(width / 2), float(height / 2), 0.f);
		Transformation3f transformation = translation * scaling;

		for (size_t index = 0, number_of_vertices = transformed_vertices.size(); index < number_of_vertices; index++)
		{
			display_vertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(transformed_vertices[index]));

			//Evito la excepción haciendo que las coordenadas de los vértices no salgan de la pantalla
			//Esto es una solución rapida hasta la implementación del algoritmo Shutherland Hodgeman
			if (display_vertices[index].coordinates()[0] >= 1280) display_vertices[index].coordinates()[0] = 1280;
			if (display_vertices[index].coordinates()[1] >= 720) display_vertices[index].coordinates()[1] = 720;

			if (display_vertices[index].coordinates()[0] <= 0) display_vertices[index].coordinates()[0] = 0;
			if (display_vertices[index].coordinates()[1] <= 0) display_vertices[index].coordinates()[1] = 0;
		}

		//shuterland_display_vertices = rasterizer->clip_ShutherlandHodgman(display_vertices, original_indices);

		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (is_frontface(transformed_vertices.data(), indices))
			{
				// Se establece el color del polígono a partir del color de su primer vértice:
				rasterizer->set_color(new_colors[*indices]);

				// Se rellena el polígono:
				rasterizer->fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
			}
		}
	}

	/**
	* Comprobamos si el poligono es frontface para dibujarlo o no (Backface Culling)
	*/
	bool Mesh::is_frontface(const Vertex * const projected_vertices, const int * const indices)
	{
		const Vertex & v0 = projected_vertices[indices[0]];
		const Vertex & v1 = projected_vertices[indices[1]];
		const Vertex & v2 = projected_vertices[indices[2]];

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}

	/**
	* Establecemos la relación jerarquica entre modelos (Pasamos al modelo un puntero a su padre)
	*/
	void Mesh::SetParent(Mesh* child_model)
	{
		this->parent = child_model;
	}

	/**
	* Establecemos los valores de rotación en el eje X del modelo
	*/
	void Mesh::SetAngleX(float value){ angleX = value; }

	/**
	* Establecemos los valores de rotación en el eje Y del modelo
	*/
	void Mesh::SetAngleY(float value){ angleY = value; }

	/**
	* Establecemos los valores de transformación del modelo
	*/
	void Mesh::SetTranslation(float x, float y, float z) { translationX = x; translationY = y; translationZ = z; };

	/**
	* Establecemos los valores de escala del modelo
	*/
	void Mesh::SetScale(float scale) { this->scale = scale; }

}