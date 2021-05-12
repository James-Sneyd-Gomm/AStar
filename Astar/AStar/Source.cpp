#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include "Node.h"

using namespace std;


void findOpenNodes(vector<Node*>& openList, map<int, Node*>& closedList, Node* curNode, Node* startNode, Node* targetNode, vector<vector<char>>& rc);


int main()
{
	vector<vector<char>> row;
	vector<char> column;

	vector<Node*> openList;
	map<int, Node*> closedList;

	vector<Node*> finalList;
	list<string> print;

	Node* startNode{ new Node };
	Node* currentNode{ new Node };
	Node* targetNode{ new Node };


	bool targetReached{ false };
	bool noPath{ false };
	int curBestHVal{ INT32_MAX };


	ifstream in_file;
	string nameOfFile;

	// Open and load maze .txt file
	cout << "Please Input Maze File Location:" << endl;
	cin >> nameOfFile;
	cin.clear();

	in_file.open(nameOfFile);

	if (in_file)
	{
		char c;
		while (in_file.get(c))
		{
			if (c != '\n')
			{
				column.push_back(c);

				if (c == 'A')
				{
					startNode->SetCol(column.size() - 1);
					startNode->SetRow(row.size());

					currentNode = startNode;
				}
				else if (c == 'B')
				{
					targetNode->SetCol(column.size() - 1);
					targetNode->SetRow(row.size());
				}
				else if (c != 'w' && c != '.')
				{
					cout << "Error: invalid character found in maze" << endl;
					cin.ignore();
					cin.get();
					return 1;
				}
			}
			else
			{
				row.push_back(column);
				column.clear();
			}
		}

		row.push_back(column);


		for (auto curRow : row)
		{
			for (auto curColumn : curRow)
			{
				cout << curColumn;
			}
			cout << endl;
		}

		in_file.close();
	}
	else
	{
		cout << "File not found." << endl;
		cin.ignore();
		cin.get();
		return 1;
	}

	// Check if maze has start and end
	if (startNode->getCords().row == -1)
	{
		cout << "This maze has no start location" << endl;
		cin.ignore();
		cin.get();
		return 1;
	}

	if (targetNode->getCords().row == -1)
	{
		cout << "This maze has no end location" << endl;
		cin.ignore();
		cin.get();
		return 1;
	}

	// Calculate path to target node
	int bestNode{ 0 };

	while (!targetReached)
	{
		findOpenNodes(openList, closedList, currentNode, startNode, targetNode, row);
		if (openList.size() != 0)
		{
			for (int i{ 0 }; i < openList.size(); i++)
			{
				if (openList[i]->getF_Value() < curBestHVal)
				{
					curBestHVal = openList[i]->getF_Value();
					bestNode = i;
				}
			}
		}
		else
		{
			noPath = true;
			break;

		}

		closedList.insert({ ((openList[bestNode]->getCords().row * row[openList[bestNode]->getCords().row].size()) + (openList[bestNode]->getCords().column) + 1), openList[bestNode] });
		currentNode = openList[bestNode];

		openList.erase(openList.begin() + abs(bestNode));
		openList.shrink_to_fit();

		if (currentNode->getCords().row == targetNode->getCords().row && currentNode->getCords().column == targetNode->getCords().column)
		{
			targetReached = true;
		}
	}

	// Find and print path to console
	if (!noPath)
	{
		while (currentNode->getParent() != nullptr)
		{
			finalList.push_back(currentNode);
			currentNode = currentNode->getParent();
		}

		for (auto i : finalList)
		{

			if (i->getCords().row < i->getParent()->getCords().row && i->getCords().column > i->getParent()->getCords().column)
			{
				print.push_back("Ne");
			}

			else if (i->getCords().row > i->getParent()->getCords().row && i->getCords().column < i->getParent()->getCords().column)
			{
				print.push_back("Sw");
			}

			else if (i->getCords().row < i->getParent()->getCords().row && i->getCords().column < i->getParent()->getCords().column)
			{
				print.push_back("Nw");
			}

			else if (i->getCords().row > i->getParent()->getCords().row && i->getCords().column > i->getParent()->getCords().column)
			{
				print.push_back("Se");
			}

			else if (i->getCords().row < i->getParent()->getCords().row)
			{
				print.push_back("N");
			}

			else if (i->getCords().row > i->getParent()->getCords().row)
			{
				print.push_back("S");
			}

			else if (i->getCords().column < i->getParent()->getCords().column)
			{
				print.push_back("W");
			}

			else if (i->getCords().column > i->getParent()->getCords().column)
			{
				print.push_back("E");
			}
		}

		print.reverse();

		for (auto c : print)
		{
			cout << c;
		}
	}


	// clean up for raw pointers
	for (auto t : openList)
	{

		delete t;
		t = NULL;
	}

	for (auto t : closedList)
	{
		delete t.second;
		t.second = NULL;
	}

	delete targetNode;
	targetNode = NULL;

	delete startNode;
	startNode = NULL;


	if (noPath)
	{
		cout << "This maze has no path to the exit" << endl;
		cin.ignore();
		cin.get();
		return 1;
	}

	cin.ignore();
	cin.get();
	return 0;
}

// Find and store open nodes attached to the current node in the maze
void findOpenNodes(vector<Node*>& openList, map<int, Node*>& closedList, Node* curNode, Node* startNode, Node* targetNode, vector<vector<char>>& rc)
{
	bool hasParent{ false };
	bool duplicate{ false };

	vector<Node*> tempVector;

	if (curNode->getParent() != nullptr)
	{
		hasParent = true;
	}

	if (hasParent)
	{
		// Cardinal Nodes

		//South Node
		if (curNode->getCords().row + 1 != curNode->getParent()->getCords().row && curNode->getCords().row + 1 <= rc.size() - 1)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//North Node
		if (curNode->getCords().row - 1 != curNode->getParent()->getCords().row && curNode->getCords().row - 1 >= 0)
		{
			if (rc[curNode->getCords().row - 1][curNode->getCords().column] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//East Node
		if (curNode->getCords().column + 1 != curNode->getParent()->getCords().column && curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row, curNode->getCords().column + 1, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//West Node
		if (curNode->getCords().column - 1 != curNode->getParent()->getCords().column && curNode->getCords().column - 1 >= 0)
		{
			if (rc[curNode->getCords().row][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row, curNode->getCords().column - 1, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		// Intercardinal Nodes
		// South West Node
		if (curNode->getCords().row + 1 <= rc.size() - 1 && curNode->getCords().column - 1 >= 0)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column - 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}

		}

		//South East Node
		if (curNode->getCords().row + 1 <= rc.size() - 1 && curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column + 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//North West Node
		if (curNode->getCords().row - 1 >= 0 && curNode->getCords().column - 1 >= 0)
		{

			if (rc[curNode->getCords().row - 1][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column - 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//North East Node
		if (curNode->getCords().row - 1 >= 0 && curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row - 1][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column + 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}
	}
	else
	{
		// Cardinal Nodes

		//South Node
		if (curNode->getCords().row + 1 <= rc.size() - 1)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//North Node
		if (curNode->getCords().row - 1 >= 0)
		{
			if (rc[curNode->getCords().row - 1][curNode->getCords().column] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//East Node
		if (curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row, curNode->getCords().column + 1, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}
		//West Node
		if (curNode->getCords().column - 1 >= 0)
		{
			if (rc[curNode->getCords().row][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row, curNode->getCords().column - 1, startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		// Intercardinal Nodes
		//South West Node
		if (curNode->getCords().row + 1 <= rc.size() - 1 && curNode->getCords().column - 1 >= 0)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column - 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}

		}
		//South East Node
		if (curNode->getCords().row + 1 <= rc.size() - 1 && curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row + 1][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row + 1,curNode->getCords().column + 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		//North West Node
		if (curNode->getCords().row - 1 >= 0 && curNode->getCords().column - 1 >= 0)
		{

			if (rc[curNode->getCords().row - 1][curNode->getCords().column - 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column - 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

		// North East Node
		if (curNode->getCords().row - 1 >= 0 && curNode->getCords().column + 1 <= rc[curNode->getCords().row].size() - 1)
		{
			if (rc[curNode->getCords().row - 1][curNode->getCords().column + 1] != 'w')
			{
				Node* tempNode{ new Node(curNode->getCords().row - 1,curNode->getCords().column + 1 , startNode, targetNode, curNode) };
				tempVector.push_back(tempNode);
			}
		}

	}

	for (int i{ 0 }; i < tempVector.size(); i++)
	{

		for (int j{ 0 }; j < openList.size(); j++)
		{
			if (tempVector[i]->getCords() == openList[j]->getCords())
			{
				duplicate = true;

				if (tempVector[i]->getF_Value() < openList[j]->getF_Value())
				{
					openList.push_back(tempVector[i]);
					tempVector.erase(tempVector.begin() + i);
					i--;

					delete openList[j];
					openList[j] = NULL;

					openList.erase(openList.begin() + j);
					openList.shrink_to_fit();

				}
				else
				{
					tempVector.erase(tempVector.begin() + i);
					i--;
					break;
				}

			}
		}

		if (!duplicate)
		{
			auto s = closedList.find(((tempVector[i]->getCords().row * rc[tempVector[i]->getCords().row].size()) + ((tempVector[i]->getCords().column) + 1)));
			if (s == closedList.end())
			{


				openList.push_back(tempVector[i]);
				tempVector.erase(tempVector.begin() + i);
				i--;
			}
		}

		duplicate = false;
	}

	tempVector.shrink_to_fit();

	for (auto i : tempVector)
	{
		delete i;
		i = NULL;
	}

}
