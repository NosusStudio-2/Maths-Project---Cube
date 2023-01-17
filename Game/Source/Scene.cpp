#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Maths.h"
#include "Defs.h"
#include "Log.h"
#include "ModuleFonts.h"

Scene::Scene() : Module()
{
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->maths->cube1.points[1 - 1] = { 100,100,100 };
	app->maths->cube1.points[2 - 1] = { 200,100,100 };
	app->maths->cube1.points[3 - 1] = { 200,200,100 };
	app->maths->cube1.points[4 - 1] = { 100,200,100 };

	app->maths->cube1.points[5 - 1] = { 100,100,200 };
	app->maths->cube1.points[6 - 1] = { 200,100,200 };
	app->maths->cube1.points[7 - 1] = { 200,200,200 };
	app->maths->cube1.points[8 - 1] = { 100,200,200 };
	app->maths->rotationPivot = { 0,0,0 };
	for (size_t i = 0; i < 8; i++)
	{
		app->maths->rotationPivot.x += app->maths->cube1.points[i].x;
		app->maths->rotationPivot.y += app->maths->cube1.points[i].y;
		app->maths->rotationPivot.z += app->maths->cube1.points[i].z;
	}
	app->maths->rotationPivot.x /= 8;
	app->maths->rotationPivot.y /= 8;
	app->maths->rotationPivot.z /= 8;

	UI = app->tex->Load("Assets/Textures/Maths-Cube_Borders.png");

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 0123456789.,;:$#'! /?%&()@ " };
	Cube_font_black = app->fonts->Load("Assets/Fonts/sprite_font_black.png", lookupTable, 6);
	Cube_font_white = app->fonts->Load("Assets/Fonts/sprite_font_white.png", lookupTable, 6);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	app->maths->isCameraRotation = true;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		app->maths->whatisrotating.x = 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		app->maths->whatisrotating.x = -1;
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		app->maths->whatisrotating.y = 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		app->maths->whatisrotating.y = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		app->maths->whatisrotating.y = -1;
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		app->maths->whatisrotating.y = 0;
	}


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		app->maths->whatisrotating.x = -1;
		app->maths->isCameraRotation = false;

	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		app->maths->whatisrotating.x = 1;
		app->maths->isCameraRotation = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		app->maths->whatisrotating.y = -1;
		app->maths->isCameraRotation = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		app->maths->whatisrotating.y = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		app->maths->whatisrotating.y = 1;
		app->maths->isCameraRotation = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		app->maths->whatisrotating.y = 0;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(UI, 0, 0);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
