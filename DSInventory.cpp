//////////////////////////////////////////////////////////////////////////
// DSInventory.cpp : Defines the entry point for the console application.
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Node.h"
#include "HashMap.h"
#include "Stack.h"
#include "Utility.h"
#include "ReOrder.h"

using namespace std;

int main()
{
	int invent_item_number;
	char invent_description[40];
	int invent_cases;
	int invent_capacity;
	float invent_price;
	int invent_year;
	int invent_month;
	int invent_day;
	int invent_min_stock_amt;
	char invent_unknown[40];
	inventNode *Tree = new inventNode();
	inventNode *root = new inventNode();
	inventNode *trueRoot = new inventNode();
	root = NULL;
	trueRoot = NULL;

	HashMap *AccountTable = new HashMap();

	ifstream inventF, accountF;
	char invenFile[50] = "inventory_data.txt";
	inventF.open(invenFile);
	char accountFile[50] = "account_data1.1.txt";
	accountF.open(accountFile);

	ofstream outAccount;
	ofstream outInvent;

	int mainInput;
	char input;

	int dest;
	bool present;
	int invoice_num = 1;

	int thisMonth;
	int thisDay;
	int thisYear;

	GetTodaysDate(thisMonth, thisDay, thisYear);


	InvoiceStack mainStack;
	ReOrderVector reorderStack;
	ReOrder reorder;

	Tree->GetData(root, inventF, trueRoot);
	AccountTable->GetData(accountF);

	do
	{
		PrintMainMenu();
		cin >> mainInput;

		switch (mainInput)
		{
		case 1:
			do
			{
				PrintEditInventory();
				cin >> input;
				bool checked = false;
				int item;

				switch ((char)toupper(input))
				{
				case 'A':
					cout << "Please Enter the Item Number that you want to Delete: " << endl;
					cin >> item;

					Tree->Check(trueRoot, item, checked);

					if (checked)
					{
						Tree->deleteNode(trueRoot, item);
					}
					else
					{
						cout << "Item Number does not exist" << endl;
					}
					break;
//caseA
				case 'B':
					cout << "Please Enter the Item Number that you want to Add: " << endl;
					cin >> item;

					Tree->Check(trueRoot, item, checked);

					if (!checked)
					{
						inventNode *entry = new inventNode();
						entry->item_number = item;

						cout << "Please Enter A Description of the Item :" << endl;
						cin.ignore();
						cin.getline(entry->description, 40);

						cout << "Please Enter the Amount of Cases on Hand: " << endl;
						cin >> entry->cases;

						cout << "Please Enter the Capacity of the Item: " << endl;
						cin >> entry->capacity;

						cout << "Please Enter the Price of the Item: " << endl;
						cin >> entry->price;

						do
						{
							cout << "Please Enter the Month, Day, Year the Record was Created in that Order" << endl;
							cin >> entry->created_month;
							cin >> entry->created_day;
							cin >> entry->created_year;

							if (!ValidDate(entry->created_month, entry->created_day, entry->created_year, thisMonth,
								thisDay, thisYear))
							{
								cout << "Your date is invalid. Try again." << endl;
							}

						} while (!ValidDate(entry->created_month, entry->created_day, entry->created_year, thisMonth,
							thisDay, thisYear));

						do
						{
							cout << "Please Enter the Month, Day And Year the Record was Last Modified in that Order" << endl;
							cin >> entry->modified_month;
							cin >> entry->modified_day;
							cin >> entry->modified_year;

							if (!ValidDate(entry->modified_month, entry->modified_day, entry->modified_year, thisMonth,
								thisDay, thisYear))
							{
								cout << "Your date is invalid. Try again." << endl;
							}

						} while (!ValidDate(entry->modified_month, entry->modified_day, entry->modified_year, thisMonth,
							thisDay, thisYear));

						cout << "Please Enter the Minimum Stock Amount: " << endl;
						cin >> entry->min_stock_amt;

						Tree->Insert(trueRoot, entry);
					}//if1
					else
					{
						cout << "Item Number already exists... " << endl;
					}
					break;
//caseB
				case 'C':
					cout << "Please Enter the Item Number that you want to Modify: " << endl;
					cin >> item;

					Tree->Check(trueRoot, item, checked);
					if (checked)
					{
						inventNode *entry = new inventNode();
						entry->item_number = item;

						cout << "Please Enter A Description of the Item :" << endl;
						cin.ignore();
						cin.getline(entry->description, 40);

						cout << "Please Enter the Amount of Cases on Hand: " << endl;
						cin >> entry->cases;

						cout << "Please Enter the Capacity of the Item: " << endl;
						cin >> entry->capacity;

						cout << "Please Enter the Price of the Item: " << endl;
						cin >> entry->price;

						do
						{
							cout << "Please Enter the Year, Month, and Day the Record was Created in that Order" << endl;
							cin >> entry->created_year;
							cin >> entry->created_month;
							cin >> entry->created_day;

							if (!ValidDate(entry->created_month, entry->created_day, entry->created_year, thisMonth,
								thisDay, thisYear))
							{
								cout << "Your date is invalid. Try again." << endl;
							}

						} while (!ValidDate(entry->created_month, entry->created_day, entry->created_year, thisMonth,
							thisDay, thisYear));

						do
						{
							cout << "Please Enter the Year, Month, and Day the Record was Last Modified in that Order" << endl;
							cin >> entry->modified_year;
							cin >> entry->modified_month;
							cin >> entry->modified_day;
							if (!ValidDate(entry->modified_month, entry->modified_day, entry->modified_year, thisMonth,
								thisDay, thisYear))
							{
								cout << "Your date is invalid. Try again." << endl;
							}

						} while (!ValidDate(entry->modified_month, entry->modified_day, entry->modified_year, thisMonth,
							thisDay, thisYear));


						cout << "Please Enter the Minimum Stock Amount: " << endl;
						cin >> entry->min_stock_amt;

						Tree->Modify(trueRoot, entry);
					}
					else
					{
						cout << "Item Number does not exist..." << endl;
					}
					break;
				}
				} while ((char)toupper(input) != 'D');
				break;
//caseC
//case 1

		case 2:
			do
			{
				acctNode *entry = new acctNode();

				PrintEditAccount();
				cin >> input;
				switch ((char)toupper(input))
				{
				case 'A':
					int dest;
					cout << "Please Enter An Account Number: " << endl;
					cin >> entry->acct_number;
					if (!AccountTable->CheckTable(entry->acct_number, dest))
					{
						cout << "Great! Please Enter the Credit Limit: " << endl;
						do
						{
							cin >> entry->cred_limit;
							if (entry->cred_limit < 0)
							{
								cout << "Error! Please enter a positive number." << endl;
							}
						} while (entry->cred_limit < 0);

						cout << "Please Enter Whether the Account is Active (1 or higher for YES, 0 or less for NO): " << endl;
						cin >> entry->active;

						cout << "Please Enter First Name" << endl;
						cin >> entry->first_name;

						cout << "Please Enter Last Name" << endl;
						cin >> entry->last_name;

						cout << "Please Enter Balance (Number)" << endl;
						cin >> entry->balance;

						cout << "Please Enter the Address. Seperate Words with Underscores (Ex. 234_Macon_Ave)" << endl;
						cin >> entry->address;

						cout << "Please Enter the City (Please place '_' between multiple words) " << endl;
						cin.ignore();
						cin.getline(entry->city, 25);

						cout << "Please Enter the State (Please place '_' between multiple words)" << endl;
						cin.getline(entry->state, 25);

						cout << "Please Enter the Zip Code" << endl;
						cin >> entry->zip;

						AccountTable->Insert(entry);
					}
					else
					{
						cout << "Sorry. Account Number already exists! Please choose another option." << endl;
					}
					break;
//caseA
				case 'B':
					int a_number;
					int hold;
					cout << "Please Enter the Account Number of the Account to be Disabled: " << endl;
					cin >> a_number;
					if (AccountTable->CheckTable(a_number, hold))
					{
						AccountTable->Deactivate(a_number);
					}
					else
					{
						cout << "Account does not exist." << endl;
					}
					break;
//caseB
				case 'C':
					int number;
					int i;
					cout << "Please Enter the Account Number of the Account to be Modified: " << endl;
					cin >> number;
					if (AccountTable->CheckTable(number, i))
					{
						AccountTable->Modify(number);
					}
					else
					{
						cout << "Account does not exist." << endl;
					}
					break;
				default:
					break;
				}

			} while ((char)toupper(input) != 'D');
			break;
//caseC
//case 2

		case 3:
		{
			InvoiceStack tempStack;
			invoiceNode entry;
			bool enough = false;

			do
			{
				cout << "Please Enter the Account Number that You will using: " << endl;
				cin >> entry.acct_number;
				if (AccountTable->CheckTable(entry.acct_number, dest) && AccountTable->IsActive(entry.acct_number))
				{
					entry.inv_number = invoice_num;
					AccountTable->CopyDataToInvoice(dest, entry);
					do
					{
						ShopMenu();
						cin >> input;

						switch ((char)toupper(input))
						{
						case 'A':
							Tree->InOrderPrintItems(trueRoot);
							break;
//caseA

						case 'B':
						{
							bool add = false;
							do
							{
								cout << "Please Enter the Item Number Item you would like: " << endl;
								cin >> entry.item_number;
								Tree->Check(trueRoot, entry.item_number, present);
								if (!present)
								{
									cout << "The Item Number is Invalid: " << endl;
								}
							} while (!present);

							do
							{
								cout << "Please Enter the Amount of the Item you would like: " << endl;
								cin >> entry.item_count;
								Tree->FindQuantityAndPrice(trueRoot, entry, enough);
								if (!enough)
								{
									cout << "Not enough items in stock. Please enter a lower amount." << endl;
								}
								else if (!AccountTable->SufficientFunds(dest, entry))
								{
									cout << "Not enough funds in your account." << endl;
								}
							} while (!enough || !AccountTable->SufficientFunds(dest, entry));
							cout << "Item has been added to your invoice." << endl;
							entry.month = thisMonth;
							entry.day = thisDay;
							entry.year = thisYear;
							tempStack.Push(entry);
							Tree->ReOrder(trueRoot, entry.item_number, reorder, add);
							if (add)
							{
								reorderStack.Push(reorder);
							}
						}
							break;
//caseB
						case 'C':
							AccountTable->PrintInvoiceHeader(entry.acct_number);
							tempStack.PrintByAccount(entry.acct_number, AccountTable);
							mainStack.AddStacktoStack(tempStack);
							tempStack.Clear();
							invoice_num++;
							break;
//caseC
						case 'D':
						{
							int item_number;
							cout << "Please Enter the Item Number that you want to Remove: " << endl;
							cin >> item_number;

							if (tempStack.Check(entry.acct_number, item_number))
							{
								tempStack.Remove(entry.acct_number, item_number);
								AccountTable->ItemRemoved(entry);
							}
							else
							{
								cout << "This item is not in your Invoice." << endl;
							}

							break;
						}//caseD

						default:
							break;
						}
					} while ((char)toupper(input) != 'E');

				}
				else if (!AccountTable->IsActive(entry.acct_number))
				{
					cout << "This Account is not active" << endl;
				}
				else
				{
					cout << "This Account Number Does Not Exist. Please Enter a Valid One: " << endl;
				}


			} while (!AccountTable->CheckTable(entry.acct_number, dest));
			break;
        //default
		}//case 3
		case 4:
		{
			int inv_number;
			do
			{
				PrintInvoiceMenu();
				cin >> input;

				switch ((char)toupper(input))
				{
				case 'A':
					cout << "Please Enter the Invoice Number that you want to Print: " << endl;
					cin >> inv_number;

					if (mainStack.CheckInvoice(inv_number))
					{
						mainStack.PrintSpecificInvoice(inv_number, AccountTable);
					}
					else
					{
						cout << "Invoice does not exist." << endl;
					}
					break;
//caseA
				case 'B':
				{
					int inv_low, inv_high;
					cout << "Please Enter the Lower Invoice Number that you want to Print: " << endl;
					cin >> inv_low;
					do
					{
						cout << "Please Enter the Higher Invoice Number that you want to Print: " << endl;
						cin >> inv_high;
						if (inv_high < inv_low)
						{
							cout << "This Value is invalid. Try again." << endl;
						}
					} while (inv_high < inv_low);


					if (mainStack.CheckInvoice(inv_low) && mainStack.CheckInvoice(inv_high))
					{
						mainStack.PrintRange(inv_low, inv_high, AccountTable);
					}
					else
					{
						cout << "Invoice does not exist." << endl;
					}
					break;
				}//caseB
				case 'C':
				{
					int acct_number;

					cout << "Please Enter the Account Number that you want to Print: " << endl;
					cin >> acct_number;

					if (mainStack.CheckAccount(acct_number))
					{
						mainStack.PrintByAccount(acct_number, AccountTable);
					}
					else
					{
						cout << "Account's invoice does not exist." << endl;
					}
					break;
				}//caseC
				case 'D':
				{
					int year, day, month;
					do
					{
						cout << "Please Enter the Month of the Invoice Requested:" << endl;
						cin >> month;
						cout << "Please Enter the Day of the Invoice Requested:" << endl;
						cin >> day;
						cout << "Please Enter the Year of the Invoice Requested:" << endl;
						cin >> year;
						if (!ValidDate(month, day, year, thisMonth,
							thisDay, thisYear))
						{
							cout << "Your date is invalid. Try again." << endl;
						}

						} while (!ValidDate(month, day, year, thisMonth,
							thisDay, thisYear));

						mainStack.PrintInvoiceByDate(month, day, year, AccountTable);

					break;
				}//caseD
				default:
					break;
				}
			} while ((char)toupper(input) != 'E');
			break;
		}//case 4
		default:
			break;
			}//switch1
	} while (mainInput != 5);

	float total = 0;
	AccountTable->PrintAccountReport();
	cout << endl;
	Tree->PrintHeader();
	Tree->InOrderReport(trueRoot, total);
	Tree->PrintFooter(total);
	reorderStack.PrintReport();
	AccountTable->PrintRawData(accountF, outAccount, accountFile);
	inventF.close();
	outInvent.open(invenFile);
	Tree->printLevelOrder(trueRoot, outInvent);
	outInvent.close();

    return 0;
}

