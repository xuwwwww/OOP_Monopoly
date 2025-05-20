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
	std::string GetName()  override { return "Control Dice"; }
	std::string GetDescription()  override { return "Choose your own dice roll value."; }
};

// RocketCard
class RocketCard : public Item {
public:
	void Use(Player* p) override;
	std::string GetName() override { return "Rocket Card"; }
	std::string GetDescription() override { return "Send a player to hospital for two turns."; }
};

// FateCard
class FateCard : public Item {
public:
	void Use(Player* p) override;
	std::string GetName() override { return "Fate Card"; }
	std::string GetDescription() override { return "Force trigger a fate event."; }
};
