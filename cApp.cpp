#include "cApp.h"
#include "main.h"
wxIMPLEMENT_APP(cApp);

bool cApp::OnInit(){
	//main_frame = new main::main();
	//main_frame->Show();
	 main_frame = new mainFrame();
	main_frame->Show(true);

	return true;
}


