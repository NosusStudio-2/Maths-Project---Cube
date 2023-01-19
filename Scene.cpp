#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "Textures.h"
#include "Render.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "Scene.h"
#include "Maths.h"
#include "Globals.h"
#include "ModuleFonts.h"
#include "glmath.h"

#include <math.h>

Scene::Scene() : Module()
{
	
}

// Destructor
Scene::~Scene()
{}

// Called before the first frame
bool Scene::Start()
{
	App->maths->cube1.points[1 - 1] = { 100,100,100 };
	App->maths->cube1.points[2 - 1] = { 200,100,100 };
	App->maths->cube1.points[3 - 1] = { 200,200,100 };
	App->maths->cube1.points[4 - 1] = { 100,200,100 };
	
	App->maths->cube1.points[5 - 1] = { 100,100,200 };
	App->maths->cube1.points[6 - 1] = { 200,100,200 };
	App->maths->cube1.points[7 - 1] = { 200,200,200 };
	App->maths->cube1.points[8 - 1] = { 100,200,200 };
	App->maths->rotationPivot = { 0,0,0 };
	for (size_t i = 0; i < 8; i++)
	{
		App->maths->rotationPivot.x += App->maths->cube1.points[i].x;
		App->maths->rotationPivot.y += App->maths->cube1.points[i].y;
		App->maths->rotationPivot.z += App->maths->cube1.points[i].z;
	}
	App->maths->rotationPivot.x /= 8;
	App->maths->rotationPivot.y /= 8;
	App->maths->rotationPivot.z /= 8;

	UI = App->tex->Load("Assets/Textures/UI_window_text.png");

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz-0123456789.,;:$#'! /?%&()@ " };
	Cube_font_black = App->fonts->Load("Assets/Fonts/sprite_font_black.png", lookupTable, 6);
	Cube_font_white = App->fonts->Load("Assets/Fonts/sprite_font_white.png", lookupTable, 6);

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
update_status Scene::Update(float dt)
{
	UI_Update();


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->maths->angles.x--;
		MatrixXd e(3, 1);
		e <<	App->maths->angles.z,
				App->maths->angles.y,
				App->maths->angles.x;
		MatrixXd p = App->maths->RotationChangeOfWritting(e, 'e', 'p');
		App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });

	/*	App->maths->whatisrotating.x = -1;
		App->maths->isCameraRotation = false;*/

	}
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	//{
	//	App->maths->whatisrotating.x = 0;
	//}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->maths->angles.x++;
		MatrixXd e(3, 1);
		e << App->maths->angles.z,
			App->maths->angles.y,
			App->maths->angles.x;
		MatrixXd p = App->maths->RotationChangeOfWritting(e, 'e', 'p');
		App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
		//App->maths->whatisrotating.x = 1;
		//App->maths->isCameraRotation = false;
	}
	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	//{
	//	App->maths->whatisrotating.x = 0;
	//}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->maths->angles.y--;
		MatrixXd e(3, 1);
		e << App->maths->angles.z,
			App->maths->angles.y,
			App->maths->angles.x;
		MatrixXd p = App->maths->RotationChangeOfWritting(e, 'e', 'p');
		App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
		//App->maths->whatisrotating.y = -1;
		//App->maths->isCameraRotation = false;
	}
	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	//{
	//	App->maths->whatisrotating.y = 0;
	//}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->maths->angles.y++;
		MatrixXd e(3, 1);
		e << App->maths->angles.z,
			App->maths->angles.y,
			App->maths->angles.x;
		MatrixXd p = App->maths->RotationChangeOfWritting(e, 'e', 'p');
		App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
		//App->maths->whatisrotating.y = 1;
		//App->maths->isCameraRotation = false;
	}
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	//{
	//	App->maths->whatisrotating.y = 0;
	//}

	App->scene_intro->primitives[0]->Update();

	return update_status::UPDATE_CONTINUE;
}

// Called each loop iteration
update_status Scene::PostUpdate(float dt)
{
	App->render->DrawTexture(UI, 0, 0);

	PrintUI();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return update_status::UPDATE_STOP;

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(UI);
	App->fonts->UnLoad(Cube_font_black);
	App->fonts->UnLoad(Cube_font_white);

	return true;
}

void Scene::PrintUI() {

	if (quaternion[0].state != State::EDITING) {
		float element = (float)quaternion[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[0].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else{ App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[1].state != State::EDITING) {
		float element = (float)quaternion[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[1].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[2].state != State::EDITING) {
		float element = (float)quaternion[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[2].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (quaternion[3].state != State::EDITING) {
		float element = (float)quaternion[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (quaternion[3].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 0), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 0), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)quaternion[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 0), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[0].state != State::EDITING) {
		float element = (float)eulerAxisAngle[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[0].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[1].state != State::EDITING) {
		float element = (float)eulerAxisAngle[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[1].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[2].state != State::EDITING) {
		float element = (float)eulerAxisAngle[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[2].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (eulerAxisAngle[3].state != State::EDITING) {
		float element = (float)eulerAxisAngle[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAxisAngle[3].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 1), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 1), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAxisAngle[3].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 3), 275 + (108 * 1), Cube_font_black, translationToText);
	}

	if (rotationVector[0].state != State::EDITING) {
		float element = (float)rotationVector[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[0].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (rotationVector[1].state != State::EDITING) {
		float element = (float)rotationVector[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[1].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (rotationVector[2].state != State::EDITING) {
		float element = (float)rotationVector[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (rotationVector[2].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 2), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 2), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)rotationVector[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 2), Cube_font_black, translationToText);
	}

	if (eulerAngles[0].state != State::EDITING) {
		float element = (float)eulerAngles[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[0].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[0].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 0), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	if (eulerAngles[1].state != State::EDITING) {
		float element = (float)eulerAngles[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[1].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[1].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 1), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	if (eulerAngles[2].state != State::EDITING) {
		float element = (float)eulerAngles[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		if (eulerAngles[2].state == State::NORMAL) {
			App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 3), Cube_font_white, translationToText);
		}
		else { App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 3), Cube_font_black, translationToText); }
	}
	else if (blinking >= 10) {
		float element = (float)eulerAngles[2].element;
		snprintf(translationToText, sizeof translationToText, "%f", element);
		App->fonts->BlitText((1023 - 879) + (143 * 2), 275 + (108 * 3), Cube_font_black, translationToText);
	}

	float element = (float)rotationMatrix[0][0].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 0), 373 + 38 + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[0][1].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 1), 373 + 38 + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[0][2].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 2), 373 + 38 + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[1][0].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 0), 383 + 10 + (38 * 2) + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[1][1].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 1), 383 + 10 + (38 * 2) + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[1][2].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 2), 383 + 10 + (38 * 2) + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[2][0].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 0), 393 + 10 + (38 * 3) + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[2][1].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 1), 393 + 10 + (38 * 3) + (108 * 3), Cube_font_white, translationToText);

	element = (float)rotationMatrix[2][2].element;
	snprintf(translationToText, sizeof translationToText, "%f", element);
	App->fonts->BlitText((1023 - 879) + (143 * 2), 393 + 10 + (38 * 3) + (108 * 3), Cube_font_white, translationToText);

	if(generalState != State::NORMAL){ blinking++; }
	else { blinking = 0; }

	if (blinking >= 20) { blinking = 0; }
}

Button Scene::ButtonDetection() {
	Button buttonDetected;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (900 - 879) &&
		App->input->GetMouseX() < (991 - 879) &&
		App->input->GetMouseY() > (260 + (109 * 0)) &&
		App->input->GetMouseY() < (300 + (109 * 0))) {
		buttonDetected = Button::PUSH1;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (900 - 879) &&
		App->input->GetMouseX() < (991 - 879) &&
		App->input->GetMouseY() > (260 + (109 * 1)) &&
		App->input->GetMouseY() < (300 + (109 * 1))) {
		buttonDetected = Button::PUSH2;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (900 - 879) &&
		App->input->GetMouseX() < (991 - 879) &&
		App->input->GetMouseY() > (260 + (109 * 2)) &&
		App->input->GetMouseY() < (300 + (109 * 2))) {
		buttonDetected = Button::PUSH3;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (900 - 879) &&
		App->input->GetMouseX() < (991 - 879) &&
		App->input->GetMouseY() > (260 + (109 * 3)) &&
		App->input->GetMouseY() < (300 + (109 * 3))) {
		buttonDetected = Button::PUSH4;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 0)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 0)) &&
		App->input->GetMouseY() > (261 + (108 * 0)) &&
		App->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION1;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 1)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 1)) &&
		App->input->GetMouseY() > (261 + (108 * 0)) &&
		App->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION2;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 2)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 2)) &&
		App->input->GetMouseY() > (261 + (108 * 0)) &&
		App->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION3;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 3)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 3)) &&
		App->input->GetMouseY() > (261 + (108 * 0)) &&
		App->input->GetMouseY() < (299 + (108 * 0))) {
		buttonDetected = Button::QUATERNION4;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 0)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 0)) &&
		App->input->GetMouseY() > (261 + (108 * 1)) &&
		App->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE1;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 1)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 1)) &&
		App->input->GetMouseY() > (261 + (108 * 1)) &&
		App->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE2;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 2)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 2)) &&
		App->input->GetMouseY() > (261 + (108 * 1)) &&
		App->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE3;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 3)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 3)) &&
		App->input->GetMouseY() > (261 + (108 * 1)) &&
		App->input->GetMouseY() < (299 + (108 * 1))) {
		buttonDetected = Button::AXISANGLE4;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 0)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 0)) &&
		App->input->GetMouseY() > (261 + (108 * 2)) &&
		App->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES1;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 1)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 1)) &&
		App->input->GetMouseY() > (261 + (108 * 2)) &&
		App->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES2;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 2)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 2)) &&
		App->input->GetMouseY() > (261 + (108 * 2)) &&
		App->input->GetMouseY() < (299 + (108 * 2))) {
		buttonDetected = Button::ROTATIONANGLES3;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 0)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 0)) &&
		App->input->GetMouseY() > (261 + (108 * 3)) &&
		App->input->GetMouseY() < (299 + (108 * 3))) {
		buttonDetected = Button::EULERANGLES1;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 1)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 1)) &&
		App->input->GetMouseY() > (261 + (108 * 3)) &&
		App->input->GetMouseY() < (299 + (108 * 3))) {
		buttonDetected = Button::EULERANGLES2;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1005 - 879 + (143 * 2)) &&
		App->input->GetMouseX() < (1132 - 879 + (143 * 2)) &&
		App->input->GetMouseY() > (261 + (108 * 3)) &&
		App->input->GetMouseY() < (299 + (108 * 3))) {
		buttonDetected = Button::EULERANGLES3;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > (1470 - 879) &&
		App->input->GetMouseX() < (1546 - 879) &&
		App->input->GetMouseY() > (775) &&
		App->input->GetMouseY() < (815)) {
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
				quaternion[i].element = App->maths->quaternion(i, 0);
			}
		}
		for (size_t i = 0; i < 4; i++)
		{
			if (eulerAxisAngle[i].state != State::EDITED)
			{
				eulerAxisAngle[i].element = App->maths->eulerAxisAngles(i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (rotationVector[i].state != State::EDITED)
			{
				rotationVector[i].element = App->maths->rotationVector(i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (eulerAngles[i].state != State::EDITED)
			{	//3 - i cz euler angles in ui is x,y,z; in MatrixXd is z,y,x
				eulerAngles[i].element = App->maths->eulerAngles(2 - i, 0);
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (rotationMatrix[i][j].state != State::EDITED)
				{
					rotationMatrix[i][j].element = App->maths->rotationMatrix(i, j);
				}
			}
		}
	}


	if (generalState != State::EDITING) { button = ButtonDetection(); }

	MatrixXd p(4, 1);
	MatrixXd e;

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
			MatrixXd newea = App->maths->RotationChangeOfWritting(newq, 'q', 'e');
			App->maths->angles.z = newea(2, 0);
			App->maths->angles.y = newea(1, 0);
			App->maths->angles.x = newea(0, 0);
			MatrixXd p = App->maths->RotationChangeOfWritting(newea, 'e', 'p');
			App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
			App->maths->edited = true;
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

			MatrixXd newea = App->maths->RotationChangeOfWritting(newp, 'p', 'e');
			App->maths->angles.z = newea(2, 0);
			App->maths->angles.y = newea(1, 0);
			App->maths->angles.x = newea(0, 0);			
			MatrixXd p = App->maths->RotationChangeOfWritting(newea, 'e', 'p');
			App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
			App->maths->edited = true;
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
			MatrixXd newea = App->maths->RotationChangeOfWritting(newv, 'v', 'e');
			App->maths->angles.z = newea(2, 0);
			App->maths->angles.y = newea(1, 0);
			App->maths->angles.x = newea(0, 0);
			MatrixXd p = App->maths->RotationChangeOfWritting(newea, 'e', 'p');
			App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
			App->maths->edited = true;
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
			App->maths->angles.z = eulerAngles[2].element;
			App->maths->angles.y = eulerAngles[1].element;
			App->maths->angles.x = eulerAngles[0].element;
			MatrixXd newea(3,1);
			newea <<	App->maths->angles.z,
						App->maths->angles.y,
						App->maths->angles.x;
			App->maths->edited = true;
			MatrixXd p = App->maths->RotationChangeOfWritting(newea, 'e', 'p');
			App->scene_intro->primitives[0]->transform.rotate((float)p(3), { (float)p(0),(float)p(1),(float)p(2) });
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
		p << 1, 1, 1, 0;
		e = App->maths->RotationChangeOfWritting(p, 'p', 'e');
		App->maths->angles.z = e(0, 0);
		App->maths->angles.y = e(1, 0);
		App->maths->angles.x = e(2, 0);
		App->scene_intro->primitives[0]->SetRotation(0, { 1, 1, 1 });
		App->scene_intro->primitives[0]->SetPos(0, 0, 0);
		break;
	default:
		break;
	}
}

Keys Scene::KeyInputs() {
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		return Keys::ENTER;
	}
	else if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		return Keys::KEYDEL;
	}
	else if (App->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_DOWN)
	{
		return Keys::KEYDOT;
	}
	else if (App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_DOWN)
	{
		return Keys::KEYMINUS;
	}
	else if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		return Keys::KEY0;
	}
	else if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		return Keys::KEY1;
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		return Keys::KEY2;
	}
	else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		return Keys::KEY3;
	}
	else if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		return Keys::KEY4;
	}
	else if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		return Keys::KEY5;
	}
	else if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		return Keys::KEY6;
	}
	else if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		return Keys::KEY7;
	}
	else if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		return Keys::KEY8;
	}
	else if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
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