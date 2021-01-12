
#include "Server.h"

Server::Server(int port) {
	this->port = port;
}

void Server::init(int port){
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0){
		// This exception parses ERRNO for us and explains what the error is
		throw("Couldn't open a new socket");
	}
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_addr.s_addr = htons(INADDR_ANY);
	serverAdress.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr*)&serverAdress, sizeof(serverAdress)) < 0) {
		throw("=> Error binding connection, the socket has already been established...");
	}

}

void Server::start(ClientHandler& ch){
		t = new thread([&ch,this]() {
			this->init(this->port);
			listen(server_socket, 5);
			while (!stop_loop)
			{
				socklen_t size = sizeof(serverAdress);
				client_socket = accept(server_socket, (struct sockaddr*)&serverAdress, &size);
				cout << client_socket << " Connected.\n";
				ch.handle(client_socket);
				close(client_socket);
				sleep(1);
			}
		});
}

void Server::stop(){
	this->stop_loop = 1;
	t->join(); // do not delete this!
}

Server::~Server() {
}

