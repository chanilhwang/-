#include "FIreworksRule.h"

FireworksRule::FireworksRule() : type(FIRE_TYPE::FIRE_INIT), size(0), minAge(0),
								 maxAge(0), minVelocity(Vector3(0, 0, 0)),
								 maxVelocity(Vector3(0, 0, 0)), damping(1.f),
								 payloadCount(0), nextRule(nullptr), isColorRandom(false)
{}

FireworksRule::~FireworksRule()
{}

void FireworksRule::setParameters(FIRE_TYPE type, float size, real minAge,
	real maxAge, const Vector3& minVelocity, const Vector3& maxVelocity,
	real damping, int count, FireworksRule* nextRule, bool isColorRandom)
{
	FireworksRule::type = type;
	FireworksRule::size = size;
	FireworksRule::minAge = minAge;
	FireworksRule::maxAge = maxAge;
	FireworksRule::minVelocity = minVelocity;
	FireworksRule::maxVelocity = maxVelocity;
	FireworksRule::damping = damping;
	FireworksRule::payloadCount = count;
	FireworksRule::nextRule = nextRule;
	FireworksRule::isColorRandom = isColorRandom;
}