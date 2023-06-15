#include<string>
#include<vector>
#include"Item.h"
#include<iostream>
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
		vector<pair<int, int>> locationExits;

	public:
		Location(
			int Id = 1,
			string name = "First Room",
			bool access = true,
			vector<Item> itemsHere = {},
			vector<string> description= { "Initial Room of the Game" },
			vector<pair<int,int>> exit = {}) {
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

		void printItems() {
			for (Item item : itemsInLocation()) {
				cout << "\t\tThere is a " << item.get_itemName() << ":\n";
				cout << "\t\t" << item.get_itemDesc() << endl;
			}
		}

		Item getDoor(int direction) {
			for (Item item : itemsInLocation()) {
				if (item.getDoorDirection() == direction) {
					return item;
				}
			}
			return {};
		}

		vector<string> getLocatDesc() {
			return locationDesc;
		}

		vector<pair<int, int>> getLocatExits() {
			return locationExits;
		}

		
};

