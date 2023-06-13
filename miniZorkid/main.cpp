#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <json/json.h>
#include <unordered_set>
#include <unordered_map>
#include "World.h"
#include "Game.h"

using namespace std;

// Unordered set with possible directions
const unordered_set<string> directionSet = {
    "north", "south", "east", "west", "back", "forward", "backward", "left", "right"
};

// Set of possible inputs to receive help
const unordered_set<string> help_keywords = {
    "h", "help", ""
};

// Set of possible inputs to quit program (Exit game).
const unordered_set<string> quit_commands = {
    "q", "quit", "exit"
};

///*
//* 
//*   Function Name:  takeUserInput
//*   Parameters:     None
//*   Description:    reads user's input from console
//*                   and transforms all text to lower-case
//*                   to allow for case-insensitive inputs.
//*            
//*/
//string takeUserInput()
//{
//    string command;
//    cout << "\t>";
//    getline(cin, command);
//
//    if (command == "")
//        return "";
//    else
//    {
//        int i;
//        for (i = 0; i < command.length(); i++)
//        {
//            command[i] = ::tolower(command[i]);
//        }
//        return command;
//    }
//}


struct UsableItems
{
    int id;
    string name;
    string description;
    vector<pair<int, int>> use;
};

struct InteractItems
{
    int id;
    string name;
    string description;
    int locationId;
    bool active;
    int alreadyUsedEvent;
};


struct EventScene
{
    int id;
    vector<string> eventScenario;
};

vector <UsableItems> useItems;
vector <InteractItems> interactItems;
vector <EventScene> events;

unordered_map <int, Location> locationMap;

World world;

/*
*   Function Name:  loadLocations
*   Description:    Loads the location.json file into the game's data.
*/
bool loadLocations() {
    ifstream configFile("locations.json");

    //Check location.json was opened properly
    if (!configFile)
    {
        cout << "Error opening game file (locations.json)." << endl;
        return false;
    }

    // Parsing the JSON data for use in main later

    Json::Value locations;
    configFile >> locations;

    // Creates a reference to the locations' json data
    const Json::Value& locationArray = locations["locations"];

    // Creates areas and populating the fields using the json data
    for (const Json::Value& locationData : locationArray) {
        string name = locationData["name"].asString();
        int id = locationData["id"].asInt();
        bool accessible = locationData["accessable"].asBool();
        
        vector<string> disc;
        // For loop to populate string vector for description
        for (const Json::Value& descriptions : locationData["description"]) {
            disc.push_back(descriptions.asString());
        };
        
        // For loop to populate vector pair for exits containing
        // direction and destination
        vector<pair<string, int>> exits;
        for (const Json::Value& exit : locationData["exits"]) {
            exits.push_back(make_pair(exit["direction"].asString(), exit["destination"].asInt()));
        };

        Location area(id, name, accessible, {},  disc, exits);

        locationMap[area.getLocationId()] = area;

        world.add_location(area);
    }

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

    //for (const string& str : rawVectorString) {
    //    maxLength = max(maxLength,  int(str.length()));
    //}
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


/*
* 
*   Function Name:  checkAction
*   Parameters:     string input
*   Description:
            *       Takes the user's input after processing it
            *       through takeUserInput and checks if the player
            *       entered a valid action.
*
*/
string checkAction(string input) {

    // Creating unordered set with all available directions.
    string direction;
    // Checking if Go was typed in command
    if (input.substr(0, 2) == "go") {
        if (input.length() > 3) {
            direction = input.substr(3);
        }
        else 
        {
            do
            {
                int tries = 0;
                if (tries > 0) {
                    cout << "Sorry it's unclear where you want to go\n"
                        "Please try again\n\n";
                }
                cout << "> Where do want to go?\n";
                cout << "> ";
                getline(cin, direction);
                tries++;
            } while (directionSet.count(direction) <= 0);
        }
        if (directionSet.count(direction) > 0) {
            return "1 " + direction;
        }
        return "";
    }
    /*
    else {
        cout << " * Sorry this actions is unavailable at this time * ";
    }
    */
    return "";
}


/*
* Function to Navigate player through the different areas of the game.
*/
int navigate(int currentPosition, string chosenDirection) {

    for (const auto& exit : locationMap[currentPosition].getLocatExits()) {
        if (exit.first == chosenDirection)
            return exit.second;
    }
    return currentPosition;
}

int main()
{

    // Welcom text for the user
    const vector <string> welcomeText = {
        "\n\t\t\t\tWelcome to Mini Zork.\n\n"
    };

    // Basic Instructions on navigating the game
    const vector <string> instructions = {
        "******************************************************************\n",
        "*       To navigate through the vast world of Mini Zork,         *\n",
        "*       you can type commands like \"go north\" or \"go east\"       *\n",
        "*                 (Qutoations not required)                      *\n",
        "*        You can also type \"go back\" to return to last area      *\n",
        "*                                                                *\n",
        "*         If at anytime you need help with these commands,       *\n",
        "*                  type 'H' or \"help\"                            *\n",
        "******************************************************************\n",
        "\n\n"
    };

    string str1 = "You are in a square shaped operation room with nothing arround you except for the bed you woke up on";
    
    const vector <string> gameBeginning = { 
        "You wake up in a surgery room wearing a patient\'s gown\n",
        "The square shaped room seems a bit claustrophobic\n",
        "********************************************************\n\n"
    };
    // Printing out Welcome text, instructions, and beginning text
    printVector(centerVectorString(welcomeText));
    printVector(centerVectorString(instructions));
    printVector(centerVectorString(gameBeginning));


    // Loading locations into areas vector from "locations.json"
    if (!loadLocations()) {
        return 1;
    }

    Game game;

    // set player's starting position to the first room's id
    int playerLocationId = 1;

    // for future use to prevent errors
    int last_location;

    // string to capture user commands within the game
    string userInput;

    // GAME START
    while (true)
    {
        cout << "\t\t" << locationMap[playerLocationId].getLocationName() << ":" << endl << endl;
        printVector(centerVectorString(locationMap[playerLocationId].getLocatDesc()));

        // converts all input to lowercase for easier processing
        game.takeUserInput();
        userInput = game.playerInput;
        cout << endl;

        // Check if user wants to quit the game
        if (quit_commands.count(userInput) > 0)
        {
            cout << "\t\t\tExitting the Game Thank You For Playing\n\n\n";
            break;
        }

        // *Temporary* checks if the user has entered "GO" 
        string action = checkAction(userInput);

        // If an action (GO) was entered use navigate
        if (action != "") {
            if (action.substr(0,1) == "1") {
                last_location = playerLocationId;
                playerLocationId = navigate(playerLocationId, action.substr(2));
                if (playerLocationId == last_location) {
                    cout << "\n\n\t\tThere is nothing in that direction.\n\n";
                }
            }
        }
        else if (help_keywords.count(userInput) > 0)
        {
            cout << "\n\t\tDo you need Help? (Y/n)\n";
            game.takeUserInput();
            userInput = game.playerInput;
            if (userInput == "" || userInput == "y")
            {
                printVector(instructions);
            }
            continue;
        }
        else
        {
            cout << "* Action in development, it will be added soon promise *\n\n\n";
        }
    };
    return 0;
}