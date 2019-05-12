#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
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
	int HP;
	

public:
	GameObject(){}
	GameObject(int pos, const char* face, Screen* screen, int hp)
		: pos(pos), screen(screen), HP(hp)
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

	char* getFace()
	{
		return face;
	}

	void draw()
	{
		if (HP > 0)
			screen->draw(pos, face);
	}

	int getHp()
	{
		return HP;
	}

	void decreaseHp()
	{
		HP--;
	}
};

class Player : public GameObject {
	char left_face[20] = "<<^_^))";
	char right_face[20] = "((^_^>>";
	bool left, right;

public:
	
	Player() {}
	Player(int pos, const char* face, Screen* screen, int hp)
		: GameObject(pos, face, screen, hp)
	{
		
	}

	void moveLeft()
	{
		
		strncpy(getFace(), left_face, strlen(left_face));
		left = true;
		right = false;

		if (getPosition() == 0)
			setPosition(getPosition());

		else
			setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		
		strncpy(getFace(), right_face, strlen(right_face));
		left = false;
		right = true;

		if (getPosition() == 110)
			setPosition(getPosition());

		else
			setPosition(getPosition() + 1);
	}

	bool getDirection()
	{
		return right;
	}

	void update()
	{

	}

};

class Enemy : public GameObject {
	int count;

public:
	Enemy() {}
	Enemy(int pos, const char* face, Screen* screen, int hp)
		: GameObject(pos, face, screen, hp), count(0)
	{
	}

	void move(int player_pos)
	{
		if (getPosition() == 0 || getPosition() == 110)
			setPosition(getPosition());

		else if(getPosition() > player_pos)
		{
			setPosition(getPosition() - 1);
		}

		else if (getPosition() < player_pos)
		{
			setPosition(getPosition() + 1);
		}
			
	}

	int counting()
	{
		count++;
		return count;
	}

	

	void update(int player_pos)
	{
		counting();
		
		if (count == 10)
		{
			move(player_pos);
			count = 0;
		}
	}
};

class Bullet : public GameObject {
	bool isFiring;
	bool check;
	Enemy enemy;

public:
	Bullet() {}
	Bullet(int pos, const char* face, Screen* screen, int hp)
		: GameObject(pos, face, screen, hp), isFiring(false), check(false)
	{
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}

	void fire(int player_pos, bool direction)
	{
		isFiring = true;
		check = direction;
		if (check == true)
			setPosition(player_pos + 5);
		else if (check == false)
			setPosition(player_pos);
	}

	void update(int enemy_pos)
	{
		if (isFiring == false) return;
		int pos = getPosition();
		
		if (check == true) {
			pos = pos + 1;
		}

		else if (check == false) {
			pos = pos - 1;
		}
		
		if (pos > enemy_pos && pos < enemy_pos + 4)
		{
			isFiring = false;
			pos = -1;
		}
		setPosition(pos);
	}
};

int main()
{
	Screen screen{ 120 };
	Player player{ 30, "<<^_^>>", &screen , 10};
	Enemy enemy{ 100, "(*__*)", &screen, 5 };
	const int max_bullets = 10;
	Bullet bullets[100];

	int i = 0;
	int j = 0;

	for (int i = 0; i < max_bullets; i++)
	{
		bullets[i] = { -1,"+",&screen,1 };
	}

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
				i = 0;
				for (i = 0; i < max_bullets; i++)
				{
					if (bullets[i].getPosition() == -1)
						break;
				}
				if (i == max_bullets)
					break;
				bullets[i].fire(player.getPosition(), player.getDirection());
				break;
			}
		}

		//draw
		player.draw();
		enemy.draw();
		for (int i = 0; i < max_bullets; i++)
			bullets[i].draw();
		
		//update
		player.update();
		enemy.update(player.getPosition());

		for (int i = 0; i < max_bullets; i++)
		{
			bullets[i].update(enemy.getPosition());
			if (enemy.getPosition() == bullets[i].getPosition())
			{
				enemy.decreaseHp();
				bullets[i].decreaseHp();
			}
		}
			
		
		screen.render();
		Sleep(66);
	}

	return 0;
}