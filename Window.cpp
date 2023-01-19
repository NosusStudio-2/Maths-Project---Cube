#include "Application.h"
#include "Window.h"

#include "Globals.h"

#include "SDL/include/SDL.h"


Window::Window() : Module()
{
	window = NULL;
	screenSurface = NULL;
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = false;
		bool borderless = false;
		bool resizable = false;
		bool fullscreen_window = false;
		bool opengl = false;

		width = 734; 
		height = 900; 
		scale = 1; 

		if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless) flags |= SDL_WINDOW_BORDERLESS;
		if (resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (fullscreen_window) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (opengl) flags |= SDL_WINDOW_OPENGL;

		window = SDL_CreateWindow("UI of the cube project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint Window::GetScale() const
{
	return scale;
}