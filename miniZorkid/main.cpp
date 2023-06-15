#include "Game.h"

using namespace std;

//// Unordered set with possible directions
//const unordered_set<string> directionSet = {
//    "north", "south", "east", "west", "back", "forward", "backward", "left", "right"
//};
//
//// Set of possible inputs to receive help
//const unordered_set<string> help_keywords = {
//    "h", "help", ""
//};
//
//struct UsableItems
//{
//    int id;
//    string name;
//    string description;
//    vector<pair<int, int>> use;
//};
//
//struct InteractItems
//{
//    int id;
//    string name;
//    string description;
//    int locationId;
//    bool active;
//    int alreadyUsedEvent;
//};
//
//
//struct EventScene
//{
//    int id;
//    vector<string> eventScenario;
//};
//
//vector <UsableItems> useItems;
//vector <InteractItems> interactItems;
//vector <EventScene> events;
//
//unordered_map <int, Location> locationMap;

int main()
{
    Game game;
    game.gameStart();
    return 0;
}