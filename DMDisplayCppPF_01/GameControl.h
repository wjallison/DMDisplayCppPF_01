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

	GameControl();
	GameControl(Vector2 corner, Vector2 sz, int back);

	virtual bool Click(Vector2 pt);

	//void CheckTypeAndDraw(GameControl* gc);

	virtual void Draw();

	virtual void Update();

	void AddChild(GameControl* gc);

	void DrawChar(int x, int y, char c, int style);
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
};

class PCDisplay : public GameControl {
private:
public:
	PC pc;
	PCDisplay(Vector2 corner, Vector2 sz, int back, PC _pc);
	void Draw() override;
	void Update(PC _pc);
};

class NPCDisplay : public GameControl {
private:
public:
	NPC* npc;
	NPCDisplay(Vector2 corner, Vector2 sz, int back, NPC* _npc);

};

class BorderBox : public GameControl {
private:
public:
	char bkchr;
	BorderBox(Vector2 corner, Vector2 sz, int back, char bkfl);

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

