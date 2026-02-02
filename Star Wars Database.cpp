#include<iostream>
#include <unordered_map>
#include <string>
#include<vector>
extern "C" {
#include "sqlite3.h"
}

using namespace std;

class Character {
public:
	int id;
	string name;
	string side;
	string bio;
	Character(int i, string n, string s,string b) : id(i), name(n), side(s), bio(b){}

	void display() {
		cout <<"ID: " << id << "\nName: " << name << "\n Side: " << side << "\n Bio :" << bio << endl;
	}
};
class Database {
private:
	sqlite3* db = nullptr;
	unordered_map<string, Character*> charactermap;

public:
	Database() {
		if (sqlite3_open("starwars.db", &db) != SQLITE_OK) {
			cerr << "Error couldn't open the file" << endl;
		}
	}
	~Database() {
		for (auto pair : charactermap) {
			delete pair.second;
		}
		sqlite3_close(db);
	}
	void loadfromSQL() {
		const char* query = "SELECT id, name, side, bio FROM Characters";
		sqlite3_stmt* stmt = nullptr;

		if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				int id = sqlite3_column_int(stmt, 0);
				const unsigned char* nametext = sqlite3_column_text(stmt, 1);
				const unsigned char* sidetext = sqlite3_column_text(stmt, 2);
				const unsigned char* biotext = sqlite3_column_text(stmt, 3);

				string name = nametext ? reinterpret_cast<const char*>(nametext) : "";
				string side = sidetext ? reinterpret_cast<const char*>(sidetext) : "";
				string bio = biotext ? reinterpret_cast<const char*>(biotext) : "";

				charactermap[name] = new Character(id, name, side, bio);
			}
		}
		sqlite3_finalize(stmt);
		cout << "System :" << charactermap.size() << " character loaded from holocron" << endl;
	}
	void findcharacter(string name) {
		if (charactermap.count(name)) {
			charactermap[name]->display();
		}
		else {
			cout << "Search error " << name << " not found in the archeves " << endl;
		}
	}
};

int main() {
	Database holocron;
	holocron.loadfromSQL();

	string input;
	cout << "\t\t\t\t\t STAR WARS MINI DATABASE " << endl;
	while (true) {
		cout << "Enter character name " << endl;
		getline(cin, input);
		if (input == "exit") 
	    break;

		holocron.findcharacter(input);
	}
	

	return 0;
}