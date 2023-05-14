/******************************************************************************/
/*!
file    ObjectManager.h
\author Hwang Chan IL
\par    email: c.hwang@digipen.edu
\par    Class:CS230
\par    Assignment:CS230 Assignment 2
\date   2017/04/30

This is the interface for a simple ObjectManger.
*/
/******************************************************************************/
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "WEVec2.h"

struct Object
{
  WEVec2   position;
  WEVec2   velocity;
  float    rotation;
  float    rotationVel;
  float    scale;
  int      objectID;
  int      texture;
  unsigned color;
};

typedef void(*Action)(void);

struct Button
{
  WEVec2   position;
  WEVec2   scale;
  int      buttonID;
  int      texture;
  Action   action;
};

class ObjectManager
{
public:
  ObjectManager(int maxObjects, int maxButtons);
  ~ObjectManager(void);

  Object* GetObjects(void);
  int GetObjectCount(void) const;

  void DeleteObject(int objectID);
  void AddObject(
    const WEVec2& pos,
    const WEVec2& Vel,
    float rotation,
    float rotationVel,
    float scale,
    int texture,
    unsigned color);

  Button* GetButtons(void);
  int GetButtonCount(void) const;

  void DeleteButton(int buttonID);
  void AddButton(
    const WEVec2& pos,
    const WEVec2& scale,
    int texture,
    Action action);

private:
	Object* objects;
	Button* buttons;

	int objectsCount;
	int buttonsCount;

	const int maxObjectCount;
	const int maxButtonCount;
};


#endif /* OBJECTMANAGER_H */
