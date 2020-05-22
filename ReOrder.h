#pragma once
#include "Node.h"
#include <vector>

using namespace std;
class ReOrderVector
{
private:
	vector<ReOrder> report;
	const char Months[12][6] = { "Jan. ", "Feb. ", "Mar. ", "Apr. ", "May  ", "Jun. ", "Jul. ",
		"Aug. ", "Sep. ", "Oct. ", "Nov. ", "Dec. " };
public:
	void Push(ReOrder entry)
	{
		return report.push_back(entry);
	}//Push

	void PrintReport()
	{
		cout << endl;
		int i = 0;
		cout << setw(75) << right << "Re_Order Report" << endl;
		cout << setw(15) << "Item Number" << setw(25) << "Quantity On Hand" << setw(25) << "Date" << setw(15) << "Description" << endl;
		for (i; i < report.size(); ++i)
		{
			cout << setw(15) << report[i].item_number << setw(25) << report[i].qty_left << setw(19) << Months[report[i].reorder_month-1] <<
				setw(2) << report[i].reorder_day << setw(5) << report[i].reorder_year <<  report[i].description << endl;
		}//for

		cout << i << " items needed to be re_ordered" << endl;
	}//PrintReport
}; //ReOrderVector (class)
