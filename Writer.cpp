#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <stack>

using namespace std;

//Define structure for shared data
struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

class WriterThread : public Thread{
	//Define the attributes for the class
	public:
		int 	threadNum;
		bool	flag;
		int 	delay;
		int 	reportId;
		
		//Create the constructor for the WriterThread
		WriterThread(int num, int del, int rep):Thread(8*1000){
			this->threadNum = num; 
			this->delay = del;
			this->reportId = rep;
		}

		virtual long ThreadMain(void) override{
			//Define variable that gives access to the shared memory
			Shared<MyShared> sharedMemory ("sharedMemory");
			while(true)
			{
				//Sleep for the delay time and record the time difference
				time_t time_1 = time(NULL);
				sleep(delay);
				time_t time_2 = time(NULL);

				int time_diff = time_2 - time_1;
				//Increment the report ID
				this->reportId++;

				//Write to shared memory
				sharedMemory->threadId=threadNum;
				sharedMemory->reportId=reportId;
				sharedMemory->delay=time_diff;

				//Exit loop to end the thread
				if(flag){
					break;
				}
			}
		}
};

int main(void)
{
	//Define required fields
	string userText;
	string delayLength;
	int threadCount = 0;
	WriterThread* thread;
	stack<WriterThread*> threadStack;
	Shared<MyShared> shared("sharedMemory", true);
	cout << "I am a writer" << endl;

	while(true){
		//Prompt the user to decide if they want to create a new thread
		cout << "Do you want to make a new thread? [y/n] " << endl;
		getline(cin, userText);

		//If the user inputs 'y' then create a new thread
		if(userText == "y"){
			//Get the delay time for the thread
			cout << "What do you want the wait time to be? " << endl;
			getline(cin, delayLength);

			//Convert the delay time to an integer
			int delay = stoi(delayLength);
			//Create the new thread
			thread = new WriterThread(++threadCount, delay, 0);
			//Push it to the stack
			threadStack.push(thread);
		}
		else {
			break;
		}
	}
	
	//For each thread, end its execution (set flag to true), delete the thread, and pop it from the stack
	int i = 0;
	while(i++ < threadStack.size()){
		thread = threadStack.top();
		thread->flag = true;
		delete thread;
		threadStack.pop();
	}
}