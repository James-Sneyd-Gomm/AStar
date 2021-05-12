#include "Node.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>

Node::Node()
{

}

Node::Node(int row, int column, Node* startNode, Node* targetNode, Node* parent)
{
	rowCol.row = row;
	rowCol.column = column;

	if (rowCol.row != startNode->getCords().row)
	{
		int dif{ abs(rowCol.row - startNode->getCords().row) };

		g_Value = abs((abs(rowCol.column - startNode->getCords().column) - dif) * 10) + (14 * dif);

	}
	else
	{
		g_Value = abs((rowCol.column - startNode->getCords().column) * 10);
	}


	if (rowCol.row != targetNode->getCords().row)
	{
		int dif{ abs(rowCol.row - targetNode->getCords().row) };

		h_Value = abs((abs(rowCol.column - targetNode->getCords().column) - dif) * 10) + (14 * dif);

	}
	else
	{
		h_Value = abs((rowCol.column - targetNode->getCords().column) * 10);
	}

	f_Value = g_Value + h_Value;

	this->parent = parent;

}