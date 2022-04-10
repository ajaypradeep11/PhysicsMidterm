#pragma once
#include <physics/iParticleContactGenerator.h>

namespace nPhysics
{
	class cParticlePlaneContact : public iParticleContactGenerator
	{
		float distanceBetweenCenter;
		glm::vec3 contactNormalParameter;

	public:
		cParticlePlaneContact(float distanceBetweenCenter, glm::vec3 contactNormalParameter);

		cParticlePlaneContact() = delete;
		cParticlePlaneContact(const cParticlePlaneContact& other) = delete;
		cParticlePlaneContact& operator=(const cParticlePlaneContact& other) = delete;

		virtual ~cParticlePlaneContact();
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}