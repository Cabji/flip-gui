#include "MyApp.h"
#include "FlipMain.h"
#include "include/StartupArgumentsParser.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <iostream>

#ifdef __WXMSW__
#include <windows.h> // Required for AttachConsole
#endif

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    // data initialization
    wxImage::AddHandler(new wxPNGHandler());
    FlipMain *frame = new FlipMain(nullptr, wxID_ANY, "Flip", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    frame->GetArgumentsParser().Parse(argc, argv);
    // get binary filename only (no absolute path)
    std::string binaryFilename = argv[0].substr(argv[0].find_last_of("/\\") + 1).ToUTF8().data();
    std::map<std::string, std::string> passedArgs = frame->GetArgumentsParser().GetArguments();
    std::string tempOut = "";

    // handle command line switches
    for (const auto &[switchName, value] : passedArgs)
    {
        if (switchName == "console")
        {
            frame->SetUseConsoleOutput(true);
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
            tempOut += "Console mode enabled. Output redirected.\n";
        }
        else if (switchName == "help")
        {
            tempOut += "Flip - process PDF file to extract data\n\nUsage: " + binaryFilename + " [--switch] [value]\n";
        }
    }

    std::cout << tempOut << std::endl;

    frame->Show(true);
    return true;
}