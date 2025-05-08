#include "Player.h"
#include "Game.h"  // 新增 Game 類別

// 外部宣告，需在實作中使用 
extern Game* gameInstance; // 將在 Game 的實作中初始化

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
	// 檢查道具是否為空指針
	if (!item) {
		std::cout << "錯誤：嘗試使用無效的道具！\n";
		return;
	}

	// 檢查玩家是否擁有該道具
	bool hasItem = false;
	size_t itemIndex = 0;
	for (size_t i = 0; i < this->item.size(); ++i) {
		if (this->item[i] == item) {
			hasItem = true;
			itemIndex = i;
			break;
		}
	}

	if (!hasItem) {
		std::cout << "錯誤：玩家未擁有此道具！\n";
		return;
	}

	// 使用道具效果
	std::cout << "玩家 " << name << " 使用了 " << item->GetName() << "！\n";
	std::cout << "效果：" << item->GetDescription() << "\n";
	
	// 呼叫道具的實際效果函數
	item->Use(this);
	
	// 使用後從玩家的道具清單中移除
	this->item.erase(this->item.begin() + itemIndex);
	
	// 釋放道具記憶體空間
	delete item;
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
	// 特殊處理銀行收付
	if (other && other->GetName() == "bank") {
		// 支付給銀行，只需減少自己的金額
		if (money < amount) {
			// 注意：此處是"被動支付"，金額不足導致餘額小於0才算破產
			std::cout << name << " 資金不足，只能支付 " << money << " 元給銀行。\n";
			int oldMoney = money;
			money = 0; // 設為0，而不是負數
			std::cout << name << " 已支付全部金額 " << oldMoney << " 元並破產！\n";
			
			// 檢查遊戲是否應該結束（破產後只剩1名玩家）
			if (gameInstance) {
				gameInstance->CheckWinCondition();
			}
		} else {
			money -= amount;
		}
		return;
	}
	
	if (name == "bank") {
		// 銀行支付給玩家，不考慮銀行餘額
		if (other) {
			// 確保玩家收款不會溢位
			if (amount > 0 && other->money > INT_MAX - amount) {
				std::cout << "警告：金額過大，設定玩家資金為最大值。\n";
				other->money = INT_MAX;
			} else {
				other->money += amount;
			}
		}
		return;
	}
	
	// 一般玩家間交易 - 被動支付
	if (money < amount) {
		// 如果沒有足夠的錢，只支付剩餘的金額，且破產
		std::cout << name << " 資金不足，只能支付 " << money << " 元。\n";
		
		// 確保收款方不會溢位
		if (money > 0 && other && other->money > INT_MAX - money) {
			std::cout << "警告：對方金額將溢位，設定為最大值。\n";
			other->money = INT_MAX;
		} else if (other) {
			other->money += money;
		}
		
		money = 0; // 設為0，表示已用完所有金錢
		std::cout << name << " 無法完成支付，已破產！\n";
		
		// 當玩家破產時（且是因為付款給其他玩家），轉移所有地產給債主
		if (other && gameInstance) {
			gameInstance->HandlePlayerBankruptcy(this, other);
			
			// 破產後立即檢查遊戲是否應該結束（只剩一人）
			if (gameInstance->CheckWinCondition()) {
				gameInstance->EndGame();
			}
		}
	}
	else {
		// 有足夠的錢，正常支付
		if (amount > 0 && other) {
			// 確保收款方不會溢位
			if (other->money > INT_MAX - amount) {
				std::cout << "警告：對方金額將溢位，設定為最大值。\n";
				other->money = INT_MAX;
			} else {
				other->money += amount;
			}
		}
		
		money -= amount; // 支付方扣款
	}
}

void Player::PrintStatus()
{
	std::cout << "玩家: " << name << " | ";
	std::cout << "位置: " << position << " | ";
	std::cout << "金錢: $" << money << std::endl;
}
