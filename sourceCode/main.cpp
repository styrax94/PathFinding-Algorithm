/*
Student ID: w1581155
Saul Barrera


Algorithms: Theory Design and Implementation
*/
#include <vector>
#include <array>
#include "color.h"
#include <cmath>
#include <queue>
#include <chrono>
#include <memory>
using namespace std;

int manhattanDistance(int startRow, int startCol, int goalRow, int goalCol);
int euclideanDistance(int startRow, int startCol, int goalRow, int goalCol);
int chebyshevDistance(int startRow, int startCol, int goalRow, int goalCol);
struct NODE
{
	shared_ptr<NODE> parent;
	int row, col, weight, totalCost, goalRow, goalCol;
	
	NODE(int tRow, int tCol, shared_ptr<NODE> tParent, int tWeight, int distanceType, int gRow, int gCol)
		:row(tRow), col(tCol), weight(tWeight), goalRow(gRow), goalCol(gCol), parent(tParent)
	{
		if(distanceType == 1)
		{
			totalCost = weight + euclideanDistance(row, col, goalRow, goalCol);
		}
		else if(distanceType ==2)
		{
			totalCost = weight + manhattanDistance(row, col, goalRow, goalCol);
		}
		else if (distanceType == 3)
		{
			totalCost = weight + chebyshevDistance(row, col, goalRow, goalCol);
		}

	}

};
struct OFFSET2D
{
	int x, y;
	OFFSET2D(int X, int Y) :x(X), y(Y)
	{
	
	}
	
};
struct COMPARECOST
{
	bool operator()(shared_ptr<NODE> n1, shared_ptr<NODE> n2)
	{
		if (n1->totalCost > n2->totalCost)
			return true;
		return false;
	}
};

#define ROW 20
#define COLOUMN 20


void printGraph(int(&graph)[ROW][COLOUMN])
{
	color::setColor(color::dark_purple, color::black);
	cout << "   ";
	for (int i = 0; i < COLOUMN; i++)
	{
		if (i <10)
			cout << " " << i << " ";
		else cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i < ROW; i++)
	{
		color::setColor(color::dark_purple, color::black);
		if (i <10)
			cout << " " << i << " ";
		else cout << i << " ";
		color::setColor(color::white, color::black);
		for (int b = 0; b < COLOUMN; b++)
		{
			cout << " " << graph[i][b] << " ";
		}
		cout << endl;
	}
}
void printGraph(int(&graph)[ROW][COLOUMN], vector<shared_ptr<NODE>> shortestPath, int goalRow, int goalCol)
{
	int totalCost;
	color::setColor(color::dark_purple, color::black);
	cout << "   ";
	for (int i = 0; i < COLOUMN; i++)
	{
		if (i <10)
			cout << " " << i << " ";
		else cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i < ROW; i++)
	{
		color::setColor(color::dark_purple, color::black);
		if (i <10)
			cout << " " << i << " ";
		else cout << i << " ";
		color::setColor(color::white, color::black);
		for (int b = 0; b < COLOUMN; b++)
		{
			color::setColor(color::white, color::black);
			for(int c = 0; c < shortestPath.size(); c++)
			{
				if(shortestPath[c]->row == i && shortestPath[c]->col == b)
				{
					//totalCost = 0;
					if(shortestPath[c]->row == goalRow && shortestPath[c]->col == goalCol)
					{
						color::setColor(color::red, color::black);
						totalCost = shortestPath[c]->weight;
					}
					else color::setColor(color::green, color::black);
					break;
				}
			
			}
			cout << " " << graph[i][b] << " ";
		}
		cout << endl;
		
	}
	cout << "The total cost of travelling to the end goal is " << totalCost << endl;
}
int userInput()
{
	int input;
	bool validInput = false;
	while (!validInput)
	{
		cin >> input;
		if(input < 0 || input >= ROW)
		{
			cout << "invalid input, try again." << endl;
		}
		else validInput = true;
	}
	return input;
}
bool checkBoundaries(int currentRow, int currentCol)
{
	if(currentRow >=0 && currentRow < ROW)
	{
		if (currentCol >= 0 && currentCol < COLOUMN)
			return true;
	}

	return false;

}

int euclideanDistance(int startRow, int startCol, int goalRow, int goalCol)
{
	return sqrt(((startRow - goalRow)*(startRow - goalRow)) + ((startCol-goalCol)*(startCol-goalCol)));

}
int manhattanDistance(int startRow, int startCol, int goalRow, int goalCol)
{
	return abs(startRow - goalRow) + abs(startCol - goalCol);
}
int chebyshevDistance(int startRow, int startCol, int goalRow, int goalCol)
{
	return max(abs(startRow - goalRow), abs(startCol-goalCol));

}
vector<shared_ptr<NODE>> findShortestPath(int(&graph)[ROW][COLOUMN], int startRow, int startCol, int goalRow, int goalCol, int distanceMetric)
{
	auto start = chrono::steady_clock::now();
	int count = 0;
	priority_queue<shared_ptr<NODE>, vector<shared_ptr<NODE>>, COMPARECOST> openNodes;
	vector<shared_ptr<NODE>>closedNodes;
	vector<OFFSET2D>offsets;
	bool neighborInClosedNode;
	bool neighborInOpenNode;

	//all possible movements from one node
	offsets.push_back(OFFSET2D(0,1));
	offsets.push_back(OFFSET2D(1,0));
	offsets.push_back(OFFSET2D(1, 1));
	offsets.push_back(OFFSET2D(0, -1));
	offsets.push_back(OFFSET2D(-1, 0));
	offsets.push_back(OFFSET2D(-1,-1));
	offsets.push_back(OFFSET2D(1,-1));
	offsets.push_back(OFFSET2D(-1, 1));
	
	openNodes.push(make_shared<NODE>(startRow,startCol,nullptr,0,distanceMetric,goalRow,goalCol));

	while (!openNodes.empty())
	{
		count++;
		shared_ptr<NODE> currentNode = openNodes.top();
		openNodes.pop();
		closedNodes.push_back(currentNode);

		/*
		cout << currentNode->row << " " << currentNode->col << endl;
		printGraph(graph, closedNodes, goalRow, goalCol);
		system("PAUSE");
		system("cls");
	*/
		vector<shared_ptr<NODE>> neighbors;
	    
		for(int i =0; i < offsets.size(); i++)
		{
			neighborInClosedNode = false;
			neighborInOpenNode = false;

			int updatedRow = currentNode->row + offsets[i].x, 
				updatedCol = currentNode->col + offsets[i].y;

			//if the new coordinates of the neighbor are in the map boundaries then continue
			//if the location in the map is accesible then continue
			if(checkBoundaries(updatedRow, updatedCol))
				if (graph[updatedRow][updatedCol] != 0)
				{
					
					for (int c = 0; c < closedNodes.size(); c++)
					{
						if (updatedRow == closedNodes[c]->row && updatedCol == closedNodes[c]->col)
						{
							neighborInClosedNode = true;
						}
					}
				
					//dont add the node to the container because the node has already been processed.
					if(!neighborInClosedNode)
					{
						neighbors.push_back(make_shared<NODE>(updatedRow, updatedCol, currentNode,
							graph[updatedRow][updatedCol] + currentNode->weight
							, distanceMetric, goalRow, goalCol));

					}
					
				}
		}
		//for all neighbors found, check if its the end, else add the neighbor to the priority queue
		for (int b = 0; b < neighbors.size(); b++)
		{
			neighborInClosedNode = false;

			//if the end, then return the path from the end to the start
			if (neighbors[b]->row == goalRow && neighbors[b]->col == goalCol)
			{
				vector<shared_ptr<NODE>>shortestPath;
				shared_ptr<NODE> currentPathNode = neighbors[b];

				while (currentPathNode != nullptr)
				{
					shortestPath.push_back(currentPathNode);
					currentPathNode = currentPathNode->parent;
				}
				
				auto end = chrono::steady_clock::now();
				auto diff = end - start;
				cout << chrono::duration <double, milli>(diff).count() << " ms" << endl;
				cout << count << endl;
				system("PAUSE");
				return shortestPath;
			}

			openNodes.push(neighbors[b]);
		}

	
	}
	vector<shared_ptr<NODE>>emptyNODE;
	return emptyNODE;
}
int main()
{

	int graph[ROW][COLOUMN]=
	{
		{1,1,1,1,1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1},
		{4,4,1,4,1,1,1,2,2,2,2,2,2,1,1,1,1,1,1,1},
		{4,4,4,4,4,4,1,1,2,3,3,3,2,1,1,1,1,1,1,1},
		{4,4,4,4,4,4,1,1,2,3,3,3,3,2,1,1,1,1,1,1},
		{1,1,4,1,1,1,1,1,2,2,3,3,3,2,1,1,2,2,1,1},
		{1,4,4,1,2,2,1,1,1,2,2,2,2,2,1,2,2,2,1,1},
		{4,2,1,1,2,2,1,1,1,1,1,1,1,1,1,2,2,1,1,1},
		{1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4},
		{1,1,2,3,3,2,2,1,1,1,1,1,1,1,1,1,1,4,4,4},
		{1,2,3,3,3,3,2,2,1,1,1,1,4,4,4,4,4,4,4,1},
		{1,2,3,2,2,2,3,2,4,1,1,1,4,4,4,4,2,1,1,1},
		{1,2,2,1,1,1,4,4,4,4,1,1,4,4,4,1,1,1,1,1},
		{1,1,4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1},
		{4,4,4,4,4,4,4,4,4,1,1,0,1,1,1,1,1,1,1,0},
		{1,1,4,4,4,4,1,1,1,2,2,0,0,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1,1,1,2,1,0,0,0,1,1,0,0,0},
		{1,2,2,2,2,2,1,1,1,2,1,1,1,0,0,0,0,0,0,0},
		{2,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0}
	};
	
	
	int startRow, startCol, goalRow, goalCol, menuChoice;
	startRow = 19;
	startCol = 0;
	goalRow = 0;
	goalCol = 19;
	bool validPoint;

	menuChoice = 0;
	while(menuChoice != 10)
	{
		printGraph(graph);
		cout << "What do you want to do?" <<endl;
		cout << "[1]: Pick Start and Goal points" << endl;
		cout << "[2]: Pick distance Metric" << endl;
		cout << "[10]: Exit" << endl;
		cin >> menuChoice;
	switch (menuChoice)
		{
		case 1:
			validPoint = false;
			cout << endl << "Pick the start point." << endl;
			
			while(!validPoint)
			{
				cout << "Row: " << endl;
				startRow = userInput();
				cout << "Coloumn: " << endl;
				startCol = userInput();
				if(graph[startRow][startCol] == 0)
				{
					cout << "Invalid starting point, try again." << endl;
				}
				else validPoint = true;
			}
			validPoint = false;
			cout << endl << "Pick the goal point." << endl;
			while(!validPoint)
			{
				cout << "Row: " << endl;
				goalRow = userInput();
				cout << "Coloumn: " << endl;
				goalCol = userInput();
				if (graph[goalRow][goalCol] == 0)
				{
					cout << "Invalid goal point, try again." << endl;
				}
				else validPoint = true;
			}
			cout << "Starting and Goal points are active." << endl;
		
			break;

		case 2:
			if(startRow >= 0)
			{
				int distChoice;

				cout << "What distance metric shall be used?" << endl;
				cout << "[1]: Euclidean" << endl;
				cout << "[2]: Manhattan" << endl;
				cout << "[3]: Chebyshev" << endl;
				cin >> distChoice;
				switch(distChoice)
				{
				case 1:
					printGraph(graph, findShortestPath(graph, startRow, startCol, goalRow, goalCol, 1), goalRow, goalCol);
					break;
				case 2:
					printGraph(graph, findShortestPath(graph,startRow,startCol,goalRow,goalCol,2),goalRow,goalCol);
					
					break;
				case 3:
					printGraph(graph, findShortestPath(graph, startRow, startCol, goalRow, goalCol, 3), goalRow, goalCol);
					break;
				default:
					cout << "That was not an option, try again." << endl;
				}
			}
			else 
			{
				cout << "Please pick a start and goal point first" << endl;
			}

			break;
		case 10:
			cout << "You have exited the program" << endl;
			
			break;
		default:
			cout << "That is not a viable option. Try again." << endl;
			
		}
	system("PAUSE");
	system("cls");
	}
	

	
	return 0;
}