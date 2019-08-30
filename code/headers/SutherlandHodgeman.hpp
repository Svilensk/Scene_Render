/*

	ESTE CÓDIGO ES UNA APROXIMACIÓN AL ALGORITMO SUTHERLAND HODGEMAN, NO ESTÁ IMPLEMENTADO, NI TAMPOCO RESUELTO
	AÑADO ESTE CÓDIGO COMO INFORMACIÓN PARA FUTURAS VERSIONES A CARÁCTER PERSONAL EN LAS CUALES ME GUSTARÍA RESOLVERLO
	PUESTO QUE POSEE ALGUNOS ELEMENTOS QUE SIRVEN COMO INICIO.

	--------------------------------
	hpp
		//int x_lineIntersect(int limit_X0, int limit_Y0, int limit_X1, int limit_Y1, int line_X0, int line_Y0, int line_X1, int line_Y1);
		//int y_lineIntersect(int limit_X0, int limit_Y0, int limit_X1, int limit_Y1, int line_X0, int line_Y0, int line_X1, int line_Y1);

		//std::vector< Point4i > clip(int* index, int poly_points[][2], int &poly_size, int x1, int y1, int x2, int y2);
		//std::vector< Point4i > clip_ShutherlandHodgman(std::vector< Point4i > poly_points, std::vector< int > indices);

	--------------------------------
	cpp


	//USADO PARA SUTHERLAND HODGEMAN
	//Devuelve el punto y de interseccion dadas dos líneas
	template< class  COLOR_BUFFER_TYPE >
	int Rasterizer< COLOR_BUFFER_TYPE >::x_lineIntersect(int limit_X0, int limit_Y0, int limit_X1, int limit_Y1, int line_X0, int line_Y0, int line_X1, int line_Y1)
	{
		return (limit_X0 * limit_Y1 - limit_Y0 * limit_X1) * (line_X0 - line_X1) - (limit_X0 - limit_X1) * (line_X0 * line_Y1 - line_Y0 * line_X1) /
			(limit_X0 - limit_X1) * (line_Y0 - line_Y1) - (limit_Y0 - limit_Y1) * (line_X0 - line_X1);

	}

	//USADO PARA SUTHERLAND HODGEMAN
	//Devuelve el punto y de interseccion dadas dos líneas
	template< class  COLOR_BUFFER_TYPE >
	int Rasterizer< COLOR_BUFFER_TYPE >::y_lineIntersect(int limit_X0, int limit_Y0, int limit_X1, int limit_Y1, int line_X0, int line_Y0, int line_X1, int line_Y1)
	{
		return (limit_X0 * limit_Y1 - limit_Y0 * limit_X1) * (line_Y0 - line_Y1) - (limit_Y0 - limit_Y1) * (line_X0 * line_X0 - line_Y0 * line_X1) /
			(limit_X0 - limit_X1) * (line_Y0 - line_Y1) - (limit_Y0 - limit_Y1) * (line_X0 - line_X1);
	}

	//USADO PARA SUTHERLAND HODGEMAN
	template< class  COLOR_BUFFER_TYPE >
	std::vector< Point4i > Rasterizer< COLOR_BUFFER_TYPE >::clip(int* index, int poly_points[][2], int &poly_size, int x1, int y1, int x2, int y2)
	{
		int new_poly_size = 0;

		std::vector< Point4i > new_points;
		std::vector< int    > indexes;

		// (ix,iy),(kx,ky) are the co-ordinate values of 
		// the points 
		for (int i = 0; i < 3; i++)
		{
			// i and k form a line in polygon 
			int k = (i + 1) % 3;
			int ix = poly_points[i][0], iy = poly_points[i][1];
			int kx = poly_points[k][0], ky = poly_points[k][1];

			// Calculating position of first point 
			// w.r.t. clipper line 
			int i_pos = (x2 - x1) * (iy - y1) - (y2 - y1) * (ix - x1);

			// Calculating position of second point 
			// w.r.t. clipper line 
			int k_pos = (x2 - x1) * (ky - y1) - (y2 - y1) * (kx - x1);

			// Case 1 : When both points are inside 
			if (i_pos < 0 && k_pos < 0)
			{
				//Only second point is added 
				new_points.push_back(Point4i({ kx, ky,0,0 }));
				indexes.push_back(*index + k);
			}

			// Case 2: When only first point is outside 
			else if (i_pos >= 0 && k_pos < 0)
			{
				// Point of intersection with edge and the second point is added 

				//new_points.push_back(Point4i({ Rasterizer< COLOR_BUFFER_TYPE >::x_lineIntersect(x1, y1, x2, y2, ix, iy, kx, ky), Rasterizer< COLOR_BUFFER_TYPE >::y_lineIntersect(x1, y1, x2, y2, ix, iy, kx, ky) }));	
				//new_index.push_back((*index) +1);

				//new_points.push_back(Point4i({ kx, ky }));		
				//new_index.push_back(*index + k);
			}

			// Case 3: When only second point is outside 
			else if (i_pos < 0 && k_pos >= 0)
			{
				//Only point of intersection with edge is added 
				//new_points.push_back(Point4i({ Rasterizer< COLOR_BUFFER_TYPE >::x_lineIntersect(x1, y1, x2, y2, ix, iy, kx, ky), Rasterizer< COLOR_BUFFER_TYPE >::y_lineIntersect(x1, y1, x2, y2, ix, iy, kx, ky) }));
				//new_index.push_back(*index + i);
			}

			// Case 4: When both points are outside 
			else
			{
				//No points are added 
			}
		}


		return new_points;
	}

	//USADO PARA SUTHERLAND HODGEMAN
	template< class  COLOR_BUFFER_TYPE >
	std::vector< Point4i > Rasterizer< COLOR_BUFFER_TYPE >::clip_ShutherlandHodgman(std::vector< Point4i > poly_points, std::vector< int > indices)
	{
		new_poly.clear();
		new_index.clear();
		//i and k are two consecutive indexes 
		int clip_points[][2] = { {0,720}, {0,0}, {1280,720}, {1280,0} };
		int clipper_size = 4;
		int poly_size = 3;
		std::vector< Point4i > new_points;

		for (int h = 0; h < indices.size(); h += 3)
		{
			int ind_1 = indices[h];
			int ind_2 = indices[h + 1];
			int ind_3 = indices[h + 2];

			int points[3][2] = {
			{poly_points[ind_1].coordinates()[0], poly_points[ind_1].coordinates()[1]},
			{poly_points[ind_2].coordinates()[0], poly_points[ind_2].coordinates()[1]},
			{poly_points[ind_3].coordinates()[0], poly_points[ind_3].coordinates()[1]} };

			for (int i = 0; i < clipper_size; i++)
			{
				int k = (i + 1) % clipper_size;

				new_points = clip(&indices[h], points, poly_size, clip_points[i][0], clip_points[i][1], clip_points[k][0], clip_points[k][1]);

				points[0][0] = new_points.at(0).coordinates()[0];
				points[0][1] = new_points.at(0).coordinates()[1];

				points[1][0] = new_points.at(1).coordinates()[0];
				points[1][1] = new_points.at(1).coordinates()[1];

				points[2][0] = new_points.at(2).coordinates()[0];
				points[2][1] = new_points.at(2).coordinates()[1];
			}

			new_poly.push_back(Point4i({ points[2][0], points[2][1] ,0,1 }));
			new_poly.push_back(Point4i({ points[0][0], points[0][1] ,0,1 }));
			new_poly.push_back(Point4i({ points[1][0], points[1][1] ,0,1 }));

		}
		//new_index;
		return new_poly;
	}



*/