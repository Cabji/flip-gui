///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Flip.h"

///////////////////////////////////////////////////////////////////////////

Main::Main( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,300 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	wxGridBagSizer* gbSizer_mainFrame;
	gbSizer_mainFrame = new wxGridBagSizer( 2, 2 );
	gbSizer_mainFrame->SetFlexibleDirection( wxBOTH );
	gbSizer_mainFrame->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxst_lblInput = new wxStaticText( this, wxID_ANY, _("Input File:"), wxDefaultPosition, wxDefaultSize, 0 );
	wxst_lblInput->Wrap( -1 );
	gbSizer_mainFrame->Add( wxst_lblInput, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxfpicker_inputFile = new wxFilePickerCtrl( this, ID_FLP_INPUTFILE, wxEmptyString, _("Select a PDF file"), _("*.pdf"), wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	wxfpicker_inputFile->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	gbSizer_mainFrame->Add( wxfpicker_inputFile, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* wxsbSizer_groupSwitches;
	wxsbSizer_groupSwitches = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Switches") ), wxVERTICAL );

	wxcBox_switchDBP = new wxCheckBox( wxsbSizer_groupSwitches->GetStaticBox(), wxID_ANY, _("Show data before processing"), wxDefaultPosition, wxDefaultSize, 0 );
	wxsbSizer_groupSwitches->Add( wxcBox_switchDBP, 0, wxALL, 5 );

	wxcBox_switchSWS = new wxCheckBox( wxsbSizer_groupSwitches->GetStaticBox(), wxID_ANY, _("Strip excessive whitespace"), wxDefaultPosition, wxDefaultSize, 0 );
	wxsbSizer_groupSwitches->Add( wxcBox_switchSWS, 0, wxALL, 5 );


	gbSizer_mainFrame->Add( wxsbSizer_groupSwitches, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	wxst_lblOutput = new wxStaticText( this, wxID_ANY, _("Output File:"), wxDefaultPosition, wxDefaultSize, 0 );
	wxst_lblOutput->Wrap( -1 );
	wxst_lblOutput->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );

	gbSizer_mainFrame->Add( wxst_lblOutput, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxfPicker_outputFile = new wxFilePickerCtrl( this, ID_FLP_OUTPUTFILE, wxEmptyString, _("Select output file location"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	wxfPicker_outputFile->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	wxfPicker_outputFile->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	gbSizer_mainFrame->Add( wxfPicker_outputFile, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxArrayString wxchoice_useTemplateChoices;
	wxchoice_useTemplate = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxchoice_useTemplateChoices, wxCB_SORT );
	wxchoice_useTemplate->SetSelection( 0 );
	gbSizer_mainFrame->Add( wxchoice_useTemplate, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	wxst_lblUseTemplate = new wxStaticText( this, wxID_ANY, _("Use template:"), wxDefaultPosition, wxDefaultSize, 0 );
	wxst_lblUseTemplate->Wrap( -1 );
	gbSizer_mainFrame->Add( wxst_lblUseTemplate, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxbtn_Launch = new wxButton( this, wxID_ANY, _("LAUNCH"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer_mainFrame->Add( wxbtn_Launch, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );


	gbSizer_mainFrame->AddGrowableCol( 1 );
	gbSizer_mainFrame->AddGrowableRow( 0 );
	gbSizer_mainFrame->AddGrowableRow( 1 );
	gbSizer_mainFrame->AddGrowableRow( 2 );
	gbSizer_mainFrame->AddGrowableRow( 3 );
	gbSizer_mainFrame->AddGrowableRow( 4 );

	this->SetSizer( gbSizer_mainFrame );
	this->Layout();
	wx_menubar_mainFrame = new wxMenuBar( 0 );
	wxMenu_File = new wxMenu();
	wxMenuItem* wxMenuItem_fileAbout;
	wxMenuItem_fileAbout = new wxMenuItem( wxMenu_File, ID_MENU_FILE_ABOUT, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	wxMenu_File->Append( wxMenuItem_fileAbout );

	wxMenuItem* wxMenuItem_fileQuit;
	wxMenuItem_fileQuit = new wxMenuItem( wxMenu_File, ID_MENU_FILE_QUIT, wxString( _("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	wxMenu_File->Append( wxMenuItem_fileQuit );

	wx_menubar_mainFrame->Append( wxMenu_File, _("File") );

	wxMenu_Log = new wxMenu();
	wxMenuItem* wxMenuItem_logProgramLog;
	wxMenuItem_logProgramLog = new wxMenuItem( wxMenu_Log, ID_MENU_LOG_PROGRAMLOG, wxString( _("Program Log") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	wxMenuItem_logProgramLog->SetBitmaps( wxNullBitmap );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	wxMenuItem_logProgramLog->SetBitmap( wxNullBitmap );
	#endif
	wxMenu_Log->Append( wxMenuItem_logProgramLog );

	wx_menubar_mainFrame->Append( wxMenu_Log, _("Log") );

	this->SetMenuBar( wx_menubar_mainFrame );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

Main::~Main()
{
}

ProgramLog::ProgramLog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	wxGridBagSizer* gbSizer_programLogFrame;
	gbSizer_programLogFrame = new wxGridBagSizer( 0, 0 );
	gbSizer_programLogFrame->SetFlexibleDirection( wxBOTH );
	gbSizer_programLogFrame->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxtextCtrl_programLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	gbSizer_programLogFrame->Add( wxtextCtrl_programLog, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );


	gbSizer_programLogFrame->AddGrowableCol( 0 );
	gbSizer_programLogFrame->AddGrowableRow( 0 );

	this->SetSizer( gbSizer_programLogFrame );
	this->Layout();

	this->Centre( wxBOTH );
}

ProgramLog::~ProgramLog()
{
}
