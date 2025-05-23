#pragma once
#include <vector>
#include <string>

class Game;
class Player;


namespace Monopoly {
	extern Game* game;
	extern Player bank;

	enum class Color {
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37,
		Reset = 0
	};

	enum class BackgroundColor {
		Black = 40,
		Red = 41,
		Green = 42,
		Yellow = 43,
		Blue = 44,
		Magenta = 45,
		Cyan = 46,
		White = 47,
		Reset = 0
	};

	std::string GetColorCode(std::string colorName);
	std::string GetBackgroundColorCode(std::string color);
	void SetTextColor(const Color& color);
	void SetBackgroundColor(const BackgroundColor& color);
	void UpdateScreen();
	// withMap參數，true 顯示地圖和玩家， false 不顯示，想要有地圖用Monopoly::GetUserChoice(question, options, true);
	/**
	 * 顯示選項清單，根據上下按鍵切換選項。
	 * 
	 * \param question
	 * \param options
	 * \param withMap 是否顯示地圖
	 * \param cmd 是否能輸入指令
	 * \return 
	 */
	int GetUserChoice(const std::string& question, const std::vector<std::string>& options, bool withMap = false, bool cmd = false);
	void WaitForEnter();
	void clearScreen();
	/**
	 * 等待ms毫秒.
	 */
	void sleepMS(const int& ms);
	/**
	 * 等待s秒.
	 */
	void sleepS(const int& s);
	std::string tappingCommand();
}
