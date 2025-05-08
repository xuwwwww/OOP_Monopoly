#include "Tile.h"
#include "Player.h"
#include "HorseRacing.h"
#include "Map.h"
#include <conio.h>
#include <string>

std::vector<std::vector<std::string>> houses = {
	{
		"   /\\",
		"  /  \\",
		" /____\\",
		" | [] |",
		" |____|"
	},
	{
		"  _____",
		" /    /\\",
		"/____/__\\",
		"| [] [] |",
		"|_______|"
	},
	{
		"   /\\  /\\  ",
		"  /__\\/__\\ ",
		"  | [] [] |",
		"  |  __   |",
		" /| |__| |\\ ",
		"/_|______|_\\"
	},
	{
		"  _______",
		" / _____ \\",
		"| |SHOP| |",
		"| |_____| |",
		"|  ___    |",
		"| |___|[] |",
		"|_________|"
	},
	{
		"  _______",
		" / _____ \\",
		"| | H + | |",
		"| |_____| |",
		"|   ___   |",
		"|  |___|  |",
		"|_________|"
	},
	{}
};

Player bank("bank", "black", 1 << 31);

StartTile::StartTile()
{
	color = "cyan";
}

void StartTile::OnLand(Player* p)
{
	bank.Pay(p, 200);
}

PropertyTile::PropertyTile(int _id, int _price, std::string _name)
{
	color = "";
	level = _id;
	owner = NULL;
	name = _name;
	price = _price;
}

int PropertyTile::GetPrice()
{
	return price;
}

int PropertyTile::GetLevel()
{
	return level;
}

void PropertyTile::Upgrade()
{
	level++;
}

Player* PropertyTile::GetOwner()
{
	return owner;
}

void PropertyTile::SetOwner(Player* p)
{
	owner = p;
	color = p->GetColor();
}

void PropertyTile::OnLand(Player* p)
{
	if (owner == NULL) {
		std::string question = "抵達" + name + "！\n土地價格為 " + std::to_string(price) + " 元\n你現在有 " + std::to_string(p->GetMoney()) + " 元\n是否要購買？";
		std::vector<std::string> options = {
		"購買",
		"放棄"
		};
		int choice = GetUserChoice(level, question, options);

		if (choice == 0) {
			if (p->BuyProperty(price)) {
				std::cout << "\n購買成功！\n";
				SetOwner(p);
			}
			else {
				std::cout << "金錢不足，無法購買。\n";
			}
		}
	}
	else if (owner == p) {
		if (level >= 2) {
			std::cout << "土地已升至最高級！\n";
			return;
		}

		std::string question = "是否要花費 300 元升級土地？\n你現在有 " + std::to_string(p->GetMoney()) + " 元";
		std::vector<std::string> options = {
		"升級",
		"放棄"
		};
		int choice = GetUserChoice(level, question, options);

		if (choice == 0) {
			if (p->BuyProperty(300)) {
				std::cout << "\n升級成功！\n";
				Upgrade();
			}
			else {
				std::cout << "金錢不足，無法升級。\n";
			}
		}
	}
	else if (owner != p) {
		int rent = price;
		if (level == 1) rent = price * 2;
		else if (level == 2) rent = price * 3;

		std::cout << "此土地為 " << owner->GetName() << " 的土地，需支付 "
			<< rent << " 元給 " << owner->GetName() << "。\n";
		std::cout << "你現在有 " << p->GetMoney() << " 元\n";
		
		p->Pay(owner, rent);
	}
}


ShopTile::ShopTile()
{
	color = "teal";
}

void ShopTile::OnLand(Player* p)
{
	std::string question = "抵達商店！是否進入商店？";
	std::vector<std::string> options = {
	"是",
	"否"
	};
	int choice = GetUserChoice(3, question, options);
	
	if (choice) return;

	std::string welcome = "歡迎光臨商店!";
	std::vector<std::string> goods = {
		"test1",
		"test2",
		"離開"
	};
	choice = GetUserChoice(3, welcome, goods);

	switch (choice) {
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}
}

HospitalTile::HospitalTile()
{
	color = "gray";
}

void HospitalTile::OnLand(Player* p)
{
	PrintHouse(4);
	if (!p->inHospital) {
		std::cout << "進醫院休息三天！";
		p->inHospital = true;
	}
	else {
		p->hosipitalDay++;
	}
}

ChanceTile::ChanceTile()
{
	color = "orange";
}

void ChanceTile::OnLand(Player* p)
{
	GetUserChoice(5, "機會格，來抽取機會吧!!!", { "抽" });
	char chance = rand() % 8;

	std::vector<PropertyTile*> ownedProperties;
	std::vector<std::string> propertyNames;
	HorseRacing miniGame;
	Map* gameMap = new Map();
	
	switch (chance) {
	case 0:
		std::cout << "意外走進賭馬場\n";
		WaitForEnter();
		miniGame.init(p);
		miniGame.gameStart();
		break;
	case 1:
		std::cout << "受傷暫停行動1次\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	case 2:
		std::cout << "生病住院2回合\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	case 3:
		std::cout << "中了樂透小獎，獲得 300 元\n";
		bank.Pay(p, 300);
		break;
	case 4:
		std::cout << "獲得免費升級一處土地的機會\n";
		
		for (int i = 0; i < 28; i++) {
			Tile* tile = gameMap->GetTileAt(i);
			PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
			if (propTile && propTile->GetOwner() == p) {
				if (propTile->GetLevel() < 2) {
					ownedProperties.push_back(propTile);
					std::string tileName = "第 " + std::to_string(i) + " 格的土地";
					propertyNames.push_back(tileName);
				}
			}
		}
		
		if (ownedProperties.empty()) {
			std::cout << "你沒有可升級的土地！\n";
		} else {
			propertyNames.push_back("放棄升級");
			int choice = GetUserChoice(4, "選擇要免費升級的土地：", propertyNames);
			
			if (choice < static_cast<int>(ownedProperties.size())) {
				PropertyTile* selectedProperty = ownedProperties[choice];
				selectedProperty->Upgrade();
				std::cout << "土地升級成功！\n";
			} else {
				std::cout << "你選擇放棄免費升級的機會。\n";
			}
		}
		break;
	case 5:
		std::cout << "獲得銀行補助金 150 元\n";
		bank.Pay(p, 150);
		break;
	case 6:
		std::cout << "違反交通規則，罰款 100 元\n";
		p->Pay(&bank, 100);
		break;
	case 7:
		std::cout << "食物中毒，住院治療，暫停行動1回合\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	}
	
	delete gameMap;
}

FateTile::FateTile()
{
	color = "purple";
}

void FateTile::OnLand(Player* p)
{
	GetUserChoice(5, "命運格，來抽取命運吧!!!", { "抽" });
	char fate = rand() % 10;

	switch (fate) {
	case 0:
		std::cout << "地上撿到錢包，失主贈100元\n";
		bank.Pay(p, 100);
		break;
	case 1:
		std::cout << "耳機掉到水溝，損失100元\n";
		p->Pay(&bank, 100);
		break;
	case 2:
		std::cout << "感冒了，需要休息一回合\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	case 3:
		std::cout << "踩到釘子，去醫院打破傷風針，暫停行動1回合\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	case 4:
		std::cout << "投資股票獲利，獲得 200 元\n";
		bank.Pay(p, 200);
		break;
	case 5:
		std::cout << "所得稅申報，需繳納 150 元\n";
		p->Pay(&bank, 150);
		break;
	case 6:
		std::cout << "朋友還錢，獲得 120 元\n";
		bank.Pay(p, 120);
		break;
	case 7:
		std::cout << "手機故障維修，花費 80 元\n";
		p->Pay(&bank, 80);
		break;
	case 8:
		std::cout << "生日收到禮金，獲得 50 元\n";
		bank.Pay(p, 50);
		break;
	case 9:
		std::cout << "突發高燒，需要住院，暫停行動1回合\n";
		p->inHospital = true;
		p->hosipitalDay = 0;
		break;
	}
}

MiniGameTile::MiniGameTile()
{
	color = "brown";
}

void MiniGameTile::OnLand(Player* p)
{
	int choice = GetUserChoice(5, "遊玩小遊戲", {
		"賭馬",
		"射龍門"
		});
	HorseRacing miniGame;
	switch (choice) {
	case 0:
		miniGame.init(p);
		miniGame.gameStart();
		break;
	case 1:
		break;
	}
}

std::string Tile::getColor()
{
	return color;
}

void Tile::OnLand(Player* p)
{
}

void Tile::PrintHouse(int level) {
	if (level < 0 || static_cast<size_t>(level) >= houses.size()) {
		std::cerr << "警告：PrintHouse 的 level 超出範圍 (" << level << ")\n";
		return;
	}
	auto house = houses[level];
	for (size_t i = 0; i < house.size(); i++) {
		std::cout << house[i] << std::endl;
	}
	std::cout << std::endl;
}

int Tile::GetUserChoice(int idx, const std::string question, const std::vector<std::string> options) {

	int selected = 0;

	while (true) {
		std::cout << "\033[2J\033[H";
		std::cout << question << "\n\n";
		PrintHouse(idx);
		for (size_t i = 0; i < options.size(); ++i) {
			if (static_cast<int>(i) == selected)
				std::cout << " > " << "【" << options[i] << "】" << "\n";
			else
				std::cout << "   " << "【" << options[i] << "】" << "\n";
		}

		int key = _getch();
		if (key == 224) {
			key = _getch();
			if (key == 72) selected = (selected - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
			if (key == 80) selected = (selected + 1) % static_cast<int>(options.size());
		}
		else if (key == '\r') {
			return selected;
		}
	}
}

