// Include important librares here
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Timber.h"
#include <SFML/Audio.hpp>

// --  Make codea easier to type with "using namespace"
using namespace sf;

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
enum class side { LEFT, RIGHT, NONE };

side branchPositions[NUM_BRANCHES];

// --  This is where our game starts from
int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1090);

	// --  Create and open a window for the game
	RenderWindow window(vm, "TimberMan!!!", Style::Fullscreen);
	
	window.setVerticalSyncEnabled(true);

	// --  Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	// --  Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	// --  Create a sprite
	Sprite spriteBackground;
	// --  Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// --  Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// --  Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// --  Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee1;
	spriteBee1.setTexture(textureBee);
	spriteBee1.setPosition(0, 800);

	Sprite spriteBee2;
	spriteBee2.setTexture(textureBee);
	spriteBee2.setPosition(0, 600);

	// --  Is the bee currently moving?
	bool beeActive1 = false;
	bool beeActive2 = false;

	// --  How fast can the bee fly
	float beeSpeed = 0.0f;

	// --  Make 3 cloud sprites from 1 texture
	Texture textureCloud;

	// --  Load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");

	// --  3 New sprites with the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// --  Position the clouds on the left of the screen at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	// --  Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// --  How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// --  Variables to control time itself
	Clock clock;

	// --  Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 40;

	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// --  Track wheter the game is running
	bool paused = true;

	// --  Draw some text
	int score = 0;

	Text scoreText;
	Text messageText;
	Text instructionsText;
	Text remainingText;

	// --  We need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// --  Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	instructionsText.setFont(font);
	remainingText.setFont(font);

	// --  Assign the actual message
	messageText.setString("Press Enter to play TimberMan!");
	scoreText.setString("Score = 0");
	instructionsText.setString("0");
	remainingText.setString("Time left");

	// --  Make it big
	messageText.setCharacterSize(60);
	scoreText.setCharacterSize(80);
	instructionsText.setCharacterSize(30);
	remainingText.setCharacterSize(30);

	// --  Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	instructionsText.setFillColor(Color::White);
	remainingText.setFillColor(Color::White);

	// --  Set outline
	messageText.setOutlineThickness(1);
	scoreText.setOutlineColor(Color::Black);
	instructionsText.setOutlineColor(Color::Black);
	remainingText.setOutlineColor(Color::Black);

	messageText.setOutlineColor(Color::Black);
	scoreText.setOutlineThickness(1);
	instructionsText.setOutlineThickness(1);
	remainingText.setOutlineThickness(1);

	remainingText.setPosition(timeBar.getPosition().x , timeBar.getPosition().y);

	// --  Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);
	
	instructionsText.setPosition(20, 120);
	instructionsText.setString("Controls:\nPress ESC for exit\nPress Enter to Restart\nPress A or Left Arrow to move left\nPress D or Right Arrow to move right");
	// --  Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// --  Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000,-2000);

		// --  Set the sprite's origin to dead centre
		// --  We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// --  Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// --  The player starts on the left
	side playerSide = side::LEFT;

	// --  Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 2000);

	// --  Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// --  Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// --  Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 780);

	// --  Some othre useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// --  Control the player input
	bool acceptInput = false;

	// --  Prepare the sounds
	// --  The background sound
	SoundBuffer backgroundBuffer;
	backgroundBuffer.loadFromFile("sound/background.wav");
	Sound backgroundSound;
	backgroundSound.setBuffer(backgroundBuffer);
	backgroundSound.setLoop(true);
	backgroundSound.setVolume(30);

	// --  The player chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chopSound;
	chopSound.setBuffer(chopBuffer);
	chopSound.setVolume(30);

	// --  The player has met his end under a branch
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound deathSound;
	deathSound.setBuffer(deathBuffer);
	deathSound.setVolume(10);

	// --  Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTimeSound;
	outOfTimeSound.setBuffer(ootBuffer);
	outOfTimeSound.setVolume(30);


	while (window.isOpen())

	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				// --  Listen for key presses again
				acceptInput = true;

				// --  hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		// --  Exit game when press Esc Key
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		// --  Start the game when press Enter Key
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// --  Reset the time and the score
			score = 0;
			timeRemaining = 6.0f;
			cloud1Active = false;
			cloud2Active = false;
			cloud3Active = false;
			beeActive1 = false;
			beeActive2 = false;
			
			// --  Make all the branches disappear
			// --  starting in the second position
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// --  Make sure the gravestone is gidden
			spriteRIP.setPosition(657, 2000);

			// --  Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
			backgroundSound.play();
		}

		// --  Wrap the player controls to make sure we are accepting input
		if (acceptInput) {
			// --  First handle pressing the right/d cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
				// --  Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				// --  Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// --  Update the branches
				updateBranches(score);

				// --  Set the log flying to the left
				spriteLog.setPosition(810, 780);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chopSound.play();
			}

			// --  First handle pressing the left/A cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
				// --  Make sure the player is on the LEFT
				playerSide = side::LEFT;
				score++;

				// --  Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// --  Update the branches
				updateBranches(score);

				// --  Set the log flying to the left
				spriteLog.setPosition(810, 780);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chopSound.play();
			}
		}

		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused) {

			// --  Measure time
			Time deltaTime = clock.restart();
			
			// --  Substract from the amount of time remaining
			timeRemaining -= deltaTime.asSeconds();

			// --  Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// --  Pause the game
				paused = true;
				backgroundSound.stop();

				// --  Change the message shown to the player
				messageText.setString("Out of time!!");

				// --  Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// --  Play the out of time sound
				outOfTimeSound.play();
			}

			ManageBee(beeActive1, beeSpeed, 200, spriteBee1, deltaTime);
			ManageBee(beeActive2, beeSpeed, 400, spriteBee2, deltaTime);

			// --  Manage the clouds
			ManageCloud(cloud1Active, cloud1Speed, spriteCloud1, deltaTime, 200, 150, 50);
			ManageCloud(cloud2Active, cloud2Speed, spriteCloud2, deltaTime, 210, 300, 150);
			ManageCloud(cloud3Active, cloud3Speed, spriteCloud3, deltaTime, 220, 350, 150);

			// --  Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// --  Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					// -- Move the sprite to the left side
					branches[i].setPosition(610, height);

					// -- Flip the sprite round the other way
					branches[i].setRotation(180);
				}else if(branchPositions[i] == side::RIGHT) {
					// -- Move the sprite to the right side
					branches[i].setPosition(1330, height);

					// -- Flip the sprite round the other way
					branches[i].setRotation(0);
				}
				else {
					// -- Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// -- Handle a flying log
			if (logActive) {
				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * deltaTime.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * deltaTime.asSeconds()));
				// --  Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x >  2000) {
					
					// --  set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 780);
				}
			}

			// -- Has the player been squished by a branch?
			if (branchPositions[5] == playerSide) {
				// -- Death
				paused = true;
				backgroundSound.stop();
				acceptInput = false;

				// --  Draw the gravestone
				if (playerSide == side::LEFT) {
					spriteRIP.setPosition(525, 760);
				}
				else {
					spriteRIP.setPosition(1200, 760);
				}


				// -- Hide the palyer
				spritePlayer.setPosition(2000, 660);

				// -- Change the text of the message
				messageText.setString("TIMBERMAN WAS SQUISHED!!");

				// -- Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// -- Play the death sound
				deathSound.play();
			}


		} // --  End if (!paused)
		/*
		****************************************
		Draw the scene
		****************************************
		*/
		// --  Clear everything from the last frame
		window.clear();
		
		// --  Draw our game scene here
		window.draw(spriteBackground);

		// --  Draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// --  Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// --  Draw the tree
		window.draw(spriteTree);

		// --  Draw the player
		window.draw(spritePlayer);

		// --  Draw the axe
		window.draw(spriteAxe);

		// --  Draw the flying log
		window.draw(spriteLog);

		// --  Draw the gravestone
		window.draw(spriteRIP);

		// --  Draw the insect
		window.draw(spriteBee1);
		window.draw(spriteBee2);

		// --  Draw the score
		window.draw(scoreText);

		// --  Draw the timebar
		window.draw(timeBar);

		window.draw(instructionsText);

		window.draw(remainingText);

		if (paused) {
			// --  Draw our message
			window.draw(messageText);
		}

		// --  Draw our game scene here
		// --  Show everything we just drew
		window.display();
	}
	return 0;
}

void ManageBee(bool& beeActive, float& beeSpeed, int beePosition, sf::Sprite& spriteBee, sf::Time& deltaTime)
{
	// --  Setup the bee
	if (!beeActive) {

		// --  How fast is the bee
		srand((int)time(0));
		beeSpeed = (rand() % 200) + 200;

		// --  How high is the bee
		srand((int)time(0) * 10);
		float height = (rand() % 500) + 500;
		spriteBee.setPosition(2000, height);

		beeActive = true;
	}
	else {
		// --  Move the bee
		spriteBee.setPosition(
			spriteBee.getPosition().x - (beeSpeed * deltaTime.asSeconds()),
			spriteBee.getPosition().y);

		// --  Has the bee reached the left-hand edge of the screen?
		if (spriteBee.getPosition().x < -100) {
			// --  Set it up ready to be a whole new bee next frame
			beeActive = false;
		}

	}
}

void ManageCloud(bool& cloudActive, float& cloudSpeed, sf::Sprite& spriteCloud, sf::Time& deltaTime,int speed, int height, int umbralHeight)
{
	if (!cloudActive) {
		// --  How fast is the cloud
		srand((int)time(0) * 10);
		cloudSpeed = (rand() % speed);

		// --  How high is the cloud 
		srand((int)time(0) * 10);
		float highHeight = (rand() % height) - umbralHeight;
		spriteCloud.setPosition(-200, highHeight);
		cloudActive = true;
	}
	else {
		spriteCloud.setPosition(
			spriteCloud.getPosition().x +
			(cloudSpeed * deltaTime.asSeconds()),
			spriteCloud.getPosition().y
		);

		// --  Has the cloud reached the right hand edge of the screen?
		if (spriteCloud.getPosition().x > 1920) {
			// --  Set it up ready to be a whole new clode next frame
			cloudActive = false;
		}
	}
}

void updateBranches(int seed)
{
	// --  Move all the branches down one place
	for (int i = NUM_BRANCHES - 1; i > 0 ; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}

	// --  Spawn a new branch at position 0
	// --  LEFT, RIGHT OR NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
		break;
	}
}
