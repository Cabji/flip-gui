#include "FlipMain.h"
#include "FlipProgramLog.h"
#include "FlipTemplateEditor.h"
#include <wx/bitmap.h>
#include <wx/bmpbndl.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>

// define custom events
wxDEFINE_EVENT(EVT_TEMPLATE_LIST_UPDATED, wxCommandEvent);

// dev-note: if you add resource const values, ensure they prepend with leading / char
const wxString FlipMain::RESOURCE_MENU_ICONS_PATH = wxT("/resources/images/menuIcons");

FlipMain::FlipMain(wxWindow *parent)
    : Main(parent)
{
}

FlipMain::FlipMain(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) : Main(parent)
{
    // defaults
    const wxString FLIP_DEFAULT_CONFIG_PATH = wxGetHomeDir() + "/.flip";
    const wxString FLIP_DEFAULT_TEMPLATE_PATH = FLIP_DEFAULT_CONFIG_PATH + "/templates";

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
}

void FlipMain::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("Developed by cabji - 2024", "About Flip", wxOK | wxICON_INFORMATION, nullptr);
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
