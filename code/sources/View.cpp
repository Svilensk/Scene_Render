/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <cmath>
#include <cassert>
#include "View.hpp"
#include <Vector.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>
#include <Point.hpp>


using namespace toolkit;
using toolkit::Point3i;

extern "C"
{
#include <targa.h>
}

namespace Project1_3DA
{
	/**
	 * View presenta la información conjunta de la escena, encargada de llamar al update y al paint de todos los modelos presentes, 
	 * ademas puede configurar la posicion, escala o rotacion de cada modelo.
	 */
	View::View(size_t width, size_t height, vector<Mesh> drawable_meshes) : width(width), height(height), drawable_meshes(drawable_meshes), Color_buffer(width, height), rasterizer(Color_buffer) 
	{ 
		//Configuración inicial de la escena, translación, rotación y escala inicial de las mallas
		this->drawable_meshes[0].SetTranslation(0, -1.5f, -10);
		this->drawable_meshes[1].SetTranslation(0, 1.5f, 1);
		this->drawable_meshes[2].SetTranslation(0, 1.f, -4);
		
		//Start: Escala
		this->drawable_meshes[0].SetScale(1.5f);
		this->drawable_meshes[1].SetScale(.05f);
		this->drawable_meshes[2].SetScale(.3f);
		
		//Start: Angle
		this->drawable_meshes[0].SetAngleX(0.f);		
		this->drawable_meshes[1].SetAngleY(.9f);
		
		//Start: Hierarchy
		this->drawable_meshes[1].SetParent(&this->drawable_meshes[0]);		
		this->drawable_meshes[2].SetParent(&this->drawable_meshes[0]);
		
		//Vector de iluminación
		lightVector = Point3f ({-1, 1, 0});
	}

	/**
	* Update actualiza los valores generales de la escena, llama a los metodos set de las diferentes mallas para actualizar su posicion, rotación o escala,
	* para después llamar al método update de cada malla dentro del vector Drawable Meshes
	*/
	void View::update()
	{
		//Actualización en cada frame de las propiedades de cada malla

		//Variables dinámicas
		static float angle;
		static float angle2;

		//Actualización de las mallas
		this->drawable_meshes[0].SetAngleX(angle += 0.002f);
		this->drawable_meshes[1].SetAngleX(angle2 -= 0.01f);

		//Actualización de cada malla
		for( auto& mesh : drawable_meshes)
		{
			mesh.update(lightVector, width, height);
		}
	}
	/**
	* Método encargado de cargar la imágen que se pasa al view, es llamada en cada frame, antes de dibujar las mallas, y vacía el buffer de píxeles en cada llamada,
	* para volver a dibujarlo continuamente
	*/
	bool View::load_image(const char * image_path)
	{
		//Cargamos la imágen
		tga_image loaded_image;

		//Si se ha podido cargar...
		if (tga_read(&loaded_image, image_path) == TGA_NOERR)
		{
			//Limpiamos el buffer previo
			image.reset(new Color_Buffer(loaded_image.width, loaded_image.height));

			//Ajustamos la imágen para su correcta visualización
			tga_convert_depth(&loaded_image, (uint8_t)image->bits_per_color());
			tga_swap_red_blue(&loaded_image);

			//Volcamos el buffer de la imágen al rasterizer
			Color_Buffer::Color * loaded_image_pixels = reinterpret_cast< Color_Buffer::Color * >(loaded_image.image_data);
			Color_Buffer::Color * loaded_image_pixels_end = loaded_image_pixels + loaded_image.width * loaded_image.height;
			Color_Buffer::Color * image_buffer_pixels = image->colors();

			while (loaded_image_pixels <  loaded_image_pixels_end)
			{
				*image_buffer_pixels++ = *loaded_image_pixels++;
			}
			
			//Liberamos el buffer
			tga_free_buffers(&loaded_image);

			return (true);
		}

		else
		{
			rasterizer.clear();

			return (false);
		}
	}

	/**
	 * Metodo de dibujo, llamado cada vez que se quieran mostrar elementos por pantalla, dibuja tanto la imágen de fondo como las mallas
	 */
	void View::paint()
	{
		//Se borra el framebúffer y se dibujan los triángulos:
		rasterizer.clear();

		//Dibujamos el fondo 
		if (image.get())
		{
			rasterizer.draw_image(0, 0, *image);
		}

		//Dibujamos las mallas
		for( auto& mesh : drawable_meshes )
		{
			mesh.paint(&rasterizer, width, height);
		}

		//Se copia el framebúffer oculto en el framebúffer de la ventana:
		rasterizer.get_color_buffer().gl_draw_pixels(0, 0);
	}
}
