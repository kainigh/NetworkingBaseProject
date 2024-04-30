#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>
#include <raylib.h>

using namespace std;

struct Message
{
	bool fromMe = false;
	string content;
};


int main(int argc, char* argv[]) {

	char hostIP[15] = "\0";
	int port[5];

	int letterCount = 0;

	const int popupwidth = 500, popupheight = 250;
	InitWindow(popupwidth, popupheight, "My first chat window!");
	SetTargetFPS(60);

	Rectangle textBox = { 20, 50, popupwidth - 40, 50 };
	bool mouseOnText = false;

	if (SDLNet_Init() == -1) {
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}

	
	while (!WindowShouldClose()) {

		if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
		else mouseOnText = false;

		if (mouseOnText)
		{
			// Set the window's cursor to the I-Beam
			SetMouseCursor(MOUSE_CURSOR_IBEAM);

			// Get char pressed (unicode character) on the queue
			int key = GetCharPressed();

			while (key > 0)
			{
				// NOTE: Only allow keys in range [32..125]
				if ((key >= 32) && (key <= 125))
				{
					hostIP[letterCount] = (char)key;
					hostIP[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
					letterCount++;
				}

				key = GetCharPressed();  // Check next character in the queue
			}

			if (IsKeyPressed(KEY_BACKSPACE))
			{
				letterCount--;
				if (letterCount < 0) letterCount = 0;
				hostIP[letterCount] = '\0';
			}


		}
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);

		DrawText("Host IP: ", 20, 30, 20, WHITE);
		DrawRectangleRec(textBox, DARKGRAY);

		DrawText(hostIP, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

		DrawText("Host Port Number: ", 20, 125, 20, WHITE);
		DrawRectangle(20, 150, popupwidth - 40, 50, DARKGRAY);

		//DrawRectangle(20, popupheight - 90, popupwidth - 40, 50, LIGHTGRAY);

		EndDrawing();
	}
	CloseWindow();

	std::cout << "Please enter the IP address: ";

	string host;
	cin >> host;

	cout << endl << "Please enter the Port number: ";

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

	
	
	//string message = "Hi there....Il faut beau";

	const int width = 500, height = 750;
	InitWindow(width, height, "My first chat window!");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);
		DrawRectangle(20, 50, width - 40, height - 150, DARKGRAY);
		DrawRectangle(20, height - 90, width - 40, 50, LIGHTGRAY);

		EndDrawing();
	}
	CloseWindow();

	string message;
	cout << "Type your message here: ";

	//getline(cin, message);

	cin >> message;


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
