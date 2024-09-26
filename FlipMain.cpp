#include "FlipMain.h"
#include "FlipProgramLog.h"
#include "FlipTemplateEditor.h"
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <wx/bitmap.h>
#include <wx/bmpbndl.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/regex.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/utils.h>
#include <wx/vector.h>
#include <wx/wfstream.h>

// define custom data types & events
// type to hold regex and substitution string pairs
typedef wxVector<std::pair<wxString, wxString>> RegexSubstitutionList;
wxDEFINE_EVENT(EVT_TEMPLATE_LIST_UPDATED, wxCommandEvent);

// dev-note: if you add resource const values, ensure they prepend with leading / char
const wxString FlipMain::RESOURCE_MENU_ICONS_PATH = wxT("/resources/images/menuIcons");

FlipMain::FlipMain(wxWindow *parent)
    : Main(parent)
{
}

FlipMain::FlipMain(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
    : Main(parent),
      FLIP_USER_HOME_PATH(wxGetHomeDir()),
      FLIP_DEFAULT_CONFIG_PATH(wxGetHomeDir() + "/.flip"),
      FLIP_DEFAULT_TEMPLATE_PATH(wxGetHomeDir() + "/.flip/templates"),
      FLIP_DEFAULT_OUTPUT_FILENAME("flipOutput.txt")
{
    // dev-note: const class members must be declared in the constructor's initialiation list
    //           not in the constructor body. (allegedly)

    //  create a FlipProgramLog <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_programLog = std::make_unique<FlipProgramLog>(this);
    m_programLog->LogMessage("Program started.");
    // create a FlipTemplateEditor <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_templateEditor = std::make_unique<FlipTemplateEditor>(this);

    this->SetSizerAndFit(this->m_mainFrameSizer);
    this->SetupMenuIcons(this->m_menuFile);

    // event handler binds
    Bind(wxEVT_MENU, &FlipMain::OnAbout, this, ID_MENU_FILE_ABOUT);
    Bind(wxEVT_MENU, &FlipMain::OnQuit, this, ID_MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &FlipMain::OnShowProgramLog, this, ID_MENU_LOG_PROGRAMLOG);
    Bind(wxEVT_MENU, &FlipMain::OnShowTemplateEditor, this, ID_MENU_FILE_TEMPLATEEDITOR);
    Bind(wxEVT_TIMER, &FlipMain::OnTemplateFilePoll, this);
    // spcific widget binds
    m_btnLaunch->Bind(wxEVT_BUTTON, &FlipMain::OnBtnLaunch, this);
    m_useTemplate->Bind(wxEVT_CHOICE, &FlipMain::OnUseTemplateChoice, this);
    m_switchDBP->Bind(wxEVT_CHECKBOX, &FlipMain::OnSwitchDBPChecked, this);

    // example of static call to LogMessage:
    // FlipProgramLog::LogMessage("This is a test.", *m_programLog);

    m_configTemplateDirs.Add(FLIP_DEFAULT_CONFIG_PATH);
    m_configTemplateDirs.Add("./templates");

    // load available templates into wxChoice widgets
    LogMessage("Looking for existing template files in user's home/.flip/templates and ./templates");
    m_tmap_userTemplates = ReadUserTemplates();
    // dev-note: UpdateTemplateChoices *should* invoke an update in FlipTemplateEditor::wxChoice widget as well
    UpdateTemplateChoices();

    // start poll timer to monitor templates folders
    m_filePollTimer.SetOwner(this); // Attach timer to FlipMain
    m_filePollTimer.Start(5000);    // Poll every 5 seconds

    // do some wxWidget positioning stuff
    this->Fit();
}

FlipMain::~FlipMain()
{
    // destructor
    m_filePollTimer.Stop();
    Unbind(wxEVT_TIMER, &FlipMain::OnTemplateFilePoll, this);
}

void FlipMain::LogAllChildWidgets()
{
    // Get the main frame's top-level window
    wxWindowList &children = GetChildren();

    // Iterate over all top-level children
    for (wxWindowList::iterator it = children.begin(); it != children.end(); ++it)
    {
        wxWindow *child = *it;
        if (child)
        {
            // Log the class name of each child window
            wxString childClass = child->GetClassInfo()->GetClassName();
            m_programLog->LogMessage("Child widget class name: " + childClass);

            // Additionally, you can recursively log children of this window if needed
            LogChildWidgetsRecursive(child);
        }
    }
}

void FlipMain::LogChildWidgetsRecursive(wxWindow *parent)
{
    // Iterate over all children of the given parent window
    wxWindowList &children = parent->GetChildren();

    for (wxWindowList::iterator it = children.begin(); it != children.end(); ++it)
    {
        wxWindow *child = *it;
        if (child)
        {
            // Log the class name of each child window
            wxString childClass = child->GetClassInfo()->GetClassName();
            wxString parentClass = parent->GetClassInfo()->GetClassName();
            m_programLog->LogMessage("Child widget of " + parentClass + ": " + childClass);

            // Recursively log children of this window
            LogChildWidgetsRecursive(child);
        }
    }
}

void FlipMain::LogMessage(wxString message)
{
    // public function to pass messages to the Program Log
    if (m_programLog)
    {
        m_programLog->LogMessage(message);
    }
    if (m_useConsoleOutput)
    {
        std::cout << message << std::endl;
    }
}

bool FlipMain::NormalizeFilePathString(wxString &path)
{
    bool success = false;
    if (!path.IsEmpty())
    {
        wxFileName wxfnPath(path);
        wxfnPath.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE);
        path = wxfnPath.GetFullPath();
        success = true;
    }
    return success;
}

void FlipMain::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Developed by cabji - 2024", "About Flip", wxOK | wxICON_INFORMATION, nullptr);
}

void FlipMain::OnBtnLaunch(wxCommandEvent &event)
{
    // Initialize local data & verify user input values
    bool success = true;
    std::set<int> processPages;
    wxString checkThese;
    wxString templateFileAbsolutePath;

    // 1. Validate m_inputFile (wxFilePickerCtrl)
    wxString inputFilePath = m_inputFile->GetPath();
    if (inputFilePath.IsEmpty())
    {
        checkThese << "Input file is empty\n";
        success = false;
    }
    else if (!wxFile::Exists(inputFilePath))
    {
        checkThese << "Input file does not exist\n";
        success = false;
    }

    // 2. Validate m_useTemplate (wxChoice)
    int templateSelectionIndex = m_useTemplate->GetSelection();
    if (templateSelectionIndex == wxNOT_FOUND)
    {
        checkThese << "No template selected\n";
        success = false;
    }
    else
    {
        // Get the template file path using the template selection
        wxString selectedTemplateFilename = m_useTemplate->GetString(templateSelectionIndex);
        templateFileAbsolutePath = m_tmap_userTemplates[selectedTemplateFilename];

        if (!wxFile::Exists(templateFileAbsolutePath))
        {
            checkThese << "Template file does not exist/is inaccessible\n";
            success = false;
        }
    }

    // 3. Validate m_outputFile (wxFilePickerCtrl) or use a default output path
    wxString outputFilePath = m_outputFile->GetPath();
    if (outputFilePath.IsEmpty())
    {
        // If no output file provided, use the default output path in the user's home directory
        wxString defaultOutputFilePath = FLIP_USER_HOME_PATH + "/" + FLIP_DEFAULT_OUTPUT_FILENAME;
        outputFilePath = defaultOutputFilePath;
        NormalizeFilePathString(defaultOutputFilePath);
        checkThese << "No output filename given, using default: " + defaultOutputFilePath + "\n";
    }
    else
    {
        // Check if the file can be created by attempting to open it for writing
        wxFileOutputStream outputStream(outputFilePath);
        if (!outputStream.IsOk())
        {
            checkThese << "Output file is unwriteable/inaccessible\n";
            success = false;
        }
    }

    // 4. process switches
    // 4a. Per page processing input validation
    std::string pageRangeString = m_ProcessPages->GetValue().ToStdString();
    LogMessage("[-p] User Defined Page Processing");
    try
    {
        processPages = fnParsePageSelection(pageRangeString);
        wxString tempOutput;
        tempOutput << "Only process pages: ";
        for (const int page : processPages)
        {
            tempOutput << page << " ";
        }
        LogMessage(tempOutput);
    }
    catch (const std::exception &e)
    {
        LogMessage("Runtime problem detected! Page process values are causing a problem so quitting.\n" + "Check your -p/--page argument values are correct (or did you forget to put an input filename?) and try again.\n" + "Caught Error: " + e.what());
    }

    // If any validation failed, return early
    if (!success)
    {
        wxMessageBox("Required values missing. Please check the following and try again: \n\n" + checkThese, "Required information missing", wxOK | wxICON_WARNING);
        return;
    }

    // At this point, all validations passed, and we can proceed with core processing
    wxMessageBox("Validation successful. Starting core processing...", "Success", wxOK | wxICON_INFORMATION);

    // *************** Start Core Program Functionality ***************
    RegexSubstitutionList regexList;
    // get regexes from template
    LoadRegexSubstitutionPairs(templateFileAbsolutePath, regexList);
    // open input PDF file
    poppler::document *inPDF = poppler::document::load_from_file(inputFilePath.ToStdString());
    if (!inPDF)
    {
        std::cout << "Error: could not open input file '" << inputFilePath << "'" << std::endl;
        return;
    }

    // get text data from PDF file
    std::vector<std::string> vec_PDFPages;
    auto numPages = inPDF->pages();

    std::cout << "Reading text data from PDF file..." << std::endl;
    // iterate the pages and extract text data into vec_PDFPageStrings
    for (auto i = 0; i < numPages; ++i)
    {
        // per-page processing: to be added
        // if (processPages.find(i) != processPages.end())
        // {
        // cout << "Processing page " << i << endl;
        // Process this page

        poppler::page *inPDFPage = inPDF->create_page(i);
        if (!inPDFPage)
        {
            std::cout << "Could not create poppler::page object, index: " << i << std::endl;
            continue;
        }

        // Extract text and ensure it's not null
        auto textData = inPDFPage->text().to_utf8();
        std::string pageText(textData.data(), textData.size());

        fnStrNormalizeNewLineChars(pageText);
        // strip whitespace: to be added
        // if (stripWhitespace)
        // {
        //     pageText = fnStrStripExcessiveWhitespace(pageText);
        // }

        // Show Data Before Proessing -dbp: to be added
        // if (bShowDataBeforeProcessing)
        // {
        //     cout << "Data Before Processing - Page " << i << endl;
        //     cout << pageText << endl;
        // }

        vec_PDFPages.push_back(pageText);
        delete inPDFPage;

        // }
        // else
        // {
        //     cout << "Skipping page " << i << endl;
        //     // Do not process this page
        // }
    }
    int pagesProcessed = vec_PDFPages.size();

    // temporary output message:
    LogMessage("Processed " + wxString::Format(wxT("%i"), pagesProcessed) + " pages from the input file.");
}

void FlipMain::OnQuit(wxCommandEvent &event)
{
    Close(true);
}

void FlipMain::OnShowProgramLog(wxCommandEvent &event)
{
    if (m_programLog)
    {
        std::cout << "Showing Program Log window" << std::endl;
        m_programLog->Show(true);
    }
}

void FlipMain::OnShowTemplateEditor(wxCommandEvent &event)
{
    if (m_templateEditor)
    {
        std::cout << "Showing Template Editor window" << std::endl;
        m_templateEditor->Show(true);
    }
}

void FlipMain::OnSwitchDBPChecked(wxCommandEvent &event)
{
    // Get the parent of the checkbox
    wxWindow *parent = m_switchDBP->GetParent();

    // Get the class name of the parent
    wxString parentClassName = parent->GetClassInfo()->GetClassName();

    // Log the class name to the Program Log
    FlipProgramLog::LogMessage("Parent class name: " + parentClassName, *m_programLog);

    // Continue with other logic if needed...
}

void FlipMain::OnTemplateFilePoll(wxTimerEvent &event)
{
    // Get the current state of the files on disk using the refactored ReadUserTemplates
    TemplateMap currentFileState = ReadUserTemplates();

    // Compare currentFileState with m_tmap_userTemplates to detect changes
    bool hasChanges = false;

    // Check for deleted files (present in m_tmap_userTemplates but not in currentFileState)
    for (const auto &pair : m_tmap_userTemplates)
    {
        // Normalize paths for comparison
        wxFileName existingFile(pair.second);
        wxString existingRelativePath = existingFile.GetFullPath();

        if (currentFileState.find(pair.first) == currentFileState.end())
        {
            std::cout << "File deleted: " << pair.second << std::endl;
            hasChanges = true;
        }
    }

    // Check for added files (present in currentFileState but not in m_tmap_userTemplates)
    for (const auto &pair : currentFileState)
    {
        // Normalize paths for comparison
        wxFileName newFile(pair.second);
        wxString newRelativePath = newFile.GetFullPath();

        if (m_tmap_userTemplates.find(pair.first) == m_tmap_userTemplates.end())
        {
            std::cout << "File added: " << pair.second << std::endl;
            hasChanges = true;
        }
    }

    // If there were changes, update m_tmap_userTemplates and the wxChoice widgets
    if (hasChanges)
    {
        std::cout << "File state of templates location has changed, updating template choices" << std::endl;
        m_tmap_userTemplates = currentFileState;
        UpdateTemplateChoices();
    }
}

void FlipMain::OnUseTemplateChoice(wxCommandEvent &event)
{
    // scope: we are in an instance of FlipMain
    // auto className = typeid(*this).name(); // get class name of object that triggered the event
    // auto pointerInfo = wxString::Format("Pointer: %p", this); // get the object's pointer address

    // get a pointer to the object that triggered the OnChoice event
    wxChoice *choice = static_cast<wxChoice *>(event.GetEventObject());
    int selection = choice->GetSelection();

    if (selection != wxNOT_FOUND)
    {
        choice->SetToolTip(m_tmap_userTemplates[choice->GetStringSelection()]);
        LogMessage("User selected template: " + choice->GetStringSelection() + " (Full path: " + m_tmap_userTemplates[choice->GetStringSelection()] + ")");
    }

    event.Skip(); // Call this to allow other event handlers to process this event
}

TemplateMap FlipMain::ReadUserTemplates()
{
    TemplateMap returnVals;

    for (const wxString &path : m_configTemplateDirs)
    {
        wxArrayString files;
        wxDir::GetAllFiles(path, &files, "*.*", wxDIR_FILES | wxDIR_DIRS);

        for (const wxString &file : files)
        {
            wxFileName fullPath(file);

            // Get the relative path from the base directory (m_configTemplateDirs item) to the file
            wxString relativePath = fullPath.GetFullPath().Mid(path.length() + 1); // +1 to remove the extra slash

            returnVals[relativePath] = fullPath.GetFullPath();
        }
    }

    return returnVals;
}

void FlipMain::SetUseConsoleOutput(const bool v)
{
    m_useConsoleOutput = v;
}

void FlipMain::SetupMenuIcons(wxMenu *menu)
{

    // Define the folder containing your menu icons, set to the relative path from the executable location
    wxString iconFolderPath = wxGetCwd() + RESOURCE_MENU_ICONS_PATH;
    // change all becksleshes to sleshes
    iconFolderPath.Replace("\\", "/");

    // Ensure the directory exists and is accessible
    wxDir iconDir(iconFolderPath);
    if (!iconDir.IsOpened())
    {
        std::cout << "Can't access icon images directory: " << iconFolderPath << std::endl;
        m_programLog->LogMessage(wxString::Format("Failed to open directory: %s", iconFolderPath));
        return;
    }

    // Attempt to get the menu title; abandon process if unavailable
    wxString parentMenuName = menu->GetTitle().Lower(); // Try to get the menu title
    if (parentMenuName.IsEmpty())
    {
        m_programLog->LogMessage("Warning: Parent menu name could not be retrieved. Abandoning process.");
        return;
    }

    // Iterate over all menu items in the given menu
    for (size_t i = 0; i < menu->GetMenuItemCount(); ++i)
    {
        wxMenuItem *menuItem = menu->FindItemByPosition(i);
        if (!menuItem)
            continue;

        // Extract the label and format it to match the expected icon filename
        wxString label = parentMenuName + "_" + menuItem->GetItemLabelText().Lower(); // Lowercase for consistency
        label.Replace(" ", "_");                                                      // Replace spaces with underscores to match file naming convention

        // Construct the full path to the expected icon file
        wxString iconPath = iconFolderPath + "/" + label + ".png";

        // Check if the icon file exists
        if (!wxFileExists(iconPath))
        {
            m_programLog->LogMessage(wxString::Format("Icon file not found for menu item: %s; iconPath: %s", label, iconPath));
            continue;
        }

        // Load the icon as a wxBitmapBundle and apply it to the menu item
        wxBitmapBundle iconBundle = wxBitmapBundle::FromBitmap(wxBitmap(iconPath, wxBITMAP_TYPE_PNG));
        if (iconBundle.IsOk())
        {
            menuItem->SetBitmap(iconBundle); // Assign the icon to the menu item
            m_programLog->LogMessage(wxString::Format("Applied icon: %s to menu item: %s", iconPath, label));
        }
        else
        {
            m_programLog->LogMessage(wxString::Format("Failed to load bitmap: %s", iconPath));
        }
    }
}

void FlipMain::UpdateTemplateChoices()
{
    std::cout << "Updating wxChoice widgets with new template data.\n\tFlipMain::m_useTemplate wxChoice widget updated" << std::endl;
    // Clear the FlipMain::wxChoice widget
    m_useTemplate->Clear();

    // Populate the FlipMain::wxChoice widget with updated template data
    for (const auto &pair : m_tmap_userTemplates)
    {
        m_useTemplate->Append(pair.first); // Add the relative file name
    }

    // update the Template Editor wxChoice widget by triggering FlipTemplateEditor::OnTemplateListUpdated with a wxCommandEvent
    wxCommandEvent event;
    m_templateEditor->OnTemplateListUpdated(event);
}

void FlipMain::LoadRegexSubstitutionPairs(const wxString &templateFilePath, RegexSubstitutionList &regexList)
{
    wxTextFile file(templateFilePath);
    if (!file.Open())
    {
        std::cout << "Unable to open template file: " << templateFilePath << std::endl;
        LogMessage("Core: Unable to open template file: " + templateFilePath);
        return;
    }

    wxString regexPattern = "^\\s*(.+)\\s*=>\\s*(.+)$"; // "regex => substitution" pattern
    wxRegEx regexValidator(regexPattern);

    for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
    {
        line = line.Trim(true).Trim(false); // Trim whitespace

        if (!fnIgnoreLine(line.ToStdString()))
        {
            if (regexValidator.Matches(line))
            {
                wxString regexString = regexValidator.GetMatch(line, 1);
                wxString substitutionString = regexValidator.GetMatch(line, 2);

                // Add the pair to the list
                regexList.push_back(std::make_pair(regexString, substitutionString));
                std::cout << "Valid regex in template file: " << line << std::endl;
                LogMessage("Valid regex in template file: " + line);
            }
            else
            {
                std::cout << "Invalid regex in template file: " << line << std::endl;
                LogMessage("Invalid regex in template file: " + line);
            }
        }
    }

    file.Close();
}