#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;
int World[10][10] =
	{
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
	};

double Convert(double radian)
{
	double pi = 3.14159;
	return (radian * (180 / pi));
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(700, 700), "Prototype!");
	window.setFramerateLimit(60);

	double centerLineDistance=0.0;
	double playerX = 4.0, playerY = 4.0;
	double playerAngle = 0.0;
	double dX, dY, stepY, stepX;
	double playerStepSize = 0.0007;
	sf::Text t;
	sf::Font font;
	font.loadFromFile("Arial.ttf");

	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		dX = ceil(playerX) - playerX;
		dY = playerY - floor(playerY);
		stepX = 0;
		stepY = 0;
		int rayX = 0, rayY = 0;
		if (sf::Event::KeyPressed){

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				playerX += playerStepSize * cos(playerAngle);
				playerY += playerStepSize * sin(playerAngle);
			}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerX -= playerStepSize * cos(playerAngle);
			playerY -= playerStepSize * sin(playerAngle);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerAngle += 0.0001;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerAngle -= 0.0001;

		/*
			*** put a function to keep player angle under 2pie ***

		*/
		/*

			put 4 functions to calculate distances 
			function to represent each quadrants 
			 getDistanceWhenAngleLiesBetween0to90
			 getDistanceWhenAngleLiesBetween90to180
			 getDistanceWhenAngleLiesBetween180to270
			 getDistanceWhenAngleLiesBetween270to360
			4 functions take playerX,playerY,playerAngle 

		*/
		/*while (stepX<=10&&stepY<=10)
		{

			if((dY + stepY) / sin(playerAngle) < (dX + stepX) / cos(playerAngle))
			{
				rayY = playerY-(dY + stepY);
				rayX = playerX+stepX;
				if(World[rayX][rayY])
				{
						centerLineDistance = (dY + stepY) / sin(playerAngle);
						break;
				}
				stepY++;
			}
			else
			{

				rayX = playerX + (dX + stepX);
				rayY = ceil(playerY-stepY);//+(dX+stepX)*tan(playerAngle);
				if (World[rayX][rayY])
					{
						centerLineDistance = (dX + stepX) / cos(playerAngle);
						break;
					}
				stepX++;
			}
		}*/
				}

		string displayText = "X:" + to_string(playerX) + ",Y:" + to_string(playerY) + ",A:" + to_string(Convert(playerAngle)) + /*",nWX:" + to_string(rayX) + ",nWY:" + to_string(rayY)+*/",dist:"+to_string(centerLineDistance);
		t.setString(displayText);
		t.setFont(font);
		t.setCharacterSize(14);
		window.clear();
		window.draw(t);
		window.display();
	}
	return 0;
}
