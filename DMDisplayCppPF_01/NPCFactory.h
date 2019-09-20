#pragma once
#include "Character.h"
#include <string>
#include <list>
#include <iterator>
#include <sstream>

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

class NPCFactory
{
public:
	list<Archetype> types = list<Archetype>();

	NPC* Create(string _type, string _name);

	void Add(string full);

	NPCFactory();
};



