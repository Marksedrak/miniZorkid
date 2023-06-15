#include "World.h"
#include "Item.h"
#include <vector>

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

		void setPlayerLocation(const Location& destination) {
			lastLocation = playerLocation;
			playerLocation = destination;
		}

		bool didNotMove(Location current) {
			return current.getLocationId() == playerLocation.getLocationId();
		}

		vector<string> getInventItems() {
			vector<string> itemNames;
			for (auto item : Inventory) {
				itemNames.push_back(item.get_itemName());
			}
			return itemNames;
		}
};

