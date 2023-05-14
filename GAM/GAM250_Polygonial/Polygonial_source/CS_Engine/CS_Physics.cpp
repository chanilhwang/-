/**
 * @file CS_Physics.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Basic physics calculation.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Physics.hpp"
#include "CS_ColliderComponent.hpp"
#include <stack>
#include <iostream>
namespace PhysicsInfo
{
    std::vector<CS_ColliderComponent*> s_colliders;      //!< Vector of regiestered colliders
    CollisionPairs                  s_collisionPairs; //!< This frames collision pairs. 
    int                             s_colliderStart = 0;  //!< The index to start updating on
    std::stack<int>                 s_pauseStack;
}

void CS_Physics::AddColliderComponent(CS_ColliderComponent * pCollider)
{
    PhysicsInfo::s_colliders.push_back(pCollider);
}

void CS_Physics::RemoveColliderComponent(CS_ColliderComponent * pCollider)
{
    for (size_t i = PhysicsInfo::s_colliderStart; i < PhysicsInfo::s_colliders.size(); ++i)
    {
        if (PhysicsInfo::s_colliders[i] == pCollider)
        {
            PhysicsInfo::s_colliders[i] = PhysicsInfo::s_colliders[PhysicsInfo::s_colliders.size() - 1];
            PhysicsInfo::s_colliders.pop_back();
			break;
        }
    }
}

void CS_Physics::RemoveAllColliderComponents()
{
	for (size_t i = 0; i < PhysicsInfo::s_colliders.size(); ++i)
	{
		PhysicsInfo::s_colliders.pop_back();
		PhysicsInfo::s_colliderStart = 0;
	}
}

void CS_Physics::GetCollisionPairs(CollisionPairs & pairs)
{
    pairs = PhysicsInfo::s_collisionPairs;
}

bool CS_Physics::IsThereCollidingWithName(int this_ID, const std::string& name)
{
	for (auto& pair : PhysicsInfo::s_collisionPairs)
	{
		if (pair.first.ID == this_ID)
			if (pair.second.name == name)
				return true;
	}
	return false;
}

bool CS_Physics::IsThereCollidingWithID(int this_ID, int other_ID)
{
	for (auto& pair : PhysicsInfo::s_collisionPairs)
	{
		if (pair.first.ID == this_ID)
			if(pair.second.ID==other_ID)
				return true;
	}
	return false;
}

bool CS_Physics::IsThereAnyColliding(const std::string & object_name)
{
	for (auto& pair : PhysicsInfo::s_collisionPairs)
	{
		if (pair.first.name == object_name)
			return true;
	}
	return false;
}

bool CS_Physics::IsThereAnyColliding(int object_ID)
{
	for(auto& pair: PhysicsInfo::s_collisionPairs)
	{
		if(pair.first.ID==object_ID)
			return true;
	}
	return false;
}

CS_Object* CS_Physics::GetCollidedObjectWithName(int this_ID, const std::string& name)
{
	for (auto& pair : PhysicsInfo::s_collisionPairs)
	{
		if (pair.first.ID == this_ID)
			if (pair.second.name == name)
				return pair.second.OwnCollider;
	}
	return nullptr;
}

CS_Object* CS_Physics::GetCollidedObjectWithID(int this_ID, int other_ID)
{
	for (auto& pair : PhysicsInfo::s_collisionPairs)
	{
		if (pair.first.ID == this_ID)
			if (pair.second.ID == other_ID)
				return pair.second.OwnCollider;
	}
	return nullptr;
}

void CS_Physics::AddCollisionPair(CollisionData First, CollisionData Second)
{
	PhysicsInfo::s_collisionPairs.push_back(std::make_pair(First, Second));
}

void CS_Physics::Update(void)
{
	
    PhysicsInfo::s_collisionPairs.clear();
    const int size = static_cast<int>(PhysicsInfo::s_colliders.size());
	
    for (int i = PhysicsInfo::s_colliderStart; i < size; ++i)
    {
        CS_ColliderComponent* pFirst = PhysicsInfo::s_colliders[i];
		if (pFirst->is_Ghost) continue;

        for (int j = PhysicsInfo::s_colliderStart; j < size; ++j)
        {
			if (i != j)
			{
				if (PhysicsInfo::s_colliders[j]->is_Ghost)
					continue;
				pFirst->TestCollision(PhysicsInfo::s_colliders[j]);
			}
        }
    }
}

void CS_Physics::Pause()
{
    PhysicsInfo::s_pauseStack.push(PhysicsInfo::s_colliderStart);
    PhysicsInfo::s_colliderStart = static_cast<int>(PhysicsInfo::s_colliders.size());
}

void CS_Physics::Resume(void)
{
    PhysicsInfo::s_colliderStart = PhysicsInfo::s_pauseStack.top();
    PhysicsInfo::s_pauseStack.pop();
}
