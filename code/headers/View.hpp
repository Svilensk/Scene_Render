/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef VIEW_HEADER
#define VIEW_HEADER


#include <memory>
#include <vector>
#include <Point.hpp>

#include "Rasterizer.hpp"
#include "Obj_Loader.hpp"
#include "Mesh.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace Project1_3DA
{
	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;
	using toolkit::Point3f;

	class View
	{
	private:
		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;

	private:
		vector<Mesh>               drawable_meshes; /*!< Vector con todas las meshes a dibujar en la escena */
		Color_Buffer               Color_buffer;    /*!< Buffer de píxeles para el rasterizer */
		Rasterizer< Color_Buffer > rasterizer;      /*!< Rasterizador para el dibujado de modelos e imagenes */

		std::auto_ptr< Color_Buffer > image; /*!< Buffer de píxeles para la imágen de fondo*/

		Point3f lightVector; /*!< Vector de iluminación para la escena */

		size_t height; /*!< Alto del viewport */
		size_t width;  /*!< Ancho del viewport*/

	public:

		View(size_t width, size_t height, vector<Mesh> drawable_meshes);

		bool load_image(const char * image_path);
		void update();
		void paint();

	};
}
#endif
