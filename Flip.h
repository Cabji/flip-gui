///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gbsizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class Main
///////////////////////////////////////////////////////////////////////////////
class Main : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_FLP_INPUTFILE = 6000,
			ID_FLP_OUTPUTFILE,
			ID_MENU_FILE_ABOUT,
			ID_MENU_FILE_QUIT,
			ID_MENU_LOG_PROGRAMLOG,
		};

		wxStaticText* wxst_lblInput;
		wxFilePickerCtrl* wxfpicker_inputFile;
		wxCheckBox* wxcBox_switchDBP;
		wxCheckBox* wxcBox_switchSWS;
		wxStaticText* wxst_lblOutput;
		wxFilePickerCtrl* wxfPicker_outputFile;
		wxChoice* wxchoice_useTemplate;
		wxStaticText* wxst_lblUseTemplate;
		wxButton* wxbtn_Launch;
		wxMenuBar* wx_menubar_mainFrame;
		wxMenu* wxMenu_File;
		wxMenu* wxMenu_Log;
		wxStatusBar* m_statusBar1;

	public:

		Main( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Flip"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 373,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~Main();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ProgramLog
///////////////////////////////////////////////////////////////////////////////
class ProgramLog : public wxFrame
{
	private:

	protected:
		wxTextCtrl* wxtextCtrl_programLog;

	public:

		ProgramLog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Program Log"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ProgramLog();

};

