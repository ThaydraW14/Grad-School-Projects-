#pragma once
#include "Node.h"
#include "Utility.h"

#define MAXSIZE 1999
#define NULL 0
class HashMap
{
private:
	acctNode **hashTable;

public:
	HashMap()
	{
		hashTable = new acctNode*[MAXSIZE];

		for (int i = 0; i < MAXSIZE; ++i)
		{
			hashTable[i] = NULL;
		}
	}//HashMap

	int hashFunction(int key)
	{
		return key % MAXSIZE;
	}//hashFunction

	void Insert(acctNode *entry)
	{
		int index = hashFunction(entry->acct_number);

		while (hashTable[index] != NULL && hashTable[index]->acct_number != entry->acct_number)
		{
			index++;
			index %= MAXSIZE;
		}

		if (hashTable[index] == NULL)
		{
			hashTable[index] = entry;
		}
	}//Insert

	void GetData(ifstream &inF)
	{

		while (!inF.eof())
		{
			acctNode *entry = new acctNode();

			inF >> entry->acct_number >> entry->cred_limit >> entry->active >> entry->first_name >> entry->last_name >>
				entry->balance >> entry->address >> entry->city >> entry->state >> entry->zip;

			if (entry->acct_number >= 0)
			{
				this->Insert(entry);
			}
		}

	}//GetData

	void Modify(int acct_number)
	{
		int i;
		bool found = this->CheckTable(acct_number, i);


		if (!found)
		{
			cout << "Sorry. Account Number does not exist. Try choosing the 'Add Account' option: " << endl;
			return;
		}
		else
		{
			cout << "Please Enter First Name" << endl;
			cin >> hashTable[i]->first_name;

			cout << "Please Enter Last Name" << endl;
			cin >> hashTable[i]->last_name;

			cout << "Please Enter the Address. Seperate Words with Underscores (Ex. 234_Macon_Ave)" << endl;
			cin >> hashTable[i]->address;

			cout << "Please Enter the City (Please place '_' between multiple words)" << endl;
			cin.ignore();
			cin.getline(hashTable[i]->city, 20);

			cout << "Please Enter the State (Please place '_' between multiple words)" << endl;
			cin.getline(hashTable[i]->state, 20);

			cout << "Please Enter the Zip Code" << endl;
			cin >> hashTable[i]->zip;
		}//else

	}//Modify

	bool CheckTable(int acct_number, int &dest)
	{
		for (int i = 0; i < MAXSIZE; ++i)
		{
			if (hashTable[i] != NULL)
			{
				if (acct_number == hashTable[i]->acct_number)
				{
					dest = i;
					return true;
				}//if2
			}//if1
		}//for

		return false;
	}//CheckTable

	void Deactivate(int acct_number)
	{
		int i;
		CheckTable(acct_number, i);

		hashTable[i]->active = false;
	}//Deactivate

	void PrintAddress(int acct_number)
	{
		int dest;
		CheckTable(acct_number, dest);

		for (int i = 0; i < 35; ++i)
		{
			if (hashTable[dest]->address[i] == '_')
			{
				hashTable[i]->address[i] == ' ';
			}
		}

		cout << hashTable[dest]->address << endl;
	}//PrintAddress

	void PrintInvoiceHeader(int acct_number)
	{
		int i;
		CheckTable(acct_number, i);

		cout << "Ship to: " << hashTable[i]->first_name << ' ' << hashTable[i]->last_name;
		cout << ' ' << setw(15) << right << "Account Number: " << hashTable[i]->acct_number;
		cout << endl;
		PrintAddress(acct_number);
		cout << endl;

	}//PrintInvoiceHeader

	bool SufficientFunds(int dest, invoiceNode entry)
	{
		float price = entry.billing;
		price *= entry.tax_amt;
		if (hashTable[dest]->balance >= price)
		{
			hashTable[dest]->balance -= price;
			return true;
		}
		return false;
	}//SufficientFunds

	void ItemRemoved(invoiceNode item)
	{
		int dest;
		for (int i = 0; i < MAXSIZE; ++i)
		{
			if (hashTable[i] != NULL)
			{
				if (item.acct_number == hashTable[i]->acct_number)
				{
					int dest = i;
					i += MAXSIZE;
				}//if2
			}//if1
		}//for

		hashTable[dest]->balance += item.billing;
	}//ItemRemoved

	void CopyDataToInvoice(int dest, invoiceNode &entry)
	{
		strcpy(entry.acct_first_name, hashTable[dest]->first_name);
		strcpy(entry.acct_last_name, hashTable[dest]->last_name);
	}//CopyDataToInvoice

	void PrintAccountReport()
	{
		int thisMonth;
		int thisYear;
		int thisDay;
		GetTodaysDate(thisMonth, thisDay, thisYear);

		char Active[2] = { 'I', 'A' };

		cout << setw(75) << "Account Summary Report" << endl;
		cout << "Date " << thisMonth << '/' << thisDay << '/' << thisYear << "<current date>" << endl;

		cout << setw(15) << "Account" << setw(15) << ' ' << "Account" << setw(15) << "Account" << setw(25) << "Account" <<
			setw(25) << "Account" << setw(15) << ' ' << "Account" << endl;
		cout << setw(15) << "Number" << setw(15) << ' ' << "Balance" << setw(15) << "Status" << setw(25) << "Last Name" <<
			setw(25) << "First Name" << setw(15) << ' ' << "Credit Limit" << endl;

		for (int i = 0; i < MAXSIZE; ++i)
		{
			if (hashTable[i] != NULL)
			{
				cout << setw(15) << hashTable[i]->acct_number << setw(15) << '$' << fixed << setprecision(2) <<
					hashTable[i]->balance << setw(15) << Active[hashTable[i]->active] << setw(25) << hashTable[i]->last_name
					<< setw(25) << hashTable[i]->first_name << setw(15) << '$' << fixed << setprecision(2) << hashTable[i]->cred_limit << endl;
			}//if
		}//for
	}//PrintAccountReport

	void PrintRawData(ifstream &inF, ofstream &outF, char filename[])
	{
		inF.close();
		outF.open(filename);
		for (int i = 0; i < MAXSIZE; ++i)
		{
			if (hashTable[i] != NULL)
			{
				outF << hashTable[i]->acct_number << ' ' << hashTable[i]->balance << ' ' << hashTable[i]->active << ' ' <<
					hashTable[i]->last_name << ' ' << hashTable[i]->first_name << ' ' << hashTable[i]->cred_limit << ' ' <<
					hashTable[i]->address << ' ' << hashTable[i]->city << ' ' << hashTable[i]->state << ' ' << hashTable[i]->zip << endl;
			}//if
		}//for
		outF.close();
	}//PrintRawData

	bool IsActive(int acct_number)
	{
		for (int i = 0; i < MAXSIZE; ++i)
		{
			if (hashTable[i] != NULL)
			{
				if (hashTable[i]->acct_number == acct_number && hashTable[i]->active)
				{
					return true;
				}
				else if (hashTable[i]->acct_number == acct_number && !hashTable[i]->active)
				{
					return false;
				}
			}//if
		}//for
		return false;
	}//IsActive
}; //End of Hash (class)

