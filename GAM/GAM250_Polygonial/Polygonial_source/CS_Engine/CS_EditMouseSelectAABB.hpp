/**
 * @file CS_EditMouseSelectAABB.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For transform component
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
class CS_Object;

class CS_EditMouseSelectAABB
{
public:
    void SetParent(CS_Object* pParent);
    void Update();

	bool IsMouseIn();
private:
	void CheckMouseIn();
	bool isMouseIn=false;
    CS_Object * m_Parent=nullptr;
};
