#pragma once
#include "Character.h"
#include <string>
#include <list>
#include <sstream>


class NPCFactory
{

};

class Archetype {
private:
public:
	string type;
	//hpx d hpy + hpz
	int hpx, hpy, hpz;
	int numWeps;
	list<int> wTH, wx, wy, wz;
	list<string> weapName;

	int IntConvert(string s);

	Archetype(string full);
	NPC* Generate(string _name);
};

