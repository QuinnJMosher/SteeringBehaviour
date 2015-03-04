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
	//agent.SetForce(Point(7, 1));
	//agent.ToggleDrag();
	agent.SetSpeedCap(10);

	Agent runner = Agent(100, 100);
	runner.SetSpeedCap(10);
	runner.ChangeBehaviour(Behaviour::Seek);
	runner.SetTarget(&agent);

	bool buttonDown = false;
	float time = 0;

    //Game Loop
    do
	{
		time += GetDeltaTime();

		//limit framerate
		if (time > 1.0f / 60.0f) {
			time -= 1.0f / 60.f;

			if (IsKeyDown('W')) {
				agent.AddForce(Point(0, 2));
			}

			if (IsKeyDown('S')) {
				agent.AddForce(Point(0, -2));
			}

			if (IsKeyDown('A')) {
				agent.AddForce(Point(-2, 0));
			}

			if (IsKeyDown('D')) {
				agent.AddForce(Point(2, 0));
			}

			//key to toggle velocity lines
		    if (IsKeyDown('L')) {
				if (!buttonDown) {
					Agent::ToggleVelocityLine();
					buttonDown = true;
				}
			} else {
				buttonDown = false;
			}

			agent.Update();
			runner.Update();
		}

		ClearScreen();

		agent.Draw();
		runner.Draw();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
