#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include "Utility.h"

using namespace std;
struct acctNode
{
	int acct_number;
	float cred_limit;
	bool active;
	char first_name[25];
	char last_name[25];
	float balance;
	char address[35];
	char city[25];
	char state[25];
	int zip;
	acctNode *next;

	acctNode() {}

	acctNode(int acct_number, float cred_limit, bool active, char first_name[25], char last_name[25],
				float balance, char address[35], char city[25], char state[25],int zip)
	{
		this->acct_number = acct_number;
		this->cred_limit = cred_limit;
		this->active = active;
		strcpy(this->first_name, first_name);
		strcpy(this->last_name, last_name);
		this->balance = balance;
		strcpy(this->address, address);
		strcpy(this->city, city);
		strcpy(this->state, state);
		this->zip = zip;
		next = NULL;
	}//acctNode(construct)
}; //acctNode (struct)

struct invoiceNode
{
	int inv_number;
	int acct_number;
	char acct_first_name[25];
	char acct_last_name[25];
	int item_number;
	char item_description[40];
	float item_price;
	float item_count;
	float tax_amt;
	float ship_cost;
	float billing;
	float qty_left;
	float amount;
	int year;
	int month;
	int day;

	invoiceNode() {}

	invoiceNode(int inv_number, int acct_number, char acct_first_name[25], char acct_last_name[25], int item_number,
		char item_description[40], float item_price, float item_count, float tax_amt,
		float ship_cost, float billing, float qty_left, float amount, int year, int month, int day)
	{
		this->inv_number = inv_number;
		this->acct_number = acct_number;
		strcpy(this->acct_first_name, acct_first_name);
		strcpy(this->acct_last_name, acct_last_name);
		this->item_number = item_number;
		strcpy(this->item_description, item_description);
		this->item_price = item_price;
		this->item_count = item_count;
		this->tax_amt = tax_amt;
		this->ship_cost = ship_cost;
		this->billing = billing;
		this->qty_left = qty_left;
		this->amount = amount;
		this->year = year;
		this->month = month;
		this->day = day;
	}//invoiceNode (construct)
}; //invoiceNode

struct ReOrder
{
	int item_number;
	int qty_left;
	int reorder_month;
	int reorder_day;
	int reorder_year;
	char description[40];

	ReOrder() {}

	ReOrder(int item_number, int qty_left, int reorder_month, int reorder_day, int reorder_year, char description[])
	{
		this->item_number = item_number;
		this->qty_left = qty_left;
		this->reorder_month = reorder_month;
		this->reorder_day = reorder_day;
		this->reorder_year = reorder_year;
		strcpy(this->description, description);
	}//ReOrder (construct)
}; //ReOrder

struct inventNode
{
	int item_number;
	char description[40];
	int cases;
	int capacity;
	float price;
	int created_year;
	int created_month;
	int created_day;
	int modified_year;
	int modified_month;
	int modified_day;
	int min_stock_amt;
	inventNode *left;
	inventNode *right;

	int thisMonth;
	int thisDay;
	int thisYear;

	const char Months[12][6] = { "Jan. ", "Feb. ", "Mar. ", "Apr. ", "May  ", "Jun. ", "Jul. ",
		"Aug. ", "Sep. ", "Oct. ", "Nov. ", "Dec. " };


	inventNode(){}

	inventNode(int item_number, char description[40], int cases, int capacity, float price, int created_year,
				int created_month, int created_day, int modified_year, int modified_month,
					int modified_day, int min_stock_amt)
	{
		this->item_number = item_number;
		strcpy(this->description, description);
		this->cases = cases;
		this->capacity = capacity;
		this->price = price;
		this->created_year = created_year;
		this->created_month = created_month;
		this->created_day = created_day;
		this->modified_year = modified_year;
		this->modified_month = modified_month;
		this->modified_day = modified_day;
		this->min_stock_amt = min_stock_amt;
		left = NULL;
		right = NULL;
	}//inventNode



	inventNode* Insert(inventNode *&root, inventNode *entry)
	{
		if (root == NULL)
		{
			root = new inventNode(entry->item_number, entry->description, entry->cases, entry->capacity,
				entry->price, entry->created_year, entry->created_month, entry->created_day,
				entry->modified_year, entry->modified_month, entry->modified_day, entry->min_stock_amt);
		}
		else
		{
			if (entry->item_number < root->item_number)
			{
				root->left = Insert(root->left, entry);
			}
			else
			{
				root->right = Insert(root->right, entry);
			}
		}
		return root;
	}//inventNode (pointer) #1



	void GetData(inventNode *root, ifstream &inF, inventNode *&savedRoot)
	{
		GetTodaysDate(thisMonth, thisDay, thisYear);
		int k = 0;
		while (!inF.eof())
		{
			char unknown[15];
			inventNode *entry = new inventNode();

			inF >> entry->item_number;
			inF >> entry->description;
			inF >> unknown;

			if (isdigit(unknown[0]))
			{
				inF >> entry->capacity >>
					entry->price >> entry->created_year >> entry->created_month >> entry->created_day
					>> entry->modified_year >> entry->modified_month >> entry->modified_day >> entry->min_stock_amt;
				entry->cases = atoi(unknown);
			}//if1
			else
			{
				inF >> entry->cases >> entry->capacity >>
					entry->price >> entry->created_year >> entry->created_month >> entry->created_day
					>> entry->modified_year >> entry->modified_month >> entry->modified_day >> entry->min_stock_amt;

				for (int i = 14; i > 0; i--)
				{
					unknown[i] = unknown[i - 1];
					if (i == 1)
					{
						unknown[0] = ' ';
					}
				}//for
				strcat(entry->description, unknown);
			}//else

			if (ValidDate(entry->created_month, entry->created_day, entry->created_year, thisMonth, thisDay, thisYear) &&
				ValidDate(entry->modified_month, entry->modified_day, entry->modified_year, thisMonth, thisDay, thisYear))
			{
				if (k == 0)
				{
					savedRoot = Insert(root, entry);
					k++;
				}
				else
				{
					Insert(root, entry);
				}
			}//if2
		}//while
	}//GetData

	// Search for node
	inventNode* Check(inventNode *root, int key, bool &present)
	{
		// Root is null or key is present at root
		if (root == NULL)
		{
			present = false;
			return root;
		}
		else if (root->item_number == key)
		{
			present = true;
			return root;
		}
		// Key is greater than root's key
		if (root->item_number < key)
			return Check(root->right, key, present);

		// Key is smaller than root's key
		return Check(root->left, key, present);
	}//inventNode (pointer) #2

	// Modify for node
	inventNode* Modify(inventNode *root, inventNode *entry)
	{
		// Root is null or key is present at root
		if (root == NULL)
		{
			return root;
		}
		else if (root->item_number == entry->item_number)
		{
			CopyNodeValues(root, entry);
			return root;
		}
		// Key is greater than root's key
		if (root->item_number < entry->item_number)
			return Modify(root->right, entry);

		// Key is smaller than root's key
		return Modify(root->left, entry);
	}//inventNode (pointer) #3

	// Find
	inventNode* FindQuantityAndPrice(inventNode *root, invoiceNode &entry, bool &sufficient)
	{
		// Root is null or key is present at root
		if (root == NULL)
		{
			return root;
		}
		else if (root->item_number == entry.item_number)
		{
			if (root->cases >= entry.item_count)
			{
				sufficient = true;
				strcpy(entry.item_description, root->description);
				root->cases -= entry.item_count;
				entry.qty_left = root->cases;
				entry.item_price = root->price;
				entry.billing = entry.item_count * root->price;
				entry.tax_amt = 0.08;
			}//if
			else
			{
				sufficient = false;
				price = 0;
			}
			return root;
		}//else if
		// Key is greater than root's key
		if (root->item_number < entry.item_number)
			return FindQuantityAndPrice(root->right, entry, sufficient);

		// Key is smaller than root's key
		return FindQuantityAndPrice(root->left, entry, sufficient);
	}//inventNode (pointer) #4

	//Copy Values from one node to another
	void CopyNodeValues(inventNode *currNode, inventNode *oldNode)
	{
		currNode->item_number = oldNode->item_number;
		strcpy(currNode->description, oldNode->description);
		currNode->cases = oldNode->cases;
		currNode->capacity = oldNode->capacity;
		currNode->price = oldNode->price;
		currNode->created_month = oldNode->created_month;
		currNode->created_year = oldNode->created_year;
		currNode->created_day = oldNode->created_day;
		currNode->modified_month = oldNode->modified_month;
		currNode->modified_year = oldNode->modified_year;
		currNode->modified_day = oldNode->modified_day;
		currNode->min_stock_amt = oldNode->min_stock_amt;

	}//CopyNodeValues

	//Find node with minimum value
	inventNode * minValueNode(inventNode* node)
	{
		struct inventNode* current = node;

		/* loop down to find the leftmost leaf */
		while (current->left != NULL)
			current = current->left;

		return current;
	}//inventNode (pointer) #5

	//Delete Node
	inventNode* deleteNode(struct inventNode* root, int key)
	{
		// base case
		if (root == NULL) return root;

		// If the key to be deleted is smaller than the root's key,
		// then it lies in left subtree

		if (key < root->item_number)
		{
			root->left = deleteNode(root->left, key);
		}
		// If the key to be deleted is greater than the root's key,
		// then it lies in right subtree
		else if (key > root->item_number)
		{
			root->right = deleteNode(root->right, key);
		}
		// if key is same as root's key, then This is the node
		// to be deleted
		else
		{

			// node with only one child or no child
			if (root->left == NULL)
			{
				inventNode *temp = root->right;
				free(root);
				return temp;
			}//if
			else if (root->right == NULL)
			{
				inventNode *temp = root->left;
				free(root);
				return temp;
			}//else if

			// node with two children: Get the inorder successor (smallest
			// in the right subtree)
			inventNode* temp = minValueNode(root->right);

			// Copy the inorder successor's content to this node
			CopyNodeValues(root, temp);

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->item_number);
		}//else

		return root;
	}//inventNode (pointer) #6

	void InOrderPrintItems(inventNode *root)
	{
		inventNode *ptr = root;
		if (ptr != NULL)
		{
			InOrderPrintItems(ptr->left);
		std::cout << ptr->item_number << ' ' << ptr->description << ' ' << fixed << setprecision(2) << ptr->price << std::endl;
			InOrderPrintItems(ptr->right);
		}//if
	}//InOrderPrintItems


	void PrintHeader()
	{
		cout << setw(75) << "Inventory Report" << endl;
		cout << setw(85) << "Items Printed in Ascending Order" << endl;
		cout << setw(15) << "Item" << setw(40) << "Item" << setw(15) << "Cases" << setw(15) << "Stock*" << setw(25) << "Date" <<
			setw(25) << "Date" << setw(15) << "Min" << endl;
		cout << setw(15) << "Number" << setw(40) << "Description" << setw(15) << "In Stock" << setw(15) << "Value" << setw(25) << "Created" <<
			setw(25) << "Modified" << setw(15) << "Inv. Amount" << endl;
	}//PrintHeader

	void InOrderReport(inventNode *root, float &total)
	{
		float stockValue;
		inventNode *ptr = root;
		if (ptr != NULL)
		{
			InOrderReport(ptr->left, total);
			stockValue = ptr->cases * ptr->price;
			total += stockValue;
			cout << setw(15) << ptr->item_number << setw(40) << ptr->description << setw(15) << ptr->cases << setw(15) << fixed
				<< setprecision(2) << stockValue <<
				setw(19) << Months[ptr->created_month-1] << setw(2) << ptr->created_day << setw(5) << ptr->created_year <<
				setw(19) << Months[ptr->modified_month-1] << setw(2) << ptr->modified_day << setw(5) << ptr->modified_year <<
				setw(15) << ptr->min_stock_amt << endl;
			InOrderReport(ptr->right, total);
		}//if
	}//InOrderReport

	void PrintFooter(float total)
	{
		cout << "Total Value of Inventory=========   " << '$' << fixed << setprecision(2) << total << endl;
		cout << setw(75) << "END OF REPORT" << endl;
	}//PrintFooter

	inventNode *ReOrder(inventNode *root, int item_number, ReOrder &entry, bool &order)
	{
		// Root is null or key is present at root
		if (root == NULL)
		{
			return root;
		}
		else if (root->item_number == item_number)
		{
			if (root->cases < root->min_stock_amt)
			{
				order = true;
				int thisMonth;
				int thisDay;
				int thisYear;
				GetTodaysDate(thisMonth, thisDay, thisYear);
				entry.item_number = root->item_number;
				entry.qty_left = root->cases;
				entry.reorder_month = thisMonth;
				entry.reorder_day = thisDay;
				entry.reorder_year = thisYear;
				int addition = root->min_stock_amt - root->cases;
				root->cases += addition;
			}//if
			return root;
		}//else if
		// Key is greater than root's key
		if (root->item_number < item_number)
			return ReOrder(root->right, item_number, entry, order);

		// Key is smaller than root's key
		return ReOrder(root->left, item_number, entry, order);
	}//inventNode (pointer ?)

	//Print used for Level Order traversal
	void printGivenLevel(inventNode* root, int level, ofstream &outF)
	{
		inventNode *ptr = root;

		if (root == NULL)
			return;
		if (level == 1)
		{
			outF << ptr->item_number << ' ' << ptr->description << ' ' << ptr->cases << ' ';
			outF << ptr->capacity << ' ' << ptr->price << ' ' << ptr->created_month << ' ' << ptr->created_day << ' ' << ptr->created_year << ' ' <<
				ptr->modified_month << ' ' << ptr->modified_day << ' ' << ptr->modified_year <<
				' ' << ptr->min_stock_amt << endl;
		}//if
		else if (level > 1)
		{
			printGivenLevel(root->left, level - 1, outF);
			printGivenLevel(root->right, level - 1, outF);
		}
	}//printGivenLevel

	// Compute the height of a tree
	int height(inventNode* node)
	{
		if (node == NULL)
			return 0;
		else
		{
			// compute the height of each subtree
			int lheight = height(node->left);
			int rheight = height(node->right);

			// use the larger one
			if (lheight > rheight)
				return(lheight + 1);
			else return(rheight + 1);
		}//else
	}//height

	//Actual function for printing in level order
	void printLevelOrder(inventNode* root, ofstream &myFile)
	{
		int h = height(root);
		int i;
		for (i = 1; i <= h; i++)
		{
			printGivenLevel(root, i, myFile);
		}
	}//printLevelOrder

}; //inventNode (struct)




