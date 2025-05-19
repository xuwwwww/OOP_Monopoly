#include "CommandHandler.h"
#include "Game.h"
#include "Monopoly.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../nlohmann/json.hpp"

// Using nlohmann json
using json = nlohmann::json;

CommandHandler::CommandHandler() : game(nullptr) {
}

void CommandHandler::Initialize() {
    // Try multiple possible paths for command.json
    std::vector<std::string> possiblePaths = {
        "../json/command.json",
        "json/command.json",
        "../../json/command.json",
        "../../../json/command.json",
        "./json/command.json"
    };
    
    bool fileOpened = false;
    std::ifstream file;
    
    for (const auto& path : possiblePaths) {
        file.open(path);
        if (file.is_open()) {
            std::cout << "Successfully opened command file at: " << path << std::endl;
            fileOpened = true;
            break;
        }
    }
    
    if (!fileOpened) {
        std::cerr << "Error: Could not open command.json file at any of the tried paths!" << std::endl;
        return;
    }

    try {
        json commandData;
        file >> commandData;

        // Parse commands from JSON
        commands.clear();
        for (const auto& cmd : commandData["commands"]) {
            Command command;
            command.name = cmd["name"];
            command.description = cmd["description"];
            command.usage = cmd["usage"];
            command.example = cmd["example"];
            commands.push_back(command);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing command.json: " << e.what() << std::endl;
    }
    
    file.close();
}

bool CommandHandler::ProcessCommand(std::shared_ptr<Player> player, const std::string& input) {
    if (input.empty() || input[0] != '/') {
        return false;
    }

    // Split the command and its arguments
    std::vector<std::string> parts = SplitCommand(input);
    if (parts.empty()) {
        return false;
    }

    // Extract command name (without the /)
    std::string cmdName = parts[0].substr(1);
    if (cmdName.empty()) {
        return false;
    }

    // Remove command from arguments
    parts.erase(parts.begin());

    bool commandResult = false;

    // Find and execute the appropriate command handler
    if (cmdName == "move") {
        commandResult = HandleMoveCommand(player, parts);
    }
    else if (cmdName == "get") {
        commandResult = HandleGetCommand(player, parts);
    }
    else if (cmdName == "give") {
        commandResult = HandleGiveCommand(player, parts);
    }
    else if (cmdName == "card") {
        commandResult = HandleCardCommand(player, parts);
    }
    else if (cmdName == "gamestate") {
        commandResult = HandleGameStateCommand(player, parts);
    }
    else if (cmdName == "info") {
        commandResult = HandleInfoCommand(player, parts);
    }
    else if (cmdName == "refresh") {
        commandResult = HandleRefreshCommand(player, parts);
    }
    else if (cmdName == "list" || cmdName == "help") {
        commandResult = HandleListCommand(player, parts);
    }
    else if (cmdName == "minigame") {
        commandResult = HandleMinigameCommand(player, parts);
    }
    else {
        std::cout << "Unknown command: " << cmdName << std::endl;
        return false;
    }

    // If command was successful and game reference is valid, save the game state
    if (commandResult && game) {
        game->SaveGame();
        std::cout << "Game state saved." << std::endl;
    }

    return commandResult;
}

std::vector<std::string> CommandHandler::GetCommandList(bool detailed) {
    std::vector<std::string> result;

    for (const auto& cmd : commands) {
        if (detailed) {
            std::string cmdInfo = "/" + cmd.name + " - " + cmd.description + "\n";
            cmdInfo += "    Usage: " + cmd.usage + "\n";
            cmdInfo += "    Example: " + cmd.example;
            result.push_back(cmdInfo);
        }
        else {
            result.push_back("/" + cmd.name + " - " + cmd.description);
        }
    }

    return result;
}

bool CommandHandler::IsValidCommand(const std::string& cmdName) {
    for (const auto& cmd : commands) {
        if (cmd.name == cmdName) {
            return true;
        }
    }
    return false;
}

void CommandHandler::SetGameReference(Game* gamePtr) {
    game = gamePtr;
}

std::vector<std::string> CommandHandler::SplitCommand(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        if (!token.empty()) {
            result.push_back(TrimString(token));
        }
    }

    return result;
}

std::string CommandHandler::TrimString(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](char c) { return std::isspace(c); });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](char c) { return std::isspace(c); }).base();
    return (start < end) ? std::string(start, end) : std::string();
}

// Command handler implementations
bool CommandHandler::HandleMoveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage: /move <position>" << std::endl;
        return false;
    }

    try {
        int position;
        if (args[0] == "to" && args.size() > 1) {
            // Find tile by name
            std::string tileName;
            for (size_t i = 1; i < args.size(); ++i) {
                tileName += args[i];
                if (i < args.size() - 1) {
                    tileName += " ";
                }
            }

            // Access map data without structured binding
            std::vector<Tile*> tiles = game->gameMap->getMap().first;
            std::vector<std::string> names = game->gameMap->getMap().second;
            
            // Find the tile by name
            auto it = std::find(names.begin(), names.end(), tileName);
            if (it == names.end()) {
                std::cout << "Tile not found: " << tileName << std::endl;
                return false;
            }
            position = static_cast<int>(std::distance(names.begin(), it));
        }
        else {
            position = std::stoi(args[0]);
            if (position < 0 || position >= game->gameMap->getSize()) {
                std::cout << "Invalid position: " << position << std::endl;
                return false;
            }
        }

        // Move player to the specified position
        player->SetPosition(position);
        std::cout << player->GetName() << " moved to position " << position << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error parsing position: " << e.what() << std::endl;
        return false;
    }
}

bool CommandHandler::HandleGetCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage: /get <amount> or /get <player> <amount>" << std::endl;
        return false;
    }

    try {
        if (args.size() == 1) {
            // Give money to the current player
            int amount = std::stoi(args[0]);
            if (amount <= 0) {
                std::cout << "Amount must be positive!" << std::endl;
                return false;
            }

            // Add money to player
            // Assuming Player has a method to add money
            player->BuyProperty(-amount); // Negative price means adding money
            std::cout << player->GetName() << " received $" << amount << std::endl;
            return true;
        }
        else if (args.size() == 2) {
            // Give money to another player
            std::string targetPlayerName = args[0];
            int amount = std::stoi(args[1]);
            
            if (amount <= 0) {
                std::cout << "Amount must be positive!" << std::endl;
                return false;
            }

            // Find the target player
            Player* targetPlayer = nullptr;
            
            // Special case for bank
            if (targetPlayerName == "bank") {
                targetPlayer = &Monopoly::bank;
            }
            // For a real implementation, search through the game's player list
            // This would require access to the players list in Game

            if (!targetPlayer) {
                std::cout << "Player not found: " << targetPlayerName << std::endl;
                return false;
            }

            // Add money to target player
            targetPlayer->BuyProperty(-amount);
            std::cout << targetPlayerName << " received $" << amount << std::endl;
            return true;
        }
        else {
            std::cout << "Usage: /get <amount> or /get <player> <amount>" << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error parsing amount: " << e.what() << std::endl;
        return false;
    }
}

bool CommandHandler::HandleGiveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    if (args.size() < 2) {
        std::cout << "Usage: /give <player> <amount>" << std::endl;
        return false;
    }

    try {
        std::string targetPlayerName = args[0];
        int amount = std::stoi(args[1]);
        
        if (amount <= 0) {
            std::cout << "Amount must be positive!" << std::endl;
            return false;
        }

        // Check if player has enough money
        if (player->GetMoney() < amount) {
            std::cout << "Not enough money!" << std::endl;
            return false;
        }

        // Find the target player
        Player* targetPlayer = nullptr;
        // This is placeholder code, replace with actual player lookup logic
        // You'll need to implement a way to search for players by name

        if (!targetPlayer) {
            std::cout << "Player not found: " << targetPlayerName << std::endl;
            return false;
        }

        // Transfer money
        player->Pay(targetPlayer, amount);
        std::cout << player->GetName() << " gave $" << amount << " to " << targetPlayerName << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error parsing amount: " << e.what() << std::endl;
        return false;
    }
}

bool CommandHandler::HandleCardCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage: /card <card_name>" << std::endl;
        return false;
    }

    // Combine all arguments to form the card name
    std::string cardName;
    for (size_t i = 0; i < args.size(); ++i) {
        cardName += args[i];
        if (i < args.size() - 1) {
            cardName += " ";
        }
    }

    // Create a specific item based on the name
    // For now, only support ControlDiceItem
    if (cardName == "±±¨î»ë¤l" || cardName == "Rocket Card") {
        player->AddItem(new ControlDiceItem());
        std::cout << player->GetName() << " received card: " << cardName << std::endl;
        return true;
    }
    else {
        std::cout << "Unknown card: " << cardName << std::endl;
        return false;
    }
}

bool CommandHandler::HandleGameStateCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage: /gamestate <state>" << std::endl;
        return false;
    }

    // This would require access to game state, which might not be directly accessible
    // This is a placeholder implementation
    std::string stateStr = args[0];
    std::cout << "Game state changed to " << stateStr << std::endl;
    
    return true;
}

bool CommandHandler::HandleInfoCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    // Display information about all players
    // This would require access to all players, which might not be directly accessible
    // This is a placeholder implementation
    std::cout << "Player information:" << std::endl;
    // In a real implementation, you would iterate through all players and print their info
    
    return true;
}

bool CommandHandler::HandleRefreshCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    // Force redraw the map
    game->Clear();
    std::cout << "Map refreshed." << std::endl;
    
    return true;
}

bool CommandHandler::HandleListCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    bool detailed = false;
    
    if (!args.empty() && args[0] == "-a") {
        detailed = true;
    }

    std::vector<std::string> cmdList = GetCommandList(detailed);
    std::cout << "Available commands:" << std::endl;
    
    for (const auto& cmd : cmdList) {
        std::cout << cmd << std::endl;
    }
    
    return true;
}

bool CommandHandler::HandleMinigameCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
    if (!game) {
        std::cout << "Game reference not set!" << std::endl;
        return false;
    }

    // Enter minigame menu
    // This would require access to the minigame functionality
    // This is a placeholder implementation
    std::cout << "Entering minigame menu..." << std::endl;
    
    return true;
}
