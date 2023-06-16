#include<string>
#include<vector>
#include<iostream>

#pragma once

using namespace std;

class Item
{
	private:
		string itemName = "";
		bool usable = false, locked = true;
		int doorDirection = 0, itemType = 0;
		string itemDescription, useOn, eventSuccess, eventFail;
		vector<Item> contained;

	public:

		// For initializing usable items
		Item createObject(
			string name = "",
			bool use = false,
			string useOnObject = "",
			string desc = ""
		){
			Item item;
			item.itemType = 1;
			item.itemName = name;
			item.usable = use;
			item.itemDescription = desc;
			item.useOn = useOnObject;
			return item;
		}

		// to initialize container items
		Item createContainer(
			string name = "",
			bool lock = true,
			string open = "",
			string fail = "",
			vector<Item> itemsInside = {},
			string description = ""
		){
			Item item;
			item.itemType = 2;
			item.itemName = name;
			item.locked = lock;
			item.eventSuccess = open;
			item.eventFail = fail;
			item.contained = itemsInside;
			item.itemDescription = description;
			return item;
		}

		// to initalize doors
		Item createDoor(
			string doorName = "",
			int direction = 0,
			bool _locked = true,
			string open = "",
			string fail = "",
			string description = ""
		){
			Item item;
			item.itemType = 3;
			item.itemName = doorName;
			item.doorDirection = direction;
			item.locked = _locked;
			item.eventSuccess = open;
			item.eventFail = fail;
			item.itemDescription = description;
			return item;
		}

		
		string get_itemName() {
			return itemName;
		}

		string get_itemDesc() {
			return itemDescription;
		}

		string get_usedOn() {
			return useOn;
		}

		bool checkUsable() {
			return usable;
		}

		int getItemType() {
			return itemType;
		}

		int getDoorDirection() {
			if (itemType == 3) {
				return doorDirection;
			}
			else
			{
				return 0;
			}
		}

		vector<Item> getContained() {
			if (!locked) {
				return contained;
			}
			else {
				return {};
			}
		}

		void open() {
			if (locked) {
				locked = false;
				eventSucceeds();
			}
			else {
				cout << "\t\tCan't open item again\n";
			}
		}

		void eventSucceeds() {
			cout << "\t\t" << eventSuccess << "\n\n\n";
		}

		void eventFails() {
			cout << "\t\t" << eventFail << "\n\n\n";
		}

		bool isEmpty() {
			if (itemName == "" && itemType == 0) {
				return true;
			}
			else
				return false;
		}

		// Equality operator
		bool operator==(const Item& other) const
		{
			return itemName == other.itemName && itemType == other.itemType;
		}
};

