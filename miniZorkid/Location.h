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

		// Prints all location items, and contained items if container is open.
		void printItems() {
			if (items.empty()) {
				cout << "\t\tThere are no items here\n\n";
			}
			for (Item item : items) {
				cout << "\t\tThere is a " << item.get_itemName() << ":\n\n";
				cout << "\t\t\t" << item.get_itemDesc() << endl;
				if (!item.getContained().empty())
				cout << "\t\tInside " << item.get_itemName() << ":\n";
				{
					for (Item subItem : item.getContained()) {
						cout << "\t\t\tThere is a " << subItem.get_itemName() << ":\n";
						cout << "\t\t\t\t" << subItem.get_itemDesc() << endl << endl;
					}
				}
			}
		}

		// Gets which Door item is in parameter direction
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

		// Called when player drops item here
		void itemDropped(Item item) {
			items.push_back(item);
		}

		// Called when player picks up item from this location
		void removeItem(Item item) {
			auto it = std::find(this->items.begin(), this->items.end(), item);
			if (it != this->items.end()) {
				cout << "\t\tTaking " << item.get_itemName() << " from " << locationName << "\n\n";
				this->items.erase(it);
			}
		}

		// Adds contained
		void openContainer(Item& item) {
			item.open();
			if (item.getItemType() == 2)
			{
				for (const Item& containedItem : item.getContained()) {
					itemDropped(containedItem);
				}
			}
		}

		void openDoor() {
			accessible = true;
			cout << "\t\tYou can now access " << this->locationName << "\n\n";
		}
};

