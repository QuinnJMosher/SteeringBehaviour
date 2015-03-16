#include "AIE.h"
#include <iostream>
#include <vector>
#include <ctime>
#include "FlockAgent.h"
#include "Agent.h"
#include "Wall.h"

//constant vars
static int SCREEN_MAX_X = 900, SCREEN_MAX_Y = 600;

int main( int argc, char* argv[] )
{	
	Initialise(SCREEN_MAX_X, SCREEN_MAX_Y, false, "My Awesome Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	srand(time(NULL));

	std::vector<FlockAgent*> world = std::vector<FlockAgent*>();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			world.emplace_back(new FlockAgent(300 + (i * 50), 200 + (j * 50)));
		}
	}

	for (int i = 0; i < world.size(); i++) {
		world[i]->SetSpeedCap(10);
		world[i]->SetWorld(&world);
		world[i]->SetNeighbourhood(150);
	}

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
					world[0]->AddForce(Point(0.0f, 2.0f));
				}

				if (IsKeyDown('S')) {
					world[0]->AddForce(Point(0.0f, -2.0f));
				}

				if (IsKeyDown('A')) {
					world[0]->AddForce(Point(-2.0f, 0.0f));
				}

				if (IsKeyDown('D')) {
					world[0]->AddForce(Point(2.0f, 0.0f));
				}
			}

			//key to toggle velocity lines
		    if (IsKeyDown('L')) {
				if (!buttonDown) {
					FlockAgent::ToggleVelocityLine();
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
				for (int i = 0; i < world.size(); i++) {
					world[i]->Update();
				}
				
			}
		}

		ClearScreen();

		for (int i = 0; i < world.size(); i++) {
			world[i]->Draw();
		}

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
