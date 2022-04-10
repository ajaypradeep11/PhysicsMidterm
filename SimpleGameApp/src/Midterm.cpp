#include <Midterm.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/Document.h>
#include <rapidjson/rapidjson.h>
#include <cstdio>

bool SetValue(const rapidjson::Value& valueIn, float& valueOut)
{
	if (!valueIn.IsNumber())
	{
		return false;
	}
	valueOut = valueIn.GetFloat();
	return true;
}

bool SetValueInRadians(const rapidjson::Value& valueIn, float& valueOut)
{
	if (!valueIn.IsNumber())
	{
		return false;
	}
	valueOut = valueIn.GetFloat();
	valueOut *= glm::pi<float>();
	return true;
}

bool ParseCannonDef(const rapidjson::Value& valueIn, sCannonDef& valueOut)
{
	if (!valueIn.IsObject())
	{
		return false;
	}
	bool result = true;
	result &= SetValueInRadians(valueIn["Pitch"], valueOut.Pitch);
	result &= SetValueInRadians(valueIn["Yaw"], valueOut.Yaw);
	result &= SetValueInRadians(valueIn["PitchMaximum"], valueOut.PitchMaximum);
	result &= SetValueInRadians(valueIn["PitchMinimum"], valueOut.PitchMinimum);
	result &= SetValueInRadians(valueIn["YawMinimum"], valueOut.YawMinimum);
	result &= SetValueInRadians(valueIn["YawMaximum"], valueOut.YawMaximum);
	result &= SetValueInRadians(valueIn["YawSpeed"], valueOut.YawSpeed);
	result &= SetValueInRadians(valueIn["PitchSpeed"], valueOut.PitchSpeed);

	return result;
}

// entry point to load midterm configuration
bool ReadConfigFromJSON(const std::string& filePath, sMidtermConfig& midtermConfig)
{
	using namespace rapidjson;
	
	FILE* fp = 0;
	fopen_s(&fp, filePath.c_str(), "rb");

	char readBuffer[15000];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	bool result = ParseCannonDef(d["Cannon"], midtermConfig.Cannon);

	fclose(fp);
	return result;
}
