#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

class Screen {
	int size;
	char* screen;

public:
	Screen(int sz) : size(sz), screen(new char[sz + 1]) {}
	~Screen() { delete[] screen; }

	void draw(int pos, const char* face)
	{
		if (face == nullptr) return;
		if (pos < 0 || pos >= size) return;
		strncpy(&screen[pos], face, strlen(face));
	}

	void render()
	{
		printf("%s\r", screen);
	}

	void clear()
	{
		memset(screen, ' ', size);
		screen[size] = '\0';
	}

	int length()
	{
		return size;
	}

};

class GameObject {
	int pos;
	char face[20];
	Screen* screen;

public:
	GameObject(int pos, const char* face, Screen* screen)
		: pos(pos), screen(screen)
	{
		strcpy(this->face, face);
	}

	int getPosition()
	{
		return pos;
	}

	void setPosition(int pos)
	{
		this->pos = pos;
	}

	virtual void draw()
	{
		screen->draw(pos, face);
	}

	virtual void update(int enemy_pos)
	{

	}

	virtual void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	virtual void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	virtual void fire(int player_pos)
	{

	}
};

class Player : public GameObject {

public:
	Player(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen)
	{
	}
};

class Enemy : public GameObject {

public:
	Enemy(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen)
	{
	}

	void moveRandom()
	{
		setPosition(getPosition() + rand() % 3 - 1);
	}

	void update(int enemy_pos)
	{
		moveRandom();
	}
};

class Bullet : public GameObject {
	bool isFiring;

public:
	Bullet(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen), isFiring(false)
	{
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}

	void fire(int player_pos)
	{
		isFiring = true;
		setPosition(player_pos);
	}

	void update(int enemy_pos)
	{
		if (isFiring == false) return;
		int pos = getPosition();
		if (pos < enemy_pos) {
			pos = pos + 1;
		}
		else if (pos > enemy_pos) {
			pos = pos - 1;
		}
		else {
			isFiring = false;
		}
		setPosition(pos);
	}
};

int main()
{
	Screen screen{ 80 };
	GameObject * gameobjects[3] = { nullptr };
	gameobjects[0] = new Player(30, "(^_^)", &screen);
	gameobjects[1] = new Enemy(60, "(*--*)", &screen);
	gameobjects[2] = new Bullet(-1, "+", &screen);

	while (true)
	{
		screen.clear();

		if (_kbhit())
		{
			int c = _getch();
			switch (c) {
			case 'a':
				gameobjects[0]->moveLeft();
				break;
			case 'd':
				gameobjects[0]->moveRight();
				break;
			case ' ':
				gameobjects[2]->fire(gameobjects[0]->getPosition());
				break;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			gameobjects[i]->draw();
		}

		for (int i = 0; i < 3; i++)
		{
			gameobjects[i]->update(gameobjects[1]->getPosition());
		}

		screen.render();
		Sleep(66);
	}

	for (int i = 0; i < 3; i++)
	{
		delete gameobjects[i];
		gameobjects[i] = nullptr;
	}

	return 0;
}