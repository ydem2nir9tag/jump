#ifndef _jump_h_
#define _jump_h_
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
/**
player is a class that keeps track of the player's jumping abilities, the shape and hit box of the player, and the position of the player. 
@clock is an sf::Clock to keep track of the jumping time
@head is an sf::RectangleShape which represents the top square of the player 
@body is an sf::RectangleShape which represents the bottom rectangle of the player
@jump is an sf::Sound that stores the jump.wav sound to be played whenever a jump is initiated
@jumpBuff is an sf::SoundBuffer which will take in the jump.wav file and then give it to jump
@t is a float which keeps track of the time from the clock in seconds
@initPos is a float which stores the initial position of the player
@curPos is a float which stores the current and instantaneous position of the player
@hits is an int that stores the ammount of hits that the player has taken
@bJump is a bool that represents if a player is in the process of jumping or not, aka it tells us if the player can jump via the up key pressed
@WINDOW6 is a const float that stores the length/height of the square part of the player
@T_0 is a const int that stores the 1 second time for the duration of the jump
@T_1 is a const int that stores the 1.1 second time for the duration of the down time after a jump
*/
class player
{
private:
	sf::Clock clock;
	sf::RectangleShape head;
	sf::RectangleShape body;
	sf::Sound jump;
	sf::SoundBuffer jumpBuff;
	float t;
	float initPos;
	float curPos;
	int hits;
	bool bJump;
	//constants
	float const WINDOW6 = 100.0f / 6.0f;
	int const T_0 = 1;
	int const T_1 = 1.1;
public:
	/**
	player() is the constructor which will initialize the shape, the sound, the positions, the hit count, and the bJump value.
	*/
	player();
	/**
	toJump() is the function which takes care of the jumping animation. It sets t to the time of the clock, and uses the given
	equation to determine where the player should be. it can also set bJump to false if need be.
	*/
	void toJump();
	/**
	startJump(bool start) takes care of initiating a jump. it sets the bjump equal to the passed in value, restarts the clock
	and plays the jump sound
	@param start is the bool to be passed into the bJump variable
	*/
	void startJump(bool start);
	//accessors
	/**
	isJump() is an accessor that reports whether or not the player can initiate another jump command
	@return bool which states whether or not the player can start another jump
	*/
	bool isJump() const { return bJump; }
	/**
	getBounds() is an accessor that gives the global bounds of the body, which is the rectangle of the player
	@return sf::FloatRect which is the bounding box of the body of the player
	*/
	sf::FloatRect getBounds() const { return body.getGlobalBounds(); }
	/**
	getHead() is an accessor that gives the head, which is the square part of the player
	@return sf::RectangleShape which is the head of the player
	*/
	sf::RectangleShape getHead() const { return head; }
	/**
	getBody() is an accessor that gives the body, which is the rectangle portion of the player
	@return sf::RectangleShape which is the body of the player
	*/
	sf::RectangleShape getBody() const { return body; }
};
/**
enemy is a class that stores the enemy's information regarding movement and whether or not a score can be granted
@clock an sf::Clock to store the time used to create movement
@box is an sf::RectangleShape that stores the enemy shape
@initPos is a float which stores the initial position of the enemy, but then it's used to store the position in general
@score is a bool which tells us if the enemy can give a score, or if it already has
@WINDOW6 is a constant which stores the length/height of the square shape for the enemy
@height is a float which stores the height the enemy will be spawned at
*/
class enemy
{
private:
	sf::Clock clock;
	sf::RectangleShape box;
	float initPos;
	bool score;
	//constants
	float const WINDOW6 = 100.0f / 6.0f;
	float const height = 4.0f * WINDOW6;
public:
	/**
	enemy() is a constructor that initializes the initPos, creates the shape of the enemy, and sets the score to true
	the score to true part tells us that the enemy can give us a score
	*/
	enemy();
	/**
	movePos() is a function that will get the elapsed time of the clock and determine where the enemy should be based on that time
	*/
	void movePos();
	/**
	giveScore() is a function to be called after an enemy gives a score, to not allow a score to be given by this
	particular enemy again
	*/
	void giveScore() { score = false; }
	//accessors
	/**
	getPos() is an accessor that gives us the position of the enemy
	@return float that is the position
	*/
	float getPos() const { return initPos + WINDOW6; }
	/**
	canScore() is an accessor that returns whether or not the enemy can grant a score/point
	@return bool that represents the ability to grant a score
	*/
	bool canScore() const { return score; }
	/**
	getBounds() is an accessor that returns a boundingbox of the enemy
	@return sf::FloatRect store the bounding box of our enemy
	*/
	sf::FloatRect getBounds() const { return box.getGlobalBounds(); }
	/**
	getShape() is an accessor that returns the enemy shape
	@return sf::RectangleShape is the enemy shape
	*/
	sf::RectangleShape getShape() const { return box; }
};

/**
game is a class that deals with updating the positions and life status of every object on the screen
it stores other data for creating the rest of the window and a sound to represent a collision
@person is a pointer to a player that we will use to access the player's functions
@block is a vector of pointers to enemies that stores all the enemies in our game 
@msg is an sf::Text that stores the text that would appear at each part of the game
@f is an sf::Font that stores the sansation.ttf font
@ground is an sf::RectangleShape that stores the shape of the ground, which never changes
@hit sf::Sound stores the hit.wav sound 
@hitBuff sf::SoundBuffer is a sound buffer that gets the hit.wav sound to be placed in hit
@score is an int that keeps track of our score
@dmg is an int that keeps track of how many enemies have hit us
*/
class game
{
private:
	player* person;
	std::vector<enemy*> block;
	sf::Text msg;
	sf::Font f;
	sf::RectangleShape ground;
	sf::Sound hit;
	sf::SoundBuffer hitBuff;
	int score;
	int dmg;

public:
	/**
	game() is a constructor that creates a player and the ground and the text and the hit sound
	it also initalizes the score and dmg to 0 
	*/
	game();
	/**
	update() is a funciton that will update the life status of each enemy, either granting a point, deleting it because it went
	off the screen, or deleting it and causing us to take a hit and then play the hit sound
	*/
	void update();
	/**
	cleanup() is a function that is kind of like our destructor, it deletes the player and any remaining enemies
	*/
	void cleanup();
	/**
	display(sf::RenderWindow& window) is a function that takes in an sf::RenderWindow& which allows us to draw on the
	window created in the main.cpp file. This function is responsible for drawing all the shapes, such as the player and enemies
	and the text. This will also report if the game has been lost, and when to call the cleanup() function
	*/
	void display(sf::RenderWindow& window);
	/**
	spawnEnemy() is a function that creates a new enemy and stores it in our vector. this is called whenever a spawn is 
	activated by our main.cpp
	*/
	void spawnEnemy();
	/**
	isAlive() is a function that returns whether or not we are alive, based on the damage we have taken
	*/
	bool isAlive();
	//accessor
	/**
	getPlayer() is an accessor that returns a pointer to our player in this game
	@return player* which is the pointer to our player
	*/
	player* getPlayer() { return person; }
};
#endif // !_jump_h_
