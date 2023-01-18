#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

enum State
{
	NORMAL,
	EDITING,
	EDITED
};

enum Button
{
	NOBUTTON,
	PUSH1,
	PUSH2,
	PUSH3,
	PUSH4,
	QUATERNION1,
	QUATERNION2,
	QUATERNION3,
	QUATERNION4,
	AXISANGLE1,
	AXISANGLE2,
	AXISANGLE3,
	AXISANGLE4,
	ROTATIONANGLES1,
	ROTATIONANGLES2,
	ROTATIONANGLES3,
	EULERANGLES1,
	EULERANGLES2,
	EULERANGLES3,
	RESET
};

enum Keys
{
	ENTER,
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY0,
	KEYDOT,
	KEYMINUS,
	KEYDEL,
	NOKEY
};

struct UI_Element
{
	State state;
	double element;
};

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

	//UI functions
	Button ButtonDetection();
	Keys KeyInputs();
	bool UIelementStateCheck(UI_Element element[], int size);
	void SetNormal(UI_Element element[], int size);
	void Input();
	void ApplyImput(UI_Element &element);
	void ResetInput();

	void UI_Update();
	void PrintUI();

private:
	State generalState = State::NORMAL;
	Button button = Button::NOBUTTON;
	Keys input[10];
	int cursor = 0;

	UI_Element quaternion[4];
	UI_Element eulerAxisAngle[4];
	UI_Element rotationVector[3];
	UI_Element eulerAngles[3];

	SDL_Texture* UI;

	uint blinking = 0;

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