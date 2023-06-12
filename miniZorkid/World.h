#include<vector>
#include"Location.h"

#pragma once
class World
{
	private:
		vector<Location> areas;

	public:
		void add_location(const Location& location) {
			areas.push_back(location);
		}

		
};

