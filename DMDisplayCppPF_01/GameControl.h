#pragma once
#include <list>
#include <iterator>
#include <cmath>
#include <string>
#include "Character.h"
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include "RollManager.h"


using namespace std;
struct Vector2 {
	float x;
	float y;
	Vector2();
	Vector2(int _x, int _y);
};


class GameControl
{
private:
public:
	list<GameControl*> children = list<GameControl*>();
	Vector2 ULCorner;
	Vector2 size;
	Vector2 LRCorner;
	int background; //consoleTextAttribute https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
	char backFill;
	string name = "";
	int childCounter = 0;

	GameControl();
	GameControl(Vector2 corner, Vector2 sz, int back);

	virtual bool Click(Vector2 pt);

	//void CheckTypeAndDraw(GameControl* gc);

	virtual void Draw();

	virtual void Update();

	void AddChild(GameControl* gc);

	void DrawChar(int x, int y, char c, int style);

	//template<typename T>
	//void UpdateNamed(string nm);

	GameControl* PullNamed(string nm);
};

class TextControl : public GameControl 
{
private:
public:
	string text;
	
	string* sourceText;

	TextControl(Vector2 corner, Vector2 sz, int back, string txt);
	TextControl(Vector2 corner, Vector2 sz, int back, string* txtSource);
	bool Click(Vector2 pt) override;

	void Draw() override;

	void Update() override;

	//void HookEvent(void (*f)(string));
};

class FilledBar : public GameControl {
private:
public:
	int total;
	int value;
	//FilledBar(Vector2 corner, Vector2 sz, int back) : GameControl(corner, sz, back);
	FilledBar(Vector2 corner, Vector2 sz, int back, int back1, int back2);
	void AdjustVal(int dVal, int dTot = 0);

	void SetVal(int val, int tot);
};

class PCDisplay : public GameControl {
private:
public:
	PC* pc;
	PCDisplay(Vector2 corner, Vector2 sz, int back, PC* _pc);
	void Draw() override;
	void Update(PC* _pc);
};

class NPCDisplay : public GameControl {
private:
public:
	bool show = false;
	NPC* npc;
	NPCDisplay(Vector2 corner, Vector2 sz, int back, NPC* _npc);
	void Update(NPC* _npc);

	void Draw() override;

	void Show(bool _show);
};

class BorderBox : public GameControl {
private:
public:
	char bkchr;
	bool switchBack = false;
	int background2;
	char bkchr2;
	BorderBox(Vector2 corner, Vector2 sz, int back, char bkfl);
	BorderBox(Vector2 corner, Vector2 sz, int back, char bkfl, int back2, char bkfl2);

	void OnOff(bool on);

	void Flip();

	void Draw() override;
};


class SevenSegment : public GameControl {
private:
public:
	GameControl segs[7];
	SevenSegment(Vector2 corner, Vector2 sz, int back);
	void Update(int num);
	void Draw() override;

	//void UpdateEventReceiver(int val);

	//void HookEvent(RollManager* r, int ind);
};

