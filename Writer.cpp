#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <stack>

using namespace std;

struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

int main(void)
{
	string userText;
	string delayLength;
	int threadCount = 0;
	WriterThread* thread;
	stack<WriterThread*> threadStack;
	Shared<MyShared> shared("sharedMemory", true);
	cout << "I am a writer" << endl;

	////////////////////////////////////////////////////////////////////////
	// This is a possible starting point for using threads and shared memory. 
	// You do not have to start with this
	////////////////////////////////////////////////////////////////////////

	while(true){
		cout << "Do you want to make a new thread? [y/n]" << endl;
		getline(cin, userText);

		if(userText == "y"){
			cout << "What do you want the wait time to be?" << endl;
			getline(cin, delayLength);

			int delay = stoi(delayLength);
			thread = new WriterThread(++threadCount, delay, 0);
			threadStack.push(thread);
		}
		else {
			break;
		}
	}
	
	int i = 0;
	for(i; i < threadStack.size(); i++){
		thread = threadStack.top();
		thread->flag = true;
		delete thread;
		threadStack.pop();
	}
}


////////////////////////////////////////////////////////////////////////
// This is a possible starting point for using threads and shared memory. 
// You do not have to start with this
////////////////////////////////////////////////////////////////////////
class WriterThread : public Thread{
	public:
		int 	threadNum;
		bool	flag;
		int 	delay;
		int 	reportId;
		
		WriterThread(int num, int del, int rep):Thread(8*1000){
			this->threadNum = num; 
			this->delay = del;
			this->reportId = rep;
		}

		virtual long ThreadMain(void) override{
			Shared<MyShared> sharedMemory ("sharedMemory");
			while(true)
			{
				time_t time_1 = time(NULL);
				sleep(delay);
				time_t time_2 = time(NULL);

				int time_diff = time_2 - time_1;
				this->reportId++;

				//Write to shared memory
				sharedMemory->threadId=threadNum;
				sharedMemory->reportId=reportId;
				sharedMemory->delay=time_diff;

				if(flag){//Exit loop to end the thread
					break;
				}
			}
		}
};