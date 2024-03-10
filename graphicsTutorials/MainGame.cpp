#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

#include <iostream>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_time(0.0f)
{

}

MainGame::~MainGame() {

}

void MainGame::run() {
	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);
	_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window =
		SDL_CreateWindow("Game Engine",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_screenWidth,
			_screenHeight, SDL_WINDOW_OPENGL);

	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	// Set up OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}

	// Set up glew (optional but recommended)
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

	// Tell SDL that we want a double buffered window so we don't get
	// any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0);

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT) {
		processInput();
		_time += 0.001;
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << '\n';
			break;
		}
	}
}

// Draws the game using OpenGL
void MainGame::drawGame()
{

	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	// Swap our buffer and draw everything to the screen
	SDL_GL_SwapWindow(_window);
}
