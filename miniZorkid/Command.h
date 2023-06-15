#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include"Direction.h"
#include"Item.h"
#pragma once

using namespace std;

const unordered_set<string> dirs = {
	"north", "south", "east", "west"
};

class Command
{
	private:
		vector<string> verbs = 
		{ "north", "south", "east", "west", "take", "pick up", "drop", "quit", "help", "q", "h"};
		string command;
		string foundVerb;
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
				else if (foundVerb == "help" || foundVerb == "h")
					return -2;
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

			}
			return 0;
		}

		int operator()(Directions direction) {
			return static_cast<int>(direction);
		}
};

