/*
    Simple udp client    
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>
#include <string>
#include <iostream>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

using namespace std;

int main(void) {
	string mg = std::to_string(1);
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
	int recv_len = 1;
    char buf[BUFLEN];
	string message;
    WSADATA wsa;
	bool makingPlay = true;
	string temp_string = "";

	bool col = false;
	bool row = false;

	bool gameover = false;

	string servidor;

	bool turn = true;

	char myid[BUFLEN];
 
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

	cout << "Ingresar ID para juego: ";
	cin >> myid;

	cout << "Insertar IP de servidor: ";
	cin >> servidor;
     
    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(servidor.c_str());
     
	if (sendto(s, myid, strlen(myid), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR) {
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	cout << endl << "Esperando..." << endl << endl;
	//while (!startgame) {
	//	memset(buf, '\0', BUFLEN);
	//	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR) {
	//		printf("recvfrom() failed with error code : %d", WSAGetLastError());
	//		//exit(EXIT_FAILURE);
	//	}

	//	if (recvfrom > 0)
	//		startgame = true;
	//}

    //start communication
	while (!gameover) {
			
			if (!gameover){

				memset(buf, '\0', BUFLEN);
				if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR) {
					printf("recvfrom() failed with error code : %d", WSAGetLastError());
					//exit(EXIT_FAILURE);
				}

				puts(buf);
			}

			if (buf[0] == 'T' && !gameover) {
				fflush(stdin);
				gameover = true;
			}else if (buf[0] != 'T' && !gameover && buf[0] != 'F') {
				turn = true;
				fflush(stdin);
				message = "";
				printf("Input: ");
				getline(cin, message);

				if (sendto(s, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR) {
					printf("recvfrom() failed with error code : %d", WSAGetLastError());
					gameover = true;
					//exit(EXIT_FAILURE);
				}

				memset(buf, '\0', BUFLEN);
				if (recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR) {
					printf("recvfrom() failed with error code : %d", WSAGetLastError());
					gameover = true;
					//exit(EXIT_FAILURE);
				}

				if (recv_len > 0)
					puts(buf);
			}else if (buf[0] == 'F'){
				if (turn) {
					cout << endl << "Fin del turno. Esperando..." << endl << endl;
					turn = false;
				}
			}
    }
 
	cout << endl << "Conexion terminada. Presionar cualquier tecla para terminar." << endl;
	cin.get();

    closesocket(s);
    WSACleanup();
 
    return 0;
}