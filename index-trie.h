#include "bits/stdc++.h"
#include "map"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
class trie
{
public:
	std::map<char, trie> children;
	std::map<std::string, int> mp;
	bool isColmplete;

	trie()
	{
		isColmplete = 0;
		children.clear();
	}
	void add_word(const char *word, std::string &path);

	std::map<std::string, int> *get_word_stats(const char *word);

	void store_trie(std::string file_path, std::string word, json &j);
};