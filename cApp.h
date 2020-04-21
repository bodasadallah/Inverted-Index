#ifndef CAPP_H
#define CAPP_H

#include "wx/wx.h"
#include "main.h"
#include "wx/app.h"
class cApp : public wxApp
{

	public:
	virtual bool OnInit();

	mainFrame* main_frame = nullptr ;

};

#endif