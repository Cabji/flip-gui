#include "MyApp.h"
#include "FlipMain.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    FlipMain* frame = new FlipMain(nullptr, wxID_ANY, "Flip", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    frame->Show(true);
    return true;
}