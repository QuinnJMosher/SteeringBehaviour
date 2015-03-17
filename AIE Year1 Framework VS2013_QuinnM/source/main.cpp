#include "AIE.h"
#include <iostream>
#include <vector>
#include <ctime>
#include "Agent.h"
#include "FlockAgent.h"
#include "Wall.h"

//constant vars
static int SCREEN_MAX_X = 900, SCREEN_MAX_Y = 600;

int main( int argc, char* argv[] )
{	
	Initialise(SCREEN_MAX_X, SCREEN_MAX_Y, false, "My Awesome Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	srand(time(NULL));

	Agent agent1 = Agent(450, 300);
	Agent agent2 = Agent(400, 250);

	agent1.SetSpeedCap(10);
	agent1.AddWander(20, 10, 1, 1);

	agent2.SetSpeedCap(10);
	agent2.AddPursue(&agent1, 1);

	std::vector<FlockAgent*> world = std::vector<FlockAgent*>();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			world.emplace_back(new FlockAgent(200 + (i * 50), 100 + (j * 50)));
		}
	}

	world.emplace_back(new FlockAgent(425, 300));

	for (int i = 0; i < world.size(); i++) {
		world[i]->SetSpeedCap(10);
		world[i]->SetWorld(&world);
		world[i]->SetNeighbourhood(100);
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
					agent1.AddForce(Point(0.0f, 2.0f));
				}

				if (IsKeyDown('S')) {
					world[0]->AddForce(Point(0.0f, -2.0f));
					agent1.AddForce(Point(0.0f, -2.0f));
				}

				if (IsKeyDown('A')) {
					world[0]->AddForce(Point(-2.0f, 0.0f));
					agent1.AddForce(Point(-2.0f, 0.0f));
				}

				if (IsKeyDown('D')) {
					world[0]->AddForce(Point(2.0f, 0.0f));
					agent1.AddForce(Point(2.0f, 0.0f));
				}
			}

			//key to toggle velocity lines
		    if (IsKeyDown('L')) {
				if (!buttonDown) {
					FlockAgent::ToggleVelocityLine();
					Agent::ToggleVelocityLine();
					buttonDown = true;
				}
			} else if (IsKeyDown(' ')) {
				if (!buttonDown) {
					pause = !pause;
					buttonDown = true;
				}
			} else if (IsKeyDown('R')) {
				if (!buttonDown) {
					FlockAgent::ToggleNeighbourhoods();
					buttonDown = true;
				}
			} else {
				buttonDown = false;
			}

			if (!pause) {
				for (int i = 0; i < world.size(); i++) {
					world[i]->Update();
				}

				agent1.Update();
				agent2.Update();
			}
		}

		ClearScreen();

		for (int i = 0; i < world.size(); i++) {
			world[i]->Draw();
		}
		
		agent1.Draw();
		agent2.Draw();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
