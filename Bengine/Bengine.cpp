// Bengine.cpp : Defines the functions for the static library.
//

#include "framework.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Bengine.h"

namespace Bengine {

	int init() {
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Tell SDL that we want a double buffered window so we don't get
		// any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}