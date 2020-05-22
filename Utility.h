#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Node.h"

using namespace std;

void PrintMainMenu()
{
	cout << setw(25) << right << "Main Menu" << endl;
	cout << "|-------------------------------------------" << endl;
	cout << "| 1) Edit Inventory" << endl;
	cout << "| 2) Edit Account" << endl;
	cout << "| 3) Shop" << endl;
	cout << "| 4) Print Invoice(s)" << endl;
	cout << "| 5) Exit System" << endl;
	cout << "|-------------------------------------------" << endl;
}//PrintMainMenu

void PrintEditInventory()
{
	cout << setw(25) << right << "Edit Inventory" << endl;
	cout << "|-------------------------------------------" << endl;
	cout << "| A) Delete Item" << endl;
	cout << "| B) Add Item" << endl;
	cout << "| C) Modify Item" << endl;
	cout << "| D) Return to Main Menu" << endl;
	cout << "|-------------------------------------------" << endl;
}//PrintEditInventory

void PrintEditAccount()
{
	cout << setw(25) << right << "Edit Account" << endl;
	cout << "|-------------------------------------------" << endl;
	cout << "| A) Add Account" << endl;
	cout << "| B) Disable Account" << endl;
	cout << "| C) Modify Account" << endl;
	cout << "| D) Return to Main Menu" << endl;
	cout << "|-------------------------------------------" << endl;
}//EditAccnt

void PrintInvoiceMenu()
{
	cout << setw(25) << right << "Print Invoice(s)" << endl;
	cout << "|-----------------------------------------------------" << endl;
	cout << "| A) Print Specific Invoice" << endl;
	cout << "| B) Print Range of Invoices[00-999]" << endl;
	cout << "| C) Print All Invoices For Specific Account Number" << endl;
	cout << "| D) Print All Invoices For MM/DD/YYYY" << endl;
	cout << "| E) Return to Main Menu" << endl;
	cout << "|-----------------------------------------------------" << endl;
}//InvoiceMenu

void ShopMenu()
{
	cout << setw(25) << right << "Shop" << endl;
	cout << "|-------------------------------------------" << endl;
	cout << "| A) View Item, Desc, & Cost" << endl;
	cout << "| B) Purchase Item" << endl;
	cout << "| C) Check Out" << endl;
	cout << "| D) Remove Item" << endl;
	cout << "| E) Return to Main Menu" << endl;
	cout << "|-------------------------------------------" << endl;
}//ShopMenu

void GetTodaysDate(int &thisMonth, int &thisDay, int &thisYear)
{
	//Gets the time inside of the class
	time_t t = time(0);   // get time now
	tm * now = localtime(&t);
	thisYear = (now->tm_year + 1900);
	thisMonth = (now->tm_mon + 1);
	thisDay = now->tm_mday;
}//TodaysDate

bool ValidDate(int month, int day, int year, int thisMonth, int thisDay, int thisYear)
{

	//Checks that from February to June, even months should have no more than 30 days
	if (month % 2 == 0 && month < 8 && day > 30)
	{
		return false;
	}
	//Checks that from September to November, odd months should have no more than 30 days
	else if (month % 2 == 1 && month > 8 && day > 30)
	{
		return false;
	}
	//Checks that February is 28 days or less, except for leap years
	else if (month == 2 && day > 28 && year % 4 != 0)
	{
		return false;
	}
	else if (day < 1 || day > 31)
	{
		return false;
	}
	else if (month < 1 || month > 12)
	{
		return false;
	}
	else if (year > thisYear)
	{
		return false;
	}
	else if (year == thisYear && month > thisMonth)
	{
		return false;
	}
	else if (year == thisYear && month == thisMonth && day > thisDay)
	{
		return false;
	}
	else if (year < 1900)
	{
		return false;
	}

	return true;
}//ValidDate
