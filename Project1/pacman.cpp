#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int height = 19;
const int width = 19;
const int ts = 25;
int point = 0, time_to_new_game = 5000;
bool player_live = true;
bool attack = false;
int ghost_time = 10000;

String map[height] = {
"IIIIIIIIIIIIIIIIIII",
"I1       I       2I",
"I II III I III II I",
"I                 I",
"I II I IIIII I II I",
"I    I   I   I    I",
"IIII III I III IIII",
"   I I       I I   ",
"IIII I IIIII I IIII",
"       I   I       ",
"IIII I IIIII I IIII",
"   I I   C   I I   ",
"IIII III I III IIII",
"I    I   I   I    I",
"I II I IIIII I II I",
"I                 I",
"I II III I III II I",
"I3       I       4I",
"IIIIIIIIIIIIIIIIIII",
};

class setSound {
public:
	SoundBuffer s1, s2, s3, s4, s5;
	Sound eat, death, winwin, bonus, pr_death;
	Music music[1];

	setSound() {
		s1.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/eat.ogg");
		s2.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/death.ogg");
		s3.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/winwin.ogg");
		s4.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/bonus.ogg");
		s5.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/pr_death.ogg");

		eat.setBuffer(s1);
		death.setBuffer(s2);
		winwin.setBuffer(s3);
		bonus.setBuffer(s4);
		pr_death.setBuffer(s5);

		music[0].openFromFile("C:/Users/dasse/Desktop/звуки_пакман/mus.ogg");
		for (int i = 0; i < 1; i++) {
			music[i].setVolume(25);
			music[i].setLoop(true);
		}
	}
};
setSound sound;

bool play = false, menu = true, game = false, puskgame = false, pob = false, die = false, pause = false;

class setMenu {
public:
	bool pusklever[5], blv[5];

	setMenu() {
		for (int i = 0; i < 5; i++)
			pusklever[i] = false;

		blv[0] = true;
		for (int i = 1; i < 5; i++)
			blv[i] = false;
	}
};

class Player {
public:
	float frame = 0;
	int x = 9, y = 11;
	int new_x = 0, new_y = 0;
	int turn = 1, delay = 0;
	bool teleportation = false;

	void update() {
		if (teleportation) {
			map[y][x] = ' ';

			x = 9;
			y = 11;

			map[y][x] = 'C';

			new_x = x;
			new_y = y;

			teleportation = false;
		}
		else {
			frame += 0.01;
			if (frame > 5)
				frame -= 5;

			delay++;
			if (delay >= 300) {
				switch (turn)
				{
				case 1:
					if (map[y][new_x + 1] != 'I')
						new_x += 1;
					break;
				case 2:
					if (map[y][new_x - 1] != 'I')
						new_x -= 1;
					break;
				case 3:
					if (map[new_y - 1][x] != 'I')
						new_y -= 1;
					break;
				case 4:
					if (map[new_y + 1][x] != 'I')
						new_y += 1;
					break;
				}

				delay = 0;
			}

			if ((map[new_y][new_x] == ' ' || map[new_y][new_x] == 'O' || map[new_y][new_x] == 'P') ||
				(attack && (map[new_y][new_x] == '1'
					|| map[new_y][new_x] == '2' || map[new_y][new_x] == '3' || map[new_y][new_x] == '4'))) {
				if (map[new_y][new_x] == ' ' || map[new_y][new_x] == 'O')
					point++;

				if (map[new_y][new_x] == ' ') {
					sound.eat.play();
					map[new_y][new_x] == 'P';
				}

				if (map[new_y][new_x] == 'O') {
					sound.bonus.play();
					map[new_y][new_x] == 'P';
				}

				if (map[new_y][new_x] == 'O')
					attack = true;

				if (point == 153) {
					sound.winwin.play();
				}

				if ((map[new_y][new_x] == '1'
					|| map[new_y][new_x] == '2' || map[new_y][new_x] == '3' || map[new_y][new_x] == '4') && (attack = true)) {
					sound.pr_death.play();
				}

				map[y][x] = 'P';

				map[new_y][new_x] = 'C';

				x = new_x;
				y = new_y;
			}

			if (new_y == 9 && (new_x == 0 || new_x == 18)) {
				if (new_x == 0)
					new_x = 17;
				else
					new_x = 1;

				map[y][x] = 'P';
				map[new_y][new_x] = 'C';

				x = new_x;
				y = new_y;
			}
		}
	}
};

class Enemy {
public:
	int x[4] = { 1, 17 , 1, 17 }, y[4] = { 1, 1, 17, 17 };
	int new_x[4] = { 0, 0, 0, 0 }, new_y[4] = { 0, 0, 0, 0 };
	int turn[4] = { 1, 1, 1, 1 }, delay = 0;
	bool teleportation = false;
	bool remove[4];

	Enemy() {
		for (int i = 0; i < 4; i++)
			remove[i] = true;
	}

	void update() {
		if (teleportation) {
			for (int i = 0; i < 4; i++)
				map[y[i]][x[i]] = ' ';

			x[0] = 1; x[1] = 17; x[2] = 1; x[3] = 17;
			y[0] = 1; y[1] = 1; y[2] = 17; y[3] = 17;

			map[y[0]][x[0]] = '1';
			map[y[1]][x[1]] = '2';
			map[y[2]][x[2]] = '3';
			map[y[3]][x[3]] = '4';

			for (int i = 0; i < 4; i++) {
				new_x[i] = x[i];
				new_y[i] = y[i];
			}

			teleportation = false;
		}
		else {
			delay++;

			if (delay >= 300) {
				for (int i = 0; i < 4; i++) {
					turn[i] = rand() % 4 + 1;

					new_x[i] = x[i];
					new_y[i] = y[i];

					switch (turn[i])
					{
					case 1:
						if (map[y[i]][new_x[i] + 1] != 'I')
							new_x[i] += 1;
						break;
					case 2:
						if (map[y[i]][new_x[i] - 1] != 'I')
							new_x[i] -= 1;
						break;
					case 3:
						if (map[new_y[i] - 1][x[i]] != 'I')
							new_y[i] -= 1;
						break;
					case 4:
						if (map[new_y[i] + 1][x[i]] != 'I')
							new_y[i] += 1;
						break;
					}
				}

				delay = 0;
			}

			for (int i = 0; i < 4; i++)
				if (remove[i]) {
					if (map[new_y[i]][new_x[i]] == ' ' || map[new_y[i]][new_x[i]] == 'O' || map[new_y[i]][new_x[i]] == 'P' ||
						map[new_y[i]][new_x[i]] == 'C') {
						if (map[new_y[i]][new_x[i]] == 'P')
							map[y[i]][x[i]] = 'P';
						else if (map[new_y[i]][new_x[i]] == ' ')
							map[y[i]][x[i]] = ' ';
						else if (map[new_y[i]][new_x[i]] == 'O')
							map[y[i]][x[i]] = 'O';
						else if (map[new_y[i]][new_x[i]] == 'C')
							if (attack == false)
								player_live = false;
							else {
								map[new_y[i]][new_x[i]] == 'P';
								remove[i] = false;
							}
						if ((point < 153) && (!player_live) && (map[new_y[i]][new_x[i]] == 'C')) {
							sound.death.play();
						}

						if (i == 0)
							map[new_y[i]][new_x[i]] = '1';
						if (i == 1)
							map[new_y[i]][new_x[i]] = '2';
						if (i == 2)
							map[new_y[i]][new_x[i]] = '3';
						if (i == 3)
							map[new_y[i]][new_x[i]] = '4';

						x[i] = new_x[i];
						y[i] = new_y[i];
					}

					if (new_y[i] == 9 && (new_x[i] == 0 || new_x[i] == 18)) {
						if (new_x[i] == 0)
							new_x[i] = 17;
						else
							new_x[i] = 1;

						map[y[i]][x[i]] = 'P';

						if (i == 0)
							map[new_y[i]][new_x[i]] = '1';
						if (i == 1)
							map[new_y[i]][new_x[i]] = '2';
						if (i == 2)
							map[new_y[i]][new_x[i]] = '3';
						if (i == 3)
							map[new_y[i]][new_x[i]] = '4';

						x[i] = new_x[i];
						y[i] = new_y[i];
					}
				}
		}
	}
};

class Score {
public:
	Sprite three[3];
	int number[3], с_score;
	bool view[3];

	Score(Texture& image) {
		for (int i = 0; i < 3; i++) {
			three[i].setTexture(image);
			three[i].setPosition(10 + 22 * i, height * ts + 10);
			three[i].setTextureRect(IntRect(0, 0, 22, 45));

			number[i] = 0;
			с_score = 0;
		}
	}
	void update() {
		if (с_score < 10) {
			number[0] = с_score;

			view[0] = true;
			view[1] = false;
			view[2] = false;
		}
		else if (с_score >= 10 && с_score < 100) {
			number[0] = с_score / 10;
			number[1] = с_score % 10;

			view[0] = true;
			view[1] = true;
			view[2] = false;
		}
		else if (с_score >= 100 && с_score < 1000) {
			number[0] = с_score / 100;
			number[1] = (с_score / 10) % 10;
			number[2] = (с_score % 100) % 10;

			view[0] = true;
			view[1] = true;
			view[2] = true;
		}
		for (int i = 0; i < 3; i++)
			three[i].setTextureRect(IntRect(22 * number[i], 0, 22, 45));
	}
};

void delete_dots() {
	for (int i = 0; i <= 2; i++)
		for (int j = 0; j <= 2; j++) {
			for (int x = 0; x < 2; x++) {
				map[7 + j * 2][i + 16 * x] = 'P';

				map[9][3 + 12 * x] = 'P';
			}

			map[9][8 + j] = 'P';
		}

	for (int i = 0; i < 2; i++) {
		map[2][1 + 16 * i] = 'O';
		map[15][1 + 16 * i] = 'O';
	}
}

int main() {
	srand(time(0));
	RenderWindow start(VideoMode(width * ts, height * ts + 3 * ts), "start");

	SoundBuffer s1;
	s1.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/eat.ogg");
	Sound sound(s1);
	sound.setVolume(50);
	sound.setLoop(true);

	SoundBuffer s2;
	s2.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/death.ogg");
	Sound sound2(s2);
	sound.setVolume(50);
	sound2.setLoop(true);

	SoundBuffer s3;
	s3.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/winwin.ogg");
	Sound sound3(s3);
	sound.setVolume(50);
	sound3.setLoop(true);

	SoundBuffer s4;
	s4.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/bonus.ogg");
	Sound sound4(s4);
	sound.setVolume(50);
	sound4.setLoop(true);

	SoundBuffer s5;
	s5.loadFromFile("C:/Users/dasse/Desktop/звуки_пакман/pr_death.ogg");
	Sound sound5(s5);
	sound.setVolume(50);
	sound5.setLoop(true);

	Music music;
	music.openFromFile("C:/Users/dasse/Desktop/звуки_пакман/mus.ogg");
	music.setVolume(25);
	music.setLoop(true);
	music.play();

	Texture sprites;
	sprites.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/sprites.png");
	Sprite plat(sprites);

	Texture lose;
	lose.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/lose.png");
	Sprite loser(lose);
	loser.setPosition(100, 210);

	Texture win;
	win.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/win.png");
	Sprite winner(win);
	winner.setPosition(100, 210);

	Texture men;
	men.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/menu.png");
	Sprite m(men);
	m.setPosition(100, 210);

	Texture st;
	st.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/start.png");
	Sprite s(st);
	s.setPosition(190, 225);

	Player player;
	Enemy enemy;

	Texture score_points;
	score_points.loadFromFile("C:/Users/dasse/Desktop/Пакман/Paint/score_points.png");
	Score score(score_points);

	delete_dots();

	while (start.isOpen())
	{
		Event event;
		while (start.pollEvent(event))
		{
			if (event.type == Event::Closed)
				start.close();

			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left) {
					Vector2i pos = Mouse::getPosition(start);

					if (menu) {
						if (pos.x >= 190 && pos.x <= 280 &&
							pos.y >= 225 && pos.y <= 250) {
							menu = false;
							play = true;
						}
					}
				}

			if (point < 153 && player_live && play)
				if (event.type == Event::KeyPressed) {
					player.new_x = player.x;
					player.new_y = player.y;

					if (event.key.code == Keyboard::Right)
						player.turn = 1;
					if (event.key.code == Keyboard::Left)
						player.turn = 2;
					if (event.key.code == Keyboard::Up)
						player.turn = 3;
					if (event.key.code == Keyboard::Down)
						player.turn = 4;
				}
		}

		if ((point == 153 || !player_live) && time_to_new_game != 0) {
			time_to_new_game--;

			if (time_to_new_game == 0) {
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						if (map[i][j] == 'P')
							map[i][j] = ' ';

				player.teleportation = true;
				enemy.teleportation = true;

				delete_dots();

				point = 0;
				player_live = true;
				time_to_new_game = 3000;
				ghost_time = 10000;
				attack = false;
			}
		}

		score.с_score = point;

		if (attack) {
			ghost_time--;

			if (ghost_time == 0) {
				for (int i = 0; i < 4; i++) {
					enemy.remove[i] = true;
					if (i == 0)
						map[enemy.y[0]][enemy.x[0]] = '1';
					if (i == 1)
						map[enemy.y[1]][enemy.x[1]] = '2';
					if (i == 2)
						map[enemy.y[2]][enemy.x[2]] = '3';
					if (i == 3)
						map[enemy.y[3]][enemy.x[3]] = '4';
				}
				ghost_time = 10000;
				attack = false;
			}
		}

		if (!player_live) {
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					if (map[i][j] != 'I')
						map[i][j] = 'P';
		}

		if (point < 153 && player_live) {
			player.update();
			enemy.update();
		}
		score.update();
		start.clear(Color::White);

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				if (map[i][j] == 'I')
					plat.setTextureRect(IntRect(0, 0, ts, ts));
				if (map[i][j] == 'C')
					plat.setTextureRect(IntRect(ts * int(player.frame), ts * player.turn, ts, ts));
				if (map[i][j] == ' ')
					plat.setTextureRect(IntRect(ts, 0, ts, ts));
				if (map[i][j] == 'O')
					plat.setTextureRect(IntRect(ts * 2, 0, ts, ts));
				if (!attack) {
					if (map[i][j] == '1')
						plat.setTextureRect(IntRect(ts * 5, ts * enemy.turn[0], ts, ts));
					if (map[i][j] == '2')
						plat.setTextureRect(IntRect(ts * 5, ts * enemy.turn[1], ts, ts));
					if (map[i][j] == '3')
						plat.setTextureRect(IntRect(ts * 5, ts * enemy.turn[2], ts, ts));
					if (map[i][j] == '4')
						plat.setTextureRect(IntRect(ts * 5, ts * enemy.turn[3], ts, ts));
				}
				else {
					if (map[i][j] == '1')
						plat.setTextureRect(IntRect(ts * 5, 0, ts, ts));
					if (map[i][j] == '2')
						plat.setTextureRect(IntRect(ts * 5, 0, ts, ts));
					if (map[i][j] == '3')
						plat.setTextureRect(IntRect(ts * 5, 0, ts, ts));
					if (map[i][j] == '4')
						plat.setTextureRect(IntRect(ts * 5, 0, ts, ts));
				}
				if (map[i][j] == 'P')
					continue;

				plat.setPosition(j * ts, i * ts);
				start.draw(plat);
			}
		if (play) {
			if (point == 153)
				start.draw(winner);

			if (!player_live)
				start.draw(loser);

			for (int i = 0; i < 3; i++)
				if (score.view[i])
					start.draw(score.three[i]);
		}
		if (menu) {
			start.draw(s);
		}

		start.display();
	}
	return 0;
}