#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include "jump.h"

int main()
{
    constexpr float WINDOW6 = 100.0f / 6.0f;                            //this is a constant for our ground height, which is also our player's square length
    sf::RenderWindow window(sf::VideoMode(450, 100), "Jumping Game");   //create a window with size 400x400 with title Jumping Game

    sf::RectangleShape ground(sf::Vector2f(450.f, 16.6f));              //create a rectangle called ground with length 450 and height 16.6
    ground.move({ 0.f,83.4f });                                         //move that 83.4 pixels down 
    ground.setFillColor(sf::Color{ 255, 255, 255 , 255 });              //fill the ground with white

    sf::Font f;                                                         //create a font f
    f.loadFromFile("sansation.ttf");                                    //set that font to comic.ttf, the provided font

    sf::RectangleShape head(sf::Vector2f(WINDOW6, WINDOW6));
    head.move({ WINDOW6,3.0f * WINDOW6 });
    head.setFillColor(sf::Color(sf::Color::Red));
    sf::RectangleShape body(sf::Vector2f(3.0f * WINDOW6, WINDOW6));
    body.move({ 0.f, 4.0f * WINDOW6 });
    body.setFillColor(sf::Color(sf::Color::Red));

    sf::Text msg;                                                       //create a text msg
    msg.setFont(f);                                                     //set the font of msg to f
    msg.setCharacterSize(20);                                           //set the size of msg to 50 pixels
    msg.setFillColor(sf::Color{ 255, 255, 255, 255 });                  //set the color of the msg to white
    msg.setString("Press any key to begin.\nPress UP to jump.\nGet hit 3 times and lose.");
                                                                        //assign the instructions to msg
    window.clear();                                                     //clear the window
    window.draw(ground);                                                //draw the ground
    window.draw(head);                                                  //draw the head (square)
    window.draw(body);                                                  //draw the body (rectangle)
    window.draw(msg);                                                   //then draw the msg
    window.display();                                                   //show this preliminary screen

    
    srand(time(NULL));                                                  //create a random seed for the rand()

    while (window.isOpen())                                             //as long as the window is open
    {
        sf::Event event;                                                //create a new event
        while (window.pollEvent(event))                                 //when we trigger an event
        {
            if (event.type == sf::Event::KeyPressed)                    //if we press a key, then we start the game
            {
                game jumper;                                            //create a game object
                sf::Clock spawn;                                        //create a timer to keep track of our enemy spawning
                float elapsed;                                          //and a float to keep track of the time elapsed
                do
                {
                    elapsed = spawn.getElapsedTime().asSeconds();       //set elapsed = to the spawn timer's time
                    if (elapsed >= 1.5)                                 //if its greater than or equal to 1.5 seconds (greater than to take care of lag)
                    {                                                   //without the greater than, my timer might not ever perfectly equal 1.5 seconds
                        spawn.restart();
                        if (rand() % 100 < 50)                          //if the random number is less than 50 (makes a 50% chance random generator)
                        {
                            jumper.spawnEnemy();                        //spawn an enemy
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   //if the up key is pressed
                    {
                        if (!jumper.getPlayer()->isJump())              //and we are not currently jumping
                        {
                            jumper.getPlayer()->startJump(true);        //then start a jump
                        }
                    }
                    if (jumper.getPlayer()->isJump())                   //if the player is jumping or has a jump started
                    {
                        jumper.getPlayer()->toJump();                   //then we run the jump function
                    }

                    sf::Event close;
                    if (window.pollEvent(close))                        //if we decide to close the game before we die using an event
                    {
                        if (close.type == sf::Event::Closed)            //and that event is closing the window
                        {
                            window.close();                             //then we close the window
                            break;                                      //and break out of the game loop
                        }
                    }
                    jumper.update();                                    //update our game positiosn and collision checks and deletions of enemies
                    jumper.display(window);                             //display our new window
                } while (jumper.isAlive());                             //while we are alive
            }
            else if (event.type == sf::Event::Closed)                   //if we never wanted to start the game
            {
                window.close();                                         //then we close the window
            }
        }
    }

    return 0;
}