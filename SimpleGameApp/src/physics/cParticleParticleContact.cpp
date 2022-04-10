#include <physics/cParticleParticleContact.h>

namespace nPhysics
{
	cParticleParticleContact::cParticleParticleContact(float distanceFromCenter, glm::vec3 normalParameter)
		: distanceBetweenCenter(distanceFromCenter)
		, contactNormalParameter(normalParameter)
	{

	}


	cParticleParticleContact::~cParticleParticleContact()
	{

	}

	size_t cParticleParticleContact::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particleVector) const
	{
        float particleRadius = 2.0f;
        size_t numContactsCreated = 0;
        if (particleVector.size() > 0) {
            for (int idxA = 0; idxA < particleVector.size() - 1; idxA++)
            {
                for (int idxB = idxA + 1; idxB < particleVector.size(); idxB++)
                {
                    float distanceParticle = glm::distance(particleVector[idxA]->GetPosition(), particleVector[idxB]->GetPosition());
                    if (particleRadius >= distanceParticle) {
                        contact->mContactNormal = glm::normalize(particleVector[idxB]->GetPosition() - particleVector[idxA]->GetPosition());
                        contact->mParticle0 = particleVector[idxB];
                        contact->mParticle1 = particleVector[idxA];
                        contact->mPenetration = distanceParticle - particleRadius;
                        contact->mRestitution = 0.0f;
                        contact++;
                        numContactsCreated++;
                    }
                }

                if (numContactsCreated >= limit)
                {
                    return numContactsCreated;
                }
            }
        }
        return numContactsCreated;
	}
}