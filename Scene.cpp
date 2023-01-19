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

#include <math.h>

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

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz-0123456789.,;:$#'! /?%&()@ " };
	Cube_font_black = app->fonts->Load("Assets/Fonts/sprite_font_black.png", lookupTable, 6);
	Cube_font_white = app->fonts->Load("Assets/Fonts/sprite_font_white.png", lookupTable, 6);

	for (size_t i = 0; i < 4; i++)
	{
		quaternion[i].element = 0;
		quaternion[i].state = State::NORMAL;
	}
	for (size_t i = 0; i < 4; i++)
	{
		eulerAxisAngle[i].element = 0;
		eulerAxisAngle[i].state = State::NORMAL;
	}
	for (size_t i = 0; i < 3; i++)
	{
		rotationVector[i].element = 0;
		rotationVector[i].state = State::NORMAL;
	}
	for (size_t i = 0; i < 3; i++)
	{
		eulerAngles[i].element = 0;
		eulerAngles[i].state = State::NORMAL;
	}
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			rotationMatrix[i][j].element = 0;
			if (i == j)
			{
				rotationMatrix[i][j].element = 1;
			}
			rotationMatrix[i][j].state = State::NORMAL;
		}
	}

	ResetInput();

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
	UI_Update();

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


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		app->maths->whatisrotating.x = -1;
		app->maths->isCameraRotation = false;

	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		app->maths->whatisrotating.x = 1;
		app->maths->isCameraRotation = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		app->maths->whatisrotating.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		app->maths->whatisrotating.y = -1;
		app->maths->isCameraRotation = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		app->maths->whatisrotating.y = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
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

	PrintUI();

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(UI);
	app->fonts->UnLoad(Cube_font_black);
	app->fonts->UnLoad(Cube_font_white);

	return true;
}

void Scene::PrintUI() {

	if (quaternion[0].state != State::EDITING) {
		float element = (float)quaternion[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[0].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else{ app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[1].state != State::EDITING) {
		float element = (float)quaternion[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[1].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[2].state != State::EDITING) {
		float element = (float)quaternion[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[2].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[3].state != State::EDITING) {
		float element = (float)quaternion[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[3].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[0].state != State::EDITING) {
		float element = (float)eulerAxisAngle[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[0].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[1].state != State::EDITING) {
		float element = (float)eulerAxisAngle[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[1].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[2].state != State::EDITING) {
		float element = (float)eulerAxisAngle[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[2].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[3].state != State::EDITING) {
		float element = (float)eulerAxisAngle[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[3].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 3), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (rotationVector[0].state != State::EDITING) {
		float element = (float)rotationVector[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[0].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (rotationVector[1].state != State::EDITING) {
		float element = (float)rotationVector[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[1].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (rotationVector[2].state != State::EDITING) {
		float element = (float)rotationVector[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[2].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (eulerAngles[0].state != State::EDITING) {
		float element = (float)eulerAngles[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[0].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 0), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	if (eulerAngles[1].state != State::EDITING) {
		float element = (float)eulerAngles[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[1].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 1), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	if (eulerAngles[2].state != State::EDITING) {
		float element = (float)eulerAngles[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[2].state == State::NORMAL) {
			app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		app->fonts->BlitText(1023 + (143 * 2), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	if(generalState != State::NORMAL){ blinking++; }
	else { blinking = 0; }

	if (blinking >= 20) { blinking = 0; }
}

Button Scene::ButtonDetection() {
	Button buttonDetected;

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
	app->input->GetMouseX() > 900 &&
	app->input->GetMouseX() < 991 &&
	app->input->GetMouseY() > (260 + (109 * 0)) &&
	app->input->GetMouseY() < (300 + (109 * 0))) {
		buttonDetected = Button::PUSH1;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > 900 &&
		app->input->GetMouseX() < 991 &&
		app->input->GetMouseY() > (260 + (109 * 1)) &&
		app->input->GetMouseY() < (300 + (109 * 1))) {
		buttonDetected = Button::PUSH2;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > 900 &&
		app->input->GetMouseX() < 991 &&
		app->input->GetMouseY() > (260 + (109 * 2)) &&
		app->input->GetMouseY() < (300 + (109 * 2))) {
		buttonDetected = Button::PUSH3;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > 900 &&
		app->input->GetMouseX() < 991 &&
		app->input->GetMouseY() > (260 + (109 * 3)) &&
		app->input->GetMouseY() < (300 + (109 * 3))) {
		buttonDetected = Button::PUSH4;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 0)) &&
		app->input->GetMouseX() < (1132 + (143 * 0)) &&
		app->input->GetMouseY() > (261 + (108 * 0)) &&
		app->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION1;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 1)) &&
		app->input->GetMouseX() < (1132 + (143 * 1)) &&
		app->input->GetMouseY() > (261 + (108 * 0)) &&
		app->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION2;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 2)) &&
		app->input->GetMouseX() < (1132 + (143 * 2)) &&
		app->input->GetMouseY() > (261 + (108 * 0)) &&
		app->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION3;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 3)) &&
		app->input->GetMouseX() < (1132 + (143 * 3)) &&
		app->input->GetMouseY() > (261 + (108 * 0)) &&
		app->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION4;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 0)) &&
		app->input->GetMouseX() < (1132 + (143 * 0)) &&
		app->input->GetMouseY() > (261 + (108 * 1)) &&
		app->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE1;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 1)) &&
		app->input->GetMouseX() < (1132 + (143 * 1)) &&
		app->input->GetMouseY() > (261 + (108 * 1)) &&
		app->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE2;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 2)) &&
		app->input->GetMouseX() < (1132 + (143 * 2)) &&
		app->input->GetMouseY() > (261 + (108 * 1)) &&
		app->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE3;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 3)) &&
		app->input->GetMouseX() < (1132 + (143 * 3)) &&
		app->input->GetMouseY() > (261 + (108 * 1)) &&
		app->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE4;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 0)) &&
		app->input->GetMouseX() < (1132 + (143 * 0)) &&
		app->input->GetMouseY() > (261 + (108 * 2)) &&
		app->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES1;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 1)) &&
		app->input->GetMouseX() < (1132 + (143 * 1)) &&
		app->input->GetMouseY() > (261 + (108 * 2)) &&
		app->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES2;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		app->input->GetMouseX() > (1005 + (143 * 2)) &&
		app->input->GetMouseX() < (1132 + (143 * 2)) &&
		app->input->GetMouseY() > (261 + (108 * 2)) &&
		app->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES3;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
	app->input->GetMouseX() > (1005 + (143 * 0)) &&
	app->input->GetMouseX() < (1132 + (143 * 0)) &&
	app->input->GetMouseY() > (261 + (108 * 3)) &&
	app->input->GetMouseY() < (299 + (108 * 3))) {
	buttonDetected = Button::EULERANGLES1;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
	app->input->GetMouseX() > (1005 + (143 * 1)) &&
	app->input->GetMouseX() < (1132 + (143 * 1)) &&
	app->input->GetMouseY() > (261 + (108 * 3)) &&
	app->input->GetMouseY() < (299 + (108 * 3))) {
	buttonDetected = Button::EULERANGLES2;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
	app->input->GetMouseX() > (1005 + (143 * 2)) &&
	app->input->GetMouseX() < (1132 + (143 * 2)) &&
	app->input->GetMouseY() > (261 + (108 * 3)) &&
	app->input->GetMouseY() < (299 + (108 * 3))) {
	buttonDetected = Button::EULERANGLES3;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
	app->input->GetMouseX() > (1470) &&
	app->input->GetMouseX() < (1546) &&
	app->input->GetMouseY() > (775) &&
	app->input->GetMouseY() < (815)) {
	buttonDetected = Button::RESET;
	}
	else { buttonDetected = Button::NOBUTTON; }

	return buttonDetected;
}

void Scene::UI_Update() {

	//from matrixxd to ui_element
	if (generalState == State::NORMAL)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (quaternion[i].state != State::EDITED)
			{
				quaternion[i].element = app->maths->quaternion(i, 0);
			}
		}
		for (size_t i = 0; i < 4; i++)
		{
			if (eulerAxisAngle[i].state != State::EDITED)
			{
				eulerAxisAngle[i].element = app->maths->eulerAxisAngles(i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (rotationVector[i].state != State::EDITED)
			{
				rotationVector[i].element = app->maths->rotationVector(i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (eulerAngles[i].state != State::EDITED)
			{	//3 - i cz euler angles in ui is x,y,z; in MatrixXd is z,y,x
				eulerAngles[i].element = app->maths->eulerAngles(2 - i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (rotationMatrix[i][j].state != State::EDITED)
				{
					rotationMatrix[i][j].element = app->maths->rotationMatrix(i, j);
				}
			}
		}
	}


	if (generalState != State::EDITING) { button = ButtonDetection(); }

	switch (button)
	{
	case NOBUTTON:
		break;
	case PUSH1:
		if (UIelementStateCheck(quaternion, 4)) {
			//Rotate the cube via quaternion
			MatrixXd newq(4, 1);
			newq <<	quaternion[0].element,
					quaternion[1].element,
					quaternion[2].element,
					quaternion[3].element;
			MatrixXd newea = app->maths->RotationChangeOfWritting(newq, 'q', 'e');
			app->maths->angles.z = newea(2, 0);
			app->maths->angles.y = newea(1, 0);
			app->maths->angles.x = newea(0, 0);
			app->maths->cube1.Reset();
			app->maths->edited = true;
			generalState = State::NORMAL;
			SetNormal(quaternion, 4);
			SetNormal(eulerAxisAngle, 4);
			SetNormal(rotationVector, 3);
			SetNormal(eulerAngles, 3);
		}
		break;
	case PUSH2:
		if (UIelementStateCheck(eulerAxisAngle, 4)) {
			//Rotate the cube via Euler axis-angle
			VectorXd newnewp(4);
			newnewp <<	eulerAxisAngle[0].element,
						eulerAxisAngle[1].element,
						eulerAxisAngle[2].element,
						eulerAxisAngle[3].element;
			newnewp.normalize();
			MatrixXd newp(4, 1);
			newp << newnewp(0, 0),
					newnewp(1, 0),
					newnewp(2, 0),
					newnewp(3, 0);

			MatrixXd newea = app->maths->RotationChangeOfWritting(newp, 'p', 'e');
			app->maths->angles.z = newea(2, 0);
			app->maths->angles.y = newea(1, 0);
			app->maths->angles.x = newea(0, 0);
			app->maths->cube1.Reset();
			app->maths->edited = true;
			generalState = State::NORMAL;
			SetNormal(quaternion, 4);
			SetNormal(eulerAxisAngle, 4);
			SetNormal(rotationVector, 3);
			SetNormal(eulerAngles, 3);
		}
		break;
	case PUSH3:
		if (UIelementStateCheck(rotationVector, 3)) {
			//Rotate the cube via Rotation vector
			MatrixXd newv(3, 1);
			newv << rotationVector[0].element,
					rotationVector[1].element,
					rotationVector[2].element;
			MatrixXd newea = app->maths->RotationChangeOfWritting(newv, 'v', 'e');
			app->maths->angles.z = newea(2, 0);
			app->maths->angles.y = newea(1, 0);
			app->maths->angles.x = newea(0, 0);
			app->maths->cube1.Reset();
			app->maths->edited = true;
			generalState = State::NORMAL;
			SetNormal(quaternion, 4);
			SetNormal(eulerAxisAngle, 4);
			SetNormal(rotationVector, 3);
			SetNormal(eulerAngles, 3);
		}
		break;
	case PUSH4:
		if (UIelementStateCheck(eulerAngles, 3)) {
			//Rotate the cube via Euler angles
			app->maths->angles.z = eulerAngles[2].element;
			app->maths->angles.y = eulerAngles[1].element;
			app->maths->angles.x = eulerAngles[0].element;
			app->maths->edited = true;
			app->maths->cube1.Reset();
			generalState = State::NORMAL;
			SetNormal(quaternion, 4);
			SetNormal(eulerAxisAngle, 4);
			SetNormal(rotationVector, 3);
			SetNormal(eulerAngles, 3);
		}
		break;
	case QUATERNION1:
		if (quaternion[0].state != State::EDITING) { quaternion[0].element = 0; }

		generalState = State::EDITING;
		quaternion[0].state = State::EDITING;

		Input();
		ApplyImput(quaternion[0]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			quaternion[0].state = State::EDITED;
			ResetInput();
		}
		break;
	case QUATERNION2:
		if (quaternion[1].state != State::EDITING) { quaternion[1].element = 0; }

		generalState = State::EDITING;
		quaternion[1].state = State::EDITING;

		Input();
		ApplyImput(quaternion[1]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			quaternion[1].state = State::EDITED;
			ResetInput();
		}
		break;
	case QUATERNION3:
		if (quaternion[2].state != State::EDITING) { quaternion[2].element = 0; }

		generalState = State::EDITING;
		quaternion[2].state = State::EDITING;

		Input();
		ApplyImput(quaternion[2]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			quaternion[2].state = State::EDITED;
			ResetInput();
		}
		break;
	case QUATERNION4:
		if (quaternion[3].state != State::EDITING) { quaternion[3].element = 0; }

		generalState = State::EDITING;
		quaternion[3].state = State::EDITING;

		Input();
		ApplyImput(quaternion[3]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			quaternion[3].state = State::EDITED;
			ResetInput();
		}
		break;
	case AXISANGLE1:
		if (eulerAxisAngle[0].state != State::EDITING) { eulerAxisAngle[0].element = 0; }

		generalState = State::EDITING;
		eulerAxisAngle[0].state = State::EDITING;

		Input();
		ApplyImput(eulerAxisAngle[0]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAxisAngle[0].state = State::EDITED;
			ResetInput();
		}
		break;
	case AXISANGLE2:
		if (eulerAxisAngle[1].state != State::EDITING) { eulerAxisAngle[1].element = 0; }

		generalState = State::EDITING;
		eulerAxisAngle[1].state = State::EDITING;

		Input();
		ApplyImput(eulerAxisAngle[1]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAxisAngle[1].state = State::EDITED;
			ResetInput();
		}
		break;
	case AXISANGLE3:
		if (eulerAxisAngle[2].state != State::EDITING) { eulerAxisAngle[2].element = 0; }

		generalState = State::EDITING;
		eulerAxisAngle[2].state = State::EDITING;

		Input();
		ApplyImput(eulerAxisAngle[2]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAxisAngle[2].state = State::EDITED;
			ResetInput();
		}
		break;
	case AXISANGLE4:
		if (eulerAxisAngle[3].state != State::EDITING) { eulerAxisAngle[3].element = 0; }

		generalState = State::EDITING;
		eulerAxisAngle[3].state = State::EDITING;

		Input();
		ApplyImput(eulerAxisAngle[3]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAxisAngle[3].state = State::EDITED;
			ResetInput();
		}
		break;
	case ROTATIONANGLES1:
		if (rotationVector[0].state != State::EDITING) { rotationVector[0].element = 0; }

		generalState = State::EDITING;
		rotationVector[0].state = State::EDITING;

		Input();
		ApplyImput(rotationVector[0]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			rotationVector[0].state = State::EDITED;
			ResetInput();
		}
		break;
	case ROTATIONANGLES2:
		if (rotationVector[1].state != State::EDITING) { rotationVector[1].element = 0; }

		generalState = State::EDITING;
		rotationVector[1].state = State::EDITING;

		Input();
		ApplyImput(rotationVector[1]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			rotationVector[1].state = State::EDITED;
			ResetInput();
		}
		break;
	case ROTATIONANGLES3:
		if (rotationVector[2].state != State::EDITING) { rotationVector[2].element = 0; }

		generalState = State::EDITING;
		rotationVector[2].state = State::EDITING;

		Input();
		ApplyImput(rotationVector[2]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			rotationVector[2].state = State::EDITED;
			ResetInput();
		}
		break;
	case EULERANGLES1:
		if (eulerAngles[0].state != State::EDITING) { eulerAngles[0].element = 0; }

		generalState = State::EDITING;
		eulerAngles[0].state = State::EDITING;

		Input();
		ApplyImput(eulerAngles[0]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAngles[0].state = State::EDITED;
			ResetInput();
		}
		break;
	case EULERANGLES2:
		if (eulerAngles[1].state != State::EDITING) { eulerAngles[1].element = 0; }

		generalState = State::EDITING;
		eulerAngles[1].state = State::EDITING;

		Input();
		ApplyImput(eulerAngles[1]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAngles[1].state = State::EDITED;
			ResetInput();
		}
		break;
	case EULERANGLES3:
		if (eulerAngles[2].state != State::EDITING) { eulerAngles[2].element = 0; }

		generalState = State::EDITING;
		eulerAngles[2].state = State::EDITING;

		Input();
		ApplyImput(eulerAngles[2]);

		if (KeyInputs() == Keys::ENTER) {
			generalState = State::NORMAL;
			eulerAngles[2].state = State::EDITED;
			ResetInput();
		}
		break;
	case RESET:
		break;
	default:
		break;
	}
}

Keys Scene::KeyInputs() {
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		return Keys::ENTER;
	}
	else if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		return Keys::KEYDEL;
	}
	else if (app->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_DOWN)
	{
		return Keys::KEYDOT;
	}
	else if (app->input->GetKey(SDL_SCANCODE_SLASH) == KEY_DOWN)
	{
		return Keys::KEYMINUS;
	}
	else if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		return Keys::KEY0;
	}
	else if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		return Keys::KEY1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		return Keys::KEY2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		return Keys::KEY3;
	}
	else if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		return Keys::KEY4;
	}
	else if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		return Keys::KEY5;
	}
	else if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		return Keys::KEY6;
	}
	else if (app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		return Keys::KEY7;
	}
	else if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		return Keys::KEY8;
	}
	else if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		return Keys::KEY9;
	}
	else {
		return Keys::NOKEY;
	}
}

bool Scene::UIelementStateCheck(UI_Element element[], int size) {
	for (size_t i = 0; i < size; i++)
	{
		if (element[i].state == State::EDITED) { return true; }
	}

	return false;
}

void Scene::SetNormal(UI_Element element[], int size) {
	for (size_t i = 0; i < size; i++)
	{
		element[i].state = State::NORMAL;
	}
}

void Scene::ApplyImput(UI_Element &element) {
	float aux = 0;
	bool negative = false;
	bool decimal = false;
	bool noImput = false;
	int currentKey = 0;

	if (KeyInputs() == Keys::NOKEY) { noImput = true; }

	for (size_t i = 0; i < 10; i++)
	{
		Keys key = input[i];

		switch (key)
		{
		case ENTER:
			break;
		case KEY1:
			currentKey = 1;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY2:
			currentKey = 2;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY3:
			currentKey = 3;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY4:
			currentKey = 4;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY5:
			currentKey = 5;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY6:
			currentKey = 6;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY7:
			currentKey = 7;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY8:
			currentKey = 8;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY9:
			currentKey = 9;
			if (!decimal) {
				int power = -1;
				for (size_t j = i; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT || input[j] == Keys::NOKEY) { break; }
					power++;
				}
				aux += (currentKey * (pow(10, power)));
			}
			else {
				int power = 0;
				bool dec = false;
				for (size_t j = 0; j < 10; j++)
				{
					if (input[j] == Keys::KEYDOT) {
						power = i - j;
						break;
					}
				}
				aux += (currentKey / (pow(10, power)));
			}
			break;
		case KEY0:
			
			break;
		case KEYDOT:
			decimal = true;
			break;
		case KEYMINUS:
			negative = true;
			break;
		case KEYDEL:

			break;
		case NOKEY:
			break;
		default:
			break;
		}
	}

	if (negative) { aux *= -1; }

	if (!noImput) {
		element.element = aux;
	}
}

void Scene::ResetInput() {
	for (size_t i = 0; i < 10; i++)
	{
		input[i] = Keys::NOKEY;
	}
	cursor = 0;
}

void Scene::Input() {
	Keys key = KeyInputs();

	switch (key)
	{
	case ENTER:
		break;
	case KEY1:
		if (cursor < 10) {
			input[cursor] = Keys::KEY1;
			cursor++;
		}
		break;
	case KEY2:
		if (cursor < 10) {
			input[cursor] = Keys::KEY2;
			cursor++;
		}
		break;
	case KEY3:
		if (cursor < 10) {
			input[cursor] = Keys::KEY3;
			cursor++;
		}
		break;
	case KEY4:
		if (cursor < 10) {
			input[cursor] = Keys::KEY4;
			cursor++;
		}
		break;
	case KEY5:
		if (cursor < 10) {
			input[cursor] = Keys::KEY5;
			cursor++;
		}
		break;
	case KEY6:
		if (cursor < 10) {
			input[cursor] = Keys::KEY6;
			cursor++;
		}
		break;
	case KEY7:
		if (cursor < 10) {
			input[cursor] = Keys::KEY7;
			cursor++;
		}
		break;
	case KEY8:
		if (cursor < 10) {
			input[cursor] = Keys::KEY8;
			cursor++;
		}
		break;
	case KEY9:
		if (cursor < 10) {
			input[cursor] = Keys::KEY9;
			cursor++;
		}
		break;
	case KEY0:
		if (cursor < 10) {
			input[cursor] = Keys::KEY0;
			cursor++;
		}
		break;
	case KEYDOT:
		if (cursor < 10) {
			input[cursor] = Keys::KEYDOT;
			cursor++;
		}
		break;
	case KEYMINUS:
		if (cursor == 0) {
			input[cursor] = Keys::KEYMINUS;
			cursor++;
		}
		break;
	case KEYDEL:
		if (cursor > 0) {
			cursor--;
			input[cursor] = Keys::NOKEY;
		}
		else {
			input[cursor] = Keys::KEY0;
		}
		break;
	case NOKEY:
		break;
	default:
		break;
	}
}