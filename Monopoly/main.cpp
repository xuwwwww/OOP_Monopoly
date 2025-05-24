#include <iostream>

#include "Game.h"
#include "Monopoly.h"

void ShowWaveLogo() {
	std::vector<std::string> logo[2] = {
		{
		"שÝשששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששß",
		"שר  __  __       _   _       ____        _          שר",
		"שר |  \\/  | ___ | \\ | | ___ |  _ \\  ___ | | __   __ שר",
		"שר | |\\/| |/ _ \\|  \\| |/ _ \\| |_) |/ _ \\| | \\ \\ / / שר",
		"שר | |  | | | | | |\\  | | | |  __/| | | | |__\\ V /  שר",
		"שר |_|  |_| |_| |_| \\_| |_| |_|   | |_| |____|| |   שר",
		"שר         \\___/       \\___/       \\___/      |_|   שר",
		"שר                                                  שר",
		"שדשששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששו"
		},
		{
		"שÝשששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששß",
		"שר          ___         ___         ___     __   __ שר",
		"שר  __  __ / _ \\ _   _ / _ \\ ____  / _ \\ _  \\ \\ / / שר",
		"שר |  \\/  | | | | \\ | | | | |  _ \\| | | | |  \\ V /  שר",
		"שר | |\\/| | |_| |  \\| | |_| | |_) | |_| | |   | |   שר",
		"שר | |  | |\\___/| |\\  |\\___/|  __/ \\___/| |__ |_|   שר",
		"שר |_|  |_|     |_| \\_|     |_|         |____|      שר",
		"שר                                                  שר",
		"שדשששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששששו"
		}
	};

	std::string loadingText[] = {
		"                    ¹Cְ¸¸ü₪J₪₪   ",
		"                   ¹Cְ¸¸ü₪J₪₪.  ",
		"                  ¹Cְ¸¸ü₪J₪₪.. ",
		"                 ¹Cְ¸¸ü₪J₪₪..."
	};


	for (int i = 0; i < 10; i++) {
		system("cls");
		std::cout << "\n";

		if (i % 2 == 0) {
			for (int j = 0; j < logo[0].size(); j++) {
				std::cout << logo[0][j] << std::endl;
			}
		}
		else if (i % 2 == 1) {
			for (int j = 0; j < logo[1].size(); j++) {
				std::cout << logo[1][j] << std::endl;
			}
		}
		std::cout << "\n" << loadingText[i % 4] << std::endl;
		Monopoly::sleepMS(300);
	}
	Monopoly::sleepMS(1000);
}

int main() {
	Game game;
	Monopoly::game = &game;

	ShowWaveLogo();
	system("cls");

	// ְֻ¬d¬O§_¦³¦sְֹ
	if (game.HasSavedGame()) {
		std::cout << "µo²{₪W¦¸¥¼§¹¦¨×÷¹Cְ¸¶i«׳¡I" << std::endl;

		// ´£¨ׁ¿ן¶µ
		std::string question = "¬O§_­n¸ü₪J₪W¦¸×÷¹Cְ¸¶i«׳¡H";
		std::vector<std::string> options = {
			"¸ü₪Jֲֲ¹Cְ¸",
			"¶}©l·s¹Cְ¸"
		};

		int choice = Monopoly::GetUserChoice(question, options);

		if (choice == 0) {
			// ¸ü₪Jֲֲ¹Cְ¸
			if (game.LoadGame()) {
				std::cout << "ִ~ִע₪W¦¸×÷¹Cְ¸¡I" << std::endl;
				game.StartGame();
			}
			else {
				std::cout << "¸ü₪J¥¢±ׁ¡A±N¶}©l·s¹Cְ¸¡C" << std::endl;
				game.InitGame();
				game.StartGame();
			}
		}
		else {
			// ¶}©l·s¹Cְ¸«e§R°£ֲֲ¦sְֹ
			game.DeleteSaveGame();
			game.InitGame();
			game.StartGame();
		}
	}
	else {
		// ¨S¦³¦sְֹ¡A¥¿±`¶}©l¹Cְ¸
		game.InitGame();
		game.StartGame();
	}

	return 0;
}
