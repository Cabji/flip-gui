#ifndef __FlipMain__
#define __FlipMain__

/**
@file
Subclass of Main, which is generated by wxFormBuilder.
*/

#include "Flip.h"

//// end generated include

#include "cabjiFunctions.h"
#include "FlipProgramLog.h"
#include "include/StartupArgumentsParser.h"
#include <memory>
#include <wx/hashmap.h>
#include <wx/timer.h>
#include <wx/msw/winundef.h>

// forward declarations to avoid circular dependancies
class FlipTemplateEditor; // FlipMain and FlipTemplateEditor share common data that their members access so we forward declare the class
class FlipDataViewer;

// declare custom types/events
WX_DECLARE_STRING_HASH_MAP(wxString, TemplateMap);
wxDECLARE_EVENT(EVT_TEMPLATE_LIST_UPDATED, wxCommandEvent);
wxDECLARE_EVENT(EVT_FLIPMAIN_LAUNCH_CLICKED, wxCommandEvent);
// type to hold regex and substitution string pairs
typedef wxVector<std::pair<wxString, wxString>> RegexSubstitutionList;

/** Implementing Main */
class FlipMain : public Main
{
public:
	/** Constructor */
	FlipMain(wxWindow *parent);
	//// end generated class members
	// cutom constructor declared below here
	FlipMain(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style);
	~FlipMain();
	StartupArgumentsParser GetArgumentsParser();
	wxString GetPDFPageText(const int pageNum);
	wxString GetProcessedPDFPageText(const int pageNum);
	int GetPDFPageTotal();
	int GetProcessedPDFPageTotal();
	int GetRegexCurrentIndex();
	int GetRegexTotal();
	bool GetSwitchValue(const wxString &switchName);
	void LogMessage(wxString message);
	static bool NormalizeFilePathString(wxString &path);
	TemplateMap ReadUserTemplates();
	void SetUseConsoleOutput(const bool v);
	void UpdateTemplateChoices();

	// widget related methods
	void LogAllChildWidgets();
	void LogChildWidgetsRecursive(wxWindow *parent);

	// public members
	static const wxString RESOURCE_MENU_ICONS_PATH;
	const wxString FLIP_USER_HOME_PATH;
	const wxString FLIP_DEFAULT_CONFIG_PATH;
	const wxString FLIP_DEFAULT_TEMPLATE_PATH;
	const wxString FLIP_DEFAULT_OUTPUT_FILENAME;
	TemplateMap m_tmap_userTemplates;

protected:
	std::unique_ptr<FlipProgramLog> m_programLog;
	std::unique_ptr<FlipTemplateEditor> m_templateEditor;
	std::unique_ptr<FlipDataViewer> m_dataViewer;

private:
	// event handlers
	void OnAbout(wxCommandEvent &event);
	void OnBtnLaunch(wxCommandEvent &event);
	void OnFlipDataViewerBtnSave(wxCommandEvent &event);
	void OnFlipDataViewerBtnContinueProcessing(wxCommandEvent &event);
	void OnFlipDataViewerBtnFinishProcessing(wxCommandEvent &event);
	void OnUseTemplateChoice(wxCommandEvent &event);
	void OnQuit(wxCommandEvent &event);
	void OnShowDataViewer(wxCommandEvent &event);
	void OnShowProgramLog(wxCommandEvent &event);
	void OnShowTemplateEditor(wxCommandEvent &event);
	void OnSwitchDBPChecked(wxCommandEvent &event);
	void OnTemplateFilePoll(wxTimerEvent &event);

	// private methods
	void LoadRegexSubstitutionPairs(const wxString &templateFilePath, RegexSubstitutionList &regexList);
	void SetupMenuIcons(wxMenu *menu);

	// private members
	StartupArgumentsParser m_startupArguments;
	bool m_useConsoleOutput = false;
	int m_currentRegex = 0;
	std::vector<std::string> m_vec_pdfData;
	std::vector<std::string> m_vec_pdfDataProcessed;
	wxArrayString m_configTemplateDirs;
	wxString m_tempOutput;
	wxTimer m_filePollTimer;
	RegexSubstitutionList m_regexList;
};

#endif // __FlipMain__
