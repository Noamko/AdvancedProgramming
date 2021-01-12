/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <pthread.h>
#include <thread>
#include <signal.h>
#include "CLI.h"
#include <sstream>

#define BUFFER_SIZE 2048

using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO : public DefaultIO {
	int client;
public:
	SocketIO(int client) : client(client) {}
	virtual string read() {
		char buffer[1];
		string str;
		int bytes_received = recv(client, buffer, 1, 0);
		if (bytes_received > 0) {
			char c = buffer[0];
			while (c != '\n') {
					str+=c;
					bytes_received = recv(client, buffer, 1, 0);
					c = buffer[0];
			}
		}
		return str;
	}
	virtual void write(string text) {
		send(client, text.c_str() ,strlen(text.c_str()), 0);
	}
	virtual void writeLine(string text) {
		write(text);
		write("\n");
	}
	virtual void writeLine(float f) {
		write(f);
		write("\n");
	}
	virtual void writeLine() {
		write("\n");
	}
	virtual void write(float f) {
		std::ostringstream ss;
		ss << f;
		string f_string(ss.str());
		send(client, f_string.c_str(), f_string.size(), 0);
	}
	virtual void read(float* f) {
		char buffer[1024];
		int bytes_received = recv(client, buffer, 100, 0);
		*f = atof(buffer);
	}
	virtual ~SocketIO() {}
};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		SocketIO* io = new SocketIO(clientID);
		CLI* cli = new CLI(io);
		cli->start();
		delete io;
		delete cli;
    }
};

class Server {
	thread* t ; 
	int server_socket, client_socket, port;
	int stop_loop = 0;
	struct sockaddr_in serverAdress;
	char buffer[BUFFER_SIZE];

public:
	Server(int port);
	virtual ~Server();
	void start(ClientHandler& ch);
	void init(int port);
	void server_loop();
	void stop();
};

#endif /* SERVER_H_ */
