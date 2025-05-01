#include "Map.h"

Map::Map() {
	InitMap();
}

void Map::LoadMap(std::string path)
{
}

void Map::PrintMap(std::vector<Player*>& players)
{
	int N = map.size();
	int half = N / 2;

	for (int i = 0; i < half; ++i) {
		std::cout << "[" << i << ". " << names[i] << "] ";
	}
	std::cout << std::endl;

	for (int i = N - 1; i >= half; --i) {
		std::cout << "[" << i << ". " << names[i] << "] ";
	}
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
