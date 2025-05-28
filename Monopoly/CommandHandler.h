#pragma once
#include <string>
#include <map>
#include <memory>
#include "Player.h"
#include <functional>

// Forward declarations
class Game;

class CommandHandler
{
public:
	/**
	 * Constructor
	 */
	CommandHandler();

	/**
	 * Initialize command handler, load commands from JSON file
	 */
	void Initialize();

	/**
	 * Process a command string
	 * @param player The player who executed the command
	 * @param input The command string (starting with /)
	 * @return true if command was valid and executed, false otherwise
	 */
	std::pair<bool, int> ProcessCommand(std::shared_ptr<Player> player, const std::string& input);

	/**
	 * Get a list of all available commands
	 * @param detailed If true, include command usage and examples
	 * @return Vector of strings with command information
	 */
	std::vector<std::string> GetCommandList(bool detailed = false);

	/**
	 * Check if a string is a valid command
	 * @param cmdName Command name without the / prefix
	 * @return true if command exists
	 */
	bool IsValidCommand(const std::string& cmdName);

	/**
	 * Set reference to game instance
	 * @param gamePtr Pointer to the Game instance
	 */
	void SetGameReference(Game* gamePtr);

private:
	std::vector<std::string> SplitCommand(const std::string& input);
	std::string TrimString(const std::string& str);

	// Helper function to find a player by name
	Player* FindPlayerByName(const std::string& name);

	// Command handlers
	bool HandleMoveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleGetCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleGiveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleCardCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleGameStateCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleInfoCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleRefreshCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleListCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);
	bool HandleMinigameCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args);

	// Reference to the game instance
	Game* game;

	// Command data structure
	struct Command {
		std::string name;
		std::string description;
		std::string usage;
		std::string example;
		std::function<bool(std::shared_ptr<Player> player, const std::vector<std::string>& args)> handler;
	};

	void showDetail(const Command& cmd);
	void showDescription(const Command& cmd);
	void showUsage(const Command& cmd);
	void showExample(const Command& cmd);

	std::map<std::string, Command> commands;
	std::map<std::string, std::string> items;
};

