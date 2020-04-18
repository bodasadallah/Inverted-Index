#include "index-trie.h"
#include <filesystem>
#include "bits/stdc++.h"
#include "constant.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

using namespace std;

namespace fs = std::filesystem;

// read from json file

map<string,vector<pair<string, int>>> read_from_json(string file_path){
	json j ;
	 std::ifstream i;
	 i.open(file_path, std::ofstream::in );
	 map<string,vector<pair<string, int>>> mp;
	 i >> j;
	for (auto &i : j.items()){
		json k = i.value();
		string word = i.key();
		for(auto &cur : k.items()){
			mp[word].push_back({cur.key(),cur.value()});
		}

	}

	i.close();
	return mp;

}
int main()
{
	// path for the input folder, later this would be entered in text_box by user
	string input_path = "/home/boda/Desktop/Data";
	trie myTrie;																// create a tire wich would build our index
	vector<string> words;														// vector of all words in the doucments, used to insert them into the trie
	string input_folder_name = input_path.substr(input_path.find_last_of('/')); // get's the input folder nam

	for (auto file : fs::directory_iterator(input_path))
	{

		string file_name = file.path();
		// opne file, and adds all words in it into a vector then in inserts them into the trie
		ifstream myfile(file_name);
		if (myfile.is_open())
		{
			string word;
			while (myfile >> word)
			{
				words.push_back(word);
			}
			myfile.close();
		}
		cout << (int)words.size() << std::endl;
		for (auto word : words)
		{
			myTrie.add_word(word.data(), file_name);
		}
	}

	//string query = "know";
	//map<string,int> *m = myTrie.get_word_stats(query.c_str());

	//for(auto i : *m){
	//cout << query <<" is found :" << i.second <<" times in:" <<i.first <<"  document \n";
	//}

	// as for now, we store the index in a txt file and a json file

	string database_file_path = DATABASE + input_folder_name + ".txt";	// path of the index txt file (name = input_folder)
	ofstream ofs;
	ofs.open(database_file_path, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	json j;
	myTrie.store_trie(database_file_path, "", j);

	ofstream my_json_file;
	my_json_file.open("my_json.json", ios::out | std::ofstream::trunc);
	my_json_file << j;
	my_json_file.close();

	map<string,vector<pair<string, int>>> m = read_from_json("my_json.json");
	for(auto i : m ){
		cout<< i.first;
		for(auto k: i.second)
			cout<< k.first<<k.second<<endl;

	}




	return 0;
};
