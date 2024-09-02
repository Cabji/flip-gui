#include "MyApp.h"
#include "FlipMain.h"
#include <wx/wx.h>
#include <wx/image.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    FlipMain *frame = new FlipMain(nullptr, wxID_ANY, "Flip", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    frame->Show(true);
    return true;
}