#include "MyApp.h"
#include "FlipMain.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <iostream>

#ifdef __WXMSW__
#include <windows.h> // Required for AttachConsole
#endif

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    bool useConsoleOutput = false;

    // Iterate through command line arguments to check for "--console"
    for (int i = 1; i < argc; ++i)
    {
        if (wxString(argv[i]).IsSameAs("--console", false))
        {
            useConsoleOutput = true;
            break;
        }
    }

    if (useConsoleOutput)
    {
#ifdef __WXMSW__
        // On Windows, attach a console if not already present
        if (!::AttachConsole(ATTACH_PARENT_PROCESS))
        {
            // Allocate a new console if none is available
            ::AllocConsole();
        }

        // Redirect stdout and stderr to the console
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
#elif defined(__WXGTK__) || defined(__WXOSX__)
        // On Unix-like systems, output is typically already visible in the terminal.
        // No special action needed unless you want to explicitly manage I/O streams.
#endif
        std::cout << "Console mode enabled. Output redirected." << std::endl;
    }

    wxImage::AddHandler(new wxPNGHandler());
    FlipMain *frame = new FlipMain(nullptr, wxID_ANY, "Flip", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    frame->SetUseConsoleOutput(useConsoleOutput);
    frame->Show(true);
    return true;
}