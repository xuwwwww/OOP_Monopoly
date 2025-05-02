#include "Tile.h"
#include "Player.h"

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
	}
};

StartTile::StartTile()
{
	color = "cyan";
}

void StartTile::OnLand(Player* p)
{
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
		std::string question = "抵達" + name + "！土地價格為 " + std::to_string(price) + " 元，是否要購買？";
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

		std::string question = "是否要花費 300 元升級土地？\n";
		std::vector<std::string> options = {
		"升級",
		"放棄"
		};
		int choice = GetUserChoice(level, question, options);

		if (choice == 0) {
			if (p->BuyProperty(price)) {
				std::cout << "\n升級成功！\n";
				Upgrade();
			}
			else {
				std::cout << "金錢不足，無法購買。\n";
			}
		}
	}
	else if (owner != p) {
		std::cout << "此土地為 " << owner->GetName() << " 的土地，支付 "
			<< price << " 給 " << owner->GetName() << "。\n";
		p->Pay(owner, price);
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
}

FateTile::FateTile()
{
	color = "purple";
}

void FateTile::OnLand(Player* p)
{
}

MiniGameTile::MiniGameTile()
{
	color = "brown";
}

void MiniGameTile::OnLand(Player* p)
{
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

