#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#define pi 3.1415926535897932384626
using namespace std;

int World[50][50];
int N;		 //Maze dimension
int won = 0, helptaken = 0;
void divideSection(int sx,int sy,int ex,int ey)
{
	if(ex-sx<2||ey-sy<2)return;

	int wallMidX=sx+1+rand()%(ex-sx-1);
	int wallMidY=sy+1+rand()%(ey-sy-1);

	for(int i=sy;i<=ey;i++)
		World[wallMidX][i]=1;

	for(int i=sx;i<=ex;i++)
		World[i][wallMidY]=1;
	
	int not_break=rand()%4;

	if(not_break!=1)
	{
		int wallBreakerX=sx+rand()%((wallMidX-1)-sx+1);
		int wallBreakerY=wallMidY;
		World[wallBreakerX][wallBreakerY]=0;
	}
	if(not_break!=2)
	{
		int wallBreakerX=wallMidX;
		int wallBreakerY=wallMidY+1+rand()%(ey-(wallMidY+1)+1);
		World[wallBreakerX][wallBreakerY]=0;
	}
	if(not_break!=3)
	{
		int wallBreakerX=wallMidX+1+rand()%(ex-(wallMidX+1)+1);
		int wallBreakerY=wallMidY;
		World[wallBreakerX][wallBreakerY]=0;
	}
	if(not_break!=4)
	{
		int wallBreakerX=wallMidX;
		int wallBreakerY=sy+rand()%((wallMidY-1)-sy+1);
		World[wallBreakerX][wallBreakerY]=0;
	}
	
	divideSection(sx,sy,wallMidX-1,wallMidY-1);
	divideSection(sx,wallMidY+1,wallMidX-1,ey);
	divideSection(wallMidX+1,wallMidY+1,ex,ey);
	divideSection(wallMidX+1,sy,ex,wallMidY-1);
}
void generateMaze(int n)
{
	srand(time(0));
	for(int i=0;i<n+2;i++){
		World[i][0]=1;
		World[i][n+1]=1;
		World[0][i]=1;
		World[n+1][i]=1;
	}
	divideSection(1,1,n,n);

}
class Player
{
public:
	double X, Y, Angle, StepSize;
	Player(double pX, double pY, double pA, double pStepSize)
	{
		X = pX;
		Y = pY;
		Angle = pA;
		StepSize = pStepSize;
	}
};

class Ray
{
public:
	double X, Y;
	Ray(int rayX = 0.0, int rayY = 0.0)
	{
		X = rayX;
		Y = rayY;
	}
};
void checkSizeConstraint(const int N)
{
	if (N < 3)
		cout << "Size specified is less than 3! Exiting the game!";
	else if (N > 45)
		cout << "Size specified is more than 45! Exiting the game!";
	else
		return;
	exit(0);
}
void runGame()
{
	const int h = 800;
	sf::RenderWindow window(sf::VideoMode(3 * h / 4, h), "Random Maze Game!");
	window.setFramerateLimit(45);

	Player player(1.5, 1.5, 0.0, 0.04);
	Ray ray(0.0, 0.0);

	sf::Text t, th, hm;
	sf::Font font;
	font.loadFromFile("monospace.ttf");
	
	t.setFont(font);
	th.setFont(font);
	hm.setFont(font);

	th.setCharacterSize(18);
	t.setCharacterSize(18);

	th.setString("You have completed the level ! \n Try to do it without using map next time :)\n Press  Q Key to end the game!");
	t.setString("Congrats! You WON WITHOUT USING HELP!!!! \n Press  Q  Key to end the game!");

	
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
		if (sf::Event::KeyPressed)
		{
			window.clear();
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Q)))
				exit(0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (World[(int)(player.X + player.StepSize * cos(player.Angle + pi / 8))][(int)player.Y] == 1)
					player.X += 0;
				else
					player.X += player.StepSize * cos(player.Angle + pi / 8);
				if (World[(int)player.X][(int)(player.Y + player.StepSize * sin(player.Angle + pi / 8))] == 1)
					player.Y += 0;
				else
					player.Y += player.StepSize * sin(player.Angle + pi / 8);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (World[(int)(player.X - player.StepSize * cos(player.Angle + pi / 8))][(int)player.Y] == 1)
					player.X -= 0;
				else
					player.X -= player.StepSize * cos(player.Angle + pi / 8);

				if (World[(int)player.X][(int)(player.Y - player.StepSize * sin(player.Angle + pi / 8))] == 1)
					player.Y -= 0;
				else
					player.Y -= player.StepSize * sin(player.Angle + pi / 8);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.Angle -= 0.03;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				player.Angle += 0.03;
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				string helpmap;
				if (won == 0)
					helptaken = 1;
				for (int i = 0; i <=N+1; i++)
				{
					for (int j = N+1; j >= 0; j--)
					{
						if ((int)player.X == i && (int)player.Y == j)
							helpmap += '*';
						else if (World[i][j] == 1)
							helpmap += "#";
						else
							helpmap = helpmap + " ";
					}
					helpmap += "\n";
				}
				hm.setFont(font);
				hm.setString(helpmap);
				window.draw(hm);
				window.display();
			}
			// keyboard input events end here....**************************************************************

			for (int i = 0; i < h; i++)
			{

				double direX = cos(player.Angle), direY = sin(player.Angle);
				double rotatingAngle = (pi) / ((double)(h * 4));
				double dirX = cos(rotatingAngle * i) * direX - sin(rotatingAngle * i) * direY;

				double dirY = cos(rotatingAngle * i) * direY + sin(rotatingAngle * i) * direX;
				int mapX = int(player.X);
				int mapY = int(player.Y);
				ray.X = dirX; //planeX * cameraX;
				ray.Y = dirY; // planeY * cameraX;

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
				int side;	 //was a NS or a EW wall hit?
				//calculate step and initial sideDist
				if (ray.X < 0)
				{
					stepX = -1;
					sideDistX = (player.X - mapX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (mapX + 1.0 - player.X) * deltaDistX;
				}
				if (ray.Y < 0)
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
					perpWallDist = (mapX - player.X + (1 - stepX) / 2) / ray.X;
				else
					perpWallDist = (mapY - player.Y + (1 - stepY) / 2) / ray.Y;

				//perpWallDist=perpWallDist/cos(2*rotatingAngle);
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
						sf::Vertex(sf::Vector2f(i, drawEnd))};

				//chooses different shades of colours for adjacent sides
				if (side == 1)
				{
					line[0].color = sf::Color(255, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
					line[1].color = sf::Color(255, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				}
				else
				{
					line[0].color = sf::Color(128, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
					line[1].color = sf::Color(128, 0, 0, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h));
				}

				//for rendereing floor
				sf::Vertex floor[] =
					{
						sf::Vertex(sf::Vector2f(i, drawEnd), sf::Color(0, 0, 255, 255 - 255 * (1.0 - (drawEnd * 2) / (double)h))),
						sf::Vertex(sf::Vector2f(i, h), sf::Color(0, 0, 255, 255))};

				//for rendering ceiling
				sf::Vertex ceilings[] =
					{
						sf::Vertex(sf::Vector2f(i, 0), sf::Color(25, 25, 25, 225)),
						sf::Vertex(sf::Vector2f(i, drawStart), sf::Color(25, 25, 25, 225))};
				// buffer the renderer for each column
				window.draw(ceilings, 2, sf::Lines);
				window.draw(floor, 2, sf::Lines);
				window.draw(line, 2, sf::Lines);

				//--------------------------line drawing for current pixel column Complete , loop proceeds to next column  --------------------------------------------------------------------------
			}

			// -----------------finally rendering the buffered  screen --------------------------------------------------------------------------------------------------------------------------
			if (won)
			{
				if (helptaken == 0)
					window.draw(t);
				else
					window.draw(th);
			}

			if ((int)player.X == N && (int)player.Y == N )
			{
				won = 1;
			}
			window.display();
		}
	}
}
int main(void)
{
	cout << "Enter a number between 3 to 45 to set as maze dimension:";
	cin >> N;
	checkSizeConstraint(N);
	generateMaze(N);
	runGame();
	return 0;
}
