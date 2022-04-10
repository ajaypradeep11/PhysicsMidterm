#pragma once
#include <physics/iParticleContactGenerator.h>

namespace nPhysics
{
	class cParticleParticleContact : public iParticleContactGenerator
	{
		float distanceBetweenCenter;
		glm::vec3 contactNormalParameter;
	public:
		cParticleParticleContact(float distanceBetweenCenter, glm::vec3 contactNormalParameter);

		virtual ~cParticleParticleContact();
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}