/*
 *  // Written By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "View.hpp"
#include "Obj_Loader.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace Project1_3DA;


int main()
{
	//Parametros del tamaño de la ventana
	const size_t width  = 1280; /*!< Parámetro de ANCHO DE PANTALLA */
	const size_t height = 720;  /*!< Parámetro de ALTO DE PANTALLA */

	// Se crea la ventana donde se visualizaran los graficos
	Window window
	(
		VideoMode(width, height),
		"Advanced 3D - 1st Exercise", 
		Style::Titlebar | Style::Close, 
		ContextSettings(32)
	);

	vector<Mesh> drawable_Meshes; /*!< Vector con todas las meshes que se dibujarán */

	// Creamos las meshes
	Mesh plan_Mesh ("../../Assets/sphere.obj"   ); /*!< Mesh que posee el modelo "Planeta"  */
	Mesh sate_Mesh ("../../Assets/Satellite.obj"); /*!< Mesh que posee el modelo "Satelite(Nave)" */
	Mesh moon_Mesh ("../../Assets/Moon.obj"     ); /*!< Mesh que posee el modelo "Luna" */

	// Añadimos a las meshes a dibujar
	drawable_Meshes.push_back(plan_Mesh);
	drawable_Meshes.push_back(sate_Mesh);
	drawable_Meshes.push_back(moon_Mesh);

	View view(width, height, drawable_Meshes); /*!< Creo la vista a renderizar  */

	// Se carga la imágen de fondo
	view.load_image("../../Assets/background.tga");

	
	// Inicializamos componentes OPENGL y los deshabilitamos (por requisitos de la practica) */
	window.setVerticalSyncEnabled(true);
	glDisable(GL_BLEND);
	glDisable(GL_DITHER);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// Preparamos el viewport para la visualización
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GLdouble(width), 0, GLdouble(height), -1, 1);

	// Bucle principal de ejecucion
	bool running = true;
	do
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				running = false;
			}
		}

		// Se actualiza la visualizacion
		view.update();

		// Se dibuja la visualizacion (Tras actualizarla)
		view.paint();

		// Se muestra el buffer dibujado en la ventana
		window.display();

	} while (running);
	return (EXIT_SUCCESS);
}