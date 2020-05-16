#pragma once

//////////////////////////////////////////////////////////
// Functions for the tasks and the task queues.
//////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

struct Task
{
	int pid;
	int process_priority;
	int arrival_time;
	int execution_time;
	int memory_REQ;
	int io_dev;
	int io_REQ_FREQ;

	// Added variables
	int exec_time_remain;
	int io_time_remain;
	int tqRemaining;
	int startTime;
	int endTime;
	bool started;
	bool transition;
	Task *next;

	Task()
	{
		next = NULL;
	}

	void Initialize()
	{
		exec_time_remain = execution_time;
		io_time_remain = io_REQ_FREQ;
		started = false;
		startTime = -1;
		endTime = -1;
		transition = false;
		next = NULL;
	}//Initialize
};//Task(struct)

class TaskList
{
private:
	Task *taskList[200];
	int size;
	int finalPos;
	int idleTime;
	// Used specifically by running(exec) queue
	bool full;
public:

	TaskList()
	{
		for (int i = 0; i < 200; i++)
		{
			taskList[i] = NULL;
		}
		size = 200;
		finalPos = 0;
		idleTime = 0;
		full = false;
	}//TaskList

	void InsertQueue(Task *entry)
	{
		taskList[finalPos] = entry;
		finalPos++;
	}//Insert

	void SetFinalPos(int newfinalPos)
	{
		finalPos = newfinalPos;
	}//SetFinalPos

	int GetFinalPos()
	{
		return finalPos;
	}

	bool IsEmpty()
	{
		if (finalPos == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}//IsEmpty

	bool GetFullStatus()
	{
		return full;
	}

	void SetFullStatus(bool status)
	{
		full = status;
	}

	void RemoveJob(Task *exiter)
	{
		int i = 0;
		while (exiter->pid != taskList[i]->pid)
		{
			i++;
		}

		for (int j = i; j < size; j++)
		{
			taskList[j] = taskList[j + 1];
		}
		finalPos--;
	}//RemoveJob

	// Jobs can now only move between one queue at a single time quantum
	// ResetTransitions() allows it to move again at the beginning of every
	// new time quantum.
	void ResetTransitions()
	{
		for (int i = 0; i < finalPos; i++)
		{
			taskList[i]->transition = false;
		}
	}

	void Idle()
	{
		if (IsEmpty())
		{
			idleTime++;
		}
	}//Idle

	int GetIdleTime()
	{
		return idleTime;
	}

	// Copies task to new queue
	// and deletes it from the original queue
	void MoveToQueue(TaskList* list1, Task *task)
	{
		int thisSize = list1->GetFinalPos();
		task->transition = true;
		list1->InsertQueue(task);
		list1->SetFinalPos(thisSize + 1);
		RemoveJob(task);
	}

	// Meant for Job Queue
	void LeaveJobQueue(TaskList *readyQueue, TaskList *waitQueue, TaskList *finishQueue, TaskList *rejectQueue,
		int time, int totalMem, int &currentMem, int io_devices, int &currentIO, int TQ, ofstream &outF)
	{
		for (int i = 0; i < size; i++)
		{
			// Prevents the entire list from being checked if it has less than
			// 200 tasks
			if (taskList[i] == NULL)
			{
				break;
			}

			if (taskList[i]->arrival_time <= time)
			{
				if (taskList[i]->memory_REQ > totalMem || taskList[i]->io_dev > io_devices)
				{
					cout << "Task " << taskList[i]->pid << " requires too many resources." << endl;
					outF << "Task " << taskList[i]->pid << " requires too many resourcs." << endl;
					MoveToQueue(rejectQueue, taskList[i]);
					i--;
				}//if2
				else if (taskList[i]->memory_REQ > currentMem)
				{
					MoveToQueue(waitQueue, taskList[i]);
					i--;
				}
				else
				{
					currentMem -= taskList[0]->memory_REQ;
					currentIO -= taskList[0]->io_dev;
					taskList[i]->tqRemaining = TQ;
					MoveToQueue(readyQueue, taskList[i]);
					i--;
				}
			}//if1
		}//for
	}//LeaveJobQueue

	// Meant for Wait Queue
	void FindAvailableTask(int currentMem, int currentIO, TaskList *readyQueue)
	{
		for (int i = 0; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			if (taskList[i]->memory_REQ <= currentMem && !taskList[i]->transition)
			{
				int thisSize = readyQueue->GetFinalPos();
				readyQueue->InsertQueue(taskList[i]);
				readyQueue->SetFinalPos(thisSize + 1);
				RemoveJob(taskList[i]);
				i--;
			}
		}//for
	}//FindAvailableTask

	// Counts down the execution time
	// Returns to queue afterwards
	void Execution(int &currentMem, int &currentIO, int clock, TaskList *finishQueue, TaskList *blockQueue,
				TaskList *readyQueue, ofstream &outF)
	{
		for (int i = 0; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			taskList[i]->exec_time_remain--;
			taskList[i]->io_time_remain--;
			if (taskList[i]->exec_time_remain <= 0)
			{
				currentMem += taskList[i]->memory_REQ;
				currentIO += taskList[i]->io_dev;
				std::cout << "Task " << taskList[i]->pid << " processed at " << clock << std::endl;
				outF << "Task " << taskList[i]->pid << " processed at " << clock << std::endl;
				taskList[i]->endTime = clock;
				MoveToQueue(finishQueue, taskList[i]);
				SetFullStatus(false);
				i--;
			}//if
			else if (taskList[i]->io_time_remain < 0 && taskList[i]->io_REQ_FREQ != 0)
			{
				taskList[i]->io_time_remain = taskList[i]->io_REQ_FREQ;
				MoveToQueue(blockQueue, taskList[i]);
				SetFullStatus(false);
				i--;
			}//else
		}//for
	}//Execution

	// Meant for Block Queue
	void BlockQueue(TaskList *readyQueue)
	{
		for (int i = 0; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			if (!taskList[i]->transition)
			{
				MoveToQueue(readyQueue, taskList[i]);
				i--;
			}
		}//for
	}//BlockQueue

	// Meant for Ready Queue
	void FirstComeFirstServed(TaskList *execHold, TaskList *finalQueue, int &currentMem, int &currentIO, int clock)
	{
		if (taskList[0] == NULL)
		{
			return;
		}
		if (!taskList[0]->transition)
		{
			if (!execHold->GetFullStatus())
			{
				if (taskList[0]->started == false)
				{
					taskList[0]->startTime = clock;
					taskList[0]->started = true;
				}

				MoveToQueue(execHold, taskList[0]);
				execHold->SetFullStatus(true);
			}//if2
		}//if1
	}//FCFS

	//Round Robin Functions
	// This is meant for execHold
	void RoundRobinExecution(TaskList *readyQueue, TaskList *finishQueue, TaskList *blockQueue,
		int &currentMem, int &currentIO, int TQ, int clock, ofstream &outF)
	{
		for (int i = 0; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			taskList[i]->exec_time_remain--;
			taskList[i]->tqRemaining--;
			if (taskList[i]->exec_time_remain == 0)
			{
				currentMem += taskList[i]->memory_REQ;
				currentIO += taskList[i]->io_dev;
				std::cout << "Task " << taskList[i]->pid << " processed at " << clock << std::endl;
				outF << "Task " << taskList[i]->pid << " processed at " << clock << std::endl;
				taskList[i]->endTime = clock;
				MoveToQueue(finishQueue, taskList[i]);
				SetFullStatus(false);
				i--;
			}//if
			else if (taskList[i]->tqRemaining == 0)
			{
				taskList[i]->tqRemaining = TQ;
				taskList[i]->io_time_remain--;
				if (taskList[i]->io_time_remain < 0)
				{
					taskList[i]->io_time_remain = taskList[i]->io_REQ_FREQ;
					MoveToQueue(blockQueue, taskList[i]);
				}
				else
				{
					MoveToQueue(readyQueue, taskList[i]);
				}
				SetFullStatus(false);
				i--;
			}//elseif
		}//for
	}//RoundRobinExecution

	//Shortest Remaining Time Functions
	// Meant for Ready Queue
	void FindShortestTimeJob(int &currentMem, int &currentIO, TaskList *execHold, TaskList *finishQueue,
		int clock)
	{
		if (taskList[0] == NULL)
		{
			return;
		}
		int shortestTime = taskList[0]->exec_time_remain;
		int shortestElement = 0;
		for (int i = 1; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			if (taskList[i]->exec_time_remain < shortestTime )
			{
				shortestTime = taskList[i]->exec_time_remain;
				shortestElement = i;
			}
		}//for

		if (!taskList[shortestElement]->transition)
		{
			if (!execHold->GetFullStatus())
			{
				if (taskList[shortestElement]->started == false)
				{
					taskList[shortestElement]->startTime = clock;
					taskList[shortestElement]->started = true;
				}

				MoveToQueue(execHold, taskList[shortestElement]);
				execHold->SetFullStatus(true);
			}//if2
		}//if1
	}//STJ

	//Highest Priority Functions
	// Meant for Ready Queue
	void FindHighestPriorityJob(int &currentMem, int &currentIO, TaskList *execHold, TaskList *finishQueue,
		int clock)
	{
		if (taskList[0] == NULL)
		{
			return;
		}
		int highestPriority = taskList[0]->process_priority;
		int highestElement = 0;
		for (int i = 1; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			if (taskList[i]->process_priority > highestPriority)
			{
				highestPriority = taskList[i]->process_priority;
				highestElement = i;
			}
		}

		if (!taskList[highestElement]->transition)
		{
			if (!execHold->GetFullStatus())
			{
				if (taskList[highestElement]->started == false)
				{
					taskList[highestElement]->startTime = clock;
					taskList[highestElement]->started = true;
				}

				MoveToQueue(execHold, taskList[highestElement]);
				execHold->SetFullStatus(true);
			}//if2
		}//if1
	}//HPJ

	void PrintFinishQueue(ofstream &outF)
	{
		for (int i = 0; i < size; i++)
		{
			if (taskList[i] == NULL)
			{
				break;
			}
			std::cout << "Task " << taskList[i]->pid << std::endl;
			std::cout << "AT: " << taskList[i]->arrival_time << ' ';
			std::cout << "ET: " << taskList[i]->exec_time_remain << ' ';
			std::cout << "Mem Req: " << taskList[i]->memory_REQ << ' ';
			std::cout << "IO Req: " << taskList[i]->io_dev << ' ';
			std::cout << "IO Freq: " << taskList[i]->io_REQ_FREQ << std::endl;
			std::cout << "Exec Start Time: " << taskList[i]->startTime << ' ';
			std::cout << "Exec End Time: " << taskList[i]->endTime << std::endl << std::endl;

			outF << "Task " << taskList[i]->pid << endl;
			outF << "AT: " << taskList[i]->arrival_time << ' ';
			outF << "ET: " << taskList[i]->exec_time_remain << ' ';
			outF << "Mem Req: " << taskList[i]->memory_REQ << ' ';
			outF << "IO Req: " << taskList[i]->io_dev << ' ';
			outF << "IO Freq: " << taskList[i]->io_REQ_FREQ << endl;
			outF << "Exec Start Time: " << taskList[i]->startTime << ' ';
			outF << "Exec End Time: " << taskList[i]->endTime << endl << endl;

		}//for
	}//PrintFinishQueue

	// Meant for Finish Queue
	bool CheckFinishQueue(TaskList *rejectQueue, TaskList *execHold, int correctSize, int clock, ofstream &outF)
	{
		int totalWaitTime = 0;
		int totalResponseTime = 0;
		int totalTurnaroundTime = 0;
		int cpuUsage = 0;
		int rejectQueueSize = rejectQueue->GetFinalPos();
		if ((finalPos + rejectQueueSize) == correctSize)
		{
			for (int i = 0; i < size; i++)
			{
				if (taskList[i] == NULL)
				{
					break;
				}
				totalWaitTime += taskList[i]->endTime - taskList[i]->execution_time;
				totalResponseTime += taskList[i]->startTime - taskList[i]->arrival_time;
				totalTurnaroundTime += taskList[i]->endTime - taskList[i]->startTime;
			}//for

			cpuUsage = clock - execHold->GetIdleTime();

			cout << "Schedule Finished" << endl;
			cout << "Average Wait Time: " << (float)totalWaitTime / finalPos << endl;
			cout << "Average Response Time: " << (float)totalResponseTime / finalPos << endl;
			cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / finalPos << endl;
			cout << "Overall CPU Usage: " << cpuUsage << endl;
			cout << "All Tasks Processed At " << clock << " Milliseconds" << endl;

			outF << "Schedule Finished" << endl;
			outF << "Average Wait Time: " << (float)totalWaitTime / finalPos << endl;
			outF << "Average Response Time: " << (float)totalResponseTime / finalPos << endl;
			outF << "Average Turnaround Time: " << (float)totalTurnaroundTime / finalPos << endl;
			outF << "Overall CPU Usage: " << cpuUsage << endl;
			outF << "All Tasks Processed At " << clock << " Milliseconds" << endl;
			return true;
		}//if
		else
		{
			return false;
		}
	}//CheckFinishQueue
};//TaskList(class)
