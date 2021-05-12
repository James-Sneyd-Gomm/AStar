#pragma once

#ifndef NODE_H
#define NODE_H

struct Cords
{
	int row{ -1 };
	int column{ -1 };

	Cords()
	{
	}

	bool operator==(const Cords& input)
	{
		if (row == input.row && column == input.column)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};


class Node
{
protected:
	Cords rowCol;
	int g_Value{ 0 };
	int h_Value{ 0 };
	int f_Value{ 0 };

	Node* parent{ nullptr };

public:
	Node();
	Node(int row, int column, Node* startNode, Node* targetNode, Node* parent);

	Cords getCords() { return rowCol; }

	int getF_Value() { return f_Value; }
	Node* getParent() { return parent; }

	void SetCol(int col) { rowCol.column = col; }
	void SetRow(int row) { rowCol.row = row; }



};


#endif // !NODE_H

