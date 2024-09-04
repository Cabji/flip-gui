#include "FlipMain.h"
#include "FlipProgramLog.h"
#include <wx/bitmap.h>
#include <wx/bmpbndl.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

FlipMain::FlipMain(wxWindow *parent)
    : Main(parent)
{
}

FlipMain::FlipMain(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) : Main(parent)
{
    this->SetSizerAndFit(this->m_mainFrameSizer);
    this->SetupMenuIcons();
    //  create a FlipProgramLog <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_programLog = std::make_unique<FlipProgramLog>(this);
    m_programLog->LogMessage("Program started.");
    // create a FlipTemplateEditor <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_templateEditor = std::make_unique<FlipTemplateEditor>(this);
    // custom constructor
    // event handler binds - menus
    Bind(wxEVT_MENU, &FlipMain::OnAbout, this, ID_MENU_FILE_ABOUT);
    m_btnLaunch->Bind(wxEVT_ENTER_WINDOW, &FlipMain::OnButtonHover, this);
    m_btnLaunch->Bind(wxEVT_LEAVE_WINDOW, &FlipMain::OnButtonLeave, this);
    Bind(wxEVT_MENU, &FlipMain::OnQuit, this, ID_MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &FlipMain::OnShowProgramLog, this, ID_MENU_LOG_PROGRAMLOG);
    Bind(wxEVT_CHOICE, &FlipMain::OnChoice, this);

    m_switchDBP->Bind(wxEVT_CHECKBOX, &FlipMain::OnSwitchDBPChecked, this);

    // example of static call to LogMessage:
    // FlipProgramLog::LogMessage("This is a test.", *m_programLog);

    wxArrayString m_configTemplatePaths;
    m_configTemplatePaths.Add(wxGetHomeDir() + "/.flip/templates");
    m_configTemplatePaths.Add("./templates");

    FlipProgramLog::LogMessage("Looking for existing template files in user's home/.flip/templates and ./templates", *m_programLog);
    m_tmap_userTemplates = ReadUserTemplates(m_configTemplatePaths);
    // loop the template values we got
    for (const auto &templatePath : m_tmap_userTemplates)
    {
        const wxString &key = templatePath.first;
        const wxString &value = templatePath.second;
        // write to wxChoice in mainFrame
        this->m_useTemplate->AppendString(key);
        // write out to programlog
        FlipProgramLog::LogMessage(key + " => " + value, *m_programLog);
    }
    this->SetPosition(this->FromDIP(wxPoint(100, 100)));
    this->SetSize(this->FromDIP(wxSize(400, 300)));
}

FlipMain::~FlipMain()
{
    // destructor
}

TemplateMap FlipMain::ReadUserTemplates(const wxArrayString &readPaths)
{
    TemplateMap returnVals;
    for (const wxString &path : readPaths)
    {
        wxArrayString files;
        wxDir::GetAllFiles(path, &files, "*.*", wxDIR_FILES);

        for (const wxString &file : files)
        {
            wxFileName fullPath(file);
            wxString relativePath = fullPath.GetFullName();
            returnVals[relativePath] = fullPath.GetFullPath();
        }
    }

    return returnVals;
}

void FlipMain::SetupMenuIcons()
{
    std::cout << "We are in SetupMenuIcons" << std::endl;
    // Define the folder containing your menu icons, set to the relative path from the executable location
    wxString iconFolderPath = wxStandardPaths::Get().GetExecutablePath();
    iconFolderPath = wxPathOnly(iconFolderPath) + "/resources/images/menuIcons/";

    // Ensure the directory exists and is accessible
    wxDir iconDir(iconFolderPath);
    if (!iconDir.IsOpened())
    {
        m_programLog->LogMessage(wxString::Format("Failed to open directory: %s", iconFolderPath));
        return;
    }

    // Prepare variables to hold file names
    wxString fileName;
    bool hasMoreFiles = iconDir.GetFirst(&fileName, "*.png", wxDIR_FILES);

    // Loop over all PNG files in the directory
    while (hasMoreFiles)
    {
        // Extract the base name of the file (without extension) and convert to integer ID
        wxString itemIDStr = wxFileName(fileName).GetName();
        long itemID;
        if (!itemIDStr.ToLong(&itemID))
        {
            m_programLog->LogMessage(wxString::Format("Invalid ID format in file: %s", fileName));
            hasMoreFiles = iconDir.GetNext(&fileName);
            continue;
        }

        // Load the bitmap from the file
        wxString fullPath = iconFolderPath + fileName;
        wxBitmapBundle iconBundle = wxBitmapBundle::FromBitmap(wxBitmap(fullPath, wxBITMAP_TYPE_PNG));
        if (!iconBundle.IsOk())
        {
            m_programLog->LogMessage(wxString::Format("Failed to load bitmap: %s", fullPath));
            hasMoreFiles = iconDir.GetNext(&fileName);
            continue;
        }

        // Get the corresponding menu item by its ID
        wxMenuItem *menuItem = m_menuFile->FindItem(static_cast<int>(itemID));
        if (menuItem)
        {
            menuItem->SetBitmap(iconBundle); // Assign the icon to the menu item
        }
        else
        {
            m_programLog->LogMessage(wxString::Format("Menu item not found for ID: %d", static_cast<int>(itemID)));
        }

        // Move to the next file
        hasMoreFiles = iconDir.GetNext(&fileName);
    }
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

void FlipMain::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Developed by cabji - 2024", "About Flip", wxOK | wxICON_INFORMATION, nullptr);
}

void FlipMain::OnChoice(wxCommandEvent &event)
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
        FlipProgramLog::LogMessage("User selected template: " + choice->GetStringSelection() + " (Full path: " + m_tmap_userTemplates[choice->GetStringSelection()] + ")", *m_programLog);
        //     choice->SetToolTip(this->m_tmap);
    }

    event.Skip(); // Call this to allow other event handlers to process this event
}

void FlipMain::OnQuit(wxCommandEvent &event)
{
    Close(true);
}

void FlipMain::OnShowProgramLog(wxCommandEvent &event)
{
    if (m_programLog)
    {
        m_programLog->Show(true);
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

void FlipMain::OnButtonHover(wxMouseEvent &event)
{
    event.Skip();
}

void FlipMain::OnButtonLeave(wxMouseEvent &event)
{
    event.Skip();
}