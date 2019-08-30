/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef MESH_HEADER
#define MESH_HEADER

#include <vector>
#include <Point.hpp>
#include "Obj_Loader.hpp"
#include "Rasterizer.hpp"
#include "Obj_Loader.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include <Projection.hpp>
#include <Rotation.hpp>
#include <Translation.hpp>
#include <Scaling.hpp>


namespace Project1_3DA
{
	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;
	using toolkit::Transformation3f;
	using namespace toolkit;

	class Mesh
	{
	private:

		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;
		typedef Point4f               Vertex;
		typedef vector< Vertex >      Vertex_Buffer;
		typedef vector< int    >      Index_Buffer;
		typedef vector< Color  >      Vertex_Colors;

	private:

		Model newModel;

		Vertex_Buffer     original_vertices; /*!< Buffer con los vertices originales del modelo      */
		Vertex_Buffer     original_normals;  /*!< Buffer con las normales originales del modelo      */
		Index_Buffer      original_indices;  /*!< Buffer con los indices originales  del modelo      */
		Vertex_Colors     original_colors;   /*!< Buffer con los vertexColors originales del modelo  */

		Vertex_Colors     new_colors;           /*!< Buffer que guardará los vertexColors actualizados tras aplicar la iluminación  */
		Vertex_Buffer     transformed_vertices; /*!< Buffer que guardará los vertices transformados tras aplicar actualizaciones de transformación, rotacion o escala  */
		Vertex_Buffer     transformed_normals;  /*!< Buffer que guardará las normales tras aplicar actualizaciones de transformación, rotacion o escala  */

		vector< Point4i > display_vertices;  /*!< Buffer de puntos transformados a coordenadas XY de pantalla  */

		//vector< Point4i > shuterland_display_vertices;

		vector<float> intensity_per_vertex;  /*!< Vector que guardará la intensidad de iluminación por vértice  */

		Mesh* parent = null; /*!< Puntero a una Mesh padre (Jerarquía de transformaciones)  */

		float angleX = 0; /*!< valor de rotación en el eje X del modelo */
		float angleY = 0; /*!< valor de rotación en el eje Y del modelo */
		float scale;  /*!< valor de escala del modelo  */

		float translationX;  /*!< Posición en el eje X  */
		float translationY;  /*!< Posición en el eje Y  */
		float translationZ;  /*!< Posición en el eje Z  */

	public:

		Mesh(std::string obj_filePath);

		Transformation3f transformation;  /*!< Matriz de transformación del modelo */
		Transformation3f ptransformation; /*!< Matriz de transformación del padre del modelo */

		Rotation3f    rotation_x;  /*!< Matriz de transformación de rotación del modelo en X */
		Rotation3f    rotation_y;  /*!< Matriz de transformación de rotación del modelo en Y */
		Translation3f translation; /*!< Matriz de transformación del modelo en X */
		Scaling3f     scaling;     /*!< Matriz de escala del modelo en X */

		void update(Point3f lightVector ,size_t width, size_t height);
		void paint(Rasterizer< Color_Buffer >* rasterizer, size_t width, size_t height);

		void SetParent(Mesh* child_model);

		void SetAngleX(float value);
		void SetAngleY(float value);
		void SetTranslation(float x, float y, float z);
		void SetScale(float scale);


	private:
		bool is_frontface(const Vertex * const projected_vertices, const int * const indices);
	};
}

#endif
