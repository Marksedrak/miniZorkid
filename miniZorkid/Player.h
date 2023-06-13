#include "World.h"
#include "Item.h"
#include <vector>

#pragma once
class Player
{
	private:
		Location playerLocation;
		vector<Item> Inventory;

	public:
		Player(Location playerLocat = {}, vector<Item> invent = {}) : playerLocation(playerLocat), Inventory(invent) {}

		Location locate_Player() { return playerLocation; }

		vector<Item> getInventory() { return Inventory; }

		void setPlayerLocation(Location destination) {
			playerLocation = destination;
		}
};

