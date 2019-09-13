#include "Character.h"

void Character::TakeHit(int dam) {
	hp -= dam;
}


PC::PC() {

}

PC::PC(string _name) {
	name = _name;
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
	if (hp > hp / 2) {

	}
	else if (hp > hp / 10 && hp < hp / 2) {
		healthCurveDesc = "Bloodied";
	}
	else if (hp > 0 && hp < hp / 10) {
		healthCurveDesc = "Nearly Dead";
	}
	else if (hp <= 0) {
		healthCurveDesc = "Dead";
	}
}