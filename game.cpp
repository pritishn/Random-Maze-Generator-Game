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

class  Player
{
public:
	double X,Y,Angle,StepSize;

	Player(double playerX,double playerY,double playerAngle,double playerStepSize)
	{
		X=playerX;
		Y=playerY;
		Angle=playerAngle;
		StepSize=playerStepSize;
	}

	 
};
class  Ray
{
	public:
	double X,Y;

	Ray(int rayX=0.0,int rayY=0.0)
	{
		X=rayX;Y=rayY;
	}
	
	
};

int main(void)
{

	const int h=800;
	sf::RenderWindow window(sf::VideoMode(h, 3*h/4), "Test Render");
	window.setFramerateLimit(45);

	string c;
	// int ray.X = 0, ray.Y = 0;
	// double player.X = 4.0, player.Y = 4.0;
	// double player.Angle = 0.0;
	
	// double player.StepSize = 0.04;
	//Player(double player.X,double player.Y,double player.Angle,double player.StepSize)
	Player player(4.0,4.0,0.0,0.04);
	Ray ray(0.0,0.0);

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
				if (World[(int)(player.X + player.StepSize * cos(player.Angle))][(int)player.Y] == 1)
					player.X += 0;
				else
					player.X += player.StepSize * cos(player.Angle);
				if (World[(int)player.X][(int)(player.Y + player.StepSize * sin(player.Angle))] == 1)
					player.Y += 0;
				else
					player.Y += player.StepSize * sin(player.Angle);
			}
	 		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if(World[(int)(player.X - player.StepSize * cos(player.Angle))][(int)player.Y]==1)
				player.X -=0;
				else
				player.X -= player.StepSize * cos(player.Angle);

				if (World[(int)player.X][(int)(player.Y - player.StepSize * sin(player.Angle))] == 1)
					player.Y -= 0;
				else
					player.Y -= player.StepSize * sin(player.Angle);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.Angle -= 0.03;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				player.Angle += 0.03;
		// keyboard input events end here....**************************************************************



		for(int i=0;i<h;i++)
		{

			double direX =cos(player.Angle), direY=sin(player.Angle);
			double rotatingAngle=(pi)/((double)(h*4));
			double dirX=cos(rotatingAngle*i)*direX-sin(rotatingAngle*i)*direY;

			double dirY = cos(rotatingAngle*i) * direY + sin(rotatingAngle*i) * direX;
			int mapX = int(player.X);
			int mapY = int(player.Y);
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
				sideDistX = (player.X - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - player.X) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (player.Y - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - player.Y) * deltaDistY;
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
				perpWallDist = (mapX - player.X + (1 - stepX) / 2) / rayDirX;
			else
				perpWallDist = (mapY - player.Y + (1 - stepY) / 2) / rayDirY;
		
			int lineHeight = (int)(h / perpWallDist);

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + h / 2;
			if (drawEnd >= h)
				drawEnd = h - 1;


//---------------------------------------- after determining distance from wall hit  , creating illusion of 3D by rendering proportionately tall lines --------------------------- 
			// rendering lines using the help of line objects  
			// for rendering walls
			sf::Vertex line[] =
			{
					sf::Vertex(sf::Vector2f(i, drawStart)),
					sf::Vertex(sf::Vector2f(i, drawEnd))
			};
			
			//chooses different shades of colours for adjacent sides
			if(side==1)
			{
				line[0].color = sf::Color(255,0,0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				line[1].color = sf::Color(255, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
			}
			else
			{
				line[0].color = sf::Color(128, 0, 0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				line[1].color = sf::Color(128, 0, 0,255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
			}

			//for rendereing floor 
			sf::Vertex floor[] =
			{
				sf::Vertex(sf::Vector2f(i, drawEnd), sf::Color(0, 0, 255, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h))),sf::Vertex(sf::Vector2f(i, h), sf::Color(0, 0, 255, 255 ))
			};

			//for rendering ceiling 
			sf::Vertex ceilings[] =
			{
				sf::Vertex(sf::Vector2f(i, 0), sf::Color(128, 128, 128, 255)),sf::Vertex(sf::Vector2f(i, drawStart), sf::Color(128, 128,128, 255))
			};
			// buffer the renderer for each column 
			window.draw(ceilings, 2, sf::Lines);
			window.draw(floor, 2, sf::Lines);
			window.draw(line, 2, sf::Lines);

//--------------------------line drawing for current pixel column Complete , loop proceeds to next column  --------------------------------------------------------------------------
		}

// -----------------finally rendering the buffered  screen --------------------------------------------------------------------------------------------------------------------------
			window.display();
	}}

	return 0;
}
