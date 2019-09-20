#include "GameControl.h".



GameControl::GameControl() 
{
	ULCorner = Vector2(0,0);
	size = Vector2(1, 1);
	LRCorner = Vector2(0, 0);
	background = 0;
	backFill = ' ';
}

GameControl::GameControl(Vector2 corner, Vector2 sz, int back) {
	ULCorner = corner;
	size = sz;
	LRCorner = Vector2(ULCorner.x + size.x, ULCorner.y + size.y);
	background = back;
	backFill = ' ';
}

bool GameControl::Click(Vector2 pt) {
	//bool lower = false;
	if (pt.x > ULCorner.x && pt.x < ULCorner.x + size.x) {
		if (pt.y > ULCorner.y && pt.y < ULCorner.y + size.y) {
			list<GameControl*>::iterator iter;
			for (iter = children.begin(); iter != children.end(); ++iter) {
				if ((*iter)->Click(pt)) {
					//lower = true;
					//break;
					return true;
				}
			}
			/*for (int i = 0; i < sizeof(children); i++) {
				children. [i].Click(pt);
			}*/
		}
	}
}

//void GameControl::CheckTypeAndDraw(GameControl* gc) {
//	BorderBox* b = dynamic_cast<BorderBox*>(gc);
//	if (b) {
//		b->Draw();
//	}
//}

void GameControl::Draw() {
	for (int i = ULCorner.x; i < ULCorner.x + size.x; i++) {
		for (int j = ULCorner.y; j < ULCorner.y + size.y; j++) {
			DrawChar(i, j, ' ', background);
		}
	}

	list<GameControl*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		(*it)->Draw();
	}
}

void GameControl::Update() {

}

void GameControl::AddChild(GameControl* gc) {
	//children.push_back(gc);
	children.push_back(gc);
	++childCounter;

}

void GameControl::DrawChar(int x, int y, char c, int style) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, style);
	std::cout << c << endl;
	//std::cout << 'k';
}

//template<typename T>
//void GameControl::UpdateNamed(string nm) {
//	if (name == nm) {
//
//	}
//	else {
//		for (auto it = children.begin(); it != children.end(); ++it) {
//			(*it)->UpdateNamed(nm, obj);
//		}
//	}
//}

GameControl* GameControl::PullNamed(string nm) {
	if (name == nm) {
		return this;
	}
	else {
		for (auto it = children.begin(); it != children.end(); ++it) {
			GameControl* ret = (*it)->PullNamed(nm);
			if (ret->name == nm) {
				return ret;
			}
		}
	}

	return new GameControl();
}





TextControl::TextControl(Vector2 corner, Vector2 sz, int back, string txt) :GameControl(corner, sz, back) {
	text = txt;
}

TextControl::TextControl(Vector2 corner, Vector2 sz, int back, string* txtSource) : GameControl(corner, sz, back){
	sourceText = txtSource;
	text = *sourceText;
}

bool TextControl::Click(Vector2 pt) {
	return false;
}

void TextControl::Draw() {
	COORD coord;
	coord.X = ULCorner.x;
	coord.Y = ULCorner.y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, background);
	std::cout << text;
}

void TextControl::Update() {
	text = *sourceText;
}

FilledBar::FilledBar(Vector2 corner, Vector2 sz, int back, int back1, int back2) :GameControl(corner, sz, back) {
	GameControl* b1 =new GameControl(corner, sz, back1);

	GameControl* b2 =new GameControl(Vector2(corner.x + sz.x, corner.y), Vector2(0, sz.y), back2);
	AddChild(b1);
	AddChild(b2);

	total = 10;
	value = 10;
}

void FilledBar::AdjustVal(int dVal, int dTot) {
	value += dVal;
	total += dTot;

	list<GameControl*>::iterator iter = children.begin();
	//value += dVal;
	//total += dTot;
	int mp = (value / total * size.x);
	(*iter)->size = Vector2(mp, 1);
	++iter;
	(*iter)->ULCorner = Vector2(ULCorner.x + mp, ULCorner.y);
	(*iter)->size = Vector2(size.x - mp, 1);
	//*iter->
	/*iter->ULCorner.x 
	iter->ULCorner.x = ULCorner.x + size.x * value / total;
	iter->size.x = ULCorner.x + size.x - ULCorner.x + size.x * value / total;*/
}

void FilledBar::SetVal(int val, int tot) {
	value = val;
	total = tot;
	list<GameControl*>::iterator iter = children.begin();
	//value += dVal;
	//total += dTot;
	int mp = (size.x * value / total);
	(*iter)->size = Vector2(mp, 1);
	++iter;
	(*iter)->ULCorner = Vector2(ULCorner.x + mp, ULCorner.y);
	(*iter)->size = Vector2(size.x - mp, 1);
}



PCDisplay::PCDisplay(Vector2 corner, Vector2 sz, int back, PC* _pc) :GameControl(corner, sz, back) {
	pc = _pc;
	TextControl* nm = new TextControl(corner, Vector2(sz.x, 1), 7, &pc->name);
	AddChild(nm);
	TextControl* hp = new TextControl(Vector2(corner.x, corner.y + 1), Vector2(sz.x, 1), 10, "HP: " + to_string(pc->hp) + " (" + to_string(pc->maxHP) + ")");
	AddChild(hp);
	FilledBar* hpbar = new FilledBar(Vector2(corner.x, corner.y + 2), Vector2(sz.x, 1), back, 235, 196);
	AddChild(hpbar);
	TextControl* ac = new TextControl(Vector2(corner.x, corner.y + 3), Vector2(sz.x, 1), 7,
		"AC: " + to_string(pc->ac));
	AddChild(ac);
	TextControl* toHit = new TextControl(Vector2(corner.x, corner.y + 4), Vector2(sz.x, 1), 7,
		"To Hit Bonus: " + to_string(pc->toHitBonus));
	AddChild(toHit);
	TextControl* dam = new TextControl(Vector2(corner.x, corner.y + 5), Vector2(sz.x, 1), 7,
		"Damage: " + to_string(pc->attackDiceNum) + "d" + to_string(pc->attackDiceType) + " + " + to_string(pc->attackBonus));
	AddChild(dam);
}

void PCDisplay::Draw() {
	for (int i = ULCorner.x; i < ULCorner.x + size.x; i++) {
		for (int j = ULCorner.y; j < ULCorner.y + size.y; j++) {
			DrawChar(i, j, ' ', background);
		}
	}
	list<GameControl*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		(*it)->Draw();
	}
}

void PCDisplay::Update(PC* _pc) {
	pc = _pc;
	list<GameControl*>::iterator it;
	it = children.begin();
	GameControl* pt = *it;
	TextControl* tx = dynamic_cast<TextControl*>(pt);
	tx->text = pc->name;
	++it;
	tx = dynamic_cast<TextControl*>(*it);
	tx->text = "HP: " + to_string(pc->hp) + " (" + to_string(pc->maxHP) + ")";
	++it;
	FilledBar* fb = dynamic_cast<FilledBar*>(*it);
	fb->SetVal(pc->hp,pc->maxHP);
	++it;
	tx = dynamic_cast<TextControl*>(*it);
	tx->text = "AC: " + to_string(pc->ac);
	++it;
	tx = dynamic_cast<TextControl*>(*it);
	tx->text = "To Hit Bonus: " + to_string(pc->toHitBonus);
	++it;
	tx = dynamic_cast<TextControl*>(*it);
	tx->text = "Damage: " + to_string(pc->attackDiceNum) + "d" + to_string(pc->attackDiceType) + " + " + to_string(pc->attackBonus);
}

NPCDisplay::NPCDisplay(Vector2 corner, Vector2 sz, int back, NPC* _npc) : GameControl(corner, sz, back) {
	npc = _npc;
	TextControl* nm = new TextControl(corner, Vector2(sz.x, 1), 7, npc->name);
	AddChild(nm);
	TextControl* health = new TextControl(Vector2(corner.x, corner.y + 1), Vector2(sz.x, 1), 10, npc->healthCurveDesc);
	AddChild(health);
	TextControl* strength = new TextControl(Vector2(corner.x, corner.y + 2), Vector2(sz.x, 1), 4, npc->strengthCurveDesc);
	AddChild(strength);
	TextControl* armed = new TextControl(Vector2(corner.x, corner.y + 3), Vector2(sz.x, 1), 7, "Armed with: " + npc->armedWith);
	AddChild(armed);
}

void NPCDisplay::Update(NPC* _npc) {
	npc = _npc;
	list<GameControl*>::iterator it;
	it = children.begin();
	TextControl* nm = dynamic_cast<TextControl*>((*it));
	nm->text = npc->name;
	++it;
	TextControl* health = dynamic_cast<TextControl*>((*it));
	health->text = npc->healthCurveDesc;
	++it;
	TextControl* str = dynamic_cast<TextControl*>((*it));
	str->text = npc->strengthCurveDesc;
	++it;
	TextControl* arm = dynamic_cast<TextControl*>((*it));
	arm->text = "Armed with: " + npc->armedWith;
}

void NPCDisplay::Draw() {
	if (show) {
		GameControl::Draw();
	}
	else {
		return;
	}
}

void NPCDisplay::Show(bool _show) {
	show = _show;
}


BorderBox::BorderBox(Vector2 corner, Vector2 sz, int back, char bkfl) : GameControl(corner, sz, back) {
	bkchr = bkfl;
}

BorderBox::BorderBox(Vector2 corner, Vector2 sz, int back, char bkfl, int back2, char bkfl2) : GameControl(corner, sz, back) {
	bkchr = bkfl2;
	background2 = back2;
}

void BorderBox::OnOff(bool on) {
	switchBack = on;
}

void BorderBox::Flip() {
	switchBack = !switchBack;
}

void BorderBox::Draw() {
	char c = bkchr;
	int bck = background;
	if (switchBack) {
		c = bkchr2;
		bck = background2;
	}

	for (int i = ULCorner.x; i < ULCorner.x + size.x; i++) {
		for (int j = ULCorner.y; j < ULCorner.y + size.y; j++) {
			if (i == ULCorner.x) {
				DrawChar(i, j, c, bck);
			}
			else if (i == ULCorner.x + size.x - 1) {
				DrawChar(i, j, c, bck);
			}
			else if (j == ULCorner.y) {
				DrawChar(i, j, c, bck);
			}
			else if (j == ULCorner.y + size.y - 1) {
				DrawChar(i, j, c, bck);
			}
			else {
				DrawChar(i, j, ' ', bck);
			}
		}
	}

	list<GameControl*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		(*it)->Draw();
	}
	//GameControl::Draw();
}

SevenSegment::SevenSegment(Vector2 corner, Vector2 sz, int back) : GameControl(corner, sz, back) {
	/*for (int i = 0; i < 7; i++) {
		segs[i] = GameControl(Vector2())
	}*/
	segs[0] = GameControl(Vector2(corner.x + 1, corner.y + 0), Vector2(3, 1), 196);
	segs[1] = GameControl(Vector2(corner.x + 0, corner.y + 1), Vector2(1, 2), 196);
	segs[2] = GameControl(Vector2(corner.x + 4, corner.y + 1), Vector2(1, 2), 196);
	segs[3] = GameControl(Vector2(corner.x + 1, corner.y + 3), Vector2(3, 1), 196);
	segs[4] = GameControl(Vector2(corner.x + 0, corner.y + 4), Vector2(1, 2), 196);
	segs[5] = GameControl(Vector2(corner.x + 4, corner.y + 4), Vector2(1, 2), 196);
	segs[6] = GameControl(Vector2(corner.x + 1, corner.y + 6), Vector2(3, 1), 196);
}

void SevenSegment::Update(int num) {
	switch (num)
	{
	case 0:
		segs[0].background = 196;
		segs[1].background = 196;
		segs[2].background = 196;
		segs[3].background = 0;
		segs[4].background = 196;
		segs[5].background = 196;
		segs[6].background = 196;
		break;
	case 1:
		segs[0].background = 0;
		segs[1].background = 0;
		segs[2].background = 196;
		segs[3].background = 0;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 0;
		break;
	case 2:
		segs[0].background = 196;
		segs[1].background = 0;
		segs[2].background = 196;
		segs[3].background = 196;
		segs[4].background = 196;
		segs[5].background = 0;
		segs[6].background = 196;
		break;
	case 3:
		segs[0].background = 196;
		segs[1].background = 0;
		segs[2].background = 196;
		segs[3].background = 196;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 196;
		break;
	case 4:
		segs[0].background = 0;
		segs[1].background = 196;
		segs[2].background = 196;
		segs[3].background = 196;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 0;
		break;
	case 5:
		segs[0].background = 196;
		segs[1].background = 196;
		segs[2].background = 0;
		segs[3].background = 196;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 196;
		break;
	case 6:
		segs[0].background = 196;
		segs[1].background = 196;
		segs[2].background = 0;
		segs[3].background = 196;
		segs[4].background = 196;
		segs[5].background = 196;
		segs[6].background = 196;
		break;
	case 7:
		segs[0].background = 196;
		segs[1].background = 0;
		segs[2].background = 196;
		segs[3].background = 0;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 0;
		break;
	case 8:
		segs[0].background = 196;
		segs[1].background = 196;
		segs[2].background = 196;
		segs[3].background = 196;
		segs[4].background = 196;
		segs[5].background = 196;
		segs[6].background = 196;
		break;
	case 9:
		segs[0].background = 196;
		segs[1].background = 196;
		segs[2].background = 196;
		segs[3].background = 196;
		segs[4].background = 0;
		segs[5].background = 196;
		segs[6].background = 0;
		break;
	default:
		segs[0].background = 0;
		segs[1].background = 0;
		segs[2].background = 0;
		segs[3].background = 0;
		segs[4].background = 0;
		segs[5].background = 0;
		segs[6].background = 0;
		break;
	}
}

void SevenSegment::Draw() {
	for (int i = ULCorner.x; i < ULCorner.x + size.x; i++) {
		for (int j = ULCorner.y; j < ULCorner.y + size.y; j++) {
			DrawChar(i, j, ' ', background);
		}
	}
	for (int i = 0; i < 7; i++) {
		segs[i].Draw();
	}
}

//void SevenSegment::UpdateEventReceiver(int val) {
//	Update(val);
//}
//
//void SevenSegment::HookEvent(RollManager* r, int ind) {
//	if (ind == 10) {
//		__hook(&RollManager::UpdateTens, r, &SevenSegment::UpdateEventReceiver);
//	}
//	else if (ind == 1) {
//		__hook(&RollManager::UpdateOnes, r, &SevenSegment::UpdateEventReceiver);
//	}
//	
//}

Vector2::Vector2() {
	x = 0;
	y = 0;
}
Vector2::Vector2(int _x, int _y) {
	x = _x;
	y = _y;
}

