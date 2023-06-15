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
		World(vector<Location> locations = {}) {}

		void add_location(const Location& location) {
			areas.push_back(location);
		}

		Location get_location(const int& index) {
			for (Location& area : areas) {
				if (area.getLocationId() == index) {
					return area;
				}
			}
			return {};
		}

		vector<Location> get_locations() {
			return areas;
		}
};

