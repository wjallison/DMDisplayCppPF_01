#include "Character.h"

void Character::TakeHit(int dam) {
	hp -= dam;
}

int Character::strToInt(string s) {
	int ret = 0;
	stringstream ss(s);
	ss >> ret;
	return ret;
}


PC::PC() {

}

PC::PC(string _name) {
	name = _name;
}

PC::PC(bool fromSave, string constructor) {
	string piece;
	list<string> pieces = list<string>();
	for (auto charReaderIt = constructor.begin(); charReaderIt != constructor.end(); ++charReaderIt) {
		if (*charReaderIt == ';') {
			pieces.push_back(piece);
			piece = "";
		}
		else {
			piece += *charReaderIt;
		}
	}
	pieces.push_back(piece);

	list<string>::iterator it = pieces.begin();
	name = *it;
	++it;
	stringstream hpss(*it);
	hp = 0;
	hpss >> hp;
	++it;
	stringstream mhpss(*it);
	maxHP = 0;
	mhpss >> maxHP;
	++it;
	stringstream acss(*it);
	ac = 0;
	acss >> ac;
	++it;
	stringstream thbss(*it);
	toHitBonus = 0;
	thbss >> toHitBonus;
	++it;
	attackDiceNum = strToInt(*it);
	++it;
	attackDiceType = strToInt(*it);
	++it;
	attackBonus = strToInt(*it);
	++it;
	boardID = *it;
}

PC::PC(string _name, int _maxHP, int _ac, int _toHitBonus, int _attackDiceNum, int _attackDiceType, int _attackBonus, string _boardID) {
	name = _name;
	maxHP = _maxHP;
	hp = maxHP;
	ac = _ac;
	toHitBonus = _toHitBonus;
	attackDiceNum = _attackDiceNum;
	attackDiceType = _attackDiceType;
	attackBonus = _attackBonus;
	boardID = _boardID;
}

PC::PC(string _name, int _hp, int _maxHP, int _ac, int _toHitBonus, int _attackDiceNum, int _attackDiceType, int _attackBonus, string _boardID) {
	name = _name;
	maxHP = _maxHP;
	hp = _hp;
	ac = _ac;
	toHitBonus = _toHitBonus;
	attackDiceNum = _attackDiceNum;
	attackDiceType = _attackDiceType;
	attackBonus = _attackBonus;
	boardID = _boardID;
}

string PC::SaveString() {
	string ret = name;
	ret += ";";
	ret += to_string(hp) + ";";
	ret += to_string(maxHP) + ";";
	ret += to_string(ac) + ";";
	ret += to_string(toHitBonus) + ";";
	ret += to_string(attackDiceNum) + ";";
	ret += to_string(attackDiceType) + ";";
	ret += to_string(attackBonus) + ";";
	ret += boardID;
}

NPC::NPC() {

}
NPC::NPC(string _name, int _maxHP, int medianHP, int _ac, string weapon, int _toHitBonus, int _attackDiceNum, int _attackDiceType, int _attackBonus, string _boardID) {
	name = _name;
	maxHP = _maxHP;
	hp = maxHP;
	if (maxHP > medianHP + medianHP * .3) {
		strengthCurveDesc = "Strong";
	}
	else if (maxHP > medianHP - medianHP * .3) {
		strengthCurveDesc = "Normal";
	}
	else {
		strengthCurveDesc = "Weak";
	}
	ac = _ac;
	armedWith = weapon;
	toHitBonus = _toHitBonus;
	attackDiceNum = _attackDiceNum;
	attackDiceType = _attackDiceType;
	attackBonus = _attackBonus;
	boardID = _boardID;

	EvaluateHealthDescriptor();
}

void NPC::TakeHit(int dam) {
	Character::TakeHit(dam);
	EvaluateHealthDescriptor();
}

void NPC::EvaluateHealthDescriptor() {
	if (hp == maxHP) {
		healthCurveDesc = "Full Health";
	}
	else if (hp > maxHP / 2) {
		healthCurveDesc = "Mildly Wounded";
	}
	else if (hp > maxHP / 10) {
		healthCurveDesc = "Bloodied";
	}
	else if (hp > 0) {
		healthCurveDesc = "Nearly Dead";
	}
	else if (hp <= 0) {
		healthCurveDesc = "Dead";
	}
}

string NPC::SaveString() {
	string ret = name + '\n';
	ret += to_string(hp) + "/" + to_string(maxHP) + '\n';
	ret += "AC: " + to_string(ac) + '\n';
	ret += "To Hit Bonus:" + to_string(toHitBonus) + '\n';
	ret += armedWith + ": " + to_string(attackDiceNum) + "d" + to_string(attackDiceType) + " + " + to_string(attackBonus) + '\n';
}