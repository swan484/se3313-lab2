#include <iostream>
#include "SharedObject.h"
#include "Semaphore.h"

using namespace std;

//Use shared structure
struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

int main(void)
{
	cout << "I am a reader" << endl;
	//Create pointer shared (not as the owner) in order to access shared data
	Shared<MyShared> shared("sharedMemory", false);

	//Create 'block' semaphore to block access to shared data
	Semaphore block("block", 1, true);
	//Create 's' semaphore to signal when shared data was changed
	Semaphore s("s", 1, true);

	while(true){
		//Wait for the shared data to change
		s.Wait();
		//Output the current shared data to the console
		cout << "Reader Thread:" << shared->threadId << " " << shared->reportId << " " << shared->delay << endl;
		//Free up the ability to change the shared data
		block.Signal();
	}
}
