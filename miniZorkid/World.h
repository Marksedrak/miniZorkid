#include<vector>
#include<iostream>
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

		void printWorld() {
			for (Location& location : areas) {
				cout << location.getLocationName() << ": " << endl;
				cout << "ID: " << endl << location.getLocationId() << endl;
				cout << "Exits: " << endl;
				for (auto exit : location.getLocatExits()) {
					cout << "direction: " << exit.first << endl;
					cout << "destination: " << exit.second << endl;
				}
				cout << endl;
			}
		}
};

