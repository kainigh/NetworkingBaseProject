#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>
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

	char hostIP[15] = "\0";
	char port[15] = "\0";

	int IPletterCount = 0;
	int PortletterCount = 0;
	string strIP;
	string strPort;

	const int popupwidth = 500, popupheight = 300;
	InitWindow(popupwidth, popupheight, "Client chat window!");
	SetTargetFPS(60);

	bool btnAction = false;

	Rectangle btnBounds = { 200, 220, 130, 50 };
	Rectangle IPtextBox = { 20, 50, popupwidth - 40, 50 };
	Rectangle PorttextBox = { 20, 150, popupwidth - 40, 50 };
	bool mouseOnText = false;

	if (SDLNet_Init() == -1) {
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}

	
	while (!WindowShouldClose()) {

		if (CheckCollisionPointRec(GetMousePosition(), IPtextBox) || CheckCollisionPointRec(GetMousePosition(), PorttextBox)) 
			mouseOnText = true;
		else 
			mouseOnText = false;

	
		

		if (mouseOnText)
		{
			btnAction = false;

			// Set the window's cursor to the I-Beam
			SetMouseCursor(MOUSE_CURSOR_IBEAM);

			// Get char pressed (unicode character) on the queue
			int key = GetCharPressed();

			while (key > 0)
			{
				// NOTE: Only allow keys in range [32..125]
				if ((key >= 32) && (key <= 125) && CheckCollisionPointRec(GetMousePosition(), IPtextBox))
				{
					hostIP[IPletterCount] = (char)key;
					hostIP[IPletterCount + 1] = '\0'; // Add null terminator at the end of the string.
					IPletterCount++;
				}

				else if ((key >= 32) && (key <= 125) && CheckCollisionPointRec(GetMousePosition(), PorttextBox))
				{
					port[PortletterCount] = (char)key;
					port[PortletterCount + 1] = '\0'; // Add null terminator at the end of the string.
					PortletterCount++;
				}

				key = GetCharPressed();  // Check next character in the queue
				

			}

			if (IsKeyPressed(KEY_BACKSPACE) && CheckCollisionPointRec(GetMousePosition(), IPtextBox))
			{
				IPletterCount--;
				if (IPletterCount < 0) IPletterCount = 0;
				hostIP[IPletterCount] = '\0';
			}

			if (IsKeyPressed(KEY_BACKSPACE) && CheckCollisionPointRec(GetMousePosition(), PorttextBox))
			{
				PortletterCount--;
				if (PortletterCount < 0) PortletterCount = 0;
				port[PortletterCount] = '\0';
			}


		}
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);

		DrawText("Host IP: ", 20, 30, 20, WHITE);
		DrawRectangleRec(IPtextBox, DARKGRAY);
		DrawText(hostIP, (int)IPtextBox.x + 5, (int)IPtextBox.y + 8, 40, MAROON);

		DrawText("Host Port Number: ", 20, 125, 20, WHITE);
		DrawRectangleRec(PorttextBox, DARKGRAY);
		DrawText(port, (int)PorttextBox.x + 5, (int)PorttextBox.y + 8, 40, MAROON);

		DrawRectangleRec(btnBounds, WHITE); // Draw button frame
		DrawText("Connect", btnBounds.x + 5, btnBounds.y + 5, 30, BLACK);

		//DrawRectangle(20, popupheight - 90, popupwidth - 40, 50, LIGHTGRAY);

		EndDrawing();

		if (CheckCollisionPointRec(GetMousePosition(), btnBounds))
		{
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				cout << "true" << endl;
				btnAction = true;

			}

		}

		if (btnAction)
		{

			//cout << "Connecting" << endl;
			// TODO: Any desired action
			strIP = hostIP;
			strPort = port;


			cout << strIP << "  " << port << endl;
			CloseWindow();

		}

	}
	
	


	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, strIP.c_str(), stoi(strPort)) == -1) {
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
	SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
	SDLNet_AddSocket(set, reinterpret_cast<SDLNet_GenericSocket>(clientSocket));
	
	const int width = 500, height = 750;
	InitWindow(width, height, "Client chat window!");
	SetTargetFPS(60);
	string typing;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("Welcome to ChArtFX!", 220, 15, 25, WHITE);
		DrawRectangle(20, 50, width - 40, height - 150, DARKGRAY);
		//DrawRectangle(20, height - 90, width - 40, 50, LIGHTGRAY);

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
				int bytesSent = SDLNet_TCP_Send(clientSocket, typing.c_str(), typing.length() + 1);
				if (bytesSent < typing.length() + 1) {
					cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
					SDLNet_TCP_Close(clientSocket);
					SDLNet_Quit();
					return 1;
				}

				typing.clear();
			}

			DrawText(typing.c_str(), 30, height - 75, 25, DARKBLUE);
		}

		

		EndDrawing();
	}
	CloseWindow();
	//string message = "Hi there";
	


	/*int bytesSent = SDLNet_TCP_Send(clientSocket, typing.c_str(), typing.length() + 1);
	if (bytesSent < typing.length() + 1) {
		cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
		SDLNet_TCP_Close(clientSocket);
		SDLNet_Quit();
		return 1;
	}*/

	//cout << "Sent " << bytesSent << " bytes to the server !" << std::endl;
	//SDLNet_TCP_Close(clientSocket);
	//SDLNet_Quit();

	
	//cout << "Incoming response: " << buffer << endl;



	cout << "Thank you for using ChArtFX !\n";
	return 0;
}
