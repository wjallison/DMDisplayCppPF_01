#include "RollManager.h"


RollManager::RollManager() {
	result = 0;
	math = "";
}

void RollManager::AddToMath(int bonus) {
	string s = "+ " + to_string(bonus) + " bonus; ";
	math = math + s;

	
}

void RollManager::AddToMath(int roll, int rolled) {
	string s = "d" + to_string(rolled) + ": " + to_string(roll) + "; ";
	math = math + s;
}

int RollManager::Roll20() {
	math = "";
	int res = rand() % 20 + 1;
	AddToMath(res, 20);
	result = res;
	//Update();
	/*__raise UpdateTens(res / 10);
	__raise UpdateOnes(res % 10);*/
	return res;
}

int RollManager::Roll(int x, int y, int z) {
	// xdy + z
	//math = "";
	int res = 0;
	int roll = 0;
	for (int i = 0; i < x; i++) {
		roll = rand() % y + 1;
		AddToMath(roll, y);
		res += roll;
	}
	res += z;
	AddToMath(z);
	result = res;
	return res;
}

int RollManager::RollToHit(Character ch) {
	math = "";
	int res = Roll20();
	AddToMath(res, 20);
	res += ch.toHitBonus;
	AddToMath(ch.toHitBonus);
	result = res;
	//__raise UpdateTens(res / 10);
	//__raise UpdateOnes(res % 10);
	return res;
}

int RollManager::RollAttack(Character ch) {
	math = "";
	int res = Roll(ch.attackDiceNum, ch.attackDiceType, ch.attackBonus);
	//__raise UpdateTens(res / 10);
	//__raise UpdateOnes(res % 10);
	return res;
}

int RollManager::RollAdvantage(Character ch, bool ad) {
	math = "(";
	int res1 = Roll20();
	AddToMath(res1,20);
	int res2 = Roll20();
	math += "d20: " + to_string(res2) + ") => ";
	int res;
	if (ad) {
		res = res1 > res2 ? res1 : res2;
	}
	else {
		res = res1 < res2 ? res1 : res2;
	}
	math += to_string(res) + " + " + to_string(ch.toHitBonus)/* + " = " + to_string(res + ch.toHitBonus)*/;
	result = res + ch.toHitBonus;
	//__raise UpdateTens(res + ch.toHitBonus / 10);
	//__raise UpdateOnes(res % 10);
	return res + ch.toHitBonus;
}

//void RollManager::AssignOnes(SevenSegment* seg) {
//	ones = seg;
//}
//void RollManager::AssignTens(SevenSegment* seg) {
//	tens = seg;
//}
//
//void RollManager::Update() {
//	ones->Update(result % 10);
//	tens->Update(result / 10);
//}

//void RollManager::UpdateOnes(int onesValue) {
//
//}
//void RollManager::UpdateTens(int tensValue) {
//
//}