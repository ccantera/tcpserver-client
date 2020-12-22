#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void main() {
	string ipAddress;
	cout << "Enter IP Address: " << endl;
	cin >> ipAddress;
	int port = 54000;
	
	//initialize
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cout << "Can't start " << wsResult << endl;
		return;
	}
	//create sock
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Can't create " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	//fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	//connect 
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cout << "Can't connect " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	//do-while
	char buf[4096];
	string input;
	do {
		//input
		cout << "> ";
		cin >> input;
		if (input.size() > 0) {
			//send input
			int sendResult = send(sock, input.c_str(), input.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				//wait for response
				ZeroMemory(buf, 4096);
				int bytesRecieved = recv(sock, buf, 4096, 9);
				if (bytesRecieved > 0) {
					//echo
					cout << "GOD> " << string(buf, 0, bytesRecieved) << endl;
				}
			}
		}
	} while (input.size() > 0);
	//close
	closesocket(sock);
	WSACleanup();
}