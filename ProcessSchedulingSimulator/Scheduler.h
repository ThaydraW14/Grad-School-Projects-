#pragma once

///////////////////////////////////////////////////////////
// This class calls and controls the scheduling algorithms in the system
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "newTask.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

class Scheduler
{
private:

	TaskList *jobQueue = new TaskList();
	TaskList *readyQueue = new TaskList();
	TaskList *waitQueue = new TaskList();
	TaskList *blockQueue = new TaskList();
	TaskList *execHold = new TaskList();
	TaskList *rejectedQueue = new TaskList();
	TaskList *finishQueue = new TaskList();

	float clock_start_time;
	int mem_units;
	int io_devices;
	int process_alg;
	int timeQuantum;

	// This will be referenced by the finishQueue to determine if all
	// tasks have been processed
	int inputSize;

public:
	Scheduler(){}

	void SetData(std::ifstream &inF)
	{
		char fodder;
		inF >> fodder >> fodder; //Reads in the "C = "
		inF >> clock_start_time;
		inF >> fodder >> fodder; //Reads in the "M = "
		inF >> mem_units;
		inF >> fodder >> fodder; //Reads in the "D = "
		inF >> io_devices;
		inF >> fodder >> fodder; //Reads in the "S = "
		inF >> process_alg;
		inF >> fodder >> fodder; //Reads in the "Q = "
		inF >> timeQuantum;
		inputSize = 0;

		while (!inF.eof())
		{
			Task *entry = new Task();

			inF >> entry->pid >> entry->process_priority >> entry->arrival_time >> entry->execution_time >> entry->memory_REQ
				>> entry->io_dev >> entry->io_REQ_FREQ;
			entry->tqRemaining;

			entry->Initialize();

			// Prevents bad input data from getting into the queue
			if (entry->pid >= 0)
			{
				jobQueue->InsertQueue(entry);
				inputSize++;
			}
		}//while
	}//SetData

	void PrintQueues(ofstream &outF)
	{
		cout << "Job Queue:" << endl;
		outF << "Job Queue:" << endl;
		jobQueue->PrintFinishQueue(outF);

		cout << "Wait Queue:" << endl;
		outF << "Wait Queue:" << endl;
		waitQueue->PrintFinishQueue(outF);

		cout << "Ready Queue:" << endl;
		outF << "Ready Queue:" << endl;
		readyQueue->PrintFinishQueue(outF);

		cout << "Running Queue:" << endl;
		outF << "Running Queue:" << endl;
		execHold->PrintFinishQueue(outF);

		cout << "Block Queue:" << endl;
		outF << "Block Queue:" << endl;
		blockQueue->PrintFinishQueue(outF);

		cout << "Reject Queue:" << endl;
		outF << "Reject Queue:" << endl;
		rejectedQueue->PrintFinishQueue(outF);

		cout << "Finish Queue:" << endl;
		outF << "Finish Queue:" << endl;
		finishQueue->PrintFinishQueue(outF);

		cout << endl << endl;
		outF << endl << endl;
	}//PrintQueues

	void Schedule()
	{
		ofstream outF;
		outF.open("Results.txt");

		int currentMem = mem_units;
		int currentIO = io_devices;
		int clock = 0;

		while (true)
		{
			jobQueue->ResetTransitions();
			waitQueue->ResetTransitions();
			readyQueue->ResetTransitions();
			execHold->ResetTransitions();
			blockQueue->ResetTransitions();

			cout << "//////////////////////////////////////" << endl;
			cout << "At clock time: " << clock << endl;
			outF << "//////////////////////////////////////" << endl;
			outF << "At clock time: " << clock << endl;

			//Checks that clock start time has been met
			if (clock >= clock_start_time)
			{
				// Called first so that the output does not appear to be skipping queues
				execHold->Idle();
				if (!execHold->IsEmpty())
				{
					if (process_alg == 2)
					{
						execHold->RoundRobinExecution(readyQueue, finishQueue, blockQueue, currentMem, currentIO, timeQuantum, clock, outF);
					}
					else
					{
						execHold->Execution(currentMem, currentIO, clock, finishQueue, blockQueue, readyQueue, outF);
					}
				}//if2

				if (!blockQueue->IsEmpty())
				{
					blockQueue->BlockQueue(readyQueue);
				}

				waitQueue->FindAvailableTask(currentMem, currentIO, readyQueue);

				switch (process_alg)
				{
				case 1:
					readyQueue->FirstComeFirstServed(execHold, finishQueue, currentMem, currentIO, clock);
					break;
				case 2:
					readyQueue->FirstComeFirstServed(execHold, finishQueue, currentMem, currentIO, clock);
					break;
				case 3:
					readyQueue->FindShortestTimeJob(currentMem, currentIO, execHold, finishQueue, clock);
					break;
				case 4:
					readyQueue->FindHighestPriorityJob(currentMem, currentIO, execHold, finishQueue, clock);
					break;
				default:
					break;
				}//switch

				jobQueue->LeaveJobQueue(readyQueue, waitQueue, finishQueue,rejectedQueue, clock, mem_units, currentMem, io_devices, currentIO, timeQuantum, outF);

				PrintQueues(outF);

				if (finishQueue->CheckFinishQueue(rejectedQueue, execHold, inputSize, clock, outF))
				{
					break;
				}
			}//if1
			else
			{
				PrintQueues(outF);
			}
			// Pauses the for loop for this many milliseconds
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			clock++;
		}//while
		outF.close();
	}//Schedule(function)
};//Scheduler(class)
