// DMDisplayCppPF_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include <list>
#include "GameControl.h"
#include "RollManager.h"
#include "FileWatcher.h"
#include "NPCFactory.h"
#include "CommandHandler.h"

using namespace std;

void ClearLastLine(int cols, int rows) {
	COORD coord;
	coord.X = 0;
	coord.Y = rows - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	string s = "";
	for (int i = 0; i < cols; i++) {
		s += " ";
	}
	cout << s;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void UpdateDiceRoller(RollManager rm, SevenSegment ones, SevenSegment tens) {
	tens.Update(rm.result / 10);
	ones.Update(rm.result % 10);
}

void UpdateFromPC(PC pc, PCDisplay display) {

}

NPCDisplay* AddNPC(NPC* npc, int index) {
	NPCDisplay* ret = new NPCDisplay(Vector2(82, index * 4 + 2), Vector2(38, 4), 18, npc);
	return ret;
}

//string ReadLineFromFile
void ReadToCmd(string path, CommandHandler* cmd) {
	std::ifstream infile(path);
	string line;
	while (std::getline(infile, line)) {
		//std::istringstream iss(line);
		cmd->ReceiveCommand(line);
	}
}

int main()
{
	//CreateProcess()
	//AllocConsole();
	//system("start \"Help\" cmd /C \"echo off & echo Help text 1 & echo Help text 2 & echo Help text 3 & pause\"");


	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	//ClearLastLine(columns, rows);
#pragma region Init

	
	
	FileWatcher fw{ "./", std::chrono::milliseconds(5000) };

	NPCFactory npcFactory = NPCFactory();

	GameControl game = GameControl(Vector2(0, 0), Vector2(120, 29), 7);
	//GameControl game = GameControl(Vector2(0, 0), Vector2(60, 15), 196);
	RollManager rollMgr = RollManager();

	//PCs
	BorderBox pcBox = BorderBox(Vector2(0, 0), Vector2(40, 29), 18, '*');
	pcBox.name = "pcBox";
	PC pcs[3];

	BorderBox pc1Box = BorderBox(Vector2(1, 1), Vector2(38, 9), 18, '#');
	pc1Box.background2 = 18;
	pc1Box.bkchr2 = '!';
	pc1Box.name = "pc1Box";
	pcs[0] = PC("Myatt");
	PCDisplay pcd1 = PCDisplay(Vector2(2, 2), Vector2(36, 7), 0, &pcs[0]);
	pcd1.name = "pcd1";
	pc1Box.AddChild(&pcd1);
	pcBox.AddChild(&pc1Box);
	game.AddChild(&pcBox);

	BorderBox pc2Box = BorderBox(Vector2(1, 10), Vector2(38, 9), 18, '#');
	pc2Box.background2 = 18;
	pc2Box.bkchr2 = '!';
	pc2Box.name = "pc2Box";
	pcs[1] = PC("Al");
	PCDisplay pcd2 = PCDisplay(Vector2(2, 11), Vector2(36, 7), 0, &pcs[1]);
	pcd2.name = "pcd2";
	pc2Box.AddChild(&pcd2);
	pcBox.AddChild(&pc2Box);
	
	BorderBox pc3Box = BorderBox(Vector2(1, 19), Vector2(38, 9), 18, '#');
	pc3Box.background2 = 18;
	pc3Box.bkchr2 = '!';
	pc3Box.name = "pc3Box";
	pcs[2] = PC("Sauron");
	PCDisplay pcd3 = PCDisplay(Vector2(2, 20), Vector2(36, 7), 0, &pcs[2]);
	pcd3.name = "pcd3";
	pc3Box.AddChild(&pcd3);
	pcBox.AddChild(&pc3Box);



	//NPCs
	list<NPC*> npcs = list<NPC*>();
	BorderBox npcBox = BorderBox(Vector2(80, 0), Vector2(40, 29), 18, '*');
	npcBox.name = "npcBox";
	game.AddChild(&npcBox);




	//Dice Rolling Area
	BorderBox diceBox = BorderBox(Vector2(50, 4), Vector2(20, 20), 18, 'X');
	SevenSegment tens = SevenSegment(Vector2(54, 10), Vector2(5, 7), 18);
	tens.name = "tens";
	tens.Update(8);
	//tens.HookEvent(&rollMgr, 10);
	//rollMgr.AssignTens(&tens);
	SevenSegment ones = SevenSegment(Vector2(61, 10), Vector2(5, 7), 18);
	ones.name = "ones";
	ones.Update(8);
	//rollMgr.AssignOnes(&ones);
	//ones.HookEvent(&rollMgr, 1);
	TextControl math = TextControl(Vector2(50, 24), Vector2(20, 2), 18, rollMgr.math);
	math.name = "math";
	diceBox.AddChild(&tens);
	diceBox.AddChild(&ones);
	game.AddChild(&diceBox);
	game.AddChild(&math);

	CommandHandler command = CommandHandler(&npcs, &npcFactory, &game, &rollMgr, &pcs[0]);
#pragma endregion

#pragma region Load

	std::ifstream archetypesFile("Archetypes.txt");
	string line;
	while (std::getline(archetypesFile, line)) {
		//std::istringstream iss(line);
		npcFactory.Add(line);
	}
	//myFile.open("Archetypes.txt");

#pragma endregion

	//game.DrawChar(5, 5, ' ', 235);
	
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//// you can loop k higher to see more color choices
	//for (int k = 1; k < 255; k++)
	//{
	//	// pick the colorattribute k you want
	//	SetConsoleTextAttribute(hConsole, k);
	//	cout << k << " I want to be nice today!" << endl;
	//}

    //std::cout << "Hello World!\n";
	npcBox.AddChild(AddNPC(npcFactory.Create("Goblin", "Test Goblin"),0));
	game.Draw();
	//game.DrawChar(0, 0, 'l', 18);
	//std::cin.ignore();

	//command.ReceiveCommand("roll pc hit 1");
	//game.Draw();
	fw.Start([&game, &command](std::string pathToWatch, FileStatus status)->void {
		// Process only regular files, all other file types are ignored
		if (!std::experimental::filesystem::is_regular_file(std::experimental::filesystem::path(pathToWatch)) && status != FileStatus::erased) {
			return;
		}

		//bool test = false;

		switch (status)
		{
		case FileStatus::created:
			//test = true;
			break;
		case FileStatus::modified:
			//test = true;
			//std::ifstream cmdFile(pathToWatch);
			//ClearLastLine(1, 1);
			ReadToCmd(pathToWatch, &command);

			break;
		case FileStatus::erased:

			break;
		default:
			break;
		}

		game.Draw();
		//game.DrawChar(0, 0, 'l', 18);
		});

	//while (true) {
	//	//game.Draw();
	//	//cin.ignore();
	//	//rollMgr.Roll20();
	//	//pcd1.Update(pcs[0]);
	//	//tens.Update(rollMgr.result / 10);
	//	//ones.Update(rollMgr.result % 10);
	//	game.Draw();


	//	string order, com = "", sub = "";
	//	//, subj = "";
	//	list<string> subj = list<string>();
	//	cin >> order;
	//	
	//	//com = order[0] + order[1];
	//	//auto iter;
	//	//int ind = 0;
	//	//for (auto iter = order.cbegin(); iter != order.cend(); iter++) {
	//	//	/*if (ind < 2) {
	//	//		continue;
	//	//	}*/
	//	//	if (*iter == ' ') {
	//	//		if (ind == 0) {
	//	//			com = sub;
	//	//			++ind;
	//	//		}
	//	//		else {
	//	//			subj.push_back(sub);
	//	//			sub = "";
	//	//		}
	//	//		}
	//	//	else { sub += *iter; }
	//	//}
	//	///*for (int i = 3; i < sizeof(order); i++) {
	//	//	subj += order[i];
	//	//}*/
	//	//if (com == "ct") {

	//	//}
	//	//else if (com == "le") {

	//	//}
	//	//else {

	//	//}

	//	ClearLastLine(columns, rows);
	//}

	//Example of drawing sine wave
	//HWND myconsole = GetConsoleWindow();
	////Get a handle to device context
	//HDC mydc = GetDC(myconsole);

	//int pixel = 0;

	////Choose any color
	//COLORREF COLOR = RGB(255, 255, 255);

	////Draw pixels
	//for (double i = 0; i < 3.1416 * 4; i += 0.05)
	//{
	//	SetPixel(mydc, pixel, (int)(50 + 25 * cos(i)), COLOR);
	//	pixel += 1;
	//}

	//ReleaseDC(myconsole, mydc);

	int hold;
	std::cin >> hold;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
