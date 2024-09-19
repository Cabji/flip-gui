#ifndef __FlipMain__
#define __FlipMain__

/**
@file
Subclass of Main, which is generated by wxFormBuilder.
*/

#include "Flip.h"

//// end generated include

#include "FlipProgramLog.h"
#include <memory>
#include <wx/hashmap.h>
#include <wx/timer.h>
#include <wx/msw/winundef.h>

// forward declarations to avoid circular dependancies
class FlipTemplateEditor; // FlipMain and FlipTemplateEditor share common data that their members access so we forward declare the class

// declare custom hashes/events
WX_DECLARE_STRING_HASH_MAP(wxString, TemplateMap);
wxDECLARE_EVENT(EVT_TEMPLATE_LIST_UPDATED, wxCommandEvent);

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
	void LogMessage(wxString message);
	static TemplateMap ReadUserTemplates(const wxArrayString &readPaths);

	// widget related methods
	void LogAllChildWidgets();
	void LogChildWidgetsRecursive(wxWindow *parent);

	// public members
	static const wxString RESOURCE_MENU_ICONS_PATH;
	const wxString
		FLIP_DEFAULT_CONFIG_PATH,
		FLIP_DEFAULT_TEMPLATE_PATH;
	TemplateMap m_tmap_userTemplates;

protected:
	std::unique_ptr<FlipProgramLog> m_programLog;
	std::unique_ptr<FlipTemplateEditor> m_templateEditor;

private:
	// event handlers
	void OnAbout(wxCommandEvent &event);
	void OnUseTemplateChoice(wxCommandEvent &event);
	void OnQuit(wxCommandEvent &event);
	void OnShowProgramLog(wxCommandEvent &event);
	void OnShowTemplateEditor(wxCommandEvent &event);
	void OnSwitchDBPChecked(wxCommandEvent &event);
	void OnTemplateFilePoll(wxTimerEvent &event);

	// private methods
	void SetupMenuIcons(wxMenu *menu);
	void UpdateTemplateChoices();

	// private members
	wxArrayString m_configTemplateDirs;
	wxTimer m_filePollTimer;
};

#endif // __FlipMain__
