#pragma once
#include "cParticle.h"
#include "iParticleForceGenerator.h"
#include <vector>

namespace nPhysics
{
	/*
	* For all fireworks, y is considered "up"
	*/

	class cFirework;
	/*
	typedef void (*fireworkUpdateHandler)(cFirework* firework, float deltaTime);
	typedef void (*fireworkInitHandler)(cFirework* firework, const glm::vec3& position, const glm::vec3& direction);
	namespace nFireworkHandler
	{
		void Quick_Init(cFirework* firework, const glm::vec3& position, const glm::vec3& direction);
		void Quick_Update(cFirework* firework, float deltaTime);
	}
	*/
	/**
	* iFireworkController
	* Controls an individual firework
	* Controls the motion
	* Responsible for setting the IsAlive flag off
	*/
	class iFireworkController
	{
	public:
		virtual ~iFireworkController() {};
		virtual void InitFirework(cFirework* firework, const glm::vec3& position, const glm::vec3& direction) = 0;
		virtual void UpdateFirework(cFirework* firework, float deltaTime) = 0;
	};
	/**
	* sFireworkInitData
	* Initialisation data for a firework
	*/
	struct sFireworkInitData
	{
		glm::vec3 Position;
		glm::vec3 Direction;
		iFireworkController* Controller;
		std::vector<cFirework*> Children;
	};
	enum class FireworkStage
	{
		One,
		Two
	};
	/**
	* cFirework
	* Represents a firework
	* May have children
	* The expectation is to set up all stages of the firework,
	* check the IsAlive flag each physics update,
	* when the IsAlive flag is false the program should:
	*      1) Remove this firework from the world
	*      2) Retrieve the children using GetChildren
	*      3) Add the children to the world
	*      3) Delete the original firework
	*/
	class cFirework : public cParticle
	{
	protected:
		// Variables to be used by controllers to control this particle
		float mAge;
		bool mIsAlive;
		FireworkStage mStage;
		float mRenderRadius;
		glm::vec4 mRenderColour;
	public:
		static size_t CountFireworksCreated;
		static size_t CountFireworksDestroyed;
		cFirework(float mass, const glm::vec3& position);
		virtual ~cFirework();

		cFirework() = delete;
		cFirework(cFirework& other) = delete;
		cFirework& operator=(cFirework& other) = delete;

		// override from cParticle
		virtual void Integrate(float deltaTime);

		// common firework methods
		FireworkStage GetStage();
		void SetStage(FireworkStage stage);
		bool GetIsAlive();
		void SetIsAlive(bool alive);
		float GetAge();

		// render methods can be overridden for animations
		virtual float GetRenderRadius();
		virtual void SetRenderRadius(float radius);
		virtual glm::vec4 GetRenderColour();
		virtual void GetRenderColour(glm::vec4& colourOut);
		virtual void SetRenderColour(const glm::vec4& colour);
		// Creates next stage children from this object
		// Caller is responsible for deleting the contents of childrenOut
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut) = 0;
	};

	class cBigKahunaFirework : public cFirework
	{	
	public:
		cBigKahunaFirework(const glm::vec3& position);
		virtual ~cBigKahunaFirework();

		cBigKahunaFirework() = delete;
		cBigKahunaFirework(cBigKahunaFirework& other) = delete;
		cBigKahunaFirework& operator=(cBigKahunaFirework& other) = delete;

		// override from cParticle
		virtual void Integrate(float deltaTime);
		virtual float GetRenderRadius();
		// Creates next stage children from this object
		// Caller is responsible for deleting the contents of childrenOut
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut);

		static cBigKahunaFirework* CreateStage1(const glm::vec3& position);
		static cBigKahunaFirework* CreateStage2(const glm::vec3& position);
	};

	class cZamPowFirework : public cFirework
	{
	protected:
		float mOffset;
	public:
		cZamPowFirework(const glm::vec3& position);
		virtual ~cZamPowFirework();

		cZamPowFirework() = delete;
		cZamPowFirework(cZamPowFirework& other) = delete;
		cZamPowFirework& operator=(cZamPowFirework& other) = delete;

		// override from cParticle
		virtual void Integrate(float deltaTime);
		virtual float GetRenderRadius();
		// Creates next stage children from this object
		// Caller is responsible for deleting the contents of childrenOut
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut);

		static cZamPowFirework* CreateStage1(const glm::vec3& position);
		static cZamPowFirework* CreateStage2(const glm::vec3& position, float vel, float coneTheta, const glm::vec3& axisX, const glm::vec3& axisY, const glm::vec3& axisZ);
	};
}