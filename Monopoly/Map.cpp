#include "Map.h"
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

Map::Map() {
	InitMap();
}

void Map::LoadMap(std::string path)
{
	std::ifstream inputFile(path);
	if (!inputFile.is_open()) {
		std::cerr << "錯誤：無法開啟地圖設定檔 '" << path << "'。將使用預設地圖。\n";
		InitMap();
		return;
	}

	for (Tile* tile : map) {
		delete tile;
	}
	map.clear();
	names.clear();

	std::string line;
	int lineNumber = 0;
	while (std::getline(inputFile, line)) {
		lineNumber++;
		if (lineNumber == 1) continue;
		if (line.empty() || line[0] == '#') continue;

		std::stringstream ss(line);
		std::string segment;
		std::vector<std::string> parts;

		while(std::getline(ss, segment, ',')) {
			parts.push_back(segment);
		}

		if (parts.size() < 2) {
			std::cerr << "警告：地圖檔第 " << lineNumber << " 行格式錯誤，已跳過。\n";
			continue;
		}

		std::string typeStr = parts[0];
		std::string nameStr = parts[1];
		int price = 0;
		int level = 0;

		if (parts.size() >= 3) {
			try {
				price = std::stoi(parts[2]);
			} catch (const std::invalid_argument&) {
				std::cerr << "警告：地圖檔第 " << lineNumber << " 行價格格式錯誤，使用預設值 0。\n";
			} catch (const std::out_of_range&) {
				std::cerr << "警告：地圖檔第 " << lineNumber << " 行價格超出範圍，使用預設值 0。\n";
			}
		}
		if (parts.size() >= 4) {
			try {
				level = std::stoi(parts[3]);
			} catch (const std::invalid_argument&) {
				std::cerr << "警告：地圖檔第 " << lineNumber << " 行等級格式錯誤，使用預設值 0。\n";
			} catch (const std::out_of_range&) {
				std::cerr << "警告：地圖檔第 " << lineNumber << " 行等級超出範圍，使用預設值 0。\n";
			}
		}

		Tile* newTile = nullptr;
		if (typeStr == "START") {
			newTile = new StartTile();
		} else if (typeStr == "PROPERTY") {
			newTile = new PropertyTile(level, price, nameStr);
		} else if (typeStr == "SHOP") {
			newTile = new ShopTile();
		} else if (typeStr == "HOSPITAL") {
			newTile = new HospitalTile();
		} else if (typeStr == "CHANCE") {
			newTile = new ChanceTile();
		} else if (typeStr == "FATE") {
			newTile = new FateTile();
		} else if (typeStr == "GAME") {
			newTile = new MiniGameTile();
		} else {
			std::cerr << "警告：地圖檔第 " << lineNumber << " 行類型 '/" << typeStr << "/' 無效，已跳過。\n";
			continue;
		}

		if (newTile) {
			addTile(newTile, nameStr);
		}
	}

	inputFile.close();

	if (map.empty()) {
		std::cerr << "警告：地圖檔未成功載入任何格子，將使用預設地圖。\n";
		InitMap();
	}
}

void Map::PrintMap(std::vector<Player*>& players)
{
    const int MAP_SIZE = map.size();
    if (MAP_SIZE != 28) {
        std::cerr << "錯誤：地圖打印邏輯目前僅支援 28 格地圖 (實際大小: " << MAP_SIZE << ")。\n";
        for (size_t i = 0; i < map.size(); ++i) std::cout << i << ": " << names[i] << std::endl;
        return;
    }
    const int GRID_WIDTH = 8;      // 頂部/底部的格子數 0–7
    const int SIDE_HEIGHT_RIGHT = 6; // 右側邊的格子數 8–13
    const int SIDE_HEIGHT_LEFT  = 6; // 左側邊的格子數 27–22
    const int CELL_WIDTH = 12;    // 每個格子的寬度
    const std::string RESET_COLOR = "\033[0m";

    //玩家位置對應玩家符號
    std::map<int, std::vector<std::string>> playerSymbols;
    for (auto* p : players) {
        std::string sym = "[" + p->GetColor().substr(0,1) + "]";
        playerSymbols[p->GetPosition()].push_back(sym);
    }

    //顏色
    auto get_color_code = [&](int idx) -> std::string {
        if (idx < 0 || idx >= MAP_SIZE) return RESET_COLOR;
        std::string c = map[idx]->getColor();
        if (c == "red")    return "\033[41;37m";
        if (c == "green")  return "\033[42;37m";
        if (c == "yellow") return "\033[43;30m";
        if (c == "blue")   return "\033[44;37m";
        if (c == "cyan")   return "\033[46;30m";
        if (c == "purple") return "\033[45;37m";
        if (c == "orange") return "\033[48;2;255;168;0;30m";
        if (c == "teal")   return "\033[46;30m";
        if (c == "brown")  return "\033[43;30m";
        if (c == "gray")   return "\033[100;37m";
        return RESET_COLOR;
    };

    //格式化
    auto format_cell = [&](int idx) {
        std::stringstream ss;
        if (idx < 0 || idx >= MAP_SIZE) {
            ss << std::setw(CELL_WIDTH) << std::setfill(' ') << "";
            return ss.str();
        }
        std::string colorCode = get_color_code(idx);
        ss << colorCode;
        std::string content = std::to_string(idx) + " " + names[idx];
        if ((int)content.length() > CELL_WIDTH - 1)
            content = content.substr(0, CELL_WIDTH - 1);
        ss << std::left << std::setw(CELL_WIDTH - 1) << std::setfill(' ') << content << " ";
        ss << RESET_COLOR;
        return ss.str();
    };

    //格式化玩家符號
    auto format_players = [&](int idx) {
        std::stringstream ss;
        if (playerSymbols.count(idx)) {
            std::string pstr;
            for (auto& s : playerSymbols[idx]) pstr += s;
            if ((int)pstr.length() > CELL_WIDTH)
                pstr = pstr.substr(0, CELL_WIDTH);
            ss << std::left << std::setw(CELL_WIDTH) << std::setfill(' ') << pstr;
        } else {
            ss << std::setw(CELL_WIDTH) << std::setfill(' ') << "";
        }
        return ss.str();
    };

    //------------------------------------繪製地圖----------------------------------------------
    //頂部邊框
    std::cout << "+";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
    std::cout << "\n";

    //頂部格子名稱 (0–7)
    std::cout << "|";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << format_cell(i) << "|";
    std::cout << "\n";

    //頂部格子玩家
    std::cout << "|";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << format_players(i) << "|";
    std::cout << "\n";

    //頂部下方邊框
    std::cout << "+";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
    std::cout << "\n";

    //中間行 (左側 27–22 + 空白 + 右側 8–13)
    int left_idx  = MAP_SIZE - 1; // 27
    int right_idx = GRID_WIDTH;   // 8
    for (int i = 0; i < SIDE_HEIGHT_LEFT; ++i) {
        bool draw_right = (i < SIDE_HEIGHT_RIGHT);

        // 名稱行
        std::cout << "|" << format_cell(left_idx--) << "|";
        std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
        if (draw_right) {
            std::cout << "|" << format_cell(right_idx++) << "|";
        } else {
            std::cout << std::string(CELL_WIDTH + 1, ' ');
        }
        std::cout << "\n";

        // 玩家行
        std::cout << "|" << format_players(left_idx + 1) << "|";
        std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
        if (draw_right) {
            std::cout << "|" << format_players(right_idx - 1) << "|";
        } else {
            std::cout << std::string(CELL_WIDTH + 1, ' ');
        }
        std::cout << "\n";

        // 中間橫線
        if (i < SIDE_HEIGHT_LEFT - 1) {
            std::cout << "+" << std::string(CELL_WIDTH, '-') << "+";
            std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
            if (draw_right && i < SIDE_HEIGHT_RIGHT - 1) {
                std::cout << "+" << std::string(CELL_WIDTH, '-') << "+";
            } else {
                std::cout << std::string(CELL_WIDTH + 1, ' ');
            }
            std::cout << "\n";
        }
    }

    // 底部上方邊框
    std::cout << "+";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
    std::cout << "\n";

    // 底部格子名稱 (21 → 14)
    std::cout << "|";
    for (int i = 21; i >= 14; --i) {
        std::cout << format_cell(i) << "|";
    }
    std::cout << "\n";

    // 底部格子玩家
    std::cout << "|";
    for (int i = 21; i >= 14; --i) {
        std::cout << format_players(i) << "|";
    }
    std::cout << "\n";

    // 底部邊框
    std::cout << "+";
    for (int i = 0; i < GRID_WIDTH; ++i) std::cout << std::string(CELL_WIDTH, '-') << "+";
    std::cout << "\n";
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
	// 清除可能存在的舊地圖資料 (如果 InitMap 被意外呼叫多次)
	for (Tile* tile : map) {
		delete tile;
	}
	map.clear();
	names.clear();

	// 依照圖片順序建立 28 個格子
	addTile(new StartTile(), "START");               // 0
	addTile(new PropertyTile(0, 200, "USA"), "USA"); // 1
	addTile(new FateTile(), "Fate");                // 2
	addTile(new PropertyTile(0, 200, "France"), "France"); // 3
	addTile(new ShopTile(), "Item Shop");           // 4
	addTile(new PropertyTile(0, 200, "Japan"), "Japan"); // 5
	addTile(new PropertyTile(0, 200, "Mexico"), "Mexico"); // 6
	addTile(new PropertyTile(0, 200, "Sweden"), "Sweden"); // 7

	addTile(new PropertyTile(0, 200, "Germany"), "Germany"); // 8
	addTile(new PropertyTile(0, 200, "UK"), "UK");       // 9
	addTile(new ChanceTile(), "Chance");             // 10
	addTile(new PropertyTile(0, 200, "Canada"), "Canada"); // 11
	addTile(new PropertyTile(0, 200, "Australia"), "Australia"); // 12
	addTile(new PropertyTile(0, 200, "Egypt"), "Egypt");   // 13

	addTile(new PropertyTile(0, 200, "Italy"), "Italy");   // 14
	addTile(new HospitalTile(), "Hospital");         // 15
	addTile(new FateTile(), "Fate");                // 16
	addTile(new PropertyTile(0, 200, "Spain"), "Spain");   // 17
	addTile(new PropertyTile(0, 200, "China"), "China");   // 18
	addTile(new PropertyTile(0, 200, "Turkey"), "Turkey"); // 19
	addTile(new PropertyTile(0, 200, "Korea"), "Korea");   // 20

	addTile(new PropertyTile(0, 200, "Brazil"), "Brazil"); // 21
	addTile(new PropertyTile(0, 200, "Uganda"), "Uganda"); // 22
	addTile(new PropertyTile(0, 200, "Russia"), "Russia"); // 23
	addTile(new ChanceTile(), "Chance");             // 24
	addTile(new PropertyTile(0, 200, "India"), "India");   // 25
	addTile(new PropertyTile(0, 200, "England"), "England"); // 26
	addTile(new PropertyTile(0, 200, "Africa"), "Africa"); // 27
}
