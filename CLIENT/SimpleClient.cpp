#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
//#include <string>
using namespace std;

int main(int argc, char* argv[]) {

	if (SDLNet_Init() == -1) {
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}

	std::cout << "Please enter the IP address: ";

	string host;
	cin >> host;

	std::cout << endl << "Please enter the Port number: ";

	int portnumber;
	cin >> portnumber;

	cout << endl;



	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, host.c_str(), portnumber) == -1) {
		cerr << "Resolve Host error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	TCPsocket clientSocket = SDLNet_TCP_Open(&ip);
	if (!clientSocket) {
		cerr << "TCP Open error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	cout << "Type your message here: ";

	string message;
	cin >> message;


	//string message = "Hi there....Il faut beau";

	int bytesSent = SDLNet_TCP_Send(clientSocket, message.c_str(), message.length() + 1);
	if (bytesSent < message.length() + 1) {
		cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
		SDLNet_TCP_Close(clientSocket);
		SDLNet_Quit();
		return 1;
	}

	cout << "Sent " << bytesSent << " bytes to the server !" << std::endl;
	//SDLNet_TCP_Close(clientSocket);
	//SDLNet_Quit();

	char buffer[1024];
	int bytesRead = SDLNet_TCP_Recv(clientSocket, buffer, sizeof(buffer));
	if (bytesRead <= 0) {
		cerr << "SDLNet TCP Recv error: " << SDLNet_GetError() << endl;
		SDLNet_TCP_Close(clientSocket);
		SDLNet_Quit();
		return 1;
	}

	cout << "Incoming response: " << buffer << endl;



	cout << "Thank you for using ChArtFX !\n";
	return 0;
}
