#include "Player.h"

Player::Player(std::string _name, std::string _color, int _money)
{
	name = _name;
	color = _color;
	money = _money;
	position = 0;
}


std::string Player::GetName()
{
	return name;
}


std::string Player::GetColor()
{
	return color;
}


int Player::GetMoney()
{
	return money;
}


int Player::GetPosition()
{
	return position;
}


void Player::SetPosition(int pos)
{
	this->position = pos;
}


std::vector<Item*> Player::GetItem()
{
	return item;
}

void Player::UseItem(Item* item)
{
}

void Player::AddItem(Item* _item)
{
	item.push_back(_item);
}

void Player::Move(int steps, int mapSize)
{
	position = (position + steps) % mapSize;
}

bool Player::BuyProperty(int price)
{
	if (money < price) {
		std::cout << "財產不足，不得購買\n";
		return false;
	}
	else {
		money -= price;
		return true;
	}
}

void Player::Pay(Player* other, int amount)
{
	other->money += amount;
	this->money -= amount;
}

void Player::PrintStatus()
{
	std::cout << "玩家: " << name << " | ";
	std::cout << "位置: " << position << " | ";
	std::cout << "金錢: $" << money << std::endl;
}
