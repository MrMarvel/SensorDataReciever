#include "DatagramSocket-master/DatagramSocket.h"
#include "Thread-master/Thread.h"  //another project
#include <iostream>
#include <string>

using namespace std;

class MyThread : public Thread {
private:
	DatagramSocket* s;

public:
	MyThread(DatagramSocket* sock) {
		s = sock;
		Thread::CreateNewThread(this);
	}

	void Run(void*) {
		char msg[4000];
		while (1) {
			s->receive(msg, 4000);
			cout << msg << endl;
		}
	}

};


int main() {
	DatagramSocket* s = new DatagramSocket(5000, "255.255.255.255", TRUE, TRUE);
	MyThread* t = new MyThread(s);

	string msg = "";

	while (1) {
		getline(cin, msg);
		s->send(msg.c_str(), msg.length());
	}
}
