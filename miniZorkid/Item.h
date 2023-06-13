#include<string>

#pragma once

using namespace std;

class Item
{
	private:
		string itemName;
		bool usable;
		string itemDescription;

	public:
		Item(string name = "", bool use = false, string desc = "") : itemName(name), usable(use), itemDescription(desc) {}
		
		string get_itemName() {
			return itemName;
		}

		string get_itemDesc() {
			return itemDescription;
		}

		bool checkUsable() {
			return usable;
		}
};

