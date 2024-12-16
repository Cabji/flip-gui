#include "MyApp.h"
#include "FlipMain.h"
#include "cabjiFunctions.h"
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
    std::vector<std::pair<std::string, std::string>> passedArgs = frame->GetArgumentsParser().GetArguments();
    std::string tempOut = "";

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
            tempOut += "\nConsole mode enabled. Output redirected.\n";
        }
        else if (switchName == "help" || switchName == "h" || switchName == "?")
        {
            tempOut += "Flip - process PDF file to extract data\n\nUsage: " + binaryFilename + " [--switch] [value]\n\n";
            tempOut += "\tSwitch Name\t\tValue\n";
            tempOut += "\t--------------------------------------------------------------------------------\n";
            tempOut += "\tauto\t\t\tN/A (Automatic LAUNCH)\n";
            tempOut += "\tconsole\t\t\tOutput program messages to console\n";
            tempOut += "\tdbp\t\t\tN/A (Show data before regex processing/stepped processing)\n";
            tempOut += "\tinput\t\t\tAbsolute or relative path to input filename\n";
            tempOut += "\toutput\t\t\tAbsolute or relative path to output filename\n";
            tempOut += "\tpage\t\t\tPage range to use (eg: 1-4 or 2,4,7,12)\n";
            tempOut += "\tquiet\t\t\tNo GUI shown\n";
            tempOut += "\tsws\t\t\tN/A (Strip excessive whitspace from input data)\n";
            tempOut += "\ttemplate\t\tAbsolute or relative path to template filename\n";
        }
        else if (switchName == "input" || switchName == "i" || switchName == "if" || switchName == "inputfile")
        {
            frame->SetInputFilename(wxString(value));
            tempOut += "  Input file set to: " + value + "\n";
        }
        else if (switchName == "dbp")
        {
            frame->SetSwitchDBP();
            tempOut += "  Switch set: -dbp (Show data before regex processing)\n";
        }
        else if (switchName == "sws")
        {
            frame->SetSwitchSWS();
            tempOut += "  Switch set: -sws (Strip excessive whitespace)\n";
        }
        else if (switchName == "page" || switchName == "p")
        {
            tempOut += "  Switch set: -" + switchName + " (Process pages) " + value + "\n";
            std::set<int> success = fnParsePageSelection(value, &tempOut);
            if (!success.empty())
            {
                frame->SetSwitchPages(wxString(frame->GetArgumentsParser().GetValue(switchName)));
            }
        }
        else if (switchName == "template" || switchName == "t")
        {
            tempOut += "  Switch set: -" + switchName + " (Template file) " + value + "\n";
            auto templateIndex = frame->m_useTemplate->FindString(value, true);
            if (templateIndex != wxNOT_FOUND)
            {
                frame->m_useTemplate->SetSelection(templateIndex);
                tempOut += "    Template: '" + value + "' was successfully selected in the UI" + "\n";
            }
            else
            {
                tempOut += "    WARNING! Template '" + value + "' was NOT found!" + "\n";
            }
        }
        else if (switchName == "output" || switchName == "o" || switchName == "of" || switchName == "outputfile")
        {
            frame->SetOutputFilename(wxString(value));
            tempOut += "  Output file set to: " + value + "\n";
        }
        else if (switchName == "auto" || switchName == "automated" || switchName == "launch" || switchName == "autolaunch" || switchName == "al")
        {
            tempOut += "  Switch set: -" + switchName + " (Automatic LAUNCH of processing)\n";
            frame->SetSwitchAutoLAUNCH();
            // dev-note: if we set the switch for automated LAUNCH, we also must call frame->DoAutoLAUNCH(); further down to start the automatic processing
        }
        else if (switchName == "quiet" || switchName == "q")
        {
            // dev-note: "quiet" mode will prevent the GUI from showing, NOT prevent output messages to console
            tempOut += "  Switch set: -" + switchName + " (No GUI shown)\n";
            frame->SetSwitchQuiet();
        }
        // switch for save program log perhaps?
        else
        {
            tempOut += "  Unknown switch: " + switchName + "\n";
        }
    }

    std::cout << tempOut << std::endl;
    if (!frame->GetSwitchValue("quiet"))
    {
        frame->Show(true);
    }
    // dev-note: automated LAUNCH will only happen if frame->SetSwitchAutoLAUNCH() is called prior to this.
    frame->DoAutoLAUNCH();

    return true;
}