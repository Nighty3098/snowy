
# include <time.h>
# include <SFML/Graphics.hpp>

using namespace sf;

#define WIDTH 1024
#define HEIGHT 768

#define MAX_FLAKES 10000
#define SPAWN_RATE 0.1f

#define SPAWN_COUNT_MIN 40
#define SPAWN_COUNT_MAX 70

#define MIN_SCALE 0.4f
#define MAX_SCALE 1.5f

#define MIN_VSPEED 120.0f
#define MAX_VSPEED 150.0f

#define MIN_HSPEED -28.0f
#define MAX_HSPEED 28.0f

//#define MIN_ALPHA 128
#define MIN_ALPHA 150
#define MAX_ALPHA 155


struct Snowflake {
	bool draw = false;
	float x, y, scale, vspeed, hspeed;

	Color color;
};

RenderWindow* wnd;
Texture* tex, * bgTex, * bgNightTex;
Sprite* spr, * bgSpr;
Clock timer;
Snowflake flakes[MAX_FLAKES];

int fcnt = 0;  // current snowflake, for cycle
float spawnTimer = 0;
bool isDayTime = true;

bool isLightFlakes = true;

int rndInt(int from, int to) {
	return from + rand() % (to - from);      // return the random number int the range
}

float rndFloat(float from, float to) {
	return from + (float)rand() / (float)RAND_MAX * (to - from);       // return the random number int the range
}

void spawnFlakes(float dt) {
	for (int i = 0; i < rndInt(SPAWN_COUNT_MIN + 1, SPAWN_COUNT_MAX); i++) {
		flakes[fcnt].draw = true; // draw - method
		flakes[fcnt].scale = rndFloat(MIN_SCALE, MAX_SCALE);
		flakes[fcnt].y = 0;
		flakes[fcnt].x = rndFloat(0, WIDTH);
		flakes[fcnt].vspeed = rndFloat(MIN_VSPEED, MAX_VSPEED);
		flakes[fcnt].hspeed = rndFloat(MIN_HSPEED, MAX_HSPEED);
		flakes[fcnt].color = Color(255, 255, 255, rndInt(MIN_ALPHA, MAX_ALPHA));

		if (++fcnt == MAX_FLAKES) {
			fcnt = 0;
		}
	}
}

void updateFlakes(float dt) { // dt - delta time
	for (int i = 0; i < MAX_FLAKES; i++) {
		if (!flakes[i].draw) {       // if flakes doesnt draw
			continue;
		}
		flakes[i].x += flakes[i].hspeed * dt;
		flakes[i].y += flakes[i].vspeed * dt;

		if (flakes[i].x < -5 || flakes[i].x > WIDTH + 5 || flakes[i].y > HEIGHT + 5) {
			flakes[i].draw = false;
		}
	}
}

void update(float dt) {
	spawnTimer += dt;
	if (spawnTimer > SPAWN_RATE) {
		spawnTimer = 0;
		spawnFlakes(dt);
	}

	updateFlakes(dt);
}

void draw() {
	wnd->draw(*bgSpr);

	for (int i = 0; i < MAX_FLAKES; i++) {
		if (!flakes[i].draw) {       // if flakes doesnt draw
			continue;
		}

		spr->setPosition(Vector2f(flakes[i].x, flakes[i].y)); //  vector - metable array
		spr->setColor(flakes[i].color);
		spr->setScale(Vector2f(flakes[i].scale, flakes[i].scale));

		wnd->draw(*spr);
	}
}

void mainloop() {
	while (wnd->isOpen()) {
		float dt = timer.restart().asSeconds();
		update(dt);

		Event evt;
		while (wnd->pollEvent(evt)) {
			if (evt.type == Event::Closed) {
				wnd->close();
			}

			if (evt.type == Event::KeyReleased) {
				if (evt.key.code == Keyboard::N) {
					isDayTime = !isDayTime;
					bgSpr->setTexture(isDayTime ? *bgTex : *bgNightTex);
				}
			}
		}

		wnd->clear(Color::Black);
		draw();
		wnd->display();
	}
}

int main() {
	srand((unsigned)time(0));

	wnd = new RenderWindow(VideoMode(WIDTH, HEIGHT), "Snow");

	tex = new Texture();
	tex->loadFromFile("particle.png");

	bgTex = new Texture();
	bgTex->loadFromFile("bg.png");

	bgNightTex = new Texture();
	bgNightTex->loadFromFile("bg_night.png");

	spr = new Sprite();
	spr->setTexture(*tex, true);
	spr->setOrigin(Vector2f(2.5f, 2.5f));

	bgSpr = new Sprite();
	bgSpr->setTexture(*bgTex, true);

	mainloop();

	delete spr, bgSpr, bgNightTex, bgTex, wnd;

	return 0;
}
