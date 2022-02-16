#include "DatagramSocket-master/DatagramSocket.h"
#include "Thread-master/Thread.h"  //another project
#include "JSON-master/json.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace nlohmann;

struct SensorData {
	float azimuth = 0;
	float pitch = 0;
	float roll = 0;
};
typedef void (*f_callback) (SensorData);

class MyThread : public Thread {
private:
	DatagramSocket* s;
	f_callback callback;
	boolean isException = false;
public:
	MyThread(DatagramSocket* sock, f_callback callback) {
		s = sock;
		this->callback = callback;
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(handle, &cci);
		cci.bVisible = false;
		SetConsoleCursorInfo(handle, &cci);
		Thread::CreateNewThread(this);
	}

	void Run(void*) {
		char msg[4000];
		while (1) {
			s->receive(msg, 4000);
			SensorData sd = toSensorData(msg);
			if(!isException) invoke(callback, sd);
		}
	}
private:
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SensorData toSensorData(string msg) {
		isException = false;
		SensorData sd = SensorData();
		int lastIndex = msg.find_last_of('}');
		msg = msg.substr(0, lastIndex + 1);
		try {
			json js = json::parse(msg);

			//SetConsoleCursorPosition(handle, { 0, 0 });
			//cout << string().assign(4000, ' ');
			//SetConsoleCursorPosition(handle, { 0, 0 });
			//cout << js.dump(4) << endl;
			sd.azimuth = js.at("gyro_azimuth");
			sd.pitch = js.at("gyro_pitch");
			sd.roll = js.at("gyro_roll");
		} catch (exception e) {
			//cout << "Exception " << e.what();
			isException = true;
		}
		return sd;
	}
	void print(SensorData data) {
	} 

};
class Server {
private:
	string ipMask;
	int port;
	DatagramSocket* socket = nullptr;
	MyThread* thread;
public:
	Server(string ipMask, int port) {
		this->ipMask = ipMask;
		this->port = port;
	}
	boolean start(f_callback callback) {
		socket = new DatagramSocket(port, ipMask.c_str(), true, true);
		if (!socket) return false;
		thread = new MyThread(socket, callback);
		return true;
	}
	~Server() {
		delete thread;
		delete socket;
	}
};


//CALLBACK EXAMPLE
void callback(SensorData) {
	cout << "CALLBACK";
}

//START EXAMPLE
int main() {
	Server* server = new Server("255.255.255.255", 25565);
	server->start(callback);

	string msg = "";

	while (1) {
		getline(cin, msg);
		//s->send(msg.c_str(), msg.length());
	}

	//public 
}
