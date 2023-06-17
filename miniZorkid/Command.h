#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include"Direction.h"
#include"Item.h"
#include"Location.h"
#include"Player.h"
#include"World.h"
#pragma once

using namespace std;

const unordered_set<string> dirs = {
	"north", "south", "east", "west"
};

class Command
{
	private:
		vector<string> verbs = 
		{ "north", "south", "east", "west", "take", "pick up", "drop", "use", "quit", "help", "q", "h", "i"};
		string command, foundVerb, item;
	public:
		Command(string command = "") : command(command) {}

		vector<string> get_Verbs(){ return verbs; }

		string getFoundVerb() { return foundVerb; }

		// Checks if entered command is in the list of commands
		// created with the Command class
		// also populates "found" which is the verb found in the
		// user's string
		bool checkCommand() {
			if (command == "") {
				return false;
			}
			else 
			{
				for (const string& verb : verbs) {
					auto itr = command.find(verb);
					if (itr != command.npos) {
						foundVerb = verb;
						return true;
					}
				}
			}
			return false;
		}

		int checkCommandType() {
			if (!checkCommand()) {
				return 0;
			}
			else
			{
				if (foundVerb == "quit" || foundVerb == "q")
					return -1;
				else if (dirs.count(foundVerb) > 0) {
					auto itr = find(dirs.begin(), dirs.end(), foundVerb);
					unordered_map<string, Directions> directionMap {
						{ "north", Directions::north},
						{ "south", Directions::south },
						{ "east", Directions::east },
						{ "west", Directions::west }
					};
					if (itr != dirs.end())
					{
						for (const auto& i : directionMap) {
							if (foundVerb == i.first)
								return static_cast<int>(i.second);
						}
					}
				}
				else if (foundVerb == "pick up" || foundVerb == "take") {
					return 5;
				}
				else if (foundVerb == "drop") {
					return 6;
				}
				else if (foundVerb == "use") {
					return 7;
				}
				else if (foundVerb == "help" || foundVerb == "h")
					return -2;
				else if (foundVerb == "i") {
					return -3;
				}
			}
			return 0;
		}


		// Simply transforms from enum type Direction to int for easier navigation.
		int operator()(Directions direction) {
			return static_cast<int>(direction);
		}
		
		// Operator overload used when player attempts to pick up item
		Item operator()(Location location) {
			for (Item item : location.itemsInLocation()) {
				auto itr = command.find((item.get_itemName()));
				if (itr != command.npos) {
					return item;
				}
			}
			return {};
		}

		// Called when player drops an item
		void operator()(Location location, Player& player){
			bool itemDropped = false;
			for (Item item : player.getInventory()) {
				auto itr = command.find((item.get_itemName()));
				if (itr != command.npos) {
					location.itemDropped(item);
					player.dropItem(item);
					cout << "\t\t" << item.get_itemName() << " dropped at "
						<< location.getLocationName() << endl;
					itemDropped = true;
				}
			}
			if (!itemDropped) {
				cout << "\t\tYou don't carry an item of that name\n\n\n" << endl;
			}
		}

		// operator overload when player uses an item.
		void operator()(Player& player, Location& location, World& world) {
			Item itemToUse;
			Item usedOn;
			
			// Checks if user entered an item in their inventory as the item they want to use 
			for (Item& item : player.getInventory()) {
				auto itr = command.find((item.get_itemName()));
				if (itr != command.npos) {
					itemToUse = item;
				}
			}
			if (itemToUse.isEmpty()) {
				cout << "\t\tNo such item in your inventory\n\n\n";
			}

			// Checks for second item, item which the first is being used on, is in the location
			// and is compatible with used item or not.
			else {
				for (Item& item : location.itemsInLocation()) {
					auto itr = command.find((item.get_itemName()));
					if (itr != command.npos) {
						usedOn = item;
					}
				}
				if (usedOn.isEmpty()) {
					cout << "\t\tSecond item doesn't exist here.\n\n\n";
				}
				else
				{
					// Checks if the item being used can be used on second item
					if (usedOn.get_itemName() == itemToUse.get_usedOn()) {
						location.openContainer(usedOn);
						if (usedOn.getItemType() == 3)
						{
							for (pair<int, int>& exit : location.getLocatExits()) {
								if (exit.first == usedOn.getDoorDirection())
								{
									for (Location& area : world.get_locations())
									{
										if (area.getLocationId() == exit.second)
										{
											area.openDoor();
											player.setPlayerLocation(area);
										}
									}
								}
							}
						}
					}
					else {
						usedOn.eventFails();
						cout << "\t\tItem " << itemToUse.get_itemName() << " can't be used on " << usedOn.get_itemName() << "\n\n\n";
					}
				}
			}
		}


};

