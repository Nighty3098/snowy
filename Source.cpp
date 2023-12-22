# include <time.h>
# include <SFML/Graphics.hpp>
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <string>
# include <cstring>

#define FONT "DeJaVuSans.ttf"

using namespace sf;
using namespace std;


struct Player {
	int total_clicks;
	int total_coins;
	int click_bonus;

	Player() {
		total_clicks = 0;
		total_coins = 0;
		click_bonus = 0;
	}
};

void click(Player&);
void update(Player&);


void click(Player& player, int m) {
	player.total_clicks = player.total_clicks + m;
	player.total_coins = player.total_coins + player.click_bonus;
}



int main() {
	RenderWindow win(VideoMode(400, 400), "Clicker");

	int q = 0;
	int w = 0;

	Font default_font;
	if (!default_font.loadFromFile(FONT)) {
		cout << "Cannot load default font" << endl;
	}

	Player p;

	Texture clicker;
	Sprite Sclicker;
	clicker.loadFromFile("launchpad.png");

	while (win.isOpen()) {
		Event event;
		while (win.pollEvent(event)) {

			if (event.type == Event::Closed) {
				win.close();
				break;
			}

			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Space) {
					click(p, 247898);

					if (p.total_clicks % 100 == 0) {
						p.click_bonus = p.click_bonus + 10;
					}
				}
			}

			if (event.type == Event::MouseButtonPressed) {
				Vector2i mouse_pos(event.mouseButton.x, event.mouseButton.y);
				Vector2f click_pos(static_cast<float> (event.mouseButton.x), (event.mouseButton.y));

				if (Sclicker.getGlobalBounds().contains(click_pos)) {
					click(p, 1);

					if (p.total_clicks % 100 == 0) {
						p.click_bonus = p.click_bonus + 10;
					}
				}

				q = event.mouseButton.x;
				w = event.mouseButton.y;
			}

		}

		win.clear(Color::White);

		Sclicker.setPosition(160.0f, 160.0f);
		Sclicker.setTexture(clicker);

		string stats = "Total clicks: ";
		stats.append(to_string(p.total_clicks));
		stats.append("\nTotal coins: ");
		stats.append(to_string(p.total_coins));
		stats.append("\nTotal bonus: ");
		stats.append(to_string(p.click_bonus));
		stats.append("\n");
		stats.append("\n");
		Text t(stats, default_font, 10);
		t.setFillColor(Color::Black);

		string coords = "\n\n\nClick coords: ";
		coords.append(to_string(q));
		coords.append(" | ");
		coords.append(to_string(w));
		Text s(coords, default_font, 10);
		s.setFillColor(Color::Black);

		win.draw(s);
		win.draw(Sclicker);
		win.draw(t);

		win.display();
	}
}
