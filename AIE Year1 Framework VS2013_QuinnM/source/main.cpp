#include "AIE.h"
#include <iostream>
#include "Agent.h"

//constant vars
static int SCREEN_MAX_X = 900, SCREEN_MAX_Y = 600;

int main( int argc, char* argv[] )
{	
	Initialise(SCREEN_MAX_X, SCREEN_MAX_Y, false, "My Awesome Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	Agent agent = Agent(450, 300);

	bool buttonDown = false;
	float time = 0;

    //Game Loop
    do
	{
		time += GetDeltaTime();

		if (time > 1.0f / 60.0f) {
			time -= 1.0f / 60.f;

			agent.Update();
		}

		ClearScreen();

		agent.Draw();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
