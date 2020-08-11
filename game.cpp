#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#define pi 3.1415926535897932384626
using namespace std;

int World[32][32];
typedef struct
{
	int x, y;	  //Node position - little waste of memory, but it allows faster generation
	void *parent; //Pointer to parent node
	int c;		  //Character to be displayed
	char dirs;	  //Directions that still haven't been explored
} Node;

Node *nodes; //Nodes array
int N;		 //Maze dimension
int won = 0, helptaken = 0;

int init()
{
	int i, j;
	Node *n;

	//Allocate memory for maze
	nodes = (Node *)calloc(N * N, sizeof(Node));
	if (nodes == NULL)
		return 1;

	//Setup crucial nodes
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			n = nodes + i + j * N;
			if (i * j % 2)
			{
				cout << i << "," << j << endl;
				n->x = i;
				n->y = j;
				n->dirs = 15; //Assume that all directions can be explored (4 youngest bits set)
				n->c = 0;
			}
			else
				n->c = 1; //Add walls between nodes
		}
	}

	return 0;
}

Node *link(Node *n)
{
	//Connects node to random neighbor (if possible) and returns
	//address of next node that should be visited

	int x, y;
	char dir;
	Node *dest;

	//Nothing can be done if null pointer is given - return
	if (n == NULL)
		return NULL;

	//While there are directions still unexplored
	while (n->dirs)
	{
		//Randomly pick one direction
		dir = (1 << (rand() % 4));
		//If it has already been explored - try again
		if (~n->dirs & dir)
			continue;
		//Mark direction as explored
		n->dirs &= ~dir;
		//Depending on chosen direction
		switch (dir)
		{
		//Check if it's possible to go right
		case 1:
			if (n->x + 2 < N)
			{
				x = n->x + 2;
				y = n->y;
			}
			else
				continue;
			break;

		//Check if it's possible to go down
		case 2:
			if (n->y + 2 < N)
			{
				x = n->x;
				y = n->y + 2;
			}
			else
				continue;
			break;

		//Check if it's possible to go left
		case 4:
			if (n->x - 2 >= 0)
			{
				x = n->x - 2;
				y = n->y;
			}
			else
				continue;
			break;

		//Check if it's possible to go up
		case 8:
			if (n->y - 2 >= 0)
			{
				x = n->x;
				y = n->y - 2;
			}
			else
				continue;
			break;
		}

		//Get destination node into pointer (makes things a tiny bit faster)
		dest = nodes + x + y * N;

		//Make sure that destination node is not a wall
		if (dest->c == 0)
		{
			//If destination is a linked node already - abort
			if (dest->parent != NULL)
				continue;

			//Otherwise, adopt node
			dest->parent = n;

			//Remove wall between nodes
			nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * N].c = 0;

			//Return address of the child node
			return dest;
		}
	}

	//If nothing more can be done here - return parent's address
	//cout<<(n->x)<<" "<<(n->y)<<endl;
	return (Node *)n->parent;
}

void draw()
{
	int i, j;
	//Outputs maze to terminal - nothing special
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			if (i < N && j < N)
				World[i][j] = nodes[j + i * N].c;
			else
				World[i][j] = 1;
		}
	}
}

void generateMaze(int x)
{
	Node *start, *last;
	N = x;
	//Seed random generator
	srand(time(NULL));

	//Initialize maze
	if (init())
	{
		cout << "Cannot Allocate enough memory!";
		exit(0);
	}
	//Setup start node
	start = nodes + 1 + N;
	start->parent = start;
	last = start;

	//Connect nodes until start node is reached and can't be left
	while ((last = link(last)) != start);
	draw();
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
	if (N % 2 == 0)
		cout << "Odd number not provided ! Exiting the game !";
	else if (N < 7)
		cout << "Size specified is less than 7! Exiting the game!";
	else if (N > 31)
		cout << "Size specified is more than 31! Exiting the game!";
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
	font.loadFromFile("Arial.ttf");
	
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
				for (int i = 0; i < N; i++)
				{
					for (int j = N - 1; j >= 0; j--)
					{
						if ((int)player.X == i && (int)player.Y == j)
							helpmap += 'o';
						else if (World[i][j] == 1)
							helpmap += "#";
						else
							helpmap = helpmap + "_";
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

			if ((int)player.X == N - 2 && (int)player.Y == N - 2)
			{
				won = 1;
			}
			window.display();
		}
	}
}
int main(void)
{
	cout << "Enter a Odd number between 7 to 31 to set as maze dimension:";
	cin >> N;
	checkSizeConstraint(N);
	generateMaze(N);
	runGame();
	return 0;
}
