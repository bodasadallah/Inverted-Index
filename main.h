#ifndef MAIN_H
#define MAIN_H

#include "wx/wx.h"

class mainFrame : public wxFrame
{
	public:
	mainFrame();
	~mainFrame();

	wxButton * index_button = nullptr;
	wxButton * search_button = nullptr;
	wxTextCtrl* path_textbox = nullptr;
	wxTextCtrl* word_textbox = nullptr;
	wxListBox * result_list = nullptr;
	wxStaticText* status_text= nullptr;
	wxStaticText* path_text= nullptr;
	wxStaticText* search_text= nullptr;
	wxButton* clear_button = nullptr;

	void on_index_button_clicked(wxCommandEvent &event);
	void on_search_button_clicked(wxCommandEvent &event);
	void on_clear_button_clicked(wxCommandEvent &event);


	wxDECLARE_EVENT_TABLE();
};

#endif