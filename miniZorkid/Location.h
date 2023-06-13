#include<string>
#include<vector>
#include"Item.h"
#pragma once

using namespace std;
class Location
{
	private:
		int locationId;
		string locationName;
		bool accessible;
		vector<Item> items;
		vector<string> locationDesc;
		vector<pair<string, int>> locationExits;

	public:
		Location(
			int Id = 1,
			string name = "First Room",
			bool access = true,
			vector<Item> itemsHere = {},
			vector<string> description= { "Initial Room of the Game" },
			vector<pair<string,int>> exit = {}) {
				locationId = Id;
				locationName = name;
				accessible = access;
				items = itemsHere;
				locationDesc = description;
				locationExits = exit;
		}

		int getLocationId() {
			return locationId;
		}

		string getLocationName() {
			return locationName;
		}

		bool checkAccessible() {
			return accessible;
		}

		vector<Item> itemsInLocation() {
			return items;
		}

		vector<string> getLocatDesc() {
			return locationDesc;
		}

		vector<pair<string, int>> getLocatExits() {
			return locationExits;
		}
};

