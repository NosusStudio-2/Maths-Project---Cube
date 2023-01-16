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
	app->maths->cube1.points[1 - 1] = { 0,0,0 };
	app->maths->cube1.points[2 - 1] = { 1,0,0 };
	app->maths->cube1.points[3 - 1] = { 1,0,1 };
	app->maths->cube1.points[4 - 1] = { 0,0,1 };
	app->maths->cube1.points[5 - 1] = { 0,1,0 };
	app->maths->cube1.points[6 - 1] = { 1,1,0 };
	app->maths->cube1.points[7 - 1] = { 1,1,1 };
	app->maths->cube1.points[8 - 1] = { 0,1,1 };

	app->maths->focalLenght = (double)(1 / 34);

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




	//if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)

	//if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)

	//if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)

	//if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)


	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)

	//if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)

	//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)

	//if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

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
