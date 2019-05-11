#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

class Screen {
	int size_x, size_y;
	char* screen_x;
	char* screen_y;

public:
	Screen(int sz_x, int sz_y) : size_x(sz_x),size_y(sz_y), screen_x(new char[sz_x + 1]) {}
	~Screen() { delete[] screen_x; }

	void draw(int pos, const char* face)
	{
		if (face == nullptr) return;
		if (pos < 0 || pos >= size_x) return;
		strncpy(&screen_x[pos], face, strlen(face));
	}

	void render()
	{
		printf("%s\r", screen_x);
	}

	void clear()
	{
		memset(screen_x, ' ', size_x);
		screen_x[size_x] = '\0';
	}

	int length()
	{
		return size_x;
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
	void draw()
	{
		screen->draw(pos, face);
	}
};

class Player : public GameObject {

public:
	Player(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen)
	{
	}


	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void update()
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

	void update()
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

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
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
	Screen screen{ 80 , 80 };
	Player player = { 30, "(^_^)", &screen };
	Enemy enemy{ 60, "(*--*)", &screen };
	Bullet bullet(-1, "+", &screen);

	while (true)
	{
		screen.clear();

		if (_kbhit())
		{
			int c = _getch();
			switch (c) {
			case 'a':
				player.moveLeft();
				break;
			case 'd':
				player.moveRight();
				break;
			case ' ':
				bullet.fire(player.getPosition());
				break;
			}
		}
		player.draw();
		enemy.draw();
		bullet.draw();

		player.update();
		enemy.update();
		bullet.update(enemy.getPosition());

		screen.render();
		Sleep(66);
	}

	return 0;
}