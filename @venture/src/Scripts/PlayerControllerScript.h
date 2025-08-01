#pragma once
#include <Flux.h>

class PlayerControllerScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	// define your private Variables here

public:
	// define your public Variables here

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	// define your public Functions here

private:
	// define your private Functions here

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{

	}

	void Update(float deltaTime) override
	{
		
	}
};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
