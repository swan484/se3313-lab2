#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <stack>
#include "Semaphore.h"

using namespace std;

//Use shared structure
struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

//Create 'block' semaphore to block access to shared data
Semaphore block("block", 1, true);
//Create 's' semaphore to signal when shared data was changed
Semaphore s("s", 1, true);

class WriterThread : public Thread{
	//Define the fields of the class
	public:
		int 	threadNum;
		bool	flag;
		int 	delay;
		int 	reportId;
		
		//Create constructor for WriterThread to intialize class fields
		WriterThread(int num, int del, int rep):Thread(8*1000){
			this->threadNum = num; 
			this->delay = del;
			this->reportId = rep;
		}

		virtual long ThreadMain(void) override{
			//Create sharedMemory to access the shared memory
			Shared<MyShared> sharedMemory ("sharedMemory");
			while(true)
			{
				//Record the inital time value and sleep for the time defined in delay
				time_t time_1 = time(NULL);
				sleep(delay);
				//Wait for access to modify the shared data
				block.Wait();
				time_t time_2 = time(NULL);

				//Get the time elapsed
				int time_diff = time_2 - time_1;
				//Increment the report ID
				this->reportId++;

				//Write to shared memory
				sharedMemory->threadId=threadNum;
				sharedMemory->reportId=reportId;
				sharedMemory->delay=time_diff;

				//Signal that the shared data has changed
				s.Signal();

				//Exit loop to end the thread
				if(flag){
					break;
				}
			}
		}
};

int main(void)
{
	//Define various required fields
	string userText;
	string delayLength;
	int threadCount = 0;
	WriterThread* thread;
	stack<WriterThread*> threadStack;
	Shared<MyShared> shared("sharedMemory", true);
	cout << "I am a writer" << endl;

	while(true){
		//Get user input to detemine if they want to make a thread
		cout << "Do you want to make a new thread? [y/n] " << endl;
		getline(cin, userText);

		//If the user entered 'y' then continue
		if(userText == "y"){
			//Get the delay for the new thread
			cout << "What do you want the wait time to be? " << endl;
			getline(cin, delayLength);

			//Convert this string delay to integer
			int delay = stoi(delayLength);
			//Create a new thread
			thread = new WriterThread(++threadCount, delay, 0);
			//Add the thread to the stack which tracks all of the running threads
			threadStack.push(thread);
		}
		else {
			break;
		}
	}
	
	//For each thread, end its execution (by setting flag to true), deleting it, and popping it from the stack
	int i = 0;
	while(i++ < threadStack.size()){
		thread = threadStack.top();
		thread->flag = true;
		delete thread;
		threadStack.pop();
	}
}