# Asteroid Dodger 🚀

A simple arcade-style game built using C++ and SFML. The objective is to dodge falling asteroids for as long as possible while controlling a spaceship.

## 🎮 Gameplay

- Start at the main menu and click "Play" to begin.
- Use **WASD** or **Arrow Keys** to move the spaceship.
- Asteroids fall faster over time.
- If you collide with an asteroid, the game ends and displays a "Game Over" screen.
- You can play again by clicking the "Play Again?" button.

## 🖼️ Assets Used

- Background images (`orig.png`, `orig2.png`, `1.png`)
- Sprites: `spaceship.png`, `asteroid.png`, `Explosion_5.png`
- Fonts: `8-bit-hud.ttf`
- Audio: `spaceshipAmbiance.wav`, `explosion.wav`

> Ensure all asset files are located in the same directory as your executable or properly configured in your resource path.

## 🛠️ Build Instructions

1. Make sure you have [SFML](https://www.sfml-dev.org/) installed.
2. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/asteroid-dodger.git
   cd asteroid-dodger 
3. Compile and run using:
```bash
g++ main.cpp -o AsteroidDodger -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./AsteroidDodgerGame


