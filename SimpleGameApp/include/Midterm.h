#pragma once
#include <extern_includes.h>

#include <string>

struct sCannonDef
{
	sCannonDef()
	{
		Pitch = -glm::quarter_pi<float>();
		Yaw = 0.0f;
		PitchMaximum = -glm::pi<float>() / 8.0f;
		PitchMinimum = -glm::half_pi<float>();
		YawMinimum = -glm::quarter_pi<float>();
		YawMaximum = glm::quarter_pi<float>();
		YawSpeed = glm::quarter_pi<float>();
		PitchSpeed = glm::quarter_pi<float>();
	}
	float Pitch;
	float Yaw;
	float PitchMinimum;
	float PitchMaximum;
	float YawMinimum;
	float YawMaximum;
	float PitchSpeed;
	float YawSpeed;
};

struct sMidtermConfig
{
	sCannonDef Cannon;
};


// entry point to load midterm configuration
bool ReadConfigFromJSON(const std::string& filePath, sMidtermConfig& midtermConfig);