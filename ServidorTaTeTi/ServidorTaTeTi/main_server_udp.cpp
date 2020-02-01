/*
Simple UDP Server
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>
#include "TaTeTi.h"
#include <string>
#include <iostream>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

string DrawGrid(string a, string s, TaTeTi t) {

	string res = "";

	for (int i = 0; i < COLS; i++) {
		for (int j = 0; j < ROWS; j++) {
			switch (t.GetPoint(j, i)) {
			case 0:
				res += " ";
				break;

			case 1:
				res += "X";
				break;

			case 2:
				res += "O";
				break;
			}
			if (j == 2) {
				res += "\n";
				if (i != 2)
					res += s + "\n";
			}
			else {
				res += "|";
			}
		}
	}

	return res;
}

int main() {
	SOCKET s;
	sockaddr_in p1, p2;
	sockaddr_in curaddr;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	//Tic Tac Toe variables
	bool endProg = false;
	TaTeTi tt;
	string toAdd = " ";
	string space = "------";
	char pick = '0';
	int colPick = 0;
	int rowPick = 0;
	char gotmsg;
	string temp_string = "1";

	bool cPicked = false;
	bool rPicked = false;
	bool errorCol = false;
	bool errorRow = false;

	slen = sizeof(si_other);

	bool player1 = false;
	string p1id = "";
	bool player2 = false;
	string p2id = "";

	string curID = "";

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//while (!player1 && !player2){

	//clear the buffer by filling null, it might have previously received data
	memset(buf, '\0', BUFLEN);

	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR) {
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	if (recv_len > 0 && !player1){
		p1id = buf;
		curID = p1id;
		p1 = si_other;
		player1 = true;
	}

	//clear the buffer by filling null, it might have previously received data
	memset(buf, '\0', BUFLEN);

	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR) {
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	if (recv_len > 0 && !player2){
		p2id = buf;
		curID = p1id;
		p2 = si_other;
		player2 = true;
	}

	recv_len = 0;

	curaddr = p1;

	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
	//printf("Data: %s\n" , buf);

	//keep listening for data
	while (player1 && player2) {

		//try to receive some data, this is a blocking call

		if (errorCol) {
			temp_string = "Columna invalida. Elegir Columna (1, 2, 3): \n";
			errorCol = false;
			cPicked = false;
		}
		else if (errorRow) {
			temp_string = "Fila invalida. Elegir Fila (1, 2, 3): \n";
			errorRow = false;
			rPicked = false;
		}
		else if (!errorCol && !errorRow && !cPicked && !rPicked) {
			temp_string = DrawGrid(toAdd, space, tt) + "\n" + "Elegir Columna (1, 2, 3): \n";
		}
		else if (!errorCol && !errorRow && cPicked && !rPicked) {
			temp_string = "Elegir Fila (1, 2, 3): \n";
		}
		else if (!errorCol && !errorRow && cPicked && rPicked) {
			bool move = true;
			if (curID == p1id) {
				move = tt.SetPoint(colPick - 1, rowPick - 1, 1);
			}
			else{
				move = tt.SetPoint(colPick - 1, rowPick - 1, 2);
			}

			if (move) {
				if (curID == p1id)
					temp_string = p1id + " hizo jugada: " + to_string(colPick) + ", " + to_string(rowPick) + ". Confirmada.\n" + DrawGrid(toAdd, space, tt) + "\n" + "Elegir Columna (1, 2, 3): \n";
				else
					temp_string = p2id + " hizo jugada: " + to_string(colPick) + ", " + to_string(rowPick) + ". Confirmada.\n" + DrawGrid(toAdd, space, tt) + "\n" + "Elegir Columna (1, 2, 3): \n";

				if (!tt.CheckVictory()) {
					if (curID == p1id) {
						curaddr = p2;
						curID = p2id;
					}
					else {
						curaddr = p1;
						curID = p1id;
					}
				}
			}
			else {
				if (curID == p1id)
					temp_string = "Jugada realizada: " + to_string(colPick) + ", " + to_string(rowPick) + ". Espacio ocupado.\n" + DrawGrid(toAdd, space, tt) + "\n" + "Elegir Columna (1, 2, 3): \n";
				else
					temp_string = "Jugada realizada: " + to_string(colPick) + ", " + to_string(rowPick) + ". Espacio ocupado.\n" + DrawGrid(toAdd, space, tt) + "\n" + "Elegir Columna (1, 2, 3): \n";
			}

			cPicked = false;
			rPicked = false;
			recv_len = 0;
		}

		if (tt.CheckVictory()) {
			if (curID == p1id)
				temp_string = "Ta Te Ti! \n\n" + DrawGrid(toAdd, space, tt) + "\n" + "Ganador: " + p1id + "\n" + "Perdedor: " + p2id + "\n";
			else
				temp_string = "Ta Te Ti! \n\n" + DrawGrid(toAdd, space, tt) + "\n" + "Ganador: " + p2id + "\n" + "Perdedor: " + p1id + "\n";
			if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &p1, slen) == SOCKET_ERROR) {
				printf("sendto() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &p2, slen) == SOCKET_ERROR) {
				printf("sendto() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			closesocket(s);
			WSACleanup();
			return 0;
		}
		else {

			if (tt.CheckTie()) {
				if (curID == p1id)
					temp_string = "Termina en empate. \n\n" + DrawGrid(toAdd, space, tt) + "\n";
				else
					temp_string = "Termina en empate. \n\n" + DrawGrid(toAdd, space, tt) + "\n";
				if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &p1, slen) == SOCKET_ERROR) {
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &p2, slen) == SOCKET_ERROR) {
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				closesocket(s);
				WSACleanup();
				return 0;
			}

			if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
				printf("sendto() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			printf("Waiting for data...");
			fflush(stdout);

			//clear the buffer by filling null, it might have previously received data
			memset(buf, '\0', BUFLEN);

			//try to receive some data, this is a blocking call
			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR) {
				printf("recvfrom() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			if (recv_len > 0) {
				if (sendto(s, DrawGrid(toAdd, space, tt).c_str(), strlen(DrawGrid(toAdd, space, tt).c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				if (!rPicked){
					if (!cPicked){
						gotmsg = buf[0];
						if (!strcmp(buf, "1"))	{
							colPick = 1;
							temp_string = "1";
							printf(temp_string.c_str());
							cPicked = true;
						}
						else if (!strcmp(buf, "2"))	{
							colPick = 2;
							temp_string = "2";
							printf(temp_string.c_str());
							cPicked = true;
						}
						else if (!strcmp(buf, "3"))	{
							colPick = 3;
							temp_string = "3";
							printf(temp_string.c_str());
							cPicked = true;
						}
						else{
							cPicked = false;
							rPicked = false;
							errorCol = true;
						}

						recv_len = 0;
					}
					else if (cPicked){
						gotmsg = buf[0];
						if (!strcmp(buf, "1"))	{
							rowPick = 1;
							temp_string = "1";
							printf(temp_string.c_str());
							rPicked = true;
							temp_string = "Fin del turno. \n";
							if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
								printf("sendto() failed with error code : %d", WSAGetLastError());
								exit(EXIT_FAILURE);
							}
						}
						else if (!strcmp(buf, "2"))	{
							rowPick = 2;
							temp_string = "2";
							printf(temp_string.c_str());
							rPicked = true;
							temp_string = "Fin del turno. \n";
							if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
								printf("sendto() failed with error code : %d", WSAGetLastError());
								exit(EXIT_FAILURE);
							}
						}
						else if (!strcmp(buf, "3"))	{
							rowPick = 3;
							temp_string = "3";
							printf(temp_string.c_str());
							rPicked = true;
							temp_string = "Fin del turno. \n";
							if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
								printf("sendto() failed with error code : %d", WSAGetLastError());
								exit(EXIT_FAILURE);
							}
						}
						else{
							rPicked = false;
							errorRow = true;
						}
					}
				}
			}else{
				cPicked = false;
				rPicked = false;
				temp_string = "Jugada invalida. Repetir. \n";
				if (sendto(s, temp_string.c_str(), strlen(temp_string.c_str()), 0, (struct sockaddr*) &curaddr, slen) == SOCKET_ERROR) {
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}