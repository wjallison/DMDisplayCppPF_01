#include "CommandHandler.h"


CommandHandler::CommandHandler() {

}

CommandHandler::CommandHandler(list<NPC*>* _npcList, NPCFactory* _factory, GameControl* _game, RollManager* _rollManager, PC* _pcs) {
	npcList = _npcList;
	factory = _factory;
	game = _game;
	rollManager = _rollManager;
	pcs = _pcs;
}

void CommandHandler::UpdateDiceScreen() {
	SevenSegment* s10 = dynamic_cast<SevenSegment*>(game->PullNamed("tens"));
	s10->Update(rollManager->result / 10);
	SevenSegment* s1 = dynamic_cast<SevenSegment*>(game->PullNamed("ones"));
	s1->Update(rollManager->result % 10);
	//game->PullNamed("tens")->Update(8)
	TextControl* m = dynamic_cast<TextControl*>(game->PullNamed("math"));
}

void CommandHandler::ReceiveCommand(string cmd) {
	list<string> pieces = list<string>();
	string builder = "";
	for (auto it = cmd.begin(); it != cmd.end(); ++it) {
		if (*it == ' ') {
			pieces.push_back(builder);
			builder = "";
		}
		else {
			builder += *it;
		}
	}
	pieces.push_back(builder);
	auto it2 = pieces.begin();

	/*
	Can be:
	roll npc hit 1
	for
	roll to hit for the first npc (1-based indexing)

	or
	roll 1 4 2
	for 
	roll 1d4+2
	*/
	if (*it2 == "roll") {
		++it2;
		if (*it2 == "pc") {
			++it2;
			if (*it2 == "hit") {
				++it2;
				stringstream ss(*it2);
				int ind = 0;
				ss >> ind;
				rollManager->RollToHit(*(pcs + ind));

				//SevenSegment* s10 = dynamic_cast<SevenSegment*>(game->PullNamed("tens"));
				//s10->Update(rollManager->result / 10);
				//SevenSegment* s1 = dynamic_cast<SevenSegment*>(game->PullNamed("ones"));
				//s1->Update(rollManager->result % 10);
				////game->PullNamed("tens")->Update(8)
				//TextControl* m = dynamic_cast<TextControl*>(game->PullNamed("math"));
				//m->text = rollManager->math;
				UpdateDiceScreen();
			}
			else if (*it2 == "atk") {
				stringstream ss(*it2);
				int ind = 0;
				ss >> ind;
				rollManager->RollAttack(*(pcs + ind));


				//SevenSegment* s10 = dynamic_cast<SevenSegment*>(game->PullNamed("tens"));
				//s10->Update(rollManager->result / 10);
				//SevenSegment* s1 = dynamic_cast<SevenSegment*>(game->PullNamed("ones"));
				//s1->Update(rollManager->result % 10);
				////game->PullNamed("tens")->Update(8)
				//TextControl* m = dynamic_cast<TextControl*>(game->PullNamed("math"));
				//m->text = rollManager->math;
				UpdateDiceScreen();
			}
			
		}
		else if (*it2 == "npc") {
			++it2;
			if (*it2 == "hit") {
				++it2;
				stringstream ss(*it2);
				int ind = 0;
				ss >> ind;

				auto it = npcList->begin();
				for (int i = 0; i < ind - 1; i++) {
					++it;
				}
				//Character c = *it;
				rollManager->RollToHit(**it);

				UpdateDiceScreen();
			}
			else if (*it2 == "atk") {
				++it2;
				stringstream ss(*it2);
				int ind = 0;
				ss >> ind;

				auto it = npcList->begin();
				for (int i = 0; i < ind - 1; i++) {
					++it;
				}
				//Character c = *it;
				rollManager->RollAttack(**it);

				UpdateDiceScreen();
			}
		}
		else {
			++it2;
			stringstream sx(*it2);
			int x = 0, y = 0, z = 0;
			sx >> x;
			++it2;
			stringstream sy(*it2);
			sy >> y;
			++it2;
			stringstream sz(*it2);
			sz >> z;

			rollManager->Roll(x, y, z);

			UpdateDiceScreen();

		}
	}
	
	/*
	Can be:
	turn npc

	or
	turn pc 2
	*/
	else if (*it2 == "turn") {
		++it2;
		if (*it2 == "npc") {
			for (auto gameIter = game->children.begin(); gameIter != game->children.end(); ++gameIter) {
				GameControl* checkName = *gameIter;
				if (checkName->name == "pcBox") {
					for (list<GameControl*>::iterator pcBoxIter = checkName->children.begin(); pcBoxIter != checkName->children.end(); ++pcBoxIter) {
						GameControl* gc = new GameControl();
						gc = *pcBoxIter;
						BorderBox* bb = dynamic_cast<BorderBox*>(gc);
						bb->OnOff(false);

					}
				}
				else if (checkName->name == "npcBox") {
					BorderBox* bb = dynamic_cast<BorderBox*>(checkName);
					bb->OnOff(true);
				}
			}
		}
		else if (*it2 == "pc") {
			++it2;
			stringstream ss(*it2);
				int ind = 0;
				ss >> ind;
			for (auto gameIter = game->children.begin(); gameIter != game->children.end(); ++gameIter) {
				if ((*gameIter)->name == "pcBox") {
					//auto pcBoxIter = (*gameIter)->children.begin();
					int ctr = 0;
					//for(int i = 0; i < )
					for (auto pcBoxIter = (*gameIter)->children.begin(); pcBoxIter != (*gameIter)->children.end(); ++pcBoxIter) {
						if (ctr == ind - 1) {
							BorderBox* bb = dynamic_cast<BorderBox*>((*pcBoxIter));
							bb->OnOff(true);
						}
						else {
							BorderBox* bb = dynamic_cast<BorderBox*>((*pcBoxIter));
							bb->OnOff(false);
						}
					}
				}
			}
		}
	}
	/*
	hit pc 1 4
	*/
	else if (*it2 == "hit") {
		++it2;
		//Character* ch;
		if (*it2 == "pc") {
			++it2;
			stringstream ss(*it2);
			int ind = 0;
			ss >> ind;
			++it2;
			stringstream sss(*it2);
			int hit = 0;
			sss >> hit;
			for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
				if ((*gameIt)->name == "pcBox") {
					list<GameControl*>::iterator pcBoxIt = (*gameIt)->children.begin();
					for (int i = 0; i < ind - 1; i++) {
						++pcBoxIt;
					}
					GameControl* gc = *((*pcBoxIt)->children.begin());
					PCDisplay* pcd = dynamic_cast<PCDisplay*>(gc);
					pcd->pc->TakeHit(hit);
					pcd->Update(pcd->pc);
					break;
				}
			}

		}
		else if (*it2 == "npc") {
			++it2;
			stringstream ss(*it2);
			int ind = 0;
			ss >> ind;
			++it2;
			stringstream sss(*it2);
			int hit = 0;
			sss >> hit;
			for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
				if ((*gameIt)->name == "npcBox") {
					list<GameControl*>::iterator npcBoxIt = (*gameIt)->children.begin();
					for (int i = 0; i < ind - 1; i++) {
						++npcBoxIt;
					}
					GameControl* gc = (*npcBoxIt);
					NPCDisplay* npcd = dynamic_cast<NPCDisplay*>(gc);
					npcd->npc->TakeHit(hit);
					npcd->Update(npcd->npc);
					break;
				}
			}

		}
	}
	/*set pc 1 name matt
	name, hp, maxhp, ac, tohit, numdice, diceval, bonus
	*/
	else if (*it2 == "set") {
		++it2;
		if (*it2 == "pc") {
			++it2;
			stringstream ss(*it2);
			int ind = 0;
			ss >> ind;
			++it2;
			for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
				if ((*gameIt)->name == "pcBox") {
					list<GameControl*>::iterator pcBoxIt = (*gameIt)->children.begin();
					for (int i = 0; i < ind - 1; i++) {
						++pcBoxIt;
					}
					GameControl* gc = *((*pcBoxIt)->children.begin());
					PCDisplay* pcd = dynamic_cast<PCDisplay*>(gc);
					if (*it2 == "name") {
						++it2;
						pcd->pc->name = *it2;
					}
					else if (*it2 == "hp") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->hp = newval;
					}
					else if (*it2 == "maxhp") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->maxHP = newval;
					}
					else if (*it2 == "ac") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->ac = newval;
					}
					else if (*it2 == "tohit") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->toHitBonus = newval;
					}
					else if (*it2 == "numdice") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->attackDiceNum = newval;
					}
					else if (*it2 == "diceval") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->attackDiceType = newval;
					}
					else if (*it2 == "bonus") {
						++it2;
						stringstream sss(*it2);
						int newval = 0;
						sss >> newval;
						pcd->pc->attackBonus = newval;
					}

					pcd->Update(pcd->pc);
				}
			}
		}

	}
	else if (*it2 == "show") {
		++it2;
		if (*it2 == "all") {
			ShowCommand(-1);
		}
		else {
			stringstream ss(*it2);
			int ind = 0;
			ss >> ind;
			ShowCommand(ind);
		}
	}
	else if (*it2 == "add") {
		++it2;
		string type = *it2;
		++it2;
		AddNPCCommand(type, *it2);
	}
	else if (*it2 == "clear") {
		ClearNPCs();
	}
	else if (*it2 == "load") {
	LoadChars();
}

	SaveChars();
	ExportNPCs();
}


void CommandHandler::ShowCommand(int ind) {
	if (ind == -1) {
		for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
			if ((*gameIt)->name == "npcBox") {
				for (list<GameControl*>::iterator npcIt = (*gameIt)->children.begin(); npcIt != (*gameIt)->children.end(); ++npcIt) {
					GameControl* gc = *npcIt;
					NPCDisplay* npcd = dynamic_cast<NPCDisplay*>(gc);
					npcd->Show(true);
				}
			}
		}
	}
	else {
		for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
			if ((*gameIt)->name == "npcBox") {
				list<GameControl*>::iterator npcIt = (*gameIt)->children.begin();
				for (int i = 0; i < ind - 1; i++) {
					++npcIt;
				}
				GameControl* gc = *((*npcIt)->children.begin());
				NPCDisplay* npcd = dynamic_cast<NPCDisplay*>(gc);
				npcd->Show(true);
			}
		}
	}
}

void CommandHandler::AddNPCCommand(string type, string name) {
	//NPCDisplay* ret = new NPCDisplay()
	for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
		if ((*gameIt)->name == "npcBox") {
			NPC* newnpc = factory->Create(type, name);
			NPCDisplay* ret = new NPCDisplay(Vector2(82, (*gameIt)->childCounter * 4 + 2), Vector2(38, 4), 18, newnpc);
			(*gameIt)->AddChild(ret);
		}
	}
}

void CommandHandler::ClearNPCs() {
	for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
		if ((*gameIt)->name == "npcBox") {
			(*gameIt)->children.clear();
			(*gameIt)->childCounter == 0;
		}
	}
}

void CommandHandler::SaveChars() {
	ofstream charFile;
	charFile.open("testSave.txt");
	
	for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
		if ((*gameIt)->name == "pcBox") {
			for (list<GameControl*>::iterator pcBoxIt = (*gameIt)->children.begin(); pcBoxIt != (*gameIt)->children.end(); ++pcBoxIt) {
				list<GameControl*>::iterator pcbox = (*pcBoxIt)->children.begin();
				PCDisplay* pcd = dynamic_cast<PCDisplay*>(*pcbox);

				charFile << pcd->pc->SaveString() + "\n";
			}
		}
	}

	charFile.close();
}

void CommandHandler::LoadChars() {
	string line;
	//string piece;
	//list<string> pieces = list<string>();
	ifstream readFile("testSave.txt");
	int ind = 0;
	while (getline(readFile, line)) {
		/*for (auto charReaderIt = line.begin(); charReaderIt != line.end(); ++charReaderIt) {
			if (*charReaderIt == ';') {
				pieces.push_back(piece);
				piece = "";
			}
			else {
				piece += *charReaderIt;
			}
		}
		pieces.push_back(piece);
		piece = "";*/


		*(pcs + ind) = PC(line);
		   
	}
}

void CommandHandler::ExportNPCs() {
	ofstream npcFile;
	npcFile.open("NPCListing.txt");

	for (list<GameControl*>::iterator gameIt = game->children.begin(); gameIt != game->children.end(); ++gameIt) {
		if ((*gameIt)->name == "npcBox") {
			for (list<GameControl*>::iterator npcIt = (*gameIt)->children.begin(); npcIt != (*gameIt)->children.end(); ++npcIt) {
				NPCDisplay* npcd = dynamic_cast<NPCDisplay*>(*npcIt);
				npcFile << npcd->npc->SaveString();
			}
		}
	}
}