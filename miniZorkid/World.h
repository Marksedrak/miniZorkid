#include<vector>
#include"Location.h"
#include"Direction.h"

#pragma once
class World
{
	private:
		vector<Location> areas;

	public:
		// World constructor
		World(vector<Location> locations = {}) : areas(locations) {}

		void add_location(const Location& location) {
			areas.push_back(location);
		}

		Location get_location(int index) {
			return areas[index];
		}



		//Location navigateTo(Location& current, string direction) {
		//	vector<pair<string, int>> currentExits = current.getLocatExits();

		//	for (const pair<string, int> exit : currentExits) {
		//		if (direction >= static_cast<int>(Directions::north) && direction == Directions::south) {
		//			switch (direction)
		//			{
		//			case 1:

		//				break;
		//			}
		//		}
		//	}
		//}
};

