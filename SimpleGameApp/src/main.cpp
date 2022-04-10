#include <iostream>
#include <graphics/graphics.h>
#include <input/input.h>
#include <graphics/cCamera.h>
#include <graphics/cameras/c3rdPersonCamera.h>
#include <graphics/cGraphicsComponent.h>
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>
#include <physics/fireworks.h>
#include <physics/random_helpers.h>
#include <algorithm>
#include <physics/cProjectile.h>
#include <Midterm.h>
#include <physics/cParticlePlaneContact.h>
#include <physics/cParticleParticleContact.h>

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);

void mainLoop();

nGraphics::sPerFrameVars PerFrameVars;
nGraphics::c3rdPersonCamera* camera = 0;
glm::mat4 cameraTarget(glm::mat4(1.0f));
nGraphics::cGraphicsComponent* skyboxGraphics;
nGraphics::cGraphicsComponent* planeGraphics;
nGraphics::cGraphicsComponent* sphereGraphics;
nGraphics::cGraphicsComponent* cylinderGraphics;
nGraphics::cGraphicsComponent* Wall1Graphics;
nGraphics::cGraphicsComponent* Wall2Graphics;
nGraphics::cGraphicsComponent* Wall3Graphics;
nGraphics::cGraphicsComponent* Wall4Graphics;

sMidtermConfig midtermConfig;
nPhysics::sProjectileDef cannonBallDef;

int main()
{
	if (!ReadConfigFromJSON("MidtermConfig.json", midtermConfig))
	{
		std::cout << "Could not read MidtermConfig.json" << std::endl;
		return 1;
	}
	// CANNON BALL read in from config
	cannonBallDef.Acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	cannonBallDef.EnergyAcceleration = 0.0f;
	cannonBallDef.Speed = 25.0f;
	cannonBallDef.Position = glm::vec3(0.f);
	cannonBallDef.Damping = 0.95f;
	cannonBallDef.Mass = 20.0f;
	cannonBallDef.Radius = 1.5f;
	cannonBallDef.Colour = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
	cannonBallDef.GroundHeight = 0.0f;

	camera = new nGraphics::c3rdPersonCamera();
	nGraphics::SetCamera(camera);
	cameraTarget = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 2.f, 0.f));
	camera->SetDistance(10.0f);
	camera->SetTargetTransform(cameraTarget);

	if (!nGraphics::Init())
	{
		std::cout << "booo graphics didn't start up right" << std::endl;
		system("pause");
		return -1;
	}
	if (!nInput::Init())
	{
		std::cout << "booo inputs didn't start up right" << std::endl;
		system("pause");
		return -1;
	}
	
	{
		std::string skyboxTexture("../Assets/skybox/alps");
		//std::string skyboxTexture("../Assets/skybox/bloody-heresy");
		//std::string skyboxTexture("../Assets/skybox/jaspercoast");
		//std::string skyboxTexture("../Assets/skybox/sorbin");
		//std::string skyboxTexture("../Assets/skybox/tidepool");
		//std::string skyboxTexture("../Assets/skybox/violentdays");
		// Loading textures to use with our meshes
		nGraphics::gTextureManager->Load(skyboxTexture.c_str(), "skybox", true);
		nGraphics::gTextureManager->Load("../Assets/WhiteSquare.bmp", "whiteSquare", false);
		nGraphics::gTextureManager->Load("../Assets/stone_wall.bmp", "stone", false);
		nGraphics::gTextureManager->Load("../Assets/grass.bmp", "grass", false);
	}
	
	{
		// Loading meshes
		std::vector<nGraphics::sMeshLoadingInfo> infos;
		nGraphics::sMeshLoadingInfo loadingInfo;

		// Skybox sphere mesh
		loadingInfo.File = "../Assets/Isoshphere_xyz_InvertedNormals.ply";
		loadingInfo.DoResize = false;
		loadingInfo.AddSubMesh("skybox");
		infos.push_back(loadingInfo);
		// Lower poly sphere mesh for spheres
		loadingInfo.File = "../Assets/SoccerBall.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f); // diameter 2 spheref
		loadingInfo.SubMeshes[0].Name = "sphere";
		infos.push_back(loadingInfo);
		// Box mesh for planes and boxes
		loadingInfo.File = "../Assets/box.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // cube 2x2x2
		loadingInfo.SubMeshes[0].Name = "box";
		infos.push_back(loadingInfo);
		// Cylinder mesh for the cannon
		loadingInfo.File = "../Assets/cylinder.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // 2x2x2
		loadingInfo.SubMeshes[0].Name = "cylinder";
		infos.push_back(loadingInfo);

		if (!nGraphics::gMeshManager->Load(infos))
		{
			std::cout << "booooo failed to load meshes" << std::endl;
			return -1;
		}
	}

	// Create the graphics components
	{
		// Skybox Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "skybox";
		graphicsDef.TexCubeMap = "skybox";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 0.0f, 0.0f);
		glm::set(graphicsDef.Scale, 500.0f, 500.0f, 500.0f);
		skyboxGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Sphere Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "sphere";
		graphicsDef.TexDiffuse = "stone";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 1.0f, 0.0f);
		glm::set(graphicsDef.Scale, 1.0f, 1.0f, 1.0f);
		sphereGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -0.2f, 0.0f);
		glm::set(graphicsDef.Scale, 50.0f, 0.2f, 50.0f);
		planeGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -50.2f, -10.0f);
		glm::set(graphicsDef.Scale, 50.0f, 0.2f, 10.0f);
		glm::set(graphicsDef.Rotation, 1.5708f, 0.0f, 0.0f);
		Wall1Graphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 10.0f, -50.2f, 0.0f);
		glm::set(graphicsDef.Scale, 10.0f, 0.2f, 50.0f);
		glm::set(graphicsDef.Rotation, 0.0f, 0.0f, 1.5708f);
		Wall2Graphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 50.2f, -10.0f);
		glm::set(graphicsDef.Scale, 50.0f, 0.2f, 10.0f);
		glm::set(graphicsDef.Rotation, 1.5708f, 0.0f, 0.0f);
		Wall3Graphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 10.0f, 50.0f, 0.0f);
		glm::set(graphicsDef.Scale, 10.0f, 0.2f, 50.0f);
		glm::set(graphicsDef.Rotation, 0.0f, 0.0f, 1.5708f);
		Wall4Graphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Cylinder Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "cylinder";
		graphicsDef.TexDiffuse = "stone";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 10.0f, 0.0f);
		glm::set(graphicsDef.Scale, 1.0f, 1.0f, 1.0f);
		cylinderGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}


	// Enter the main loop
	mainLoop();

	// Clean up
	delete planeGraphics;
	delete sphereGraphics;
	delete skyboxGraphics;
	delete camera;

	nGraphics::Shutdown();

	// all done!
	return 0;
}

void mainLoop()
{
	nInput::gInputManager->ClearState();

	nGraphics::Focus();

	bool continueMainLoop = true;

	float previousTime = static_cast<float>(glfwGetTime());
	
	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;

	// BEGIN PROJECT 2 SETUP
	float particleRadius = 1.0f;
	glm::vec4 particleColor(1.0f, 0.0f, 0.0f, 1.0f);

	std::vector<nPhysics::cParticle*> particles;
	nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld(20);

	

	nPhysics::cParticlePlaneContact groundGenerator(2.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	world->AddContactContactGenerator(&groundGenerator);

	nPhysics::cParticleParticleContact particleParticleCollision(2.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	world->AddContactContactGenerator(&particleParticleCollision);


	/*nPhysics::cContactGenerator xWall(-50.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	world->AddContactContactGenerator(&xWall);

	nPhysics::cContactGenerator xWallNeg(50.2f, glm::vec3(-1.0f, 0.0f, 0.0f));
	world->AddContactContactGenerator(&xWallNeg);

	nPhysics::cContactGenerator zWall(-50.2f, glm::vec3(0.0f, 0.0f, 1.0f));
	world->AddContactContactGenerator(&zWall);

	nPhysics::cContactGenerator zWallNeg(50.2f, glm::vec3(0.0f, 0.0f, -1.0f));
	world->AddContactContactGenerator(&zWallNeg);*/



	// Fireworks will handle their own "gravity"
	//nPhysics::cParticleGravityGenerator gravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));

	nInput::cKey* key1 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_1);
	nInput::cKey* key2 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_2);


	while (continueMainLoop)
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// FPS TITLE STUFF
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				nGraphics::SetWindowTitle(newTitle);
				// reset times and counts
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}
		world->TimeStep(deltaTime);

		// Safety, mostly for first frame
		if (deltaTime == 0.f)
		{
			deltaTime = 0.03f;
		}

		// TODO: update inputs/controls/physics
		// update the cannon controls
		bool keyUp = nInput::IsKeyDown::Up();
		bool keyDown = nInput::IsKeyDown::Down();
		if (keyUp != keyDown)
		{
			float pitchSpeed = midtermConfig.Cannon.PitchSpeed;
			if (keyUp)
			{
				pitchSpeed = -midtermConfig.Cannon.PitchSpeed;
			}
			midtermConfig.Cannon.Pitch += pitchSpeed * deltaTime;
			midtermConfig.Cannon.Pitch = glm::clamp(midtermConfig.Cannon.Pitch, midtermConfig.Cannon.PitchMinimum, midtermConfig.Cannon.PitchMaximum);
		}
		bool keyLeft = nInput::IsKeyDown::Left();
		bool keyRight = nInput::IsKeyDown::Right();
		if (keyLeft != keyRight)
		{
			float yawSpeed = midtermConfig.Cannon.YawSpeed;
			if (keyRight)
			{
				yawSpeed = -midtermConfig.Cannon.YawSpeed;
			}
			midtermConfig.Cannon.Yaw += yawSpeed * deltaTime;
			midtermConfig.Cannon.Yaw = glm::clamp(midtermConfig.Cannon.Yaw, midtermConfig.Cannon.YawMinimum, midtermConfig.Cannon.YawMaximum);
		}
		glm::mat4 launchMatrix = glm::mat4(1.0);
		launchMatrix = glm::eulerAngleXYZ(midtermConfig.Cannon.Pitch, 0.0f, midtermConfig.Cannon.Yaw);
		glm::vec4 launchDirection(0.f, 1.0f, 0.0f, 0.0f);
		launchDirection = launchMatrix * launchDirection;
		glm::vec3 launchPosition = launchDirection * 4.0f;

		// FIRE!!!
		if (key1->IsJustPressed())
		{
			cannonBallDef.Position = launchPosition;
			cannonBallDef.Direction = launchDirection;
			nPhysics::cProjectile* projectile = new nPhysics::cProjectile(cannonBallDef);
			particles.push_back(projectile);
			world->AddParticle(projectile);
		}

		// update the camera
		camera->Update(deltaTime);

		// done with all the updates involving input, so clear it out
		nInput::gInputManager->ClearState();

		// begin setting per-frame vars
		camera->GetEyePosition(PerFrameVars.EyePosition);
		camera->GetViewMatrix(PerFrameVars.ViewMatrix);
		camera->GetProjectionMatrix(PerFrameVars.ProjectionMatrix);
		// end setting per-frame vars

		nGraphics::BeginFrame(PerFrameVars);

		// begin per-item rendering
		skyboxGraphics->Render();
		Wall1Graphics->Render();
		Wall2Graphics->Render();
		Wall3Graphics->Render();
		Wall4Graphics->Render();
		planeGraphics->Render();
		// render the cannon
		// rotate it to point down the negative-z axis
		glm::mat4 cannonMatrix = glm::mat4(1.0);
		cannonMatrix = glm::eulerAngleXYZ(midtermConfig.Cannon.Pitch, 0.0f, midtermConfig.Cannon.Yaw);
		//cannonMatrix = glm::rotate(cannonMatrix, midtermConfig.Cannon.Pitch, glm::vec3(1.0f, 0.0, 0.0f));
		//cannonMatrix = glm::rotate(cannonMatrix, midtermConfig.Cannon.Yaw, glm::vec3(0.0f, 0.0, 1.0f));
		cannonMatrix = glm::translate(cannonMatrix, glm::vec3(0.0f, 1.0, 0.0f));
		cannonMatrix = glm::scale(cannonMatrix, glm::vec3(0.6f, 2.0, 0.6f));
		cylinderGraphics->GetVars()->ModelMatrix = cannonMatrix;
		cylinderGraphics->Render();
		// render the projectiles
		for (nPhysics::cParticle* p : particles)
		{
			glm::mat4 projectileMatrix(1.0f);
			projectileMatrix = glm::translate(projectileMatrix, p->GetPosition());
			projectileMatrix = glm::scale(projectileMatrix, glm::vec3(particleRadius));
			sphereGraphics->GetVars()->ModelMatrix = projectileMatrix;
			sphereGraphics->GetVars()->ModelColor = particleColor;
			sphereGraphics->Render();
		}
		// end per-item rendering

		nGraphics::EndFrame();


		// clean up any dead projectiles
		for (std::vector<nPhysics::cParticle*>::iterator it = particles.begin(); it != particles.end(); )
		{
			if ((*it)->GetIsAlive())
			{
				it++;
			}
			else
			{
				std::vector<nPhysics::cParticle*>::iterator removeIt = it;
				nPhysics::cParticle* deadParticle = *it;
				it = particles.erase(removeIt);
				world->RemoveParticle(deadParticle);
				delete deadParticle;
			}
		}


		// Exit conditions: press escape or close the window by the 'x' button
		if (!(nInput::IsKeyUp::Escape() && !nGraphics::WindowShouldClose()))
		{
			continueMainLoop = false;
		}
	}
	for (nPhysics::cParticle* p : particles )
	{
		world->RemoveParticle(p);
		delete p;
	}
	particles.clear();
	// clean up!
	delete world;
}

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
	// TODO: Generate an orthonormal basis, using xVec and zVec.
	//       The input vectors may be manipulated, however the 
	//       returned axes must essentially be:
	//       x-axis: sourced from xVec
	//       y-axis: generated using math!
	//       z-axis: sourced from zVec

	// Generate y, by crossing z and x.
	glm::vec3 x(xVec);
	glm::vec3 z(zVec);
	glm::vec3 y(glm::cross(z, x));
	// Ensure z is orthogonal to both x and y.
	z = glm::cross(x, y);
	// Normalize everything.
	x = glm::normalize(x);
	y = glm::normalize(y);
	z = glm::normalize(z);
	// Return the result.
	return glm::mat3(x, y, z);
}