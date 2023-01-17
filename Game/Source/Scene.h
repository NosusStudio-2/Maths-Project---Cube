#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* UI;

	// Fonts
	int Cube_font_black = -1;
	int Cube_font_white = -1;

	//UI elements
	char quaternion1Text[10] = { "\0" };
	char quaternion2Text[10] = { "\0" };
	char quaternion3Text[10] = { "\0" };
	char quaternion4Text[10] = { "\0" };

	char axisAngle1Text[10] = { "\0" };
	char axisAngle2Text[10] = { "\0" };
	char axisAngle3Text[10] = { "\0" };
	char axisAngle4Text[10] = { "\0" };

	char rotationVector1Text[10] = { "\0" };
	char rotationVector2Text[10] = { "\0" };
	char rotationVector3Text[10] = { "\0" };

	char eulerAngles1Text[10] = { "\0" };
	char eulerAngles2Text[10] = { "\0" };
	char eulerAngles3Text[10] = { "\0" };

	char rotationMatrix1Text[10] = { "\0" };
	char rotationMatrix2Text[10] = { "\0" };
	char rotationMatrix3Text[10] = { "\0" };
	char rotationMatrix4Text[10] = { "\0" };
	char rotationMatrix5Text[10] = { "\0" };
	char rotationMatrix6Text[10] = { "\0" };
	char rotationMatrix7Text[10] = { "\0" };
	char rotationMatrix8Text[10] = { "\0" };
	char rotationMatrix9Text[10] = { "\0" };
};

#endif // __SCENE_H__