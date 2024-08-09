#include "FlipMain.h"

FlipMain::FlipMain( wxWindow* parent )
:
Main( parent )
{

}
#include <wx/msgdlg.h>
#include "FlipProgramLog.h"

FlipMain::FlipMain(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : Main( parent )
{
    // custom constructor
    // event hanler binds - menus
    Bind(wxEVT_MENU, &FlipMain::OnAbout, this, ID_MENU_FILE_ABOUT);
    Bind(wxEVT_MENU, &FlipMain::OnQuit, this, ID_MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &FlipMain::OnShowProgramLog, this, ID_MENU_LOG_PROGRAMLOG);

    // // create a FlipProgramLog object which is a child of this (FlipMain)
    m_programLog = new FlipProgramLog(this);
}

FlipMain::~FlipMain()
{
    // destructor
    delete m_programLog;
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