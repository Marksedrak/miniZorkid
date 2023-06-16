#include<string>
#include<vector>
#include"Item.h"
#include<iostream>
#include<algorithm>
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
			if (items.empty()) {
				cout << "\t\tThere are no items here\n\n";
			}
			for (Item item : itemsInLocation()) {
				cout << "\t\tThere is a " << item.get_itemName() << ":\n";
				cout << "\t\t" << item.get_itemDesc() << endl;
				for (Item subItem : item.getContained()) {
					cout << "\t\tInside " << item.get_itemName() << ":\n";
					cout << "\t\t\tThere is a " << item.get_itemName() << ":\n";
					cout << "\t\t\t" << item.get_itemDesc() << endl;
				}
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

		void itemDropped(Item item) {
			items.push_back(item);
		}

		void removeItem(Item item) {
			auto it = std::find(this->items.begin(), this->items.end(), item);
			if (it != this->items.end()) {
				cout << "\t\tTaking " << item.get_itemName() << " from " << locationName << "\n\n";
				this->items.erase(it);
			}
		}

		void openContainer(Item item) {
			for (Item containedItem : item.getContained()) {
				itemDropped(containedItem);
			}
		}

		void openDoor() {
			accessible = true;
		}
};

