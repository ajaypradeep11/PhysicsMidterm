#include <physics/cParticlePlaneContact.h>

namespace nPhysics
{
	cParticlePlaneContact::cParticlePlaneContact(float distanceFromCenter,glm::vec3 normalParameter)
		: distanceBetweenCenter(distanceFromCenter)
		, contactNormalParameter(normalParameter)
	{

	}


	cParticlePlaneContact::~cParticlePlaneContact()
	{

	}

	size_t cParticlePlaneContact::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		for (std::vector<cParticle*>::iterator p = particles.begin(); p != particles.end(); p++)
		{
			float particlePosY = (*p)->GetPosition().y;
			float particlePosX = (*p)->GetPosition().x;
			float particlePosZ = (*p)->GetPosition().z;

			printf("%f  position ", particlePosY);
			printf("\n");
			//if (particlePosY < distanceBetweenCenter)
			//{
			//	// create a contact!
			//	contact->mContactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
			//	contact->mParticle0 = *p;
			//	contact->mParticle1 = nullptr;
			//	contact->mPenetration = distanceBetweenCenter - particlePosY;
			//	contact->mRestitution = 0.7f;
			//	contact++;
			//	numContactsCreated++;
			//}


			//if (particlePosZ < -50)
			//{
			//	// create a contact!
			//	contact->mContactNormal = glm::vec3(0.0f, 0.0f, 1.0f);
			//	contact->mParticle0 = *p;
			//	contact->mParticle1 = nullptr;
			//	contact->mPenetration = (-50) - particlePosZ;
			//	contact->mRestitution = 0.7f;
			//	contact++;
			//	numContactsCreated++;
			//}

			//if (particlePosZ > 50)
			//{
			//	// create a contact!
			//	contact->mContactNormal = glm::vec3(0.0f, 0.0f, -1.0f);
			//	contact->mParticle0 = *p;
			//	contact->mParticle1 = nullptr;
			//	contact->mPenetration = 50 - particlePosZ;
			//	contact->mRestitution = 0.7f;
			//	contact++;
			//	numContactsCreated++;
			//}
			
			
			if (particlePosX < -50)
			{
				// create a contact!
				contact->mContactNormal = glm::vec3(1.0f, 0.0f, 0.0f);
				contact->mParticle0 = *p;
				contact->mParticle1 = nullptr;
				contact->mPenetration = (-50) - particlePosX;
				contact->mRestitution = 0.7f;
				contact++;
				numContactsCreated++;
			}

			if (particlePosX > 50)
			{
				// create a contact!
				contact->mContactNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
				contact->mParticle0 = *p;
				contact->mParticle1 = nullptr;
				contact->mPenetration = 50 - particlePosX;
				contact->mRestitution = 0.7f;
				contact++;
				numContactsCreated++;
			}
			
		

			if (numContactsCreated >= limit)
			{
				return numContactsCreated;
			}
		}
		return numContactsCreated;
	}
}