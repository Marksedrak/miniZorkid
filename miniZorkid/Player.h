#include "Location.h"
#include "Item.h"
#include <vector>
#include <iostream>

#pragma once
class Player
{
	private:
		int playerId;
		Location playerLocation;
		Location lastLocation;
		vector<Item> Inventory;

	public:
		Player(int id, Location playerLocat = {}, Location last = {}, vector<Item> invent = {}) : playerId(id), playerLocation(playerLocat), Inventory(invent) {}

		Location locate_Player() { return playerLocation; }

		Location getLastLocation() { return lastLocation; }

		vector<Item> getInventory() { return Inventory; }

		void setPlayerLocation(Location destination) {
			lastLocation = playerLocation;
			playerLocation = destination;
		}

		bool didNotMove(Location current) {
			return current.getLocationId() == playerLocation.getLocationId();
		}

		vector<string> getInventItemNames() {
			if (Inventory.empty()) {
				return { "Such Empty" };
			}
			else
			{
				vector<string> itemNames;
				for (auto item : Inventory) {
					itemNames.push_back(item.get_itemName());
				}
				return itemNames;
			}
		}

		void pickUpItem(Item item) {
			if (item.isEmpty()) {
				cout << "\t\tNo valid item name detected" << endl << "\t\tPlease enter a valid item name after your command\n\n\n";
			}
			else {
				if (item.getItemType() != 1) {
					cout << "\t\tCan't pick up " << item.get_itemName() << "\n\n\n";
				}
				else {
					Inventory.push_back(item);
					playerLocation.removeItem(item);
				}
			}
		}

		void dropItem(Item item) {
			auto it = find(Inventory.begin(), Inventory.end(), item);
			if (it != Inventory.end()) {
				cout << "\t\tDropping " << item.get_itemName() << "\n\n\n";
				Inventory.erase(it);
				playerLocation.itemDropped(item);
			}
		}

};

