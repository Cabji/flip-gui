#include "FlipProgramLog.h"

FlipProgramLog::FlipProgramLog( wxWindow* parent )
:
ProgramLog( parent )
{
    Bind(wxEVT_CLOSE_WINDOW, &FlipProgramLog::OnClose, this);
}


void FlipProgramLog::LogMessage(const wxString& message)
{
    // this is where we send messages to the Program Log wxTextCtrl wdiget
    this->wxtextCtrl_programLog->AppendText(message + "\n");
}

void FlipProgramLog::OnClose(wxCloseEvent& event) {
    Hide();  // Hide the frame instead of closing it
}
