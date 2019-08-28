#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
 int screenWidth;                // Console screen size X (columns)
 int screenHeight;

int map[10][10]=
{
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(500, 500), "Prototype!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
    

    
		float playerX=1.0,playerY=1.0;
		float playerAngle=0;

		float dX,dY;

		float playerStepSize=0.5;

		char key_code;
		


		 switch(key_code)
						{
								//forward
								case 119:
								case 87:
											playerX+=playerStepSize*cos(playerAngle);
											playerY+=playerStepSize*sin(playerAngle);

										break;

								//backwards
								case 115:
								case 83:

											playerX-=playerStepSize*cos(playerAngle);
											playerY-=playerStepSize*sin(playerAngle);
										break;


								//left
								case 65:
								case 97:
										playerAngle+=0.1;
										break;


								//right
								case 100:
								case 68:

										playerAngle-=0.1;
										break;


						}
         window.clear();
						
						//{
							 // left key is pressed: move our character
                            sf::Font font;
                            font.loadFromFile("Arial.ttf");
                            sf::Text text("hello",font);
                          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  text.setFillColor(sf::Color::Red);
                          else text.setFillColor(sf::Color::Blue);
							window.draw(text);
					//	}
					

		

       
        //window.draw(shape);
        window.display();
    }
		//rotate FOV by 'b' angletext.setFillColor(sf::Color::Red);
	/*
		x=dirX*cos(b)-dirY*sin(b);
		y=dirX*sin(b)+dirY*cos(b);
		dirX=x;
		dirY=y;
	*/

}