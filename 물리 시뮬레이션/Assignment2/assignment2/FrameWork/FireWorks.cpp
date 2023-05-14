#include "FireWorks.h"
#include "random.h"

static Random crandom;

Fireworks::Fireworks()
{
	rules[0].setParameters(FIRE_TYPE::FIRE_CHILD, 1, 2,
		1, Vector3(-3, 0, -3), Vector3(3, 20, 3),
		0.999f, 5, nullptr, false);

	rules[1].setParameters(FIRE_TYPE::FIRE_INIT, 2, 3,
		4, Vector3(-200, 7, -200), Vector3(200, 7, 200),
		0.1f, 30, &rules[2], true);

	rules[2].setParameters(FIRE_TYPE::FIRE_CHILD, 0.5, 1,
		5, Vector3(-6, 5, -6), Vector3(6, 5, 6),
		0.5f, 10, nullptr, false);
}

Fireworks::~Fireworks()
{
	unsigned int size = fireworks.size();
	for (unsigned int i = 0; i < size; i++)
	{
		delete fireworks[i];
	}
	fireworks.clear();

	size = erased_fire.size();
	for (unsigned int i = 0; i < size; i++)
	{
		delete erased_fire[i];
	}
	erased_fire.clear();
}

void Fireworks::update(float duration)
{
	std::vector<Fire*> temp;
	std::vector<Fire*>::iterator iter;
	Fire* fire = nullptr;

	for (iter = fireworks.begin(); iter != fireworks.end();)
	{
		fire = *iter;
		if (fire->update(duration)) {
			temp.push_back(fire);
			iter = fireworks.erase(iter);
		}
		else
			iter++;
	}

	for (iter = temp.begin(); iter != temp.end(); iter++)
	{
		fire = *iter;
		if (fire->type == FIRE_TYPE::FIRE_INIT) {
			create(fire);
			erased_fire.push_back(fire);
			fire->clearHistory();
		}
	}
}

Fire* Fireworks::create()
{
	Fire* fire = nullptr;
	if (erased_fire.empty())
		fire = new Fire();
	else {
		fire = erased_fire.back();
		erased_fire.pop_back();
	}
	
	fire->type = FIRE_TYPE::FIRE_INIT;
	fireworks.push_back(fire);

	return fire;
}

void Fireworks::create(Fire* parent)
{
	FireworksRule* r = parent->rule;
	Particle* p = parent->particle;

	unsigned int count = r->payloadCount;
	for (unsigned int i = 0; i < count; i++) {
		Fire* fire = nullptr;
		float randomAge = static_cast<float>(crandom.randomReal(r->minAge, r->maxAge));
		Vector3 randomVel = p->getVelocity();
		Vector3 color = parent->color;

		if (erased_fire.empty())
			fire = new Fire();
		else {
			fire = erased_fire.back();
			erased_fire.pop_back();
		}

		randomVel += crandom.randomVector(r->minVelocity, r->maxVelocity);

		if(r->isColorRandom)
			color = crandom.randomVector(Vector3(0, 0, 0), Vector3(1, 1, 1));
		
		fire->type = r->type;
		if (r->type == FIRE_TYPE::FIRE_INIT)
			fire->setRule(r->nextRule);

		fire->setParameter(r->size, randomAge, p->getPosition(),
			randomVel, color, r->damping);

		fireworks.push_back(fire);
	}
}

void Fireworks::draw(bool isShadow)
{
	unsigned int size = fireworks.size();

	for (unsigned int i = 0; i < size; i++)
	{
		if (!isShadow)
			fireworks[i]->drawHistory();
		fireworks[i]->draw(isShadow);
	}
}