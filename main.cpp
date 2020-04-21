#include "index-trie.h"
#include <filesystem>
#include "bits/stdc++.h"
#include "constant.h"
#include "nlohmann/json.hpp"
#include "main.h"
#include "wx/wx.h"
#include <wx/filedlg.h>
// for convenience
using json = nlohmann::json;

using namespace std;

namespace fs = std::filesystem;
string DATA_BASE_PATH;
wxBEGIN_EVENT_TABLE(mainFrame, wxFrame)

    //EVT_BUTTON(1001,OnButtonClicked)
    EVT_BUTTON(INDEX_BUTTON_ID, mainFrame::on_index_button_clicked)
	EVT_BUTTON(SEARCH_BUTTON_ID, mainFrame::on_search_button_clicked)
	EVT_BUTTON(CLEAR_BUTTON_ID, mainFrame::on_clear_button_clicked)
	EVT_BUTTON(BROWSE_BUTTON_ID, mainFrame::on_browse_button_clicked)
wxEND_EVENT_TABLE()

std::string getexepath()
{
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
}

mainFrame::mainFrame() : wxFrame(nullptr, MAIN_FORM_ID, "Inverted Index", wxPoint(30, 30), wxSize(1000, 800))
{
	//button1 = new wxButton(this,1001,"Click ME", wxPoint(10,10),wxSize(150,50));
	//	textbox1 = new wxTextCtrl(this,wxID_ANY,"hint", wxPoint(10,70),wxSize(300,30));
	//	listbox1 = new wxListBox(this,wxID_ANY, wxPoint(10,110),wxSize(300,300));
	path_text = new wxStaticText(this, PATH_TEXT_ID, "Please enter the input folder path here: ", wxPoint(10, 30));
	path_textbox = new wxTextCtrl(this, PATH_TEXT_BOX_ID, "", wxPoint(10, 50), wxSize(300, 40));
	index_button = new wxButton(this, INDEX_BUTTON_ID, "Inedx", wxPoint(450, 50), wxSize(100, 40));
	browse_button = new wxButton(this, BROWSE_BUTTON_ID, "Browse", wxPoint(330, 50), wxSize(70, 40));
	result_list = new wxListBox(this, RESULT_LIST_ID, wxPoint(10, 130), wxSize(600, 600));
	search_text = new wxStaticText(this, SEARCH_TEXT_ID, "Enter the search word here: ", wxPoint(650, 380));
	word_textbox = new wxTextCtrl(this, WORD_TEXT_BOX_ID, "", wxPoint(650, 400), wxSize(150, 40));
	search_button = new wxButton(this, SEARCH_BUTTON_ID, "Search", wxPoint(650, 460), wxSize(100, 40));
	status_text = new wxStaticText(this, STATUS_TEXT_ID, "", wxPoint(650, 600));
	clear_button = new wxButton(this, CLEAR_BUTTON_ID, "Clear", wxPoint(750, 460), wxSize(100, 40));

	string cur_folder_path = getexepath();
	cur_folder_path = cur_folder_path.substr(0, cur_folder_path.find_last_of('/') + 1);
	DATA_BASE_PATH = cur_folder_path + DATABASE;
	if (!fs::exists(DATA_BASE_PATH))
	{
		fs::create_directory(DATA_BASE_PATH);
	}
}
mainFrame::~mainFrame() {}

map<string, vector<pair<string, int>>> read_from_json(string file_path)
{
	json j;
	std::ifstream i;
	i.open(file_path, std::ofstream::in);
	map<string, vector<pair<string, int>>> mp;
	i >> j;
	for (auto &i : j.items())
	{
		json k = i.value();
		string word = i.key();
		for (auto &cur : k.items())
		{
			mp[word].push_back({cur.key(), cur.value()});
		}
	}

	i.close();
	return mp;
}

void mainFrame::on_browse_button_clicked(wxCommandEvent &event)
{

	wxFileDialog openFileDialog(this, _("Choose the input Folder"), "", "",
				    "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return; // the user changed idea...
	// proceed loading the file chosen by the user;
	// this can be done with e.g. wxWidgets input streams:
	string input_path = (string)openFileDialog.GetPath();
	input_path = input_path.substr(0, input_path.find_last_of('/') + 1);
	path_textbox->SetValue(input_path);
}

void mainFrame::on_index_button_clicked(wxCommandEvent &event)
{

	// path for the input folder, later this would be entered in text_box by user
	string input_path = (string)path_textbox->GetValue();
	trie myTrie;									// create a tire wich would build our index
	vector<string> words;								// vector of all words in the doucments, used to insert them into the trie
	string input_folder_name = input_path.substr(input_path.find_last_of('/') + 1); // get's the input folder nam
	bool already_indexed = 0;
	for (auto file : fs::directory_iterator(DATA_BASE_PATH))
	{
		string full_path = file.path();
		string cur_file_name = full_path.substr(full_path.find_last_of('/') + 1, full_path.find_last_of('.') - full_path.find_last_of('/') - 1);
		if (cur_file_name == input_folder_name)
			already_indexed = 1;
	}

	if (already_indexed)
		status_text->SetLabel("This Folder is already Indexed");
	else
		status_text->SetLabel("Indexing Folder!!!!");
	if (!already_indexed)
	{
		for (auto file : fs::directory_iterator(input_path))
		{
			words.clear();
			string file_name = file.path();
			// opne file, and adds all words in it into a vector then in inserts them into the trie
			ifstream myfile(file_name);
			if (myfile.is_open())
			{
				string word;
				while (myfile >> word)
				{
					if (word.find('.') != string::npos)
						word.erase(word.find('.'));
					if (word.find(',') != string::npos)
						word.erase(word.find(','));
					words.push_back(word);
				}
				myfile.close();
			}

			for (auto word : words)
			{
				myTrie.add_word(word.data(), file_name);
			}
		}

		// as for now, we store the index in a txt file and a json file

		string database_file_path = DATA_BASE_PATH + input_folder_name + ".txt"; // path of the index txt file (name = input_folder)
		ofstream ofs;
		ofs.open(database_file_path, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
		json j;
		myTrie.store_trie(database_file_path, "", j);
		string database_json_file_path = DATA_BASE_PATH + input_folder_name + ".json"; // path of the index json file (name = input_folder)
		ofstream my_json_file;
		my_json_file.open(database_json_file_path, ios::out | std::ofstream::trunc);
		my_json_file << j;
		my_json_file.close();

		status_text->SetLabel("Finished Indexing");
	}
}

void mainFrame::on_search_button_clicked(wxCommandEvent &event)
{
	string input_path = (string)path_textbox->GetValue();
	string word = (string)word_textbox->GetValue();
	string data_base_file_name = DATA_BASE_PATH + input_path.substr(input_path.find_last_of('/') + 1) + ".json";
	if (!fs::exists(data_base_file_name))
		status_text->SetLabel("Please Inedx this Folder first!");
	else
	{
		if (word == "")
			status_text->SetLabel("Please enter a word first!");
		else
		{
			map<string, vector<pair<string, int>>> m = read_from_json(data_base_file_name);
			if (m.find(word) == m.end())
				result_list->AppendString("There's no results for " + word);
			else
			{
				vector<pair<string, int>> result = m[word];
				result_list->AppendString("The result of " + word + " is:");
				for (auto i : result)
				{
					string s = to_string(i.second) + " times in " + i.first;
					result_list->AppendString(s);
				}
			}
		}
	}
}

void mainFrame::on_clear_button_clicked(wxCommandEvent &event)
{
	result_list->Clear();
}
