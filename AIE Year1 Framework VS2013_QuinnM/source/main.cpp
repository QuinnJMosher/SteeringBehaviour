#include "AIE.h"
#include <iostream>
#include <vector>
#include "Agent.h"
#include "Wall.h"

//constant vars
static int SCREEN_MAX_X = 900, SCREEN_MAX_Y = 600;

int main( int argc, char* argv[] )
{	
	Initialise(SCREEN_MAX_X, SCREEN_MAX_Y, false, "My Awesome Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	srand(time(NULL));

	Agent agent = Agent(450, 350);
	//agent.ToggleDrag();
	agent.SetSpeedCap(10);

	Agent agent2 = Agent(400, 300);
	Agent agent3 = Agent(500, 300);
	agent2.SetSpeedCap(10);
	agent2.AddEvade(&agent, 0.5, 0);
	agent2.AddPursue(&agent3, 0.5, 0);

	agent3.SetSpeedCap(10);
	agent3.AddEvade(&agent2, 0.5, 0);
	agent3.AddPursue(&agent, 0.5, 0);

	agent.AddPursue(&agent2, 0.5, 0);
	agent.AddEvade(&agent3, 0.5, 0);

	/*Agent agent4 = Agent(450, 250);
	agent4.AddPursue(&agent, 0.3, 0);
	agent4.AddPursue(&agent2, 0.3, 0);
	agent4.AddPursue(&agent3, 0.3, 0);*/

	Agent agent5 = Agent(450, 300);
	agent5.SetSpeedCap(4);
	agent5.AddWander(3, 0.4, 1, 1);
	agent5.AddPursue(&agent, 0.1, 0);

	bool buttonDown = false;
	bool pause = false;
	float time = 0;

    //Game Loop
    do
	{
		time += GetDeltaTime();

		//limit framerate
		if (time > 1.0f / 60.0f) {
			time -= 1.0f / 60.f;

			if (!pause) {
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
			}

			//key to toggle velocity lines
		    if (IsKeyDown('L')) {
				if (!buttonDown) {
					Agent::ToggleVelocityLine();
					buttonDown = true;
				}
			} else if (IsKeyDown(' ')) {
				if (!buttonDown) {
					pause = !pause;
					buttonDown = true;
				}
			} else {
				buttonDown = false;
			}

			if (!pause) {
				agent.Update();
				agent2.Update();
				agent3.Update();
				//agent4.Update();
				
			}
			agent5.Update();
		}

		ClearScreen();

		agent.Draw();
		agent2.Draw();
		agent3.Draw();
		//agent4.Draw();
		agent5.Draw();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
