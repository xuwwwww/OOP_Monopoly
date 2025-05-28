#include "Map.h"
#include "Monopoly.h"

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

	// 清除原有的地圖格子
	for (Tile* tile : map) {
		delete tile;
	}
	map.clear();
	names.clear();

	// 讀取 JSON 檔案內容
	std::string jsonContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	inputFile.close();

	try {
		// 解析 JSON 資料
		// 注意：此處使用簡易的 JSON 解析方法，實際專案中建議使用 JSON 解析庫
		// JSON 格式預期如下：
		/*
		{
			"tiles": [
				{
					"type": "START",
					"name": "起點",
					"price": 0,
					"level": 0
				},
				{
					"type": "PROPERTY",
					"name": "台北",
					"price": 200,
					"level": 0
				},
				...
			]
		}
		*/

		// 簡易解析 - 這裡僅作示範，實際應使用 JSON 庫如 nlohmann/json
		// 使用逐行讀取方式模擬 JSON 解析
		std::stringstream ss(jsonContent);
		std::string line;
		bool inTilesArray = false;

		int num = 0;

		while (std::getline(ss, line)) {
			// 跳過空白行和註解
			if (line.empty() || line.find("//") == 0) continue;

			// 開始解析 tiles 陣列
			if (line.find("\"tiles\"") != std::string::npos) {
				inTilesArray = true;
				continue;
			}

			// 不在 tiles 陣列中，跳過
			if (!inTilesArray) continue;

			// 結束 tiles 陣列
			if (line.find("]") != std::string::npos) {
				inTilesArray = false;
				continue;
			}

			// 解析 tile 物件
			if (line.find("{") != std::string::npos) {
				std::string typeStr = "";
				std::string nameStr = "";
				int price = 0;
				int level = 0;

				// 讀取 tile 物件屬性直到發現 }
				std::string objLine;
				while (std::getline(ss, objLine) && objLine.find("}") == std::string::npos) {
					// 解析 type
					if (objLine.find("\"type\"") != std::string::npos) {
						size_t start = objLine.find(":") + 1;
						size_t end = objLine.find(",", start);
						if (end == std::string::npos) end = objLine.length();
						typeStr = objLine.substr(start, end - start);
						// 移除引號和空白
						typeStr.erase(std::remove(typeStr.begin(), typeStr.end(), '\"'), typeStr.end());
						typeStr.erase(std::remove(typeStr.begin(), typeStr.end(), ' '), typeStr.end());
					}

					// 解析 name
					if (objLine.find("\"name\"") != std::string::npos) {
						size_t start = objLine.find(":") + 1;
						size_t end = objLine.find(",", start);
						if (end == std::string::npos) end = objLine.length();
						nameStr = objLine.substr(start, end - start);
						// 移除引號和空白
						nameStr.erase(std::remove(nameStr.begin(), nameStr.end(), '\"'), nameStr.end());
						nameStr.erase(std::remove(nameStr.begin(), nameStr.end(), ' '), nameStr.end());
					}

					// 解析 price
					if (objLine.find("\"price\"") != std::string::npos) {
						size_t start = objLine.find(":") + 1;
						size_t end = objLine.find(",", start);
						if (end == std::string::npos) end = objLine.length();
						std::string priceStr = objLine.substr(start, end - start);
						// 移除空白
						priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ' '), priceStr.end());
						try {
							price = std::stoi(priceStr);
						}
						catch (...) {
							std::cerr << "警告：價格格式錯誤 '" << priceStr << "'，使用預設值 0。\n";
						}
					}

					// 解析 level
					if (objLine.find("\"level\"") != std::string::npos) {
						size_t start = objLine.find(":") + 1;
						size_t end = objLine.find(",", start);
						if (end == std::string::npos) end = objLine.length();
						std::string levelStr = objLine.substr(start, end - start);
						// 移除空白
						levelStr.erase(std::remove(levelStr.begin(), levelStr.end(), ' '), levelStr.end());
						try {
							level = std::stoi(levelStr);
						}
						catch (...) {
							std::cerr << "警告：等級格式錯誤 '" << levelStr << "'，使用預設值 0。\n";
						}
					}
				}

				// 根據解析出的資料建立合適的 Tile 物件
				Tile* newTile = nullptr;
				if (typeStr == "START") {
					newTile = new StartTile(num);
				}
				else if (typeStr == "PROPERTY") {
					newTile = new PropertyTile(level, price, nameStr, num);
				}
				else if (typeStr == "SHOP") {
					newTile = new ShopTile(num);
				}
				else if (typeStr == "HOSPITAL") {
					newTile = new HospitalTile(num);
				}
				else if (typeStr == "CHANCE") {
					newTile = new ChanceTile(num);
				}
				else if (typeStr == "FATE") {
					newTile = new FateTile(num);
				}
				else if (typeStr == "GAME") {
					newTile = new MiniGameTile(num);
				}
				else {
					std::cerr << "警告：未知的格子類型 '" << typeStr << "'，已跳過。\n";
					continue;
				}

				// 將建立的格子添加到地圖中
				if (newTile) {
					addTile(newTile, nameStr);
				}

				num++;
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "錯誤：解析地圖檔時發生錯誤：" << e.what() << "。將使用預設地圖。\n";
		// 清除可能部分載入的地圖
		for (Tile* tile : map) {
			delete tile;
		}
		map.clear();
		names.clear();
		InitMap();
		return;
	}

	// 如果沒有成功載入任何格子，使用預設地圖
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
	const int SIDE_HEIGHT_LEFT = 6; // 左側邊的格子數 27–22
	const int CELL_WIDTH = 16;    // 每個格子的寬度
	const std::string RESET_COLOR = "\033[0m";

	//玩家位置對應玩家符號
	std::map<int, std::vector<std::string>> playerSymbols;
	char symbol = 'A';
	for (auto* p : players) {
		// std::string sym = "[" + p->GetColor().substr(0,1) + "]";
		std::string sym = "[" + std::string(1, symbol) + "]";
		std::string coloredSym = Monopoly::GetColorCode(p->GetColor()) + sym + "\033[0m";
		playerSymbols[p->GetPosition()].push_back(coloredSym);
		symbol++;
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
		if (map[idx]->getLevel() != 0)
			content += "(" + std::to_string(map[idx]->getLevel()) + ")";
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
			int visible_len = 0;

			for (auto& s : playerSymbols[idx]) {
				pstr += s;
				visible_len += 3;
			}

			// 自行補足空白（總寬度 CELL_WIDTH）
			int pad = CELL_WIDTH - visible_len;
			pstr += std::string(pad, ' ');
			ss << pstr;
		}
		else {
			ss << std::string(CELL_WIDTH, ' ');
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
	int left_idx = MAP_SIZE - 1; // 27
	int right_idx = GRID_WIDTH;   // 8
	for (int i = 0; i < SIDE_HEIGHT_LEFT; ++i) {
		bool draw_right = (i < SIDE_HEIGHT_RIGHT);

		// 名稱行
		std::cout << "|" << format_cell(left_idx--) << "|";
		std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
		if (draw_right) {
			std::cout << "|" << format_cell(right_idx++) << "|";
		}
		else {
			std::cout << std::string(CELL_WIDTH + 1, ' ');
		}
		std::cout << "\n";

		// 玩家行
		std::cout << "|" << format_players(left_idx + 1) << "|";
		std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
		if (draw_right) {
			std::cout << "|" << format_players(right_idx - 1) << "|";
		}
		else {
			std::cout << std::string(CELL_WIDTH + 1, ' ');
		}
		std::cout << "\n";

		// 中間橫線
		if (i < SIDE_HEIGHT_LEFT - 1) {
			std::cout << "+" << std::string(CELL_WIDTH, '-') << "+";
			std::cout << std::string((GRID_WIDTH - 2) * (CELL_WIDTH + 1) - 1, ' ');
			if (draw_right && i < SIDE_HEIGHT_RIGHT - 1) {
				std::cout << "+" << std::string(CELL_WIDTH, '-') << "+";
			}
			else {
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
	addTile(new StartTile(0), "START");               // 0
	addTile(new PropertyTile(0, 200, "USA", 1), "USA"); // 1
	addTile(new FateTile(2), "Fate");                // 2
	addTile(new PropertyTile(0, 200, "France", 3), "France"); // 3
	addTile(new ShopTile(4), "Item Shop");           // 4
	addTile(new PropertyTile(0, 200, "Japan", 5), "Japan"); // 5
	addTile(new PropertyTile(0, 200, "Mexico", 6), "Mexico"); // 6
	addTile(new PropertyTile(0, 200, "Sweden", 7), "Sweden"); // 7

	addTile(new PropertyTile(0, 200, "Germany", 8), "Germany"); // 8
	addTile(new PropertyTile(0, 200, "UK", 9), "UK");       // 9
	addTile(new ChanceTile(10), "Chance");             // 10
	addTile(new PropertyTile(0, 200, "Canada", 11), "Canada"); // 11
	addTile(new PropertyTile(0, 200, "Australia", 12), "Australia"); // 12
	addTile(new PropertyTile(0, 200, "Egypt", 13), "Egypt");   // 13

	addTile(new PropertyTile(0, 200, "Italy", 14), "Italy");   // 14
	addTile(new HospitalTile(15), "Hospital");         // 15
	addTile(new FateTile(16), "Fate");                // 16
	addTile(new PropertyTile(0, 200, "Spain", 17), "Spain");   // 17
	addTile(new PropertyTile(0, 200, "China", 18), "China");   // 18
	addTile(new PropertyTile(0, 200, "Turkey", 19), "Turkey"); // 19
	addTile(new PropertyTile(0, 200, "Korea", 20), "Korea");   // 20

	addTile(new PropertyTile(0, 200, "Brazil", 21), "Brazil"); // 21
	addTile(new PropertyTile(0, 200, "Uganda", 22), "Uganda"); // 22
	addTile(new PropertyTile(0, 200, "Russia", 23), "Russia"); // 23
	addTile(new ChanceTile(24), "Chance");             // 24
	addTile(new PropertyTile(0, 200, "India", 25), "India");   // 25
	addTile(new PropertyTile(0, 200, "England", 26), "England"); // 26
	addTile(new PropertyTile(0, 200, "Africa", 27), "Africa"); // 27
}
