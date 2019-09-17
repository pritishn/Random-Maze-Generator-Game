#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#define pi 3.1415926535897932384626
using namespace std;

int World[10][10] =
	{
		{1,1,1,1,1,1,1,1,1,1},
		{1,1,0,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,1},
		{1,0,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,1,0,1},
		{1,0,0,0,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1}
	};

int main()
{

	sf::RenderWindow window(sf::VideoMode(400, 400), "Prototype!");
	int h=400;
	window.setFramerateLimit(45);
	string c;
	int rayX = 0, rayY = 0;
	double centerLineDistance=0.0;
	double playerX = 4.0, playerY = 4.0;
	double playerAngle = 0.0;
	
	double playerStepSize = 0.04;
	sf::Text t;
	sf::Font font;
	font.loadFromFile("Arial.ttf");

	
	while (window.isOpen())
	{
		
		//app window close events ***************************************************************************
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//app window close events end here ******************************************************************


		// keyboard input events....*********************************************************************
		if (sf::Event::KeyPressed){
			window.clear();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (World[(int)(playerX + playerStepSize * cos(playerAngle))][(int)playerY] == 1)
					playerX += 0;
				else
					playerX += playerStepSize * cos(playerAngle);
				if (World[(int)playerX][(int)(playerY + playerStepSize * sin(playerAngle))] == 1)
					playerY += 0;
				else
					playerY += playerStepSize * sin(playerAngle);
			}
	 		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if(World[(int)(playerX - playerStepSize * cos(playerAngle))][(int)playerY]==1)
				playerX -=0;
				else
				playerX -= playerStepSize * cos(playerAngle);

				if (World[(int)playerX][(int)(playerY - playerStepSize * sin(playerAngle))] == 1)
					playerY -= 0;
				else
					playerY -= playerStepSize * sin(playerAngle);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				playerAngle -= 0.03;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				playerAngle += 0.03;
		// keyboard input events end here....**************************************************************



		for(int i=0;i<700;i++)
		{

			double direX =cos(playerAngle), direY=sin(playerAngle);
			double rotatingAngle=(pi)/1200.00;
			double dirX=cos(rotatingAngle*i)*direX-sin(rotatingAngle*i)*direY;

			double dirY = cos(rotatingAngle*i) * direY + sin(rotatingAngle*i) * direX;
			int mapX = int(playerX);
			int mapY = int(playerY);
			double rayDirX = dirX ; //planeX * cameraX;
			double rayDirY = dirY ;// planeY * cameraX;

			//length of ray from current position to next x or y-side
			double sideDistX;
			double sideDistY;

			//length of ray from one x or y-side to next x or y-side
			double deltaDistX = abs(1 / dirX);
			double deltaDistY = abs(1 / dirY);	
			double perpWallDist;

			//what direction to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit = 0; //was there a wall hit?
			int side;	//was a NS or a EW wall hit?
			//calculate step and initial sideDist
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (playerX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (playerY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
			}
			//perform DDA
			while (hit == 0)
			{
				//jump to next map square, OR in x-direction, OR in y-direction
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				//Check if ray has hit a wall
				if (World[mapX][mapY] > 0)
					hit = 1;
			}
			//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
			if (side == 0)
				perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
			else
				perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
		
			int lineHeight = (int)(h / perpWallDist);

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + h / 2;
			if (drawEnd >= h)
				drawEnd = h - 1;

			sf::VertexArray triangle(sf::Triangles, 2);

			sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(i, drawStart)),
					sf::Vertex(sf::Vector2f(i, drawEnd))};
			
			if(side==1){line[0].color = sf::Color(255,0,0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				line[1].color = sf::Color(255, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
			}
			else
			{
				line[0].color = sf::Color(128, 0, 0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				line[1].color = sf::Color(128, 0, 0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
			}

			sf::Vertex floor[] =
				{sf::Vertex(sf::Vector2f(i, drawEnd), sf::Color(0, 0, 255, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h))),sf::Vertex(sf::Vector2f(i, h), sf::Color(0, 0, 255, 255 ))};

			sf::Vertex ceilings[] =
				{sf::Vertex(sf::Vector2f(i, 0), sf::Color(128, 128, 128, 255)),sf::Vertex(sf::Vector2f(i, drawStart), sf::Color(128, 128,128, 255))};
			window.draw(ceilings, 2, sf::Lines);
			window.draw(floor, 2, sf::Lines);
			window.draw(line, 2, sf::Lines);
		}
			window.display();
	}}
	return 0;
}
