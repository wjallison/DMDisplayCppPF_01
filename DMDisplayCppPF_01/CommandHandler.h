#pragma once
#include <list>
#include <iterator>
#include "Character.h"
#include "GameControl.h"
#include "NPCFactory.h"
#include "RollManager.h"
#include <sstream>
#include <iostream>
#include <fstream>


using namespace std;
class CommandHandler
{
public:
	list<NPC*>* npcList;/* = new list<NPC>();*/
	NPCFactory* factory;
	GameControl* game;
	RollManager* rollManager;
	PC* pcs;


	CommandHandler();
	CommandHandler(list<NPC*>* _npcList, NPCFactory* _factory, GameControl* _game, RollManager* _rollManager, PC* _pcs);

	void UpdateDiceScreen();

	void ReceiveCommand(string cmd);

	void ShowCommand(int ind);

	void AddNPCCommand(string type, string name);

	void ClearNPCs();

	void SaveChars();

	//void SaveString

	void LoadChars();

	void ExportNPCs();
};

