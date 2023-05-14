/**
 * @file CS_Components.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Basic form of components.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_ComponentTypes.hpp"
class CS_JsonObject;
class CS_Object;
class CS_Component
{
public:
	CS_Component(CS_ComponentTypes m_component_type, bool shouldBeOnlyOne = true);
    virtual ~CS_Component();
    virtual void ReadFromFile(CS_JsonObject& jsonObject)=0;
	virtual void WriteToFile(CS_JsonObject& jsonObject) = 0;
    virtual void Init(void);
    virtual void Update(float dt) = 0;
    virtual CS_Component* Clone() const = 0;
    void SetParent(CS_Object* pParent);
    CS_Object * GetParent()const;
    CS_ComponentTypes GetComponentType();
	bool ShouldBeOnlyOneInOneObject() const;
	virtual void ShowGui()=0;
	bool is_Dead;
private:
    CS_Component(const CS_Component& rhs) = delete;
protected:
	const bool ShouldBeOnlyOne;
    CS_Object * m_Parent;
    const CS_ComponentTypes m_component_type;
};

