#include "FlipProgramLog.h"

FlipProgramLog::FlipProgramLog( wxWindow* parent )
:
ProgramLog( parent )
{
    Bind(wxEVT_CLOSE_WINDOW, &FlipProgramLog::OnClose, this);
}

wxString FlipProgramLog::timeNow()
{
    return wxDateTime::Now().FormatISOTime();
}

void FlipProgramLog::LogMessage(const wxString &message)
{
    // this is where we send messages to the Program Log wxTextCtrl wdiget
    this->wxtextCtrl_programLog->AppendText("[" + timeNow() + "] " + message + "\n");
}

void FlipProgramLog::LogMessage(const wxString &message, FlipProgramLog &targetWidget) {
    // Cast targetWidget to FlipProgramLog to access the inherited wxTextCtrl
    FlipProgramLog* logFrame = static_cast<FlipProgramLog*>(&targetWidget);

    // Assuming the wxTextCtrl is named `wxtextCtrl_programLog` in the generated ProgramLog class
    if (logFrame->wxtextCtrl_programLog) {
        logFrame->wxtextCtrl_programLog->AppendText("[" + FlipProgramLog::timeNow() + "] " + message + "\n");
    }
}

void FlipProgramLog::OnClose(wxCloseEvent& event) {
    Hide();  // Hide the frame instead of closing it
}
