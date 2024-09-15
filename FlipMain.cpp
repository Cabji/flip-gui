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
    // custom constructor

    //  create a FlipProgramLog <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_programLog = std::make_unique<FlipProgramLog>(this);
    m_programLog->LogMessage("Program started.");
    this->SetSizerAndFit(this->m_mainFrameSizer);
    this->SetupMenuIcons(this->m_menuFile);

    // event handler binds - menus
    Bind(wxEVT_MENU, &FlipMain::OnAbout, this, ID_MENU_FILE_ABOUT);
    m_btnLaunch->Bind(wxEVT_ENTER_WINDOW, &FlipMain::OnButtonHover, this);
    m_btnLaunch->Bind(wxEVT_LEAVE_WINDOW, &FlipMain::OnButtonLeave, this);
    Bind(wxEVT_MENU, &FlipMain::OnQuit, this, ID_MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &FlipMain::OnShowProgramLog, this, ID_MENU_LOG_PROGRAMLOG);
    Bind(wxEVT_MENU, &FlipMain::OnShowTemplateEditor, this, ID_MENU_FILE_TEMPLATEEDITOR);
    Bind(wxEVT_CHOICE, &FlipMain::OnChoice, this);

    m_switchDBP->Bind(wxEVT_CHECKBOX, &FlipMain::OnSwitchDBPChecked, this);

    // example of static call to LogMessage:
    // FlipProgramLog::LogMessage("This is a test.", *m_programLog);

    wxArrayString configTemplatePaths;
    configTemplatePaths.Add(wxGetHomeDir() + "/.flip/templates");
    configTemplatePaths.Add("./templates");

    FlipProgramLog::LogMessage("Looking for existing template files in user's home/.flip/templates and ./templates", *m_programLog);
    m_tmap_userTemplates = ReadUserTemplates(configTemplatePaths);
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

    // create a FlipTemplateEditor <wxFrame> object which is a child of this (FlipMain <wxFrame>)
    m_templateEditor = std::make_unique<FlipTemplateEditor>(this);

    // Trigger the event to notify FlipTemplateEditor about the change
    wxCommandEvent event(EVT_TEMPLATE_LIST_UPDATED);
    wxPostEvent(this, event);
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

void FlipMain::OnButtonHover(wxMouseEvent &event)
{
    event.Skip();
}

void FlipMain::OnButtonLeave(wxMouseEvent &event)
{
    event.Skip();
}