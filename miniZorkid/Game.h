#include "World.h"
#include "Player.h"
#include "Item.h"
#include "Direction.h"
#include "Command.h"
#include <string>
#include <cstring>
#include <iostream>
#include "json/json.h"
#include <fstream>
#include <unordered_set>
#pragma once

    // Basic Instructions on navigating the game
    const vector <string> instructions = {
        "******************************************************************\n",
        "******************        Navigation:         ********************\n",
        "*                                                                *\n",
        "*       To navigate through the vast world of Mini Zork,         *\n",
        "*       you can type commands like \"go north\" or \"go east\"       *\n",
        "*                   (Qutoations not required)                    *\n",
        "*            For now you can only use compass directions:        *\n",
        "*                 ( north, south, east, west )                   *\n",
        "*                                                                *\n",
        "*         If at anytime you need help with these commands,       *\n",
        "*                  type 'H' or \"help\"                            *\n",
        "******************************************************************\n",
        "\n\n"
    };

    // Welcom text for the user
    const vector <string> welcomeText = {
        "\n\t\t\t\t\tWelcome to Mini Zork.\n\n"
    };

    // Game's Beginning
    const vector <string> gameBeginning = {
        "You wake up in a surgery room wearing a patient\'s gown\n",
        "The square shaped room seems a bit claustrophobic\n",
        "********************************************************\n\n"
    };



class Game
{
private:
    World gameWorld;
    Player gamePlayer;
    bool gameState = true;


public:
    string playerInput;

    /*Command traverse();*/

        // Game Constructor
        Game(World world = World(), Player player = Player(1), string input = "") : gameWorld(world), gamePlayer(player), playerInput(input) {
            loadLocations();
            gamePlayer.setPlayerLocation(gameWorld.get_location(1));
        }


        // get World
        World getWorldMap() { return gameWorld; }

        // Get player
        Player getPlayer() { return gamePlayer; }
		
       /*
        *   Function Name:  takeUserInput
        *   Parameters:     None
        *   Description:    reads user's input from console
        *                   and transforms all text to lower-case
        *                   to allow for case-insensitive inputs.
        */
        void takeUserInput()
        {
            string command;
            cout << "\t>";
            getline(cin, command);

            if (command == "")
                playerInput = "";
            else
            {
                int i;
                for (i = 0; i < command.length(); i++)
                {
                    command[i] = ::tolower(command[i]);
                }
                playerInput = command;
            }
        }

        /*
        *   Function Name:  loadLocations
        *   Description:    Loads the location.json file into the game's data.
        */
        bool loadLocations() {

            try
            {
                ifstream configFile("locations.json");
        
                // Parsing the JSON data for use in main later
                Json::Value locations;
                configFile >> locations;

                // Creates a reference to the locations' json data
                const Json::Value& locationArray = locations["locations"];

                // Creates areas and populating the fields using the json data
                for (const Json::Value& locationData : locationArray) {
                    string name = locationData["name"].asString();
                    int id = locationData["id"].asInt();
                    bool accessible = locationData["accessible"].asBool();

                    vector<Item> locationItems;
                    // loop through items in location
                    if (locationData.isMember("items")) {
                        for (const Json::Value& items : locationData["items"]) {
                            string name = items["name"].asString();
                            bool useable = items["useable"].asBool();
                            string description = items["description"].asString();
                            Item item(name, useable, description);
                            locationItems.push_back(item);
                        }
                    }
                    vector<string> disc;
                    // For loop to populate string vector for description
                    for (const Json::Value& descriptions : locationData["description"]) {
                        disc.push_back(descriptions.asString());
                    };

                    // For loop to populate vector pair for exits containing
                    // direction and destination
                    vector<pair<int, int>> exits;
                    for (const Json::Value& exit : locationData["exits"]) {
                        if (exit["direction"].asInt() && exit["destination"].asInt()) {
                            int direction = exit["direction"].asInt();
                            int destination = exit["destination"].asInt();
                            exits.push_back(make_pair(direction, destination));
                        }
                        else {
                            cout << "Invalid exit data: " << exit << endl;
                        }
                    };

                    Location area(id, name, accessible, locationItems, disc, exits);

                    this->gameWorld.add_location(area);
                }

                return true;
            }
            catch (const std::exception& ex)
            {
                cout << "Exception Caught in loadLocations: " << ex.what() << endl;
                return false;
            }
        }

        /*
            Function that runs the game and manages it
        */
        bool gameStart() {
            Location currentLocation;

            // Printing out Welcome text, instructions, and beginning text
            printVector(centerVectorString(welcomeText));
            printVector(centerVectorString(instructions));
            printVector(centerVectorString(gameBeginning));

                
            // GAME START / Game Running
            while (gameState)
            {
                currentLocation = gamePlayer.locate_Player();
                cout << "\t\t" << currentLocation.getLocationName() << ":" << endl << endl;
                printVector(centerVectorString(currentLocation.getLocatDesc()));
                currentLocation.printItems();

                // converts all input to lowercase for easier processing
                takeUserInput();
                cout << endl;

                Command command(playerInput);

                // Check which type of command is entered
                int action = command.checkCommandType();
            
                switch (action)
                {
                case 0:     cout << "\n\t\tNo Command Detected\n\n";
                            break;
                case -1:    endGame();
                            break;
                case -2:    getHelp();
                            break;
                case 1:     navigate(command(Directions::north));
                            break;
                case 2:     navigate(command(Directions::east));
                            break;
                case 3:     navigate(static_cast<int>(Directions::west));
                            break;
                case 4:     navigate(static_cast<int>(Directions::south));
                            break;

                default:
                    cout << "* Action in development, it will be added soon promise *\n\n\n";
                    break;
                }
            };
            return true;
        }

        /*
        *   Function Name:  centerVectorString
        *   Parameters:     vetor<string> rawVectorString: vector string to be centered
        *   Description:    Creates a centered version of the input vector string
        *                   and returns the centered vector of strings
        */
        vector<string> centerVectorString(vector<string> rawVectorString) {

            int width = 125;

            vector<string> centeredVectorString;

            for (const string& str : rawVectorString) {
                int strLength = static_cast<int>(str.length());
                int padding = abs(((width - strLength) / 2));
                if (padding % 2 != 0) {
                    padding++;
                }
                string centerString = string(padding, ' ') + str;
                centeredVectorString.push_back(centerString);
            }

            return centeredVectorString;

        }

        /*
        *   Function that prints the input vector string
        */
        void printVector(vector<string> inputVectorString) {
            for (const string& str : inputVectorString) {
                cout << str;
            }
            cout << endl;
        }

        // Navigate through the world
        void navigate(int direction) {
            Location currentLocation = gamePlayer.locate_Player();
            for (pair<int,int>& exit : currentLocation.getLocatExits()) {
                if (exit.first == direction)
                {
                    for (Location &location : getWorldMap().get_locations())
                    {
                        if (location.getLocationId() == exit.second)
                        {
                            if (location.checkAccessible())
                                gamePlayer.setPlayerLocation(location);
                            else {
                                cout << "\t\t\tYou can't go this way at the moment, you're blocked.\n\n";
                                break;
                            }
                        }
                    }
                }
            }
            if (gamePlayer.didNotMove()) {
                cout << "\t\t\tThere is nothing in that direction\n\n\n";
            }
        }

        // Ends the game when quit or q is input but user
        void endGame() {
            cout << "\t\t\tExitting Game, Thank you for Playing\n\n" << endl;
            gameState = false;
        }

        // Help users
        void getHelp() {
            cout << "\n\t\tDo you need Help? (Y/n)\n";
            takeUserInput();
            if (playerInput == "" || playerInput == "y")
            {
                printVector(centerVectorString(instructions));
            }
        }


};

