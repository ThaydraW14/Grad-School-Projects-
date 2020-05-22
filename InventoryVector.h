#pragma once
#include "Node.h"
#include <iostream>
#include <vector>

using namespace std;
class InventoryVector
{
public:
	vector<inventNode> inventoryVector;

	void Push(inventNode entry)
	{
		inventoryVector.push_back(entry);
	}//Push

	void GetData(ifstream &inF)
	{
		while (!inF.eof())
		{
			char unknown[15];
			inventNode entry = inventNode();

			inF >> entry.item_number;
			inF >> entry.description;
			inF >> unknown;

			if (isdigit(unknown[0]))
			{
				inF >> entry.capacity >>
					entry.price >> entry.created_year >> entry.created_month >> entry.created_day
					>> entry.modified_year >> entry.modified_month >> entry.modified_day >> entry.min_stock_amt;
				entry.cases = atoi(unknown);
			}//if
			else
			{
				inF >> entry.cases >> entry.capacity >>
					entry.price >> entry.created_year >> entry.created_month >> entry.created_day
					>> entry.modified_year >> entry.modified_month >> entry.modified_day >> entry.min_stock_amt;

				for (int i = 14; i > 0; i--)
				{
					unknown[i] = unknown[i - 1];
					if (i == 1)
					{
						unknown[0] = ' ';
					}
				}//for
				strcat(entry.description, unknown);
			}//else

			cout << entry.description << endl;
			Push(entry);
		}//while
	}//GetData

	void Print()
	{
		for (int i = 0; i < inventoryVector.size(); ++i)
		{
			cout << inventoryVector[i].item_number << endl;
		}//for
	}//Print


}; //End of InventoryVector (class)
