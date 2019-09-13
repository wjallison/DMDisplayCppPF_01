#pragma once
#include "Character.h"
#include <string>
#include <stdio.h>
//#include "GameControl.h"


//[event_source(native)] 
class RollManager
{
private:
	
public:
	int result;
	string math;
	RollManager();
	void AddToMath(int bonus);
	void AddToMath(int roll, int rolled);
	int Roll20();
	int Roll(int x, int y, int z);
	int RollToHit(Character ch);
	int RollAttack(Character ch);
	int RollAdvantage(Character ch, bool ad = true);

	//GameControl* test;

	/*SevenSegment ones;
	SevenSegment tens;

	void AssignOnes(SevenSegment* seg);
	void AssignTens(SevenSegment* seg);

	void Update();*/

	/*__event void UpdateTens(int tensValue);
	__event void UpdateOnes(int onesValue);*/
};

