#include <iostream>
#include "SharedObject.h"

using namespace std;

//Define structure for shared data
struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

int main(void)
{
	cout << "I am a reader" << endl;
	//Define variable that gives access to the shared memory
	Shared<MyShared> shared("sharedMemory", false);

	while(true){
		//Each second, print the state of the shared data
		cout << "Reader Thread:" << shared->threadId << " " << shared->reportId << " " << shared->delay << endl;
		sleep(1);
	}
}
