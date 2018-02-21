/*
Max Fanning
Physics Collision Engine
June 3rd, 2017
*/
#include "WinMain.h"
#include "Physics.h"
#include <XEngine.h>
#include <vector>

//---------------------------------GLOBALS---------------------------------

int GWidth  = 800; //Graphics resolution
int GHeight = 600;

Physics World( 0.0f, 0.75f, 10 ); //Create a new physics instance with gravity pointing downwards and using 10 iterations

int gMouseSprite;
int gMouseX = 0; //Mouse coordinates
int gMouseY = 0;

Vertex* gDragVertex = nullptr; //The vertex that is currently dragged around with the mouse

//---------------------------------FUNCTION PROTOTYPES---------------------------------

void DispatchMouseClick(int x, int y);
void DrawHUD(int x, int y, int width, int height);
void DrawHUDRectangle(Math::Vector2 const &hudVector1, int width, int height, int offsetX, int offsetY);
void Initialize();
void DrawMouse();
void MoveVertex();

//---------------------------------MAIN & GAMELOOP---------------------------------

bool GameLoop(float deltaTime)
{
	DrawMouse();
	DrawHUD(0, 0, 130, GHeight);

	DispatchMouseClick(GetMouseScreenX(), GetMouseScreenY());
	MoveVertex();

	World.Update();
	World.Render();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start("Collision Engine", hInstance, GWidth, GHeight, 0);

	Initialize();

	X::Run(GameLoop);
	X::Stop();
	return 0;
}

//---------------------------------FUNCTION DEFINITIONS---------------------------------

void DispatchMouseClick(int x, int y)
{
	if (IsMousePressed(Mouse::LBUTTON))
	{
		if ((x >= 40 && x <= 90) && (y >= 40 && y <= 90)) //if mouse clicked on default box in hud
		{
			World.CreateBox(true, x - 25, y - 25, 50, 50, 3.0f);
		}
		else if ((x >= 28 && x <= 103) && (y >= 130 && y <= 205)) //medium box
		{
			World.CreateBox(true, x - 25, y - 25, 75, 75, 3.0f);
		}
		else if ((x >= 15 && x <= 115) && (y >= 245 && y <= 345)) //large box
		{
			World.CreateBox(true, x - 25, y - 25, 100, 100, 3.0f);
		}
		else if ((x >= 15 && x <= 115) && (y >= 385 && y <= 435)) //rectangle
		{
			World.CreateBox(true, x - 25, y - 25, 100, 50, 3.0f);
		}
		else if ((x >= 40 && x <= 90) && (y >= 510 && y <= 560))
		{
			World.CreateTriangle(true, x - 25, y - 25, 50, 50, 3.0f);
		}
	}

	if (IsMouseDown(Mouse::LBUTTON)) //Set a new vertex to be dragged around if the left mouse button is hit
	{
		if (!gDragVertex)
		{
			gDragVertex = World.FindVertex(x, y);
		}
		else
		{
			gDragVertex = nullptr;
		}
	}
	else
	{
		gDragVertex = nullptr;
	}
}

void Initialize()
{
	SetTexturePath("../Data/Images");
	gMouseSprite = LoadTexture("physics_cursor.png");
}

void DrawHUD(int x, int y, int width, int height)
{
	//HUD Border
	Math::Vector2 hudVector1 = Math::Vector2(static_cast<float>(x),			static_cast<float>(y));
	Math::Vector2 hudVector2 = Math::Vector2(static_cast<float>(x + width), static_cast<float>(y));
	Math::Vector2 hudVector3 = Math::Vector2(static_cast<float>(x + width), static_cast<float>(y + height));
	Math::Vector2 hudVector4 = Math::Vector2(static_cast<float>(x),			static_cast<float>(y + height));

	Math::Vector2 hudVector5 = Math::Vector2(static_cast<float>(x),			473.0f);
	Math::Vector2 hudVector6 = Math::Vector2(static_cast<float>(x + width), 473.0f);

	DrawScreenLine(hudVector1, hudVector2, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(hudVector2, hudVector3, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(hudVector3, hudVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(hudVector4, hudVector1, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(hudVector5, hudVector6, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));

	//Default Box
	DrawHUDRectangle(hudVector1, 50, 50, 40, 40);

	//Medium Box
	DrawHUDRectangle(hudVector1, 75, 75, 28, 130);

	//Big Box
	DrawHUDRectangle(hudVector1, 100, 100, 15, 245);

	//Rectangle
	DrawHUDRectangle(hudVector1, 100, 50, 15, 385);

	//Triangle
	Math::Vector2 triVector1 = Math::Vector2(static_cast<float>(hudVector1.x + 65), static_cast<float>(GHeight - 90));
	Math::Vector2 triVector2 = Math::Vector2(static_cast<float>(triVector1.x - 25), static_cast<float>(triVector1.y + 50));
	Math::Vector2 triVector3 = Math::Vector2(static_cast<float>(triVector1.x + 25), static_cast<float>(triVector1.y + 50));
	Math::Vector2 triVector4 = Math::Vector2(static_cast<float>(triVector1.x),		static_cast<float>(triVector1.y + 31));

	DrawScreenLine(triVector1, triVector2, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(triVector2, triVector3, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(triVector3, triVector1, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(triVector1, triVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(triVector2, triVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(triVector3, triVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));

	DrawScreenCircle(triVector1, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(triVector2, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(triVector3, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(triVector4, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}

void DrawHUDRectangle(Math::Vector2 const &hudVector1, int width, int height, int offsetX, int offsetY)
{
	Math::Vector2 boxVector1 = Math::Vector2(hudVector1.x + offsetX, hudVector1.y + offsetY);
	Math::Vector2 boxVector2 = Math::Vector2(boxVector1.x + width, boxVector1.y);
	Math::Vector2 boxVector3 = Math::Vector2(boxVector1.x + width, boxVector1.y + height);
	Math::Vector2 boxVector4 = Math::Vector2(boxVector1.x, boxVector1.y + height);
	Math::Vector2 boxVector5 = Math::Vector2(boxVector1.x + (width / 2), boxVector1.y + (height / 2));

	DrawScreenLine(boxVector1, boxVector2, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(boxVector2, boxVector3, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(boxVector3, boxVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(boxVector4, boxVector1, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(boxVector1, boxVector3, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	DrawScreenLine(boxVector2, boxVector4, Math::Vector4(1.0f, 0.0f, 1.0f, 1.0f));

	DrawScreenCircle(boxVector1, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(boxVector2, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(boxVector3, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(boxVector4, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawScreenCircle(boxVector5, 1.5f, Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}

void DrawMouse()
{
	gMouseX = GetMouseScreenX();
	gMouseY = GetMouseScreenY();

	DrawSprite(gMouseSprite, Math::Vector2(static_cast<float>(gMouseX), static_cast<float>(gMouseY)));
}

void MoveVertex()
{
	if (gDragVertex != nullptr)
	{
		gDragVertex->mPosition = Math::Vector2((float)gMouseX, (float)gMouseY);
	}
}