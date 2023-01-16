#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Maths;
class ModuleFonts;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Maths* maths;
	ModuleFonts* fonts;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	uint frames;
	float dt;
};

extern App* app;

#endif	// __APP_H__