#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <json/json.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Unordered set with possible directions
const unordered_set<string> directionSet = {
    "north", "south", "east", "west", "back", "forward", "backward", "left", "right"
};

const unordered_set<string> help_keywords = {
    "h", "help", ""
};

const unordered_set<string> quit_commands = {
    "q", "quit", "exit"
};

string takeUserInput()
{
    string command;

    getline(cin, command);

    if (command == "")
        return "";
    else
    {
        int i;
        for (i = 0; i < command.length(); i++)
        {
            command[i] = tolower(command[i]);
        }

        return command;
    }
}

struct Location
{
    int id;
    string name;
    string descrition;
    vector<pair<string,int>> exits;
};

vector<Location> areas;

unordered_map <int, Location> locationMap;

int loadLocations() {
    ifstream configFile("locations.json");

    //Check location.json was opened properly
    if (!configFile)
    {
        cout << "Error opening game file (locations.json)." << endl;
        return 1;
    }

    // Parsing the JSON data for use in main later

    Json::Value locations;
    configFile >> locations;

    // Creates a reference to the locations' json data
    const Json::Value& locationArray = locations["locations"];

    // Creates areas and populating the fields using the json data
    for (const Json::Value& locationData : locationArray) {
        Location area;
        area.descrition = locationData["description"].asString();
        area.name = locationData["name"].asString();
        area.id = locationData["id"].asInt();
        
        for (const Json::Value& exit : locationData["exits"]) {
            area.exits.push_back(make_pair(exit["direction"].asString(), exit["destination"].asInt()));
        };

        locationMap[area.id] = area;

        /*cout << "Location: " << area.name << endl;
        cout << "Descritption: " << area.descrition << endl;
        cout << "Exits: " << endl;
        for (pair<string, int> exit : area.exits) {
            cout << exit.first << " at which reaches id: " << exit.second << endl;
        }
        cout << endl;*/

        areas.push_back(area);
    }

    return 0;
}

/*
    readUserInput takes the user's input after processing it
    and gets the action specified by the user and returns the proper
    string
*/
string cehckAction(string input) {

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

    for (const auto& exit : locationMap[currentPosition].exits) {
        if (exit.first == chosenDirection)
            return exit.second;
    }
    return currentPosition;
}

int main()
{

    // Welcom text for the user
    string welcomeText = 
        "\n\tWelcome to Mini Zork.\n\n"
        "\tYou wake up in a surgery room wearing a patient\'s gown\n"
        "\tThe square shaped room seems a bit claustrophobic\n\n";

    // Basic Instructions on navigating the game
    const string instructions = 
        "\tTo navigate through the vast world of Mini Zork, you can type commands like \"go north\" or \"go east\"\n\t(Qutoations not required) to traverse to the next area\n"
        "\tYou can also type \"go back\" to return to last area\n"
        "\tIf at anytime you need help with these commands, type 'H'\n\n";

    // Printing out Welcome text and instructions
    cout << instructions;
    cout << welcomeText;

    // Loading locations into areas vector from "locations.json"
    if (loadLocations() != 0) {
        return 1;
    }

    // set player's starting position to the first room's id
    int playerLocationId = 1;

    // for future use to prevent errors
    int last_location;

    // string to capture user commands within the game
    string userInput;

    while (true)
    {
        cout << "\t" << locationMap[playerLocationId].descrition << endl << endl;

        cout << "> ";

        // converts all input to lowercase for easier processing
        userInput = takeUserInput();
        cout << endl;

        // Check if user wants to quit the game
        if (quit_commands.count(userInput) > 0)
        {
            cout << "\tExitting the Game Thank You For Playing\n\n\n";
            break;
        }

        // *Temporary* checks if the user has entered "GO" 
        string action = cehckAction(userInput);

        // If an action (GO) was entered use navigate
        if (action != "") {
            if (action.substr(0,1) == "1") {
                last_location = playerLocationId;
                playerLocationId = navigate(playerLocationId, action.substr(2));
                if (playerLocationId == last_location) {
                    cout << "\n\tThere is nothing in that direction.\n\t" << playerLocationId << endl << endl;

                }
            }
        }
        else if (help_keywords.count(userInput) > 0)
        {
            cout << "> Do you need Help? (Y/n)\n";
            cout << "> ";
            userInput = takeUserInput();
            if (userInput == "" || userInput == "y")
            {
                cout << instructions;
            }
            continue;
        }
        else
        {
            cout << "\t* Action in development, it will be added soon promise * \n\n\n";
        }
    };
    return 0;
}