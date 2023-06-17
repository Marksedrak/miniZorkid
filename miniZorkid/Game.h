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
        "*                                                                *\n",
        "******************         Items:             ********************\n",
        "*                                                                *\n",
        "*              You can Pick up, Drop, and Use items              *\n",
        "*                                                                *\n",
        "*      To pick up ites: use commands \"take\" or \"pick up\"         *\n",
        "*           To drop items: use the \"drop\" command                *\n",
        "*         To use items: use command \"use (item) on (item)\"       *\n",
        "*         If at anytime you need help with these commands,       *\n",
        "*                                                                *\n",
        "*             Type \"I\" or \"i\" to access your inventory           *\n",
        "*                                                                *\n",
        "*                      type 'H' or \"help\"                        *\n",
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
        World& getWorldMap() { return gameWorld; }

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

        /// Function: loadItems
        ///     Function loads items in a location using their specific type as reference
        ///
        /// Parameters:
        ///     locationData
        ///         locationData is a reference to a Value in the json file
        ///     container
        ///         container is the name of the member of which the items are located
        /// Returns:
        ///     Returns a vector of Items in each location.
       
        vector<Item> loadItems(const Json::Value& locationData) {
            vector<Item> locationItems;
            for (const Json::Value& itemData : locationData) {
                int type = itemData["type"].asInt();
                Item item;
                string name, description, useOn, opened, fail;
                bool locked, useable;
                vector<Item> contained;
                switch (type)
                {
                case 1:
                    name = itemData["name"].asString();
                    useable = itemData["useable"].asBool();
                    useOn = itemData["useOn"].asString();
                    description = itemData["description"].asString();
                    item = item.createObject(name, useable, useOn, description);
                    break;
                case 2:     name = itemData["name"].asString();
                    description = itemData["description"].asString();
                    locked = itemData["locked"].asBool();
                    opened = itemData["opened"].asString();
                    fail = itemData["fail"].asString();

                    // Recursively calls loadItems for contained items to load contained
                    contained = loadItems(itemData["contained"]);
                    item = item.createContainer(name, locked, opened, fail, contained, description);
                    break;
                case 3:   name = itemData["name"].asString();
                    int direction = itemData["direction"].asInt();
                    description = itemData["description"].asString();
                    locked = itemData["locked"].asBool();
                    opened = itemData["opened"].asString();
                    fail = itemData["fail"].asString();
                    item = item.createDoor(name, direction, locked, opened, fail, description);
                    break;
                }

                // Checks if there is a contained member and loads the contained vector with the items
                locationItems.push_back(item);
            }
            return locationItems;
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

                // Creates a reference to the locations json data
                const Json::Value& locationArray = locations["locations"];

                // Creates areas and populating the fields using the json data
                for (const Json::Value& locationData : locationArray) {
                    string name = locationData["name"].asString();
                    int id = locationData["id"].asInt();
                    bool accessible = locationData["accessible"].asBool();

                    // Checks if this location has items and loops through if it does
                    vector<Item> locationItems;
                    if (locationData.isMember("items")) {
                        locationItems = loadItems(locationData["items"]);
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

                    gameWorld.add_location(area);
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
            // Printing out Welcome text, instructions, and beginning text
            printVector(centerVectorString(welcomeText));
            printVector(centerVectorString(instructions));
            printVector(centerVectorString(gameBeginning));

                
            // GAME START / Game Running
            while (gameState)
            {
                cout << "***********************************************************************************************************************\n";
                cout << "\t\t" << gamePlayer.locate_Player().getLocationName() << ":" << endl << endl;
                printVector(centerVectorString(gamePlayer.locate_Player().getLocatDesc()));
                cout << "\n\n\n";
                gamePlayer.locate_Player().printItems();
                cout << "\n\n\n";
                // converts all input to lowercase for easier processing
                takeUserInput();
                cout << endl;

                Command command(playerInput);

                // Check which type of command is entered
                int action = command.checkCommandType();
            
                executeCommand(command, action);
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
            bool blocked = false;
            for (pair<int,int>& exit : currentLocation.getLocatExits()) {
                if (exit.first == direction)
                {   
                    for (Location& location : getWorldMap().get_locations())
                    {
                        if (location.getLocationId() == exit.second)
                        {
                            if (location.checkAccessible())
                                gamePlayer.setPlayerLocation(location);
                            else {
                                cout << "\t\t\tYou can't go this way at the moment\n";
                                cout << "\t\t\t" << currentLocation.getDoor(direction).get_itemName() << " is locked,"
                                    << " you'll need to unlock it first.\n" << endl;
                                blocked = true;
                            }
                        }
                    }
                }
            }
            if (gamePlayer.didNotMove(currentLocation) && !blocked) {
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

        // Function takes command and calls appropriate function to execute it.
        void executeCommand(Command command, int action) {
            Item item;
            switch (action)
            {
            case 0:     cout << "\n\t\tNo Command Detected\n\n";
                break;
            case -1:    endGame();
                break;
            case -2:    getHelp();
                break;
            case -3:    cout << "\t\tPlayer's Inventory:\n\n\n";
                printVector((centerVectorString(gamePlayer.getInventItemNames())));
                cout << "\n\n";
                break;
            case 1:     navigate(command(Directions::north));
                break;
            case 2:     navigate(command(Directions::east));
                break;
            case 3:     navigate(command(Directions::west));
                break;
            case 4:     navigate(command(Directions::south));
                break;
            case 5:     
                        item = command(gamePlayer.locate_Player());
                        gamePlayer.pickUpItem(item);
                break;
            case 6:     command(gamePlayer.locate_Player(), gamePlayer);
                break;

            case 7:     command(gamePlayer, gamePlayer.locate_Player(), getWorldMap());
                break;
            }
        }


};

