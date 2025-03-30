#pragma once
#include "DataProvider.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <atomic>
#include <vector>
//#include "SharedVector.h"


//#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library
#pragma comment(lib, "Ws2_32.lib")

#define PORT 5000
#define BUFFER_SIZE 1024

class TCPConMan : public DataProvider
{
    int n_mode;
    std::string str_ip;
    int n_port; 

private:

    SOCKET Socket, clientSocket;
    sockaddr_in clientAddr;
    int clientAddrSize;

public:

    enum Mode {
        Sender = 0,
        Receiver
    };

    void Exit() {
        m_Exit = 1;
    }

    TCPConMan() : Socket(0), clientSocket(0),n_mode(0), n_port(0),
        str_ip(""){
        pSharevec = 0; 
        clientAddrSize = sizeof(clientAddr);
    }

	void Init(IniReader* m_pReader, std::string strSectionName) {

		str_ip = m_pReader->getstring(strSectionName, "Host", "");
		n_port = m_pReader->getint(strSectionName, "Port", 0);
		n_mode = m_pReader->getint(strSectionName, "Mode", 0);

		sockaddr_in serverAddr;
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);


		Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (Socket == INVALID_SOCKET) {
			int err = WSAGetLastError();
			printf("TCPConMan::TCPConMan socket failed: %d\n", err);
			WSACleanup();
			return;
		}

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(n_port);

		if (n_mode == Mode::Receiver)
		{

			serverAddr.sin_addr.s_addr = INADDR_ANY;

			if (bind(Socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
				int err = WSAGetLastError();
				printf("TCPConMan::TCPConMan bind failed: %d\n", err);
				closesocket(Socket);
				WSACleanup();
				return;
			}

			if (listen(Socket, 1) == SOCKET_ERROR) {
				int err = WSAGetLastError();
				printf("TCPConMan::TCPConMan  listen failed: %d\n", err);
				closesocket(Socket);
				WSACleanup();
				return;
			}
		}
		else
		{
			//Mode::Sender
			inet_pton(AF_INET, str_ip.c_str(), &serverAddr.sin_addr);

			if (connect(Socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {

				int err = WSAGetLastError();
				printf("TCPConMan::TCPConMan connect failed: %d\n", err);
				closesocket(Socket);
				WSACleanup();
				return;
			}

		}
		m_bConnected = 1;
	}

	void start() {
		try {
			do {
				printf(" Waiting for Sender connection\n");
				clientSocket = accept(Socket, (struct sockaddr*)&clientAddr, &clientAddrSize);
				if (clientSocket == INVALID_SOCKET) {
					int err = WSAGetLastError();
					printf("TCPConMan::TCPConMan accept failed: %d\n", err);
					m_bConnected = 0;
					return;
				}

				receiveMessage();
			} while (m_Exit != 1);
		}

		catch (const std::exception& e) {
			std::cout << " a standard exception was caught, with message: '"
				<< e.what() << "'\n";

		}

	}

	void receiveMessage() {

		int recvbuflen = 512;
		char recvbuf[512];
		int iResult;
		printf(" Connection received.\n");
		m_bConnected = 2;

		do {
			iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				LineData oData(iResult, (uint8_t*)recvbuf);
				if (iResult == 4 && (recvbuf[0] == 'E' || recvbuf[0] == 'e') &&
					(recvbuf[1] == 'X' || recvbuf[1]) == 'x' &&
					(recvbuf[2] == 'I' || recvbuf[2]) == 'i' &&
					(recvbuf[0] == 'T' || recvbuf[0] == 't')) {
					oData.flag = 0xFE;
				}

				if(pSharevec)
					pSharevec->push_back(std::move(oData));
				else
					printf("TCPConMan::TCPConMan No shared Q is provided.\n");
			}
			else if (iResult == 0) {
				m_bConnected = 1;
				printf("Connection closed\n");
			}
			else {
				int err = WSAGetLastError();
				printf("TCPConMan::TCPConMan recv failed: %d\n", err);
			}
		} while (iResult > 0 && m_Exit != 1);
	}

    void sendMessage(const uint8_t* message, int sz) {
        send(Socket, (char*)message, sz, 0);
    }

    ~TCPConMan() {
        closesocket(Socket); 
        closesocket(clientSocket);
        WSACleanup();
    }
};


