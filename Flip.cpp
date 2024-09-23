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
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	m_mainFrameSizer = new wxGridBagSizer( 2, 2 );
	m_mainFrameSizer->SetFlexibleDirection( wxBOTH );
	m_mainFrameSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_mainFrameSizer->SetMinSize( wxSize( 1,1 ) );
	m_lblInput = new wxStaticText( this, wxID_ANY, _("Input File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblInput->Wrap( -1 );
	m_mainFrameSizer->Add( m_lblInput, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_inputFile = new wxFilePickerCtrl( this, ID_FLP_INPUTFILE, wxEmptyString, _("Select a PDF file"), _("*.pdf"), wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	m_inputFile->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	m_mainFrameSizer->Add( m_inputFile, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* m_groupSwitches;
	m_groupSwitches = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Switches") ), wxVERTICAL );

	wxGridBagSizer* m_switchesSizer;
	m_switchesSizer = new wxGridBagSizer( 0, 0 );
	m_switchesSizer->SetFlexibleDirection( wxBOTH );
	m_switchesSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_switchDBP = new wxCheckBox( m_groupSwitches->GetStaticBox(), wxID_ANY, _("Show data before processing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_switchDBP->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_switchDBP->SetToolTip( _("Raw data pullled from the PDF will be shown before processing occurs. (-dbp)") );

	m_switchesSizer->Add( m_switchDBP, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_switchSWS = new wxCheckBox( m_groupSwitches->GetStaticBox(), wxID_ANY, _("Strip excessive whitespace"), wxDefaultPosition, wxDefaultSize, 0 );
	m_switchSWS->SetToolTip( _("Strip excessive whitespace from the pulled data. (-sws)") );

	m_switchesSizer->Add( m_switchSWS, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lblProcessPages = new wxStaticText( m_groupSwitches->GetStaticBox(), wxID_ANY, _("Only process these pages:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblProcessPages->Wrap( -1 );
	m_lblProcessPages->SetToolTip( _("Define which pages to process in the input file. (-p)\n\nUse number values for the pages. You can define \nindividual pages seperated with commas: \n2,4,6,8\n\nRanges of page with a dash:\n1-5\n\nOr a combination of both: \n2,4,6-8,12") );

	m_switchesSizer->Add( m_lblProcessPages, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_ProcessPages = new wxTextCtrl( m_groupSwitches->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_switchesSizer->Add( m_ProcessPages, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );


	m_switchesSizer->AddGrowableCol( 1 );

	m_groupSwitches->Add( m_switchesSizer, 1, wxALL|wxEXPAND, 5 );


	m_mainFrameSizer->Add( m_groupSwitches, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	m_lblOutput = new wxStaticText( this, wxID_ANY, _("Output File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblOutput->Wrap( -1 );
	m_lblOutput->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );

	m_mainFrameSizer->Add( m_lblOutput, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_outputFile = new wxFilePickerCtrl( this, ID_FLP_OUTPUTFILE, wxEmptyString, _("Select output file location"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	m_outputFile->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_outputFile->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	m_mainFrameSizer->Add( m_outputFile, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxArrayString m_useTemplateChoices;
	m_useTemplate = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_useTemplateChoices, wxCB_SORT );
	m_useTemplate->SetSelection( 0 );
	m_mainFrameSizer->Add( m_useTemplate, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_lblUseTemplate = new wxStaticText( this, wxID_ANY, _("Use template:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblUseTemplate->Wrap( -1 );
	m_mainFrameSizer->Add( m_lblUseTemplate, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_btnLaunch = new wxButton( this, flipID_MAIN_BTN_LAUNCH, _("LAUNCH"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mainFrameSizer->Add( m_btnLaunch, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );


	m_mainFrameSizer->AddGrowableCol( 1 );
	m_mainFrameSizer->AddGrowableRow( 0 );
	m_mainFrameSizer->AddGrowableRow( 1 );
	m_mainFrameSizer->AddGrowableRow( 2 );
	m_mainFrameSizer->AddGrowableRow( 3 );
	m_mainFrameSizer->AddGrowableRow( 4 );

	this->SetSizer( m_mainFrameSizer );
	this->Layout();
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFile_About;
	m_menuFile_About = new wxMenuItem( m_menuFile, ID_MENU_FILE_ABOUT, wxString( _("About") ) + wxT('\t') + wxT("Ctrl+I"), _("Show information about program"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuFile_About->SetBitmaps( wxNullBitmap );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuFile_About->SetBitmap( wxNullBitmap );
	#endif
	m_menuFile->Append( m_menuFile_About );

	wxMenuItem* m_menuFile_TemplateEditor;
	m_menuFile_TemplateEditor = new wxMenuItem( m_menuFile, ID_MENU_FILE_TEMPLATEEDITOR, wxString( _("Template Editor") ) + wxT('\t') + wxT("Ctrl+T"), _("Open the Template Editor"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFile_TemplateEditor );

	wxMenuItem* m_menuFile_Quit;
	m_menuFile_Quit = new wxMenuItem( m_menuFile, ID_MENU_FILE_QUIT, wxString( _("Quit") ) + wxT('\t') + wxT("Ctrl+Q"), _("Quit the program"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFile_Quit );

	m_menuBar->Append( m_menuFile, _("File") );

	m_menuLog = new wxMenu();
	wxMenuItem* m_menuLog_ProgramLog;
	m_menuLog_ProgramLog = new wxMenuItem( m_menuLog, ID_MENU_LOG_PROGRAMLOG, wxString( _("Program Log") ) + wxT('\t') + wxT("Ctrl+L"), _("Opens the Program Log"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuLog_ProgramLog->SetBitmaps( wxNullBitmap );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuLog_ProgramLog->SetBitmap( wxNullBitmap );
	#endif
	m_menuLog->Append( m_menuLog_ProgramLog );

	m_menuBar->Append( m_menuLog, _("Log") );

	this->SetMenuBar( m_menuBar );

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

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

	wxGridBagSizer* m_programLogSizer;
	m_programLogSizer = new wxGridBagSizer( 0, 0 );
	m_programLogSizer->SetFlexibleDirection( wxBOTH );
	m_programLogSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxtextCtrl_programLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_programLogSizer->Add( wxtextCtrl_programLog, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );


	m_programLogSizer->AddGrowableCol( 0 );
	m_programLogSizer->AddGrowableRow( 0 );

	this->SetSizer( m_programLogSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

ProgramLog::~ProgramLog()
{
}

TemplateEditor::TemplateEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	wxGridBagSizer* m_templateEditorSizer;
	m_templateEditorSizer = new wxGridBagSizer( 0, 0 );
	m_templateEditorSizer->SetFlexibleDirection( wxBOTH );
	m_templateEditorSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_filePickerAddNew = new wxFilePickerCtrl( this, wxID_ANY, wxT("Add new template..."), _("Add new template file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	m_templateEditorSizer->Add( m_filePickerAddNew, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	m_btnAddTemplate = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateEditorSizer->Add( m_btnAddTemplate, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxArrayString m_templatesExistingChoices;
	m_templatesExisting = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_templatesExistingChoices, 0 );
	m_templatesExisting->SetSelection( 0 );
	m_templatesExisting->SetToolTip( _("Select an existing template to load it into the editor below") );

	m_templateEditorSizer->Add( m_templatesExisting, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	m_btnRemoveTemplate = new wxButton( this, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateEditorSizer->Add( m_btnRemoveTemplate, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_templateEditor = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	m_templateEditor->SetFont( wxFont( 9, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Fixedsys") ) );
	m_templateEditor->SetToolTip( _("Template Editor Help\n\n// this is a comment\n# this is a comment\n// blank lines are ignored (whitespace only is also ignored)\n\nleft side regex => right side substitution\n// ' => ' is the seperator, including leading and trailing whitespace: space, equals, greater than, space\n\ncat => dog\n// will change cat to dog\ncockroaches => \n// will make all cockroaches disappear") );
	m_templateEditor->SetMinSize( wxSize( -1,150 ) );

	m_templateEditorSizer->Add( m_templateEditor, wxGBPosition( 2, 0 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND, 5 );


	m_templateEditorSizer->AddGrowableCol( 1 );
	m_templateEditorSizer->AddGrowableRow( 2 );

	this->SetSizer( m_templateEditorSizer );
	this->Layout();
	m_templateEditorStatusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

TemplateEditor::~TemplateEditor()
{
}
