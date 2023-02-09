#include <iostream>
#include "SharedObject.h"

using namespace std;

struct MyShared {
	int threadId;
	int reportId;
	int delay;
};

int main(void)
{
	cout << "I am a reader" << endl;
	Shared<MyShared> shared("sharedMemory", false);

	while(true){
		cout << "Reader Thread:" << shared->threadId << " " << shared->reportId << " " << shared->delay << endl;
		sleep(1);
	}
}
