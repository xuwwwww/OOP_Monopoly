#include "Tile.h"
#include "Player.h"
#include "HorseRacing.h"
#include "SheLongMen.h"
#include "Map.h"
#include "Item.h"
#include "Monopoly.h"
#include "Game.h"

std::vector<std::vector<std::string>> houses = {
	{
		"   /\\",
		"  /  \\",
		" /____\\",
		" | [] |",
		" |____|\n"
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
		"/_|______|_\\\n"
	},
	{
		"  _______",
		" / _____ \\",
		"| |SHOP | |",
		"| |_____| |",
		"|  ___    |",
		"| |___|[] |",
		"|_________|\n"
	},
	{
		"  _______",
		" / _____ \\",
		"| | H + | |",
		"| |_____| |",
		"|   ___   |",
		"|  |___|  |",
		"|_________|\n"
	},
	{

	}
};

string getHouse(int level) {
	if (level < 0 || static_cast<size_t>(level) >= houses.size()) {
		std::cerr << "警告：PrintHouse 的 level 超出範圍 (" << level << ")\n";
		return "";
	}
	string result("");

	auto house = houses[level];
	for (size_t i = 0; i < house.size(); i++) {
		result += house[i] + '\n';
	}
	return result;
}


StartTile::StartTile(int n)
{
	color = "cyan";
	number = n;
}

std::string StartTile::GetName()
{
	return name;
}

void StartTile::OnLand(Player* p)
{
	Monopoly::bank.Pay(p, 200);
	std::cout << "經過起點格: 獎勵200元\n";
}

PropertyTile::PropertyTile(int _id, int _price, std::string _name, int n)
{
	color = "";
	level = _id;
	owner = NULL;
	name = _name;
	price = _price;
	number = n;
}

int PropertyTile::getLevel()
{
	return level;
}

std::string PropertyTile::GetName()
{
	return name;
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
	Monopoly::UpdateScreen();

	if (owner == NULL) {
		std::string question = "抵達 " + name + "！\n土地價格為 " + std::to_string(price) + " 元\n你現在有 " + std::to_string(p->GetMoney()) + " 元，是否要購買？";
		std::vector<std::string> options = {
		"購買",
		"放棄"
		};
		int choice = Monopoly::GetUserChoice(getHouse(0) + question, options, true);

		if (choice == 0) {
			if (p->BuyProperty(price)) {
				std::cout << "\n購買成功！\n";
				p->AddProperty(this);
				SetOwner(p);
				level++;
			}
			else {
				std::cout << "金錢不足，無法購買。\n";
			}
		}
	}
	else if (owner == p) {
		if (level > 2) {
			std::vector<std::string> sellOptions = {
				"否，保留土地",
				"是，升級土地",
			};
			std::string sellQuestion = "土地已升至最高級！\n是否要出售這塊土地？你可以獲得 150 元。";
			int sellChoice = Monopoly::GetUserChoice(getHouse(level - 1) + sellQuestion, sellOptions, true);

			if (sellChoice == 1) {
				p->SellProperty(this, 150);
				owner = nullptr;
				level = 0;
				std::cout << "\n土地已出售，獲得 150 元！\n";
				Monopoly::WaitForEnter();
			}
			// return;
		}
		else {
			std::string question = "是否要花費 300 元升級土地？\n你現在有 "
				+ std::to_string(p->GetMoney()) + " 元"
				+ "或是出售這塊土地可以獲得 150 元。";
			std::vector<std::string> options = {
			"升級土地",
			"放棄",
			"出售土地（獲得 150 元）"
			};
			int choice = Monopoly::GetUserChoice(getHouse(level - 1) + question, options, true);

			if (choice == 0) {
				if (p->BuyProperty(300)) {
					std::cout << "\n升級成功！\n";
					Upgrade();
				}
				else {
					std::cout << "金錢不足，無法升級。\n";
				}
			}
			else if (choice == 1) {
				return;
			}
			else if (choice == 2) {
				p->SellProperty(this, 150);
				owner = nullptr;
				level = 0;
				std::cout << "\n土地已出售，獲得 150 元！\n";
			}
		}
	}
	else if (owner != p) {
		int rent = price;
		if (level == 1) rent = price * 2;
		else if (level == 2) rent = price * 3;

		p->Pay(owner, rent);

		std::cout << "此土地為 " << owner->GetName() << " 的土地，需支付 "
			<< rent << " 元給 " << owner->GetName() << "。\n";
		std::cout << "支付後剩餘 " << p->GetMoney() << " 元\n";
	}
}


ShopTile::ShopTile(int n)
{
	color = "teal";
	number = n;
}

void ShopTile::OnLand(Player* p)
{
	std::cout << "玩家 " << p->GetName() << " 進入商店\n";

	std::string question = "抵達商店！是否進入商店？";
	std::vector<std::string> options = {
		"是",
		"否"
	};

	int choice = Monopoly::GetUserChoice(getHouse(3) + question, options, true);
	if (choice == 1) return;

	// 商品列表
	std::vector<std::string> itemNames = {
		"控制骰子，300元",
		"火箭卡，300元",
		"命運卡，300元",
		"摧毀卡，300元"
	};
	std::vector<int> itemPrices = {
		300, 300, 300, 300
	};
	std::vector<Item*> items = {
		new ControlDiceItem(),
		new RocketCard(),
		new FateCard(),
		new DestroyCard()
	};

	do {
		std::vector<std::string> goods;

		for (size_t i = 0; i < itemNames.size(); ++i) {
			if (!items[i])
				goods.push_back(itemNames[i] + "（已購買）");
			else
				goods.push_back(itemNames[i]);
		}

		goods.push_back("離開");

		std::string welcome = "歡迎光臨商店!";
		choice = Monopoly::GetUserChoice(getHouse(3) + welcome, goods, true);

		// 檢查是否選擇離開
		if (choice == static_cast<int>(goods.size()) - 1) break;

		// 確保索引有效
		if (choice >= 0 && choice < itemNames.size()) {
			if (!items[choice]) {
				std::cout << "此商品已經購買過，無法再次購買。\n";
			}
			else if (p->BuyItem(itemPrices[choice])) {
				p->AddItem(items[choice]);
				items[choice] = nullptr;
				std::cout << "購買成功！\n";
				break;
			}
			else {
				std::cout << "餘額不足，購買失敗。\n";
			}
		}
		Monopoly::WaitForEnter();
	} while (true);

	for (int i = 0; i < items.size(); ++i) {
		if (items[i]) delete items[i];
	}
}

HospitalTile::HospitalTile(int n)
{
	number = n;
	color = "gray";
}

void HospitalTile::OnLand(Player* p)
{
	Monopoly::UpdateScreen();

	std::cout << getHouse(4);
	if (!p->inHospital) {
		std::cout << "進醫院休息三天！";
		p->hospitalDay = 3;
		p->inHospital = true;
	}
	else {
		p->hospitalDay--;
	}
}

ChanceTile::ChanceTile(int n)
{
	number = n;
	color = "orange";
}

void ChanceTile::OnLand(Player* p)
{
	Monopoly::GetUserChoice(getHouse(5) + "機會格，來抽取機會吧!!!", { "抽" }, true);
	char chance = rand() % 9;

	std::vector<PropertyTile*> ownedProperties;
	std::vector<std::string> propertyNames;
	HorseRacing miniGame1;
	SheLongMen miniGame2;
	Map* gameMap = Monopoly::game->gameMap;

	switch (chance) {
	case 0:
		std::cout << "意外走進賭馬場\n";
		Monopoly::WaitForEnter();
		miniGame1.init(p);
		miniGame1.gameStart();
		break;
	case 1:
		std::cout << "受傷暫停行動1次\n";
		p->inHospital = true;
		p->hospitalDay = 1;
		break;
	case 2:
		std::cout << "生病住院2回合\n";
		p->inHospital = true;
		p->hospitalDay = 2;
		break;
	case 3:
		std::cout << "中了樂透小獎，獲得 300 元\n";
		Monopoly::bank.Pay(p, 300);
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
		}
		else {
			propertyNames.push_back("放棄升級");
			int choice = Monopoly::GetUserChoice(getHouse(4) + "選擇要免費升級的土地：", propertyNames, true);

			if (choice < static_cast<int>(ownedProperties.size())) {
				PropertyTile* selectedProperty = ownedProperties[choice];
				selectedProperty->Upgrade();
				std::cout << "土地升級成功！\n";
			}
			else {
				std::cout << "你選擇放棄免費升級的機會。\n";
			}
		}
		break;
	case 5:
		std::cout << "獲得銀行補助金 150 元\n";
		Monopoly::bank.Pay(p, 150);
		break;
	case 6:
		std::cout << "違反交通規則，罰款 100 元\n";
		p->Pay(&Monopoly::bank, 100);
		break;
	case 7:
		std::cout << "食物中毒，住院治療，暫停行動1回合\n";
		p->inHospital = true;
		p->hospitalDay = 1;
		break;
	case 8:
		std::cout << "意外走進賭場\n";
		Monopoly::WaitForEnter();
		miniGame2.init(p);
		miniGame2.gameStart();
		break;
	}
}

FateTile::FateTile(int n)
{
	number = n;
	color = "purple";
}

void FateTile::OnLand(Player* p)
{
	Monopoly::GetUserChoice(getHouse(5) + "命運格，來抽取命運吧!!!", { "抽" }, true);
	char fate = rand() % 10;

	switch (fate) {
	case 0:
		std::cout << "地上撿到錢包，失主贈100元\n";
		Monopoly::bank.Pay(p, 100);
		break;
	case 1:
		std::cout << "耳機掉到水溝，損失100元\n";
		p->Pay(&Monopoly::bank, 100);
		break;
	case 2:
		std::cout << "感冒了，需要休息一回合\n";
		p->inHospital = true;
		p->hospitalDay = 1;
		break;
	case 3:
		std::cout << "踩到釘子，去醫院打破傷風針，暫停行動1回合\n";
		p->inHospital = true;
		p->hospitalDay = 1;
		break;
	case 4:
		std::cout << "投資股票獲利，獲得 200 元\n";
		Monopoly::bank.Pay(p, 200);
		break;
	case 5:
		std::cout << "所得稅申報，需繳納 150 元\n";
		p->Pay(&Monopoly::bank, 150);
		break;
	case 6:
		std::cout << "朋友還錢，獲得 120 元\n";
		Monopoly::bank.Pay(p, 120);
		break;
	case 7:
		std::cout << "手機故障維修，花費 80 元\n";
		p->Pay(&Monopoly::bank, 80);
		break;
	case 8:
		std::cout << "生日收到禮金，獲得 50 元\n";
		Monopoly::bank.Pay(p, 50);
		break;
	case 9:
		std::cout << "突發高燒，需要住院，暫停行動1回合\n";
		p->inHospital = true;
		p->hospitalDay = 1;
		break;
	}
}

MiniGameTile::MiniGameTile(int n)
{
	number = n;
	color = "brown";
}

void MiniGameTile::OnLand(Player* p)
{
	int choice = Monopoly::GetUserChoice(getHouse(5) + "遊玩小遊戲", {
		"賭馬",
		"射龍門"
		}, true);
	HorseRacing miniGame1;
	SheLongMen miniGame2;
	switch (choice) {
	case 0:
		miniGame1.init(p);
		miniGame2.gameStart();
		break;
	case 1:
		miniGame2.init(p);
		miniGame2.gameStart();
		break;
	}
}

std::string Tile::getColor()
{
	return color;
}

std::string Tile::GetName()
{
	return std::string();
}

void Tile::OnLand(Player* p)
{
}

int Tile::getLevel()
{
	return 0;
}

//void PrintHouse(int level) {
//	if (level < 0 || static_cast<size_t>(level) >= houses.size()) {
//		std::cerr << "警告：PrintHouse 的 level 超出範圍 (" << level << ")\n";
//		return;
//	}
//	auto house = houses[level];
//	for (size_t i = 0; i < house.size(); i++) {
//		std::cout << house[i] << std::endl;
//	}
//	std::cout << std::endl;
//}



