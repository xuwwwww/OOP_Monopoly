#pragma once
#include <iostream>

class Player;

class Item {
public:
	virtual void Use(Player* p) = 0;
	virtual std::string GetName() = 0;
	virtual std::string GetDescription() = 0;
	virtual ~Item() = default;
};

// ControlDiceItem
class ControlDiceItem : public Item {

public:
	void Use(Player* p) override;
	std::string GetName()  override { return "控制骰子"; }
	std::string GetDescription()  override { return "選擇骰子點數前進。"; }
};
