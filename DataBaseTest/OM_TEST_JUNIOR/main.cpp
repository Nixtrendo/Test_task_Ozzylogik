#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include "sql_functions.h"
using namespace std;
using CSV_type = vector<vector<string>>; //2d vector 171x6
const std::vector<char> dictionary = { 'q','w','e','r','t','y','u','i','o','p','a','s','d','f',
										'g','h','j','k','l','z','x','c','v','b','n','m','1','2',
										'3','4','5','6','7','8','9','0' }; //36 symbols
void SplitString(string s, vector<string>& v) {
	string temp = "";
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] == ';') {
			v.push_back(temp);
			temp = "";
		}
		else {
			temp.push_back(s[i]);
		}

	}
	v.push_back(temp);
}
void write_csv(const CSV_type& i_csv, const string& path) {
	ofstream csvFile;
	csvFile.open(path);
	for (auto cols : i_csv) {
		for (auto str : cols) {
			csvFile << str << ';';
		}
		csvFile << "\n";
	}
	csvFile.close();
}
CSV_type create_csv(const string& path) {
	//arguments
	srand(time(NULL));
	CSV_type csv;
	vector<string> to_add;//temp vector to as row
	string generated_str; // random generated string
	int string_counter = 0, random_index;
	while (string_counter != 1020) { //create first 1020 strings
		to_add = {};
		for (int j = 0; j < 6; j++) {
			generated_str = "";
			for (int i = 0; i < 8; i++) {
				random_index = rand() % 36;
				generated_str.push_back(dictionary[random_index]);
			}
			string_counter++;
			to_add.push_back(generated_str);
		}
		csv.push_back(to_add);
	}
	//create last 4 strings
	to_add = {};
	for (int j = 0; j < 4; j++) {
		generated_str = "";
		for (int i = 0; i < 8; i++) {
			random_index = rand() % 36;
			generated_str.push_back(dictionary[random_index]);
		}
		to_add.push_back(generated_str);
	}
	csv.push_back(to_add);

	//write csv file
	write_csv(csv, path);
	return csv;
}
void redacted_csv(const string& path) {
	ifstream csvFile;
	CSV_type csv;
	vector<string> to_add;
	string i_line;
	csvFile.open(path);
	while (getline(csvFile, i_line)) {
		to_add = {};
		SplitString(i_line, to_add);
		csv.push_back(to_add);
	}
	csvFile.close();


	vector<int> rows_to_delete;
	for (int i = 0; i < 171; i++) {
		for (int j = 0; j < 6; j++) {
			if (csv[i][j][0] == 'e' || csv[i][j][0] == 'u' || csv[i][j][0] == 'i' || csv[i][j][0] == 'o' || csv[i][j][0] == 'a') {
				rows_to_delete.push_back(i);
				break;
			}
			for (int c = 0; c < 8; c++) {
				if (csv[i][j][c] == '1' || csv[i][j][c] == '3' || csv[i][j][c] == '5' || csv[i][j][c] == '7' || csv[i][j][c] == '9') {
					csv[i][j][c] = '#';
				}
			}
		}
	}
	for (int i = rows_to_delete.size() - 1; i >= 0; i--) {
		csv.erase(csv.begin() + rows_to_delete[i]);
	}
	string new_name = "redacted_" + path;
	write_csv(csv, new_name);
}

int main() {
	string path = "random_strings.csv";
	const char* dir = "D:\\SQL\\CODES.db";
	CSV_type csv = create_csv(path);
	redacted_csv(path);
	createDB(dir);
	createTable(dir);
	insertData(dir, csv);
	updateData(dir);
	system("pause");
	return 0;
}