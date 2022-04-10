#include <physics/fireworks.h>
#include <physics/random_helpers.h>
#include <algorithm>

namespace nPhysics
{
	size_t cFirework::CountFireworksCreated = 0;
	size_t cFirework::CountFireworksDestroyed = 0;

	cFirework::cFirework(float mass, const glm::vec3& position)
		: cParticle(mass, position)
		, mIsAlive(true)
		, mAge(0.0f)
		, mStage(FireworkStage::One)
		, mRenderRadius(0.5f)
		, mRenderColour(0.f, 0.f, 0.f, 1.f)
	{
		CountFireworksCreated++;
	}
	cFirework::~cFirework()
	{
		CountFireworksDestroyed++;
	}
	// override from cParticle
	void cFirework::Integrate(float deltaTime)
	{
		mAge += deltaTime;
		cParticle::Integrate(deltaTime);
	}

	// common firework methods
	FireworkStage cFirework::GetStage()
	{
		return mStage;
	}
	void cFirework::SetStage(FireworkStage stage)
	{
		mStage = stage;
	}
	float cFirework::GetAge()
	{
		return mAge;
	}
	bool cFirework::GetIsAlive()
	{
		return mIsAlive;
	}
	void cFirework::SetIsAlive(bool alive)
	{
		mIsAlive = alive;
	}
	float cFirework::GetRenderRadius()
	{
		return mRenderRadius;
	}
	void cFirework::SetRenderRadius(float radius)
	{
		mRenderRadius = radius;
	}
	glm::vec4 cFirework::GetRenderColour()
	{
		return mRenderColour;
	}
	void cFirework::GetRenderColour(glm::vec4& colourOut)
	{
		colourOut = mRenderColour;
	}
	void cFirework::SetRenderColour(const glm::vec4& colour)
	{
		mRenderColour = colour;
	}
	cBigKahunaFirework::cBigKahunaFirework(const glm::vec3& position)
		: cFirework(1.0f, position)
	{
		
	}
	cBigKahunaFirework::~cBigKahunaFirework()
	{

	}

	// override from cParticle
	void cBigKahunaFirework::Integrate(float deltaTime)
	{
		if (!mIsAlive)
		{
			// The dead don't move
			return;
		}
		// Integrate normally
		cFirework::Integrate(deltaTime);

		if (mStage == FireworkStage::One)
		{
			// First stage alive conditions
			if (GetVelocity().y < 0.0f)
			{
				mIsAlive = false;
			}
		}
		else if (mStage == FireworkStage::Two)
		{
			// Second stage alive conditions
			if (mAge > 3.0f)
			{
				mIsAlive = false;
			}
		}
	}
	float cBigKahunaFirework::GetRenderRadius()
	{
		if (mStage == FireworkStage::One || mAge < 2.0f)
		{
			return mRenderRadius;
		}
		return (3.0f - mAge) * mRenderRadius;
	}

	// Creates next stage children from this object
	// Caller is responsible for deleting the contents of childrenOut
	void cBigKahunaFirework::GenerateChildren(std::vector<cFirework*>& childrenOut)
	{
		if (mStage != FireworkStage::One)
		{
			return;
		}
		glm::vec3 position = GetPosition();
		for (size_t idx = 0; idx < 100; idx++)
		{
			childrenOut.push_back(CreateStage2(position));
		}
	}

	cBigKahunaFirework* cBigKahunaFirework::CreateStage1(const glm::vec3& position)
	{
		// stage 1's are heavy and sluggish
		cBigKahunaFirework* result = new cBigKahunaFirework(position);
		result->SetStage(FireworkStage::One);
		result->SetMass(10.0f);
		result->SetVelocity(glm::vec3(0.0f, 15.0f, 0.0f));
		result->SetAcceleration(glm::vec3(0.0f, -3.0f, 0.0f));
		result->SetDamping(0.9f);
		return result;
	}

	cBigKahunaFirework* cBigKahunaFirework::CreateStage2(const glm::vec3& position)
	{
		// stage 2's are light with lots of drag and start fast
		cBigKahunaFirework* result = new cBigKahunaFirework(position);
		result->SetStage(FireworkStage::Two);
		result->SetAcceleration(glm::vec3(0.0f, -3.f, 0.0f));
		result->SetMass(0.1f);
		result->SetVelocity(getRandomVec3(40.f));
		result->SetDamping(0.15f);
		result->SetRenderColour(glm::vec4(1.0f, 1.0f, 0.5f, 1.f));
		result->SetRenderRadius(0.25f);
		return result;
	}


	cZamPowFirework::cZamPowFirework(const glm::vec3& position)
		: cFirework(1.0f, position)
	{
		mOffset = getRandom(glm::two_pi<float>());
	}
	cZamPowFirework::~cZamPowFirework()
	{

	}

	// override from cParticle
	void cZamPowFirework::Integrate(float deltaTime)
	{
		if (!mIsAlive)
		{
			// The dead don't move
			return;
		}
		if (mStage == FireworkStage::One)
		{
			// First stage zig-zags
			float yVel = GetVelocity().y;
			float theta = 20.f * (mAge + mOffset);
			float amplitude = 5.f;
			SetVelocity(glm::vec3(amplitude * glm::sin(theta), yVel, amplitude * glm::cos(theta)));
		}
		// Integrate normally
		cFirework::Integrate(deltaTime);
		
		if (mStage == FireworkStage::One)
		{
			// First stage alive conditions
			if (GetPosition().y >= 20.0f)
			{
				mIsAlive = false;
			}
		}
		else if (mStage == FireworkStage::Two)
		{
			// Second stage alive conditions
			if (mAge > 2.0f)
			{
				mIsAlive = false;
			}
		}
	}
	float cZamPowFirework::GetRenderRadius()
	{
		if (mStage == FireworkStage::One || mAge < 1.0f)
		{
			return mRenderRadius;
		}
		return (2.0f - mAge) * mRenderRadius;
	}
	// Creates next stage children from this object
	// Caller is responsible for deleting the contents of childrenOut
	void cZamPowFirework::GenerateChildren(std::vector<cFirework*>& childrenOut)
	{
		glm::vec3 position(GetPosition());
		glm::vec3 axisY(GetVelocity());
		glm::vec3 axisX(1.0f, 0.0f, 0.0f);
		glm::vec3 axisZ(glm::cross(axisX, axisY));
		axisX = glm::cross(axisY, axisZ);

		axisX = glm::normalize(axisX);
		float vel = glm::length(axisY);
		axisY /= vel;
		axisZ = glm::normalize(axisZ);
		for (size_t idx = 0; idx < 50; idx++)
		{
			childrenOut.push_back(CreateStage2(position, vel, glm::quarter_pi<float>(), axisX, axisY, axisZ));
		}
		for (size_t idx = 0; idx < 50; idx++)
		{
			childrenOut.push_back(CreateStage2(position, vel, glm::quarter_pi<float>()*0.4f, axisX, axisY, axisZ));
		}
	}

	cZamPowFirework* cZamPowFirework::CreateStage1(const glm::vec3& position)
	{
		cZamPowFirework* result = new cZamPowFirework(position);
		result->SetStage(FireworkStage::One);
		// accelerates up!
		result->SetAcceleration(glm::vec3(0.0f, 15.f, 0.0f));
		result->SetMass(5.0f);
		result->SetVelocity(glm::vec3(0.f, 5.f, 0.f));
		result->SetDamping(0.999f);
		result->SetRenderColour(glm::vec4(0.0f, 0.8f, 0.8f, 1.f));
		result->SetRenderRadius(0.25f);
		return result;
	}
	cZamPowFirework* cZamPowFirework::CreateStage2(const glm::vec3& position, float vel, float coneTheta, const glm::vec3& axisX, const glm::vec3& axisY, const glm::vec3& axisZ)
	{
		// zam-pow fireworks explode into a random cone direction
		//float coneTheta = getRandom(0.0f, glm::quarter_pi<float>());
		//float coneTheta = glm::quarter_pi<float>();
		glm::vec3 velocity = glm::rotate(axisY, coneTheta, axisZ);
		velocity = glm::rotate(velocity, getRandom(0.0f, glm::two_pi<float>()), axisY);
		velocity *= vel;
		//
		cZamPowFirework* result = new cZamPowFirework(position);
		result->SetStage(FireworkStage::Two);
		result->SetAcceleration(glm::vec3(0.0f, -19.81f, 0.0f));
		result->SetMass(1.3f);
		result->SetVelocity(velocity);
		result->SetDamping(0.9f);
		result->SetRenderColour(glm::vec4(1.0f, 0.0f, 1.0f, 1.f));
		result->SetRenderRadius(0.4f);
		return result;
	}
}