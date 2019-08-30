/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <vector>
#include <string>
#include <Point.hpp>

namespace Project1_3DA
{
	using std::vector;
	using toolkit::Point4f;
	using toolkit::Point3i;
	using toolkit::Point3f;

	class Model
	{
		std::string error; /*!< String con errores para la depuracion */

	public:

		//El constructor requiere de una ruta al modelo obligatoriamente
		Model( const std::string & model_filePath );

		//Conjuntos del modelo 
		vector< Point4f > objVertex;    /*!< Vector que alberga los valores de cada vertice del modelo */
		vector< Point4f > objNormals;   /*!< Vector que guarda cada valor de las normales de cada vertice del modelo */
		vector< Point3i > objTriangles; /*!< Vector que posee grupos de tres vectores que forman un triángulo*/

		vector< Point3f > objVertexColor; /*!< Vector que posee el color de cada vértice del modelo*/
	};
}
#endif // !TINYOBJLOADER_IMPLEMENTATION
