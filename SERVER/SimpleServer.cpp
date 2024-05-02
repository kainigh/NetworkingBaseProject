#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;

struct Message
{
	bool fromMe = false;
	string content;
};




int main(int argc, char* argv[]) {

	vector<Message> log{Message{false, "Waiting for someone to talk to..."}};

	SDLNet_SocketSet set = SDLNet_AllocSocketSet(8);

	if (SDLNet_Init() == -1) {
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}

	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, nullptr, 4242) == -1) {
		cerr << "Resolve Host error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	TCPsocket serverSocket = SDLNet_TCP_Open(&ip);
	if (!serverSocket) {
		cerr << "TCP Open error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	/*const int width = 500, height = 750;
	InitWindow(width, height, "Server chat window!");
	SetTargetFPS(60);

	string typing;*/
	//while (!WindowShouldClose()) {
	//	BeginDrawing();
	//	ClearBackground(GRAY);
	//	DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);
	//	DrawRectangle(20, 50, width - 40, height - 150, DARKGRAY);

	//	for (int msg = 0; msg < log.size(); msg++)
	//	{
	//		DrawText(log[msg].content.c_str(), 30, 75 + (msg * 30), 15, log[msg].fromMe ? SKYBLUE : PURPLE);	
	//	}


	//	DrawRectangle(20, height - 90, width - 40, 50, LIGHTGRAY);

	//	int inputChar = GetCharPressed();
	//	if (inputChar != 0) //A character is pressed on the keyboard
	//	{
	//		typing += static_cast<char>(inputChar);
	//	}
	//	if (typing.size() > 0)
	//	{
	//		if (IsKeyPressed(KEY_BACKSPACE)) typing.pop_back();
	//		else if (IsKeyPressed(KEY_ENTER))
	//		{
	//			//Send the message typing to the server here!
	//			log.push_back(Message{ true, typing });
	//			typing.clear();
	//		}

	//		DrawText(typing.c_str(), 30, height - 75, 25, DARKBLUE);
	//	}
	//	EndDrawing();
	
	const int width = 500, height = 750;
	InitWindow(width, height, "Server chat window!");
	SetTargetFPS(60);

		TCPsocket clientSocket;
		while (true) {


			clientSocket = SDLNet_TCP_Accept(serverSocket);

			if (clientSocket) {

				std::cout << "A client reached the server!" << endl;
				SDLNet_AddSocket(set, reinterpret_cast<SDLNet_GenericSocket>(clientSocket));
				break;
			}
		}
			string typing;

			while (!WindowShouldClose()) {
				BeginDrawing();
				ClearBackground(GRAY);
				DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);
				DrawRectangle(20, 50, width - 40, height - 150, DARKGRAY);
				if (SDLNet_CheckSockets(set, 0) != 0) {
					char buffer[1024];
					int bytesRead = SDLNet_TCP_Recv(clientSocket, buffer, sizeof(buffer));
					if (bytesRead > 0) {
						std::cout << "Incoming message: " << buffer << endl;

						log.push_back(Message{ false, buffer });
					}
				}

				for (int msg = 0; msg < log.size(); msg++)
				{
					DrawText(log[msg].content.c_str(), 30, 75 + (msg * 30), 15, log[msg].fromMe ? SKYBLUE : PURPLE);
				}


				DrawRectangle(20, height - 90, width - 40, 50, LIGHTGRAY);

				int inputChar = GetCharPressed();
				if (inputChar != 0) //A character is pressed on the keyboard
				{
					typing += static_cast<char>(inputChar);
				}
				if (typing.size() > 0)
				{
					if (IsKeyPressed(KEY_BACKSPACE)) typing.pop_back();
					else if (IsKeyPressed(KEY_ENTER))
					{
						//Send the message typing to the server here!
						log.push_back(Message{ true, typing });
						SDLNet_TCP_Send(clientSocket, typing.c_str(), typing.length() + 1);
						typing.clear();
					}

					DrawText(typing.c_str(), 30, height - 75, 25, DARKBLUE);
				}
				EndDrawing();

	}
	CloseWindow();




	std::cout << "Thank you for using ChArtFX !\n";
	return 0;
}
