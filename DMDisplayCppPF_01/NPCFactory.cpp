#include "NPCFactory.h"




Archetype::Archetype(string full) {
	list<string> subs;
	
	
	string sub = "";
	for (int i = 0; i < subs.size(); i++) {
		if (full[i] == ';') {
			subs.push_back(sub);
			sub = "";
		}
		else {
			sub += full[i];
		}		
	}
	subs.push_back(sub);

	list<string>::iterator it;
	it = subs.begin();
	type = *it;
	++it;
	hpx = IntConvert(*it);
	++it;
	hpy = IntConvert(*it);
	++it;
	hpz = IntConvert(*it);
	++it;
	numWeps = 0;
	while (it != subs.end()) {
		weapName.push_back(*it);
		++it;
		wx.push_back(IntConvert(*it));
		++it;
		wy.push_back(IntConvert(*it));
		++it;
		wz.push_back(IntConvert(*it));
		++it;
		wTH.push_back(IntConvert(*it));
		++it;
		numWeps++;
	}
}

NPC* Archetype::Generate(string _name) {
	int hp = hpz;
	for (int i = 0; i < hpx; i++) {
		hp += rand() % hpy + 1;
	}
	int ind = rand() % numWeps;
	list<string>::iterator itWep;
	list<int>::iterator itwx, itwy, itwz, itwTH;
	itWep = weapName.begin();
	itwx = wx.begin();
	itwy = wy.begin();
	itwz = wz.begin();
	itwTH = wTH.begin();
	for (int i = 0; i < ind; i++) {
		++itWep;
		++itwx;
		++itwy;
		++itwz;
		++itwTH;
	}

	return new NPC(_name, hp, hpx * hpy / 2 + hpz, 10, *itWep, *itwTH, *itwx, *itwy, *itwz, "" + type[0]);
}

NPC* NPCFactory::Create(string _type, string _name) {
	list<Archetype>::iterator it;
	//it = types.begin();
	for (it = types.begin(); it != types.end(); ++it) {
		if (it->type == _type) {
			return it->Generate(_name);
		}
	}
	return new NPC();
}

void NPCFactory::Add(string full) {
	types.push_back(Archetype(full));
}

int Archetype::IntConvert(string s) {
	stringstream strstr(s);
	int res = 0;
	strstr >> res;
	return res;
}