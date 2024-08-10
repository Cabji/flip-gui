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
    // create a FlipProgramLog <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_programLog = std::make_unique<FlipProgramLog>(this);

    TemplateMap m_tmap_userTemplates;
    wxArrayString m_configTemplatePaths;
    m_configTemplatePaths.Add("c:/Users/catfi/.flip/templates");

    m_tmap_userTemplates = ReadUserTemplates(m_configTemplatePaths);
    // write the values we got to the Program Log
    m_programLog->LogMessage("Flip Main Frame constructed.");
    FlipProgramLog::LogMessage("This is a test.", *m_programLog);
 
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
        wxDir::GetAllFiles(path, &files, "*.template", wxDIR_FILES);

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