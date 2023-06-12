#include<string>
#include<vector>

using namespace std;

class Location
{
	private:
		int locationId;
		string locationName;
		vector<string> locationDesc;
		vector<pair<string, int>> locationExits;

	public:
		Location initLocation(int Id, string name, vector<string> description, vector<pair<string, int>> exit) {
			locationId = Id;
			locationName = name;
			locationDesc = description;
			locationExits = exit;
		}

		int getLocationId() {
			return locationId;
		}

		string getLocationName() {
			return locationName;
		}

		vector<string> getLocatDesc() {
			return locationDesc;
		}

		vector<pair<string, int>> getLocatExits() {
			return locationExits;
		}
};

