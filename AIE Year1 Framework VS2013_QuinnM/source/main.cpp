#include "AIE.h"
#include <iostream>
#include "Graph.h"
#include "Agent.h"
#include "Wall.h"

//constant vars
static int SCREEN_MAX_X = 900, SCREEN_MAX_Y = 600;

int main( int argc, char* argv[] )
{	
	Initialise(SCREEN_MAX_X, SCREEN_MAX_Y, false, "My Awesome Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	//create agent
	Agent agent = Agent(100, 100, 200);

	//create graph
	unsigned int GraphPointSpriteId = CreateSprite("images/invaders/invaders_7_01.png", 30, 30, true, SColour(255, 0, 0, 255));
	Graph graph = Graph();
	graph.AddNode(50, 350);
	graph.AddNode(50, 300);
	graph.AddNode(50, 250);

	graph.AddNode(250, 200);
	graph.AddNode(450, 200);
	graph.AddNode(650, 200);

	graph.AddNode(850, 250);
	graph.AddNode(850, 300);
	graph.AddNode(850, 350);

	graph.AddNode(650, 400);
	graph.AddNode(450, 400);
	graph.AddNode(250, 400);

	//get node names
	std::vector<int>names = graph.GetNames();

	//create edges
	for (int i = 0; i < names.size() - 1; i++) {
		graph.AddConnections(names[i], names[i + 1]);
	}
	graph.AddConnections(names[names.size() - 1], names[0]);

	//make walls
	std::vector<Wall> walls = std::vector<Wall>();
	walls.emplace_back(Wall(450, 300, 700, 40));
	walls.emplace_back(Wall(450, 100, 30, 150));

	//apply graph to agent
	agent.SetGraph(&graph);
	agent.SetWalls(&walls);

    //Game Loop
    do
	{
        ClearScreen();

		

		//inputs
		if (IsKeyDown('1')) {
			agent.GoTo(800, 500);
		}

		if (IsKeyDown('2')) {
			agent.GoTo(800, 100);
		}

		if (IsKeyDown('3')) {
			agent.GoTo(100, 100);
		}

		//update agent
		agent.Update(GetDeltaTime());

		//draw
		agent.Draw();
		for (int i = 0; i < walls.size(); i++) {
			walls[i].Draw();
		}

		//draw all nodes in graph and their connections
		for (int i = 0; i < names.size(); i++) {
			float x, y;//get position
			graph.GetNodePos(names[i], x, y);

			MoveSprite(GraphPointSpriteId, x, y);//draw node
			DrawSprite(GraphPointSpriteId);

			std::vector<int> edges = graph.GetNodesConectedTo(names[i]);//get edges

			for (int j = 0; j < edges.size(); j++) {
				float edgeX, edgeY;
				graph.GetNodePos(edges[j], edgeX, edgeY);//get edge end

				DrawLine(x, y, edgeX, edgeY, SColour(0, 0, 255, 255));//draw edge
			}

		}

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
