#pragma once
#include <string>

using namespace std;
class Character
{
private:
public:
	string name = "";
	int hp = 10;
	int maxHP = 10;
	int ac = 10;
	int toHitBonus = 0;
	int attackDiceNum = 1;
	int attackDiceType = 4; 
	int attackBonus = 0; 
	

	string boardID;

	virtual void TakeHit(int dam);
};

class PC : public Character {
private:
public:
	PC();
	PC(string _name);
	PC(string _name, int _maxHP, int _ac, int _toHitBonus, int _attackDiceNum, int _attackDiceType, int _attackBonus, string _boardID);
	//string weapon;
};

class NPC : public Character {
private:
public:
	string strengthCurveDesc;
	string armedWith;
	
	string healthCurveDesc;

	NPC();
	NPC(string _name, int _maxHP, int medianHP, int _ac, string weapon, int _toHitBonus, int _attackDiceNum, int _attackDiceType, int _attackBonus, string _boardID);

	void TakeHit(int dam) override;
	void EvaluateHealthDescriptor();
	//void EvaluateStrength
};

