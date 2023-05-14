/******************************************************************************/
/*!
file    ObjectManager.cpp
\author Hwang Chan IL
\par    email: c.hwang@digipen.edu
\par    Class:CS230
\par    Assignment:CS230 Assignment 2
\date   2017/04/30

This is the interface for a simple ObjectManger.
*/
/******************************************************************************/

#include "ObjectManager.h"
#include "WEDebugTools.h" 


ObjectManager::ObjectManager(int maxObjects, int maxButtons)
	:maxObjectCount(maxObjects), maxButtonCount(maxButtons), objectsCount(0), buttonsCount(0)
{
	objects = new Object[maxObjects];
	buttons = new Button[maxButtons];
}

ObjectManager::~ObjectManager(void)
{
	delete[] objects;
	delete[] buttons;
}

Object* ObjectManager::GetObjects(void)
{
	return objects;
}

int ObjectManager::GetObjectCount(void) const
{
	return objectsCount;
}

void ObjectManager::DeleteObject(int objectID)
{
	objects[objectID] = objects[objectsCount - 1];
	objects[objectID].objectID = objectID;
	objectsCount -= 1;
}

void ObjectManager::AddObject(const WEVec2& pos,
						  	  const WEVec2& Vel,
							  float rotation,
							  float rotationVel,
							  float scale,
							  int texture,
							  unsigned color)
{
	if (objectsCount < maxObjectCount)
	{
		objects[objectsCount].position = pos;
		objects[objectsCount].velocity = Vel;
		objects[objectsCount].rotation = rotation;
		objects[objectsCount].rotationVel = rotationVel;
		objects[objectsCount].scale = scale;
		objects[objectsCount].texture = texture;
		objects[objectsCount].color = color;
		objects[objectsCount].objectID = objectsCount;
		objectsCount += 1;
	}
}


Button* ObjectManager::GetButtons(void)
{
	return buttons;
}

int ObjectManager::GetButtonCount(void) const
{
	return buttonsCount;
}

void ObjectManager::DeleteButton(int buttonID)
{
	buttons[buttonID] = buttons[buttonsCount - 1];
	buttons[buttonID].buttonID = buttonID;
	buttonsCount -= 1;
}

void ObjectManager::AddButton(const WEVec2& pos,
							  const WEVec2& scale,
							  int texture,
							  Action action)
{
	if (buttonsCount < maxButtonCount)
	{
		buttons[buttonsCount].position = pos;
		buttons[buttonsCount].scale = scale;
		buttons[buttonsCount].texture = texture;
		buttons[buttonsCount].action = action;
		buttons[buttonsCount].buttonID = buttonsCount;
		buttonsCount += 1;
	}
}