#include "FlipMain.h"

FlipMain::FlipMain( wxWindow* parent )
:
Main( parent )
{

}
#include <wx/msgdlg.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include "FlipProgramLog.h"

FlipMain::FlipMain(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : Main( parent )
{
    // custom constructor    
    // event handler binds - menus
    Bind(wxEVT_MENU, &FlipMain::OnAbout, this, ID_MENU_FILE_ABOUT);
    Bind(wxEVT_MENU, &FlipMain::OnQuit, this, ID_MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &FlipMain::OnShowProgramLog, this, ID_MENU_LOG_PROGRAMLOG);
    Bind(wxEVT_CHOICE, &FlipMain::OnChoice, this);

    // create a FlipProgramLog <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_programLog = std::make_unique<FlipProgramLog>(this);
    m_programLog->LogMessage("Program started.");
    // example of static call to LogMessage: 
    // FlipProgramLog::LogMessage("This is a test.", *m_programLog);

    wxArrayString m_configTemplatePaths;
    m_configTemplatePaths.Add(wxGetHomeDir() + "/.flip/templates");
    m_configTemplatePaths.Add("./templates");

    FlipProgramLog::LogMessage("Looking for existing template files in user's home/.flip/templates and ./templates", *m_programLog);
    m_tmap_userTemplates = ReadUserTemplates(m_configTemplatePaths);
    // loop the template values we got
    for (const auto& templatePath : m_tmap_userTemplates)
    {
        const wxString& key = templatePath.first;
        const wxString& value = templatePath.second;
        // write to wxChoice in mainFrame
        this->wxchoice_useTemplate->AppendString(key);
        // write out to programlog
        FlipProgramLog::LogMessage(key + " => " + value, *m_programLog);
    }
    this->SetPosition(this->FromDIP(wxPoint(100,100)));
    this->SetSize(this->FromDIP(wxSize(400,300)));
}

FlipMain::~FlipMain()
{
    // destructor
}

TemplateMap FlipMain::ReadUserTemplates(const wxArrayString &readPaths)
{
    TemplateMap returnVals;
        for (const wxString& path : readPaths) {
        wxArrayString files;
        wxDir::GetAllFiles(path, &files, "*.*", wxDIR_FILES);

        for (const wxString& file : files) {
            wxFileName fullPath(file);
            wxString relativePath = fullPath.GetFullName();
            returnVals[relativePath] = fullPath.GetFullPath();
        }
    }

    return returnVals;
}

void FlipMain::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Developed by cabji - 2024", "About Flip", wxOK | wxICON_INFORMATION, nullptr);
}

void FlipMain::OnChoice(wxCommandEvent& event)
{
    // scope: we are in an instance of FlipMain
    //auto className = typeid(*this).name(); // get class name of object that triggered the event
    //auto pointerInfo = wxString::Format("Pointer: %p", this); // get the object's pointer address
    
    // get a pointer to the object that triggered the OnChoice event
    wxChoice* choice = static_cast<wxChoice*>(event.GetEventObject());
    int selection = choice->GetSelection();

    if (selection != wxNOT_FOUND) {
        choice->SetToolTip(m_tmap_userTemplates[choice->GetStringSelection()]);
        FlipProgramLog::LogMessage("User selected template: " + choice->GetStringSelection() + " (Full path: " + m_tmap_userTemplates[choice->GetStringSelection()] + ")", *m_programLog);
    //     choice->SetToolTip(this->m_tmap);
    }

    event.Skip();  // Call this to allow other event handlers to process this event
}

void FlipMain::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void FlipMain::OnShowProgramLog(wxCommandEvent& event)
{
    if (m_programLog)
    {
        m_programLog->Show(true);
    }
}