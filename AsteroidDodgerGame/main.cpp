#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// Function that draws the starting menu to the screen
void drawMenu (sf::RenderWindow& window) {
    
    sf::Font font ("8-bit-hud.ttf");
    
    sf::RectangleShape titleBox({400, 100});
    titleBox.setPosition({200, 150});
    titleBox.setFillColor(sf::Color::Black);
    
    sf::RectangleShape playButton({200, 50});
    playButton.setPosition({300,300});
    playButton.setFillColor(sf::Color::Green);
    
    sf::Text titleText(font, "Asteroid Dodger");
    titleText.setPosition({225, 200});
    titleText.setCharacterSize(20);
    
    sf::Text playButtonText(font, "Play");
    playButtonText.setPosition({350,317});
    playButtonText.setCharacterSize(20);
     
    sf::Texture backgroundTexture ("orig.png");
    sf::Sprite backgroundSprite (backgroundTexture);
    backgroundSprite.setScale({1.4,2});
    
    window.draw(backgroundSprite);
    window.draw(playButton);
    window.draw(titleBox);
    window.draw(titleText);
    window.draw(playButtonText);
    
}

// Function that draws a game over screen and features a play again button
void drawGameOver (sf::RenderWindow& window) {
    
    sf::Font font ("8-bit-hud.ttf");
    
    sf::Text gameOverText(font, "Game Over!");
    gameOverText.setPosition({225, 200});
    gameOverText.setCharacterSize(30);
    
    sf::RectangleShape playButton({200, 50});
    playButton.setPosition({300,300});
    playButton.setFillColor(sf::Color::Green);
    
    sf::Text playButtonText(font, "Play Again?");
    playButtonText.setPosition({305,325});
    playButtonText.setCharacterSize(15);
    
    sf::Texture gameOverTexture ("orig2.png");
    sf::Sprite gameOverSprite (gameOverTexture);
    gameOverSprite.setScale({1.4, 2});
    
    window.draw(gameOverSprite);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(gameOverText);
}

// Enumeration used to track the game state to handle drawing and calling functions
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

int main () {
    
    // Initializing the game state to menu so it starts on the main menu
    GameState currentState = MENU;
    
    // Initialze window with size and title
    sf::RenderWindow window(sf::VideoMode({800,600}), "Asteroid Dodger", sf::Style::Close);
    
    // Create and load in png for asteroid texture
    sf::Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("asteroid.png")) {
        return -1;
    }
    
    // Create and load in png for spaceship texture
    sf::Texture spaceshipTexture;
    if (!spaceshipTexture.loadFromFile("spaceship.png")) {
        return -1;
    }
    
    // Create sapceship sprite and pass in the png texture
    sf::Sprite spaceshipSprite (spaceshipTexture);
    
    // Initializing the spaceship position in center of screen, changing size of sprite and intializing speed of ship
    sf::Vector2f spaceshipSpritePosition (340,400);
    spaceshipSprite.setPosition(spaceshipSpritePosition);
    spaceshipSprite.setScale({0.5,0.5});
    float spaceshipSpeed = 200.f;
    
    // Clock initilaztion for "animation"
    sf::Clock clock;
    float totalTime = 0;

    
    // Creating vector of sprites to hold all the asteroids that will be drawn to the screen; chose vector because it will be updated dynamically and we don't know how many we will need to be drawn; also initialized a clock for spawning the asteroids
    std::vector <sf::Sprite> asteroids;
    sf::Clock asteroidSpawnTimer;
    
    //Score
    sf::Font font;
    if (!font.openFromFile("8-bit-hud.ttf")) {
        return -1;
    }
    sf::Text scoreText(font);
    
    // Collision sound
    sf::SoundBuffer explosionBuffer;
    if (!explosionBuffer.loadFromFile("explosion.wav")) {
        return -1;
    }
    sf::Sound explosionSound(explosionBuffer);
    
    // Background sounds during gameplay
    sf::SoundBuffer spaceshipAmbianceBuffer("spaceshipAmbiance.wav");
    sf::Sound spaceshipAmbianceSound (spaceshipAmbianceBuffer);
    
    // Background image during gameplay
    sf::Texture playingBackgroundTexture ("1.png");
    sf::Sprite playingBackgroundSprite (playingBackgroundTexture);
    playingBackgroundSprite.setScale({1.4, 2});
    
    sf::Texture explosionTexture ("Explosion_5.png");
    
    // Game loop
    while (window.isOpen()) {
        
        // Gameplay logic
        if (currentState == PLAYING) {
            
            // Playing, looping, and stopping the background sounds
            if (spaceshipAmbianceSound.getStatus() != sf::SoundSource::Status::Playing) {
                spaceshipAmbianceSound.setLooping(true);
                spaceshipAmbianceSound.play();
            }
            
            // Will be used for animation and score
            float deltaTime = clock.restart().asSeconds();
            totalTime += deltaTime;
            
            // Movement controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                spaceshipSpritePosition.y -= spaceshipSpeed * deltaTime;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                spaceshipSpritePosition.x -= spaceshipSpeed * deltaTime;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                spaceshipSpritePosition.y += spaceshipSpeed * deltaTime;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                spaceshipSpritePosition.x += spaceshipSpeed * deltaTime;
            }
            
            // Handling spaceship wrapping and upper/lower borders
            if (spaceshipSpritePosition.x > 850) {
                spaceshipSpritePosition.x = -50;
            } else if (spaceshipSpritePosition.x < -100) {
                spaceshipSpritePosition.x = 845;
            } else if (spaceshipSpritePosition.y > 500) {
                spaceshipSpritePosition.y = 500;
            } else if (spaceshipSpritePosition.y < -50) {
                spaceshipSpritePosition.y = -50;
            }
            
            // Updating spaceship position after the controls
            spaceshipSprite.setPosition(spaceshipSpritePosition);
            
            // Spawns a new asteroid every 0.75 seconds until the totalTime reaches 20 seconds and then increases the rate to every 0.5 seconds to increase difficulty; passes the asteroid texture into the new asteroid being spawned; chooses a random coordinate on the x axis and y=-50 for asteroid spawn; makes the asteroids smaller; adds the asteroid into the vector; restarts the clock to spawn a new asteroid
            float asteroidSpawnRate = 0.75f;
            if (totalTime >= 20) {
                asteroidSpawnRate = 0.5f;
            }
            if (asteroidSpawnTimer.getElapsedTime().asSeconds() > asteroidSpawnRate) {
                sf::Sprite newAsteroid (asteroidTexture);
                newAsteroid.setPosition(sf::Vector2f (rand() % 800, -150));
                newAsteroid.setScale({0.25f, 0.25f});
                asteroids.push_back(newAsteroid);
                asteroidSpawnTimer.restart();
            }
            
            // For loop iterates through the vector from beginning to end; gets position of current asteroid and moves the asteroid down the screen using deltaTime; updates position
            for (auto it = asteroids.begin(); it != asteroids.end();) {
                sf::Vector2f pos = it->getPosition();
                float asteroidSpeed = 200.f;
                if (totalTime >= 20) {
                    asteroidSpeed = 250.f;
                } else if (totalTime >= 40) {
                    asteroidSpeed = 300.f;
                } else if (totalTime >= 60) {
                    asteroidSpeed = 350.f;
                }
                pos.y += asteroidSpeed * deltaTime;
                it->setPosition(pos);
                
                // Distance based collision detection; calculates the distance in x and y axis positions then uses pythagorean theorem to calculate final distance to center of sprite; on collision everything gets reset, the collision sound will play, the music will stop, and the game over screen will appear
                float dx = pos.x - spaceshipSpritePosition.x;
                float dy = pos.y - spaceshipSpritePosition.y;
                float distance = sqrt(dx*dx + dy*dy);
                if (distance < 80.0f) {
                    asteroids.clear();
                    asteroidSpawnRate = 0.75f;
                    spaceshipSpritePosition = {340,400};
                    spaceshipSprite.setPosition(spaceshipSpritePosition);
                    explosionSound.play();
                    currentState = GAME_OVER;
                    spaceshipAmbianceSound.stop();
                    break;
                }
                
                // After asteroids leave screen they are deleted from the vector
                if (pos.y > 650) {
                    it = asteroids.erase(it);
                } else {
                    ++it;
                }
            }
            
            // Score handling
            int score = totalTime;
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setPosition({50, 50});
            
        }
        
        // Closes window
        while (auto event = window.pollEvent()) {
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Handles mouse input on the main menu screen
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                
                if (currentState == MENU && mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    
                    if (mouseButtonPressed->position.x >= 300 && mouseButtonPressed->position.x <= 500 && mouseButtonPressed->position.y >= 300 && mouseButtonPressed->position.y <= 350) {
                        
                        totalTime = 0;
                        clock.restart();
                        currentState = PLAYING;
                        
                    }
                }
                
                // Handles mouse input on the game over screen
                if (currentState == GAME_OVER && mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    
                    if (mouseButtonPressed->position.x >= 300 && mouseButtonPressed->position.x <= 500 && mouseButtonPressed->position.y >= 300 && mouseButtonPressed->position.y <= 350) {
                        
                        currentState = MENU;
                        spaceshipAmbianceSound.stop();
                        
                    }
                }
            }
        
        }
        
        // Makes window black
        window.clear(sf::Color::Black);
        
        // Handles the game state by calling functions
        if (currentState == MENU) {
            drawMenu(window);
        } else if (currentState == PLAYING) {
            
            // Draws the background image to the window
            window.draw(playingBackgroundSprite);
            
            // Draws the spaceship sprite to the window
            window.draw(spaceshipSprite);
            
            // Draws the asteroids onto the window
            for (int i = 0; i < asteroids.size(); i++) {
                window.draw(asteroids[i]);
            }
            
            // Displayes score in window
            window.draw(scoreText);
            
        } else if (currentState == GAME_OVER) {
            drawGameOver(window);
        }
        
        // Displays everything
        window.display();
        
    }
    
}
