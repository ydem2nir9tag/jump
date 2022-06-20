#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "jump.h"


//game function implementations
game::game()
	:score(0), dmg(0)
{
	person = new player;												//create a new player

	ground = sf::RectangleShape(sf::Vector2f(450.f, 16.6f));            //create a rectangle called ground with length 450 and height 16.6
	ground.move({ 0.f,83.4f });                                         //move that 83.4 pixels down 
	ground.setFillColor(sf::Color::White);								//make it white

	f.loadFromFile("sansation.ttf");									//load the sansation font

	msg.setFont(f);                                                     //set the font of msg to f
	msg.setCharacterSize(20);                                           //set the size of msg to 50 pixels
	msg.setFillColor(sf::Color::White);									//set the color of the msg to white
	std::string num = std::to_string(score);							//make our score into a string called num
	msg.setString("Score: " + num);										//and use that to make our final message

	if (!hitBuff.loadFromFile("hit.wav"))								//load the hit.wav sound into our buffer
	{
		std::cout << "hit sound not loaded\n";							//if it fails, print this
	}
	hit.setBuffer(hitBuff);												//put that sound buffer into our sf::Sound
}

void game::update()
{
	for (size_t i = 0; i < block.size(); ++i)								//for every enemy in our game
	{
		block[i]->movePos();												//we move it
		if (block[i]->getPos() < 0 && block[i]->canScore())					//and if it's off the screen and can grant us a point
		{
			score++;														//then we get a point
			block[i]->giveScore();											//and set the enemy to "already gave us a point"
		}
		else if (!block[i]->canScore())										//if it can't give us a point (it already has)
		{
			delete block[i];												//delete it
			block.erase(block.begin() + i);									//erase it from our vector
			i--;															//decrement i so we don't skip any other enemies
		}
		else if (block[i]->getBounds().intersects(person->getBounds()))		//or finally, if it intersects the player
		{	
			hit.play();														//play the hit sound
			delete block[i];												//delete it
			block.erase(block.begin() + i);									//erase it from our vector
			i--;															//decrement i so we don't skip any other enemies
			dmg++;															//increment our damage taken
		}
	}
}

void game::cleanup()
{
	delete person;								//delete our player
	for (size_t i = 0; i < block.size(); ++i)	//delete all remaining enemies
	{
		delete block[i];
		block.erase(block.begin() + i);
		i--;
	}
}

void game::display(sf::RenderWindow& window)
{
	std::string num = std::to_string(score);			//turn our score into a string
	msg.setString("Score: " + num);						//use that to make our message
	if (!isAlive())										//if we are dead
	{
		msg.setString("Score: " + num + "   Game Over");//then also say game over
	}
	window.clear();										//clear our window
	window.draw(person->getBody());						//draw our player
	window.draw(person->getHead());
	for (size_t i = 0; i < block.size(); ++i)			//draw every enemy
	{
		window.draw(block[i]->getShape());
	}	
	window.draw(ground);								//draw the ground
	window.draw(msg);									//and the message
	window.display();									//and display all of that
	if (!isAlive())										//if this is the last display because we are dead
	{
		cleanup();										//then run the cleanup
	}
}

void game::spawnEnemy()
{	
	enemy* temp = new enemy;	//house a new enemy in temp
	block.push_back(temp);		//and push it back onto our vector
}

bool game::isAlive()
{
	if (dmg >= 3)		//if we take 3 or more damage (it should never go more, but just in case)
	{
		return false;	//then we are dead
	}
	else				//otherwise we are alive
	{
		return true;
	}
}


//player function implementations
player::player()
	: initPos(50.f), curPos(50.f), hits(0), bJump(false)	//our initial position and current position are where the head is
{
	//loading the jump sound
	if (!jumpBuff.loadFromFile("jump.wav"))		//load jump.wav into our buffer
	{
		std::cout << "jump sound not loaded\n";	//if this fails, then we print this
	}
	jump.setBuffer(jumpBuff);					//load that sound into my sf::sound jump
	//drawing player
	head = sf::RectangleShape(sf::Vector2f(WINDOW6, WINDOW6));			//the head is a square 
	head.move({ WINDOW6,3.0f * WINDOW6 });								//that is placed one square away from the left edge, and half way down
	head.setFillColor(sf::Color(sf::Color::Red));						//and it's red
	body = sf::RectangleShape(sf::Vector2f(3.0f * WINDOW6, WINDOW6));	//the body is a rectangle, 3 times the lengt of the square
	body.move({ 0.f, 4.0f * WINDOW6 });									//it is placed at the left edge and 2/3 the way down
	body.setFillColor(sf::Color(sf::Color::Red));						//and it's also red
}

void player::toJump()
{
	t = clock.getElapsedTime().asSeconds();						//set our time t = the clock's time
	if (t <= T_0)												//if our time t is less than T (1 second)
	{
		curPos = initPos;										//we set our curPos = to the initial position minus the equation's height
		curPos -= 8 * WINDOW6 * t * (T_0 - t) / (T_0 * T_0);	//this gives us the position it should be at
		head.setPosition({ WINDOW6, curPos });					//so we set it's position to that height, and retain the horizontal position
		body.setPosition({ 0, curPos + WINDOW6 });
	}
	else if (t >= T_0 && t < T_1)								//if it's past our jump time, and we are on jump cooldown (between 1 and 1.1 seconds)
	{
		curPos = initPos;										//we set our curPos = to the initial position which is on the floor
		head.setPosition({ WINDOW6, curPos });					//and we set the position of our player to that position
		body.setPosition({ 0, curPos + WINDOW6 });
	}
	else if (t >= T_1)											//once we pass our cooldown time
	{
		bJump = false;											//we are no longer jumping
	}
}

void player::startJump(bool start)								
{
	bJump = start;		//we set our jump equal to the start bool value (it's gonna be true)
	clock.restart();	//we then restart our jumping clock										
	jump.play();		//and then we play the jump sound
}


//enemy function implementations
enemy::enemy()
{
	initPos = 450.f;											//each enemy spawns at the right edge of the window
	box = sf::RectangleShape(sf::Vector2f(WINDOW6, WINDOW6));	//it is a sqaure
	box.setFillColor(sf::Color(sf::Color::Yellow));				//that's yellow
	box.move({ initPos, height });								//and it begins at those spawn points
	score = true;												//it can still provide us score
}

void enemy::movePos()
{
	float elapsed = clock.getElapsedTime().asSeconds();	//create a timer for our movement
	if (elapsed >= 0.01)								//if the timer has gone for 0.01 seconds or more (or more to check for lag)
	{
		initPos -= 2;									//decrease the initial position by 2 pixels (which is our current position)
		box.move({ -2.f, 0.f });						//move our enemy two pixels to the left
		clock.restart();								//and restart the clock
	}
}