#include<string>
#include<vector>

using namespace std;

class Location
{
	private:
		int locationId;
		string locationName;
		bool accessible;
		vector<string> locationDesc;
		vector<pair<string, int>> locationExits;

	public:
		Location(int Id = 1, string name = "First Room", bool access = true, vector<string> description = { "Initial Room of the Game" }, vector<pair<string, int>> exit = {}) {
			locationId = Id;
			locationName = name;
			accessible = access;
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

		vector<string> getLocatDesc() {
			return locationDesc;
		}

		vector<pair<string, int>> getLocatExits() {
			return locationExits;
		}
};

