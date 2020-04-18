#include "index-trie.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
using namespace std;

void trie::add_word(const char *word, std::string &path)
{
	if (*word == '\0')
	{
		isColmplete = 1;
		trie::mp[path]++;
	}
	else
	{
		char cur = *word;
		if (children.find(cur) == children.end())
		{
			trie a;
			children[cur] = a;
		}

		children[cur].add_word(word + 1, path);
	}
}

std::map<std::string, int> *trie::get_word_stats(const char *word)
{

	if (*word == '\0')
	{

		return &mp;
	}
	else
	{

		char cur = *word;
		if (children.find(cur) == children.end())
			return nullptr;

		return children[cur].get_word_stats(word + 1);
	}
}

void trie::store_trie(string file_path, string word, json &j)
{

	if (isColmplete)
	{
		// this is a word
		// add a new json entry

		auto word_map = mp;
		ofstream my_file;
		my_file.open(file_path, ios::app);
		my_file << word << ": \n";
		vector<pair<string, int>> v;
		for (auto entry : word_map)
		{
			string path = entry.first;
			int freq = entry.second;
			my_file << freq << " time in : " << path << "\n";
			v.push_back({path, freq});
		}
		json js(mp);
		j[word] = js;

		return;
	}

	else
	{
		for (auto child : children)
		{

			trie *sub_trie = &child.second;

			sub_trie->store_trie(file_path, word + child.first, j);
		}
	}
}