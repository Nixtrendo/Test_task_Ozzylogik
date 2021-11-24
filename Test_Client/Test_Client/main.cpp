#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
ofstream out("C:\\Prog.exe", ios::binary);
void FileReceive(char* recvbuf, int recvbuflen)
{
	if (out.is_open())
	{
		out.write(recvbuf, recvbuflen);
		ZeroMemory(&recvbuf, recvbuflen);
	}
}
void main()
{
	string ipAddress = "127.0.0.1";
	int port = 54000;

	// WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}


	char buf[4096];
	string userInput;


	// Wait for response
	ZeroMemory(buf, 4096);
	unsigned int fileSize = 0;
	recv(sock, (char*)&fileSize, sizeof(unsigned int), 0);
	int bytesReceived = recv(sock, buf, fileSize, 0);
	if (bytesReceived > 0)
	{
		FILE* infile = fopen("response.txt", "wb");
		fwrite(buf, 1, sizeof(buf), infile);
		fclose(infile);
	}



	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}