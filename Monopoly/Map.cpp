#include "Map.h"
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>

Map::Map() {
	InitMap();
}

void Map::LoadMap(std::string path)
{
}

void Map::PrintMap(std::vector<Player*>& players)
{
	//配置
	const int MAP_SIZE = map.size();
	if (MAP_SIZE != 24) { //可以再換
		std::cerr << "錯誤：地圖打印邏輯目前僅支援 24 格地圖。" << std::endl;
		return; 
	}
	const int GRID_WIDTH = 7;  // 頂部/底部的格子數
	const int GRID_HEIGHT = 5; // 側邊的格子數
	const int CELL_WIDTH = 12; // 每個格子的寬度
	const std::string RESET_COLOR = "\\033[0m";

	//玩家位置 對應 玩家
	std::map<int, std::vector<std::string>> playerSymbols;
	for (const auto& player : players) {
		std::string symbol = "[" + player->GetColor().substr(0, 1) + "]"; //如[R]
		playerSymbols[player->GetPosition()].push_back(symbol);
	}

	//獲取格子顏色
	auto get_color_code = [&](int index) -> std::string {
		if (index < 0 || index >= MAP_SIZE) return RESET_COLOR;

		// 暫用Tile的color
		std::string color = map[index]->getColor();
		// 從Game::GetBackgroundColorCode而來
		if (color == "red")        return "\\033[41;37m";
		if (color == "green")      return "\\033[42;37m";
		if (color == "yellow")     return "\\033[43;30m";
		if (color == "blue")       return "\\033[44;37m";
		if (color == "cyan")       return "\\033[46;30m";
		if (color == "purple")     return "\\033[45;37m";
		if (color == "orange")     return "\\033[48;2;255;168;0;30m"; 
		if (color == "teal")       return "\\033[46;30m";
		if (color == "brown")      return "\\033[43;30m";
		if (color == "gray")       return "\\033[100;37m";
		return RESET_COLOR;
	};

	//輔助函數 : 格式化格子內容
	auto format_cell = [&](int index) {
		std::stringstream ss;
		if (index < 0 || index >= MAP_SIZE) {
			ss << std::setw(CELL_WIDTH) << std::setfill(' ') << "";
			return ss.str();
		}

		std::string colorCode = get_color_code(index);
		ss << colorCode;

		//內容: 編號和名稱
		std::string content = std::to_string(index) + " " + names[index];
		if (static_cast<int>(content.length()) > CELL_WIDTH - 1) {
			content = content.substr(0, CELL_WIDTH - 1); // 超出cell width就截斷
		}
		ss << std::left << std::setw(CELL_WIDTH - 1) << std::setfill(' ') << content << " "; //留一個空格

		ss << RESET_COLOR;
		return ss.str();
	};

	//輔助函數：格式化玩家標記
	auto format_players = [&](int index) {
		std::stringstream ss;
		if (playerSymbols.count(index)) {
			std::string p_str;
			for(size_t i = 0; i < playerSymbols[index].size(); ++i) {
				const auto& sym = playerSymbols[index][i];
				p_str += sym;
			}
			if (static_cast<int>(p_str.length()) > CELL_WIDTH) {
				p_str = p_str.substr(0, CELL_WIDTH); //截斷
			}
			ss << std::left << std::setw(CELL_WIDTH) << std::setfill(' ') << p_str;
		} else {
			ss << std::setw(CELL_WIDTH) << std::setfill(' ') << "";
		}
		return ss.str();
	};

	//------------------------------------繪製地圖----------------------------------------------
	//頂部邊框
	std::cout << "+";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
	std::cout << std::endl;

	// 頂部格子名稱
	std::cout << "|";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << format_cell(i) << "|";
	std::cout << std::endl;

	// 頂部格子玩家
	std::cout << "|";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << format_players(i) << "|";
	std::cout << std::endl;

	// 頂部下方邊框 & 中間行上邊框
	std::cout << "+";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
	std::cout << std::endl;

	// 中間行 (左邊 + 空白 + 右邊)
	int left_idx = MAP_SIZE - 1; // 左側起始索引 如果是24就是23
	int right_idx = GRID_WIDTH; // 右側起始索引  同上，7
	for (int i = 0; i < GRID_HEIGHT - 2; ++i) {
		// 左側格子名稱
		std::cout << "|" << format_cell(left_idx) << "|";
		// 中間空白
		std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) -1 , ' ');
		// 右側格子名稱
		std::cout << "|" << format_cell(right_idx) << "|" << std::endl;

		// 左側格子玩家
		std::cout << "|" << format_players(left_idx--) << "|";
		// 中間空白
		std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1)-1, ' ');
		// 右側格子玩家
		std::cout << "|" << format_players(right_idx++) << "|" << std::endl;

		// 中間行邊框
        if (i < GRID_HEIGHT - 3) {
		    std::cout << "+" << std::string(CELL_WIDTH, '-') << "+";
            std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
		    std::cout << "+" << std::string(CELL_WIDTH, '-') << "+" << std::endl;
        }
	}

    // 底部上方邊框 & 中間行下邊框
	std::cout << "+";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
	std::cout << std::endl;


	// 底部格子名稱 18~12
	std::cout << "|";
	for (int i = GRID_WIDTH + GRID_HEIGHT - 3; i >= GRID_WIDTH + GRID_HEIGHT - 2 - (GRID_WIDTH-1) ; --i) {
        std::cout << format_cell(i) << "|";
    }
	std::cout << std::endl;

	// 底部格子玩家
	std::cout << "|";
	for (int i = GRID_WIDTH + GRID_HEIGHT - 3; i >= GRID_WIDTH + GRID_HEIGHT - 2 - (GRID_WIDTH-1) ; --i) {
        std::cout << format_players(i) << "|";
    }
	std::cout << std::endl;

	// 底部邊框
	std::cout << "+";
	for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
	std::cout << std::endl;
}

std::pair<std::vector<Tile*>, std::vector<std::string>> Map::getMap()
{
	return std::make_pair(map, names);
}

void Map::addTile(Tile* tile, std::string name)
{
	map.push_back(tile);
	names.push_back(name);
}

Tile* Map::GetTileAt(int pos)
{
	return map[pos];
}

int Map::getSize()
{
	return map.size();
}

void Map::InitMap()
{
	addTile(new StartTile(), "起點");
	addTile(new PropertyTile(0, 200, "台北"), "台北");
	addTile(new ShopTile(), "商店");
	addTile(new PropertyTile(0, 200, "新北"), "新北");
	addTile(new PropertyTile(0, 200, "桃園"), "桃園");
	addTile(new HospitalTile(), "醫院");
	addTile(new PropertyTile(0, 200, "新竹"), "新竹");
	addTile(new PropertyTile(0, 200, "苗栗"), "苗栗");
	addTile(new PropertyTile(0, 200, "台中"), "台中");
	addTile(new MiniGameTile(), "遊戲");
	addTile(new PropertyTile(0, 200, "彰化"), "彰化");
	addTile(new PropertyTile(0, 200, "南投"), "南投");
	addTile(new PropertyTile(0, 200, "雲林"), "雲林");
	addTile(new FateTile(), "命運");
	addTile(new PropertyTile(0, 200, "嘉義"), "嘉義");
	addTile(new PropertyTile(0, 200, "台南"), "台南");
	addTile(new ChanceTile(), "機會");
	addTile(new PropertyTile(0, 200, "高雄"), "高雄");
	addTile(new PropertyTile(0, 200, "屏東"), "屏東");
	addTile(new MiniGameTile(), "遊戲");
	addTile(new PropertyTile(0, 200, "台東"), "台東");
	addTile(new PropertyTile(0, 200, "花蓮"), "花蓮");
	addTile(new PropertyTile(0, 200, "宜蘭"), "宜蘭");
}
