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
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/spinbutt.h>

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
			flipID_MAIN_BTN_LAUNCH,
			ID_MENU_FILE_ABOUT,
			ID_MENU_FILE_TEMPLATEEDITOR,
			ID_MENU_FILE_QUIT,
			ID_MENU_FILE_DATAVIEWER,
			ID_MENU_LOG_PROGRAMLOG,
		};

		wxGridBagSizer* m_mainFrameSizer;
		wxStaticText* m_lblInput;
		wxFilePickerCtrl* m_inputFile;
		wxCheckBox* m_switchDBP;
		wxCheckBox* m_switchSWS;
		wxStaticText* m_lblProcessPages;
		wxTextCtrl* m_ProcessPages;
		wxStaticText* m_lblOutput;
		wxFilePickerCtrl* m_outputFile;
		wxStaticText* m_lblUseTemplate;
		wxButton* m_btnLaunch;
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxMenu* m_menuLog;
		wxStatusBar* m_statusBar;

	public:
		wxChoice* m_useTemplate;

		Main( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Flip"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 410,339 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

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

		ProgramLog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Flip :: Program Log"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ProgramLog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TemplateEditor
///////////////////////////////////////////////////////////////////////////////
class TemplateEditor : public wxFrame
{
	private:

	protected:
		wxFilePickerCtrl* m_filePickerAddNew;
		wxButton* m_btnAddTemplate;
		wxChoice* m_templatesExisting;
		wxButton* m_btnRemoveTemplate;
		wxTextCtrl* m_templateEditor;
		wxStatusBar* m_templateEditorStatusBar;

	public:

		TemplateEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Flip :: Template Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 456,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~TemplateEditor();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DataViewer
///////////////////////////////////////////////////////////////////////////////
class DataViewer : public wxFrame
{
	private:

	protected:
		wxGridBagSizer* m_dataviewerSizer;
		wxStaticText* m_lblDataBefore;
		wxStaticText* m_lblDataAfter;
		wxStaticText* m_lblSpinPages;
		wxTextCtrl* m_dataBefore;
		wxTextCtrl* m_dataAfter;
		wxSpinButton* m_spinPages;
		wxButton* m_btnContinueProcessing;
		wxButton* m_btnFinishProcessing;
		wxChoice* m_saveFormat;
		wxButton* m_btnSave;

	public:

		DataViewer( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Flip :: Data Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~DataViewer();

};

