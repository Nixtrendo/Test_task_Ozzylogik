#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void FileSend(SOCKET FileSendSocket, const char* FilePath)
{
	char* newfilename;
	unsigned long iFileSize = 0;
	long size;     //file size
	ifstream infile(FilePath, ios::in | ios::binary | ios::ate);
	size = infile.tellg();     //retrieve get pointer position
	infile.seekg(0, ios::beg);     //position get pointer at the begining of the file
	newfilename = new char[size];     //initialize the buffer
	infile.read(newfilename, size);     //read file to buffer
	infile.close();     //close file
	send(FileSendSocket, (char*)&size, sizeof(unsigned int), 0);
	send(FileSendSocket, newfilename, (int)strlen(newfilename), 0);

}
int main() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Cant initizize winsock" << endl;
		WSACleanup();
		return 0;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Cant create socket" << endl;
		WSACleanup();
		return 0;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));
	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}
	closesocket(listening);

	FileSend(clientSocket, "response.txt");

	closesocket(clientSocket);
	WSACleanup();
}