#pragma once
#include "stdafx.h"
#include <iostream>

using namespace std;

struct Task
{
	int pid;
	float arrival_time;
	float execution_time;
	int memory_REQ;
	int io_dev;
	int io_REQ_FREQ;

	int exec_time_remain;
	int io_time_remain;
	int tqRemaining;
	Task *next;

	Task()
	{
		next = NULL;
	}

	Task(int pid, float arrival_time, float execution_time, int memory_REQ,
	int io_dev, int io_REQ_FREQ)
	{
		this->pid = pid;
		this->arrival_time = arrival_time;
		this->execution_time = execution_time;
		this->memory_REQ = memory_REQ;
		this->io_dev = io_dev;
		this->io_REQ_FREQ = io_REQ_FREQ;
		exec_time_remain = execution_time;
		io_time_remain = io_REQ_FREQ;
		next = NULL;
	}//Task(function

	void Initialize()
	{
		exec_time_remain = execution_time;
		io_time_remain = io_REQ_FREQ;
		next = NULL;
	}//Initialize
};//Task(struct)

class TaskList
{
private:
	Task *head;

public:

	TaskList()
	{
		head = NULL;
	}

	void InsertQueue(Task *entry)
	{
		if (head == NULL)
		{
			head = entry;
			return;
		}
		if (head->next == NULL)
		{
			head->next = entry;
			return;
		}

		Task *ptr = head;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = entry;
	}//InsertQueue

	void MoveToBackofList()
	{
		Task *temp = head;
		head = head->next;

		Task *ptr = head;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
	}//Move2BackofList

	Task *FirstOut()
	{
		Task *temp = head;
		head = head->next;
		return temp;
	}//FirstOut

	Task *GetHead()
	{
		return head;
	}

	bool IsEmpty()
	{
		if (head == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}//IsEmpty

	void LeaveJobQueue(TaskList *readyQueue, TaskList *waitQueue, TaskList *finishQueue,
		int time, int totalMem, int currentMem, int TQ)
	{
		while (head->arrival_time <= time)
		{
			Task *temp = head;
			head = head->next;

			if (head->memory_REQ > totalMem)
			{
				finishQueue->InsertQueue(temp);
			}
			else if (head->memory_REQ > currentMem)
			{
				waitQueue->InsertQueue(temp);
			}
			else
			{
				temp->tqRemaining = TQ;
				readyQueue->InsertQueue(temp);
			}
		}//while

		Task *ptr = head;
		while (ptr->next != NULL)
		{
			if (ptr->next->arrival_time <= time)
			{
				Task *temp = ptr->next;
				ptr->next = temp->next;

				if (temp->memory_REQ > totalMem)
				{
					finishQueue->InsertQueue(temp);
				}
				else if (temp->memory_REQ > currentMem)
				{
					waitQueue->InsertQueue(temp);
				}
				else
				{
					readyQueue->InsertQueue(temp);
				}

				if (ptr->next == NULL)
				{
					break;
				}

			}//if
			ptr = ptr->next;
			break;
		}//while
		std::cout << "hhe" << std::endl;
	}//LeaveJobQueue

	// Wait Queue
	void FindAvailableTask(int currentMem, int currentIO, TaskList *readyQueue)
	{
		if (head->memory_REQ < currentMem  /*&& head->io_dev < currentIO*/)
		{
			Task *temp = head;
			head = head->next;
			readyQueue->InsertQueue(temp);
		}

		Task *ptr = head;

		while (ptr->next != NULL)
		{
			if (ptr->next->memory_REQ < currentMem  /*&& ptr->next->io_dev < currentIO*/)
			{
				Task *temp = ptr->next;
				ptr->next = temp->next;
				readyQueue->InsertQueue(temp);
			}
			ptr = ptr->next;
		}//while
	}//FindAvailableTask

	// Counts down the execution time
	// Returns to queue afterwards
	void Execution(int &currentMem, int &currentIO, int time, TaskList *finishQueue, TaskList *blockQueue)
	{
		while (head != NULL)
		{
			head->exec_time_remain--;
			if (head->exec_time_remain == 0)
			{
				Task *temp = head;
				head = head->next;
				currentMem += temp->memory_REQ;
				currentIO += temp->io_dev;
				finishQueue->InsertQueue(temp);
				std::cout << temp->pid << " processed at " << time << std::endl;
			}
			else
			{
				head->io_time_remain = head->io_REQ_FREQ;
				Task *temp = head;
				head = head->next;
				currentMem += temp->memory_REQ;
				currentIO += temp->io_dev;
				blockQueue->InsertQueue(temp);
			}
			head = head->next;
		}//while
	}//Execution

	void BlockQueue(TaskList *execHold)
	{
		while (head->io_time_remain -= 1 && head->io_time_remain <= 0)
		{
			Task *temp = head;
			head = head->next;
			execHold->InsertQueue(temp);
		}
		Task *ptr = head;
		while (ptr->next != NULL)
		{
			ptr->next->io_time_remain--;
			if (ptr->next->io_time_remain == 0)
			{
				Task *temp = ptr->next;
				ptr->next = temp->next;
				execHold->InsertQueue(temp);
			}
			ptr = ptr->next;
		}//while
	}//BlockQueue

	void FirstComeFirstServed(TaskList *execHold, int &currentMem, int &currentIO)
	{
		if (head->io_dev < currentIO)
		{
			Task *temp = head;
			head = head->next;
			execHold->InsertQueue(temp);
			currentMem -= temp->memory_REQ;
			currentIO -= temp->io_dev;
		}
	}//FCFS

	//Round Robin Functions
	void RoundRobinExecution(TaskList *readyQueue, TaskList *finishQueue, TaskList *blockQueue,
		int &currentMem, int &currentIO, int TQ, int clock)
	{
		while (head != NULL)
		{
			head->exec_time_remain--;
			if (head->exec_time_remain == 0)
			{
				Task *temp = head;
				head = head->next;
				currentMem += temp->memory_REQ;
				currentIO += temp->io_dev;
				finishQueue->InsertQueue(temp);
				std::cout << temp->pid << " processed at " << clock << std::endl;
			}
			else if (head->tqRemaining == 0)
			{
				Task *temp = head;
				head = head->next;
				currentMem += temp->memory_REQ;
				currentIO += temp->io_dev;
				temp->tqRemaining = TQ;
				readyQueue->InsertQueue(temp);
			}
			else
			{
				head->io_time_remain = head->io_REQ_FREQ;
				Task *temp = head;
				head = head->next;
				currentMem += temp->memory_REQ;
				currentIO += temp->io_dev;
				blockQueue->InsertQueue(temp);
			}
			head = head->next;
		}//while
	}//RoundRobinExecution

	//Shortest Remaining Time Functions
	void FindShortestTimeJob(int &currentMem, int &currentIO, TaskList *execHold)
	{
		Task *shortestTask;
		int shortestTime = head->execution_time;
		Task *ptr = head;
		while (ptr->next != NULL)
		{
			if (ptr->next->execution_time < shortestTime)
			{
				shortestTime = ptr->next->execution_time;
				shortestTask = ptr->next;
			}
			ptr = ptr->next;
		}//while

		if (shortestTask->next->io_dev > currentIO)
		{
			if (shortestTask == head)
			{
				head = head->next;
				execHold->InsertQueue(shortestTask);
				currentMem -= shortestTask->memory_REQ;
				currentIO -= shortestTask->io_dev;
			}
		}//if
	}//FindShortestTimeJob

	void PrintFinishQueue()
	{
		if (head == NULL)
		{
			std::cout << "No processes completed." << std::endl;
		}
		else
		{
			Task *ptr = head;
			while (ptr != NULL)
			{
				std::cout << ptr->arrival_time << std::endl;
				std::cout << ptr->execution_time << std::endl;
				std::cout << ptr->memory_REQ << std::endl;
				std::cout << ptr->io_dev << std::endl;
				std::cout << ptr->io_REQ_FREQ << std::endl;
			}
		}//else
	}//PrintFinishQueue

};//TaskList (struct)
