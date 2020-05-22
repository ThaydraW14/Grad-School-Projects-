#pragma once
#include <iostream>
#include <iomanip>
#include "Node.h"
#include "HashMap.h"
#include <vector>

using namespace std;
class InvoiceStack
{
private:
	vector<invoiceNode> stack;
	float sales_tax = 0.08;
	float tax_amt = 0;
	float ship_cost = 0;
	float total_cost = 0;
public:
	void Push(invoiceNode entry)
	{
		stack.push_back(entry);
	}//push

	void Pop()
	{
		stack.pop_back();
	}//pop

	void Clear()
	{
		return stack.clear();
	}//clear

	int GetAccountNumber(int inv_num)
	{
		for (int i = 0; i < stack.size(); i++)
		{
			if (stack[i].inv_number == inv_num)
			{
				return stack[i].acct_number;
			}
		}//for
	}//GetAccntNumber

	bool Check(int acct_number, int item_number)
	{
		for (int i = 0; i < stack.size(); i++)
		{
			if ((stack[i].acct_number == acct_number) && (stack[i].item_number == item_number))
			{
				return true;
			}
		}//for
		return false;
	}//check

	bool CheckAccount(int acct_number)
	{
		for (int i = 0; i < stack.size(); i++)
		{
			if ((stack[i].acct_number == acct_number))
			{
				return true;
			}
		}//for
		return false;
	}//CheckAccnt

	bool CheckInvoice(int inv_number)
	{
		for (int i = 0; i < stack.size(); i++)
		{
			if (inv_number == stack[i].inv_number)
			{
				return true;
			}
		}//for
		return false;
	}//CheckInvoice

	void Remove(int acct_number, int item_number)
	{
		int j;
		for (int i = 0; i < stack.size(); i++)
		{
			if ((stack[i].acct_number == acct_number) && (stack[i].item_number == item_number))
			{
				j = i;
			}
		}//for

		stack.erase(stack.begin() + j);
	}//Remove

	void AddStacktoStack(InvoiceStack tempStack)
	{
		for (int i = 0; i < tempStack.stack.size(); ++i)
		{
			stack.push_back(tempStack.stack[i]);
		}
	}//AddStackToStack

	void PrintByAccount(int acct_number, HashMap *hash)
	{
		hash->PrintInvoiceHeader(acct_number);
		int j;
		bool firstTime = true;
		for (int i = 0; i < stack.size(); ++i)
		{
			if (acct_number == stack[i].acct_number)
			{
				if (firstTime)
				{
					j = 1;
					cout << setw(75) << "INVOICE NUMBER: " << stack[i].inv_number << endl;
					cout << setw(15) << "Item Number" << setw(40) << "Description" << setw(15) << "Quantity" <<
						setw(15) << "Unit Cost" << setw(15) << "Total Cost" << endl;

					firstTime = false;
				}//if2
				cout << j << ')' << setw(14) << stack[i].acct_number << setw(40) << stack[i].item_description << setw(15) <<
					stack[i].item_count << setw(15) << stack[i].item_price << setw(15) << stack[i].billing << endl;
				j++;

				tax_amt += stack[i].billing * sales_tax;
				total_cost += stack[i].billing;
			}//if1
		}//for

		if (total_cost > 100)
		{
			ship_cost = 0;
		}
		else
		{
			ship_cost = 25;
		}
		total_cost += tax_amt + ship_cost;

		cout << "Tax Amount   $" << tax_amt << endl;
		cout << "Shipping     $" << ship_cost << endl;
		cout << "INV. TOTAL   $" << total_cost << endl;
		tax_amt = 0;
		ship_cost = 0;
		total_cost = 0;
	}//PrintByAccnt

	void PrintSpecificInvoice(int invoice_number, HashMap *hash)
	{
		int acct;

		for (int i = 0; i < stack.size(); i++)
		{
			if (invoice_number == stack[i].inv_number)
			{
				acct = GetAccountNumber(invoice_number);
				hash->PrintInvoiceHeader(acct);
				cout << setw(75) << "INVOICE NUMBER: " << invoice_number << endl;
				cout << setw(15) << "Item Number" << setw(40) << "Description" << setw(15) << "Quantity" <<
					setw(15) << "Unit Cost" << setw(15) << "Total Cost" << endl;

				cout << setw(14) << stack[i].acct_number << setw(40) << stack[i].item_description << setw(15) <<
					stack[i].item_count << setw(15) << stack[i].item_price << setw(15) << stack[i].billing << endl;
				tax_amt += stack[i].billing * sales_tax;
				total_cost += stack[i].billing;
			}//if
		}//for

		if (total_cost > 100)
		{
			ship_cost = 0;
		}
		else
		{
			ship_cost = 25;
		}
		total_cost += tax_amt + ship_cost;

		cout << "Tax Amount   $" << tax_amt << endl;
		cout << "Shipping     $" << ship_cost << endl;
		cout << "INV. TOTAL   $" << total_cost << endl;

		tax_amt = 0;
		ship_cost = 0;
		total_cost = 0;
	}//PrintSpecificInvoice

	void PrintInvoiceByDate(int month, int day, int year, HashMap *hash)
	{
		int acct;
		for (int i = 0; i < stack.size(); i++)
		{
			if (stack[i].day == day && stack[i].month == month && stack[i].year == year)
			{
				acct = GetAccountNumber(stack[i].inv_number);
				hash->PrintInvoiceHeader(acct);
				cout << setw(75) << "INVOICE NUMBER: " << stack[i].inv_number << endl;
				cout << setw(15) << "Item Number" << setw(40) << "Description" << setw(15) << "Quantity" <<
					setw(15) << "Unit Cost" << setw(15) << "Total Cost" << endl;
				cout << setw(14) << stack[i].acct_number << setw(40) << stack[i].item_description << setw(15) <<
					stack[i].item_count << setw(15) << stack[i].item_price << setw(15) << stack[i].billing << endl;
				tax_amt += stack[i].billing * sales_tax;
				total_cost += stack[i].billing;
			}//if
		}//for
		if (total_cost > 100)
		{
			ship_cost = 0;
		}
		else
		{
			ship_cost = 25;
		}
		total_cost += tax_amt + ship_cost;

		cout << "Tax Amount   $" << tax_amt << endl;
		cout << "Shipping     $" << ship_cost << endl;
		cout << "INV. TOTAL   $" << total_cost << endl;

		tax_amt = 0;
		ship_cost = 0;
		total_cost = 0;
	}//PrintInvoiceByDate

	void PrintRange(int inv_low, int inv_high, HashMap *hash)
	{
		int acct;
		for (int i = inv_low; i <= inv_high; ++i)
		{
			acct = GetAccountNumber(i);
			hash->PrintInvoiceHeader(acct);
			PrintByAccount(acct, hash);

		}//for

	}//PrintRange
};  //End of InvoiceStack (class)
