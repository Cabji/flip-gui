#include "FlipMain.h"
#include "FlipTemplateEditor.h"
#include <wx/regex.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

FlipTemplateEditor::FlipTemplateEditor(FlipMain *parent)
	: TemplateEditor(parent),
	  m_teCurrentTemplate(wxEmptyString),
	  m_teAutoSaveTimer(this)
{
	// dev-note: m_existingTemplates points to -> FlipMain::m_useTemplate which is
	//			 inherited from -> Flip base class. m_useTemplates is defined as a
	//			 public member of Flip class in wxFormBuilder so it is accessible
	//			 beyond direct child classes!
	//			 m_existingTemplates hides the wxChoice member in the base class and is
	//			 a pointer in FlipTemplateEditor.

	// set pointers to parent objects
	m_mainFrame = parent;
	m_wxChoice_Templates = parent->m_useTemplate;

	// bind event handlers
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
	Bind(wxEVT_TIMER, &FlipTemplateEditor::OnTemplateEditorAutoSaveTimeout, this);
	m_btnAddTemplate->Bind(wxEVT_BUTTON, &FlipTemplateEditor::OnBtnAddTemplate, this);
	m_templatesExisting->Bind(wxEVT_CHOICE, &FlipTemplateEditor::OnTemplateChoiceChanged, this);
	m_templateEditor->Bind(wxEVT_TEXT, FlipTemplateEditor::OnTemplateEditorTextChanged, this);
	parent->Bind(EVT_TEMPLATE_LIST_UPDATED, &FlipTemplateEditor::OnTemplateListUpdated, this);

	// set default values
	m_filePickerAddNew->SetInitialDirectory(m_mainFrame->FLIP_DEFAULT_CONFIG_PATH);
}

void FlipTemplateEditor::OnBtnAddTemplate(wxCommandEvent &event)
{
	// get filename info from m_filePickerAddNew
	wxString target = m_filePickerAddNew->GetTextCtrlValue();
	if (!target)
	{
		// we didn't get anything from the file picker control
		std::cout << "TE::OnBtnAdd: target value was empty" << std::endl;
		m_mainFrame->LogMessage("TE::OnBtnAdd: target value was empty");
		return;
	}

	// get filename & path seperately
	wxFileName absoluteFilename(target);
	wxString path = absoluteFilename.GetPath();
	wxString filename = absoluteFilename.GetFullName();

	// check path existence & create if required
	if (!wxDirExists(path) && path != "")
	{
		if (!wxFileName::Mkdir(path, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
		{
			std::cout << "TE::OntnAdd: Could not create directory for new template file: " << target << std::endl;
			return;
		}
		std::cout << "TE::OnBtnAdd: Created directory: " << path << " for new template creation." << std::endl;
		m_mainFrame->LogMessage("TE::OnBtnAdd: Created directory: " + path + " for new template creation.");
	}

	// create new template file
	wxFileStream wxfs_Target(target);
	if (!wxfs_Target.IsOk())
	{
		std::cout << "TE::OnBtnAdd: creation of new template file failed" << std::endl;
		m_mainFrame->LogMessage("TE::OnBtnAdd: creation of new template file failed");
		return;
	}

	std::cout << "Template Editor wants to make a new file at: " << target << std::endl;
}

void FlipTemplateEditor::OnClose(wxCloseEvent &event)
{
	std::cout << "Hiding Template Editor window" << std::endl;
	Hide(); // Hide the frame instead of closing it
}

void FlipTemplateEditor::OnTemplateChoiceChanged(wxCommandEvent &event)
{
	std::cout << "Template Choice in Template Editor changed" << std::endl;
	// get the selectionIndex of the wxChoice widget
	int selectionIndex = m_templatesExisting->GetSelection();
	if (selectionIndex == wxNOT_FOUND)
	{
		// No valid selection, so exit
		return;
	}

	// // Get the selected template name
	wxString selectedTemplateFilename = m_templatesExisting->GetString(selectionIndex);

	// Look up the corresponding file path in the m_tmap_userTemplates map
	// dev-note: m_teCurrentTemplate is the absolute path and filename to the currently selected template file in Template Editor
	m_teCurrentTemplate = m_mainFrame->m_tmap_userTemplates[selectedTemplateFilename];
	std::cout << "Selected template path: " << m_teCurrentTemplate << '\n'
			  << "Selected template name: " << selectedTemplateFilename << std::endl;

	// // Check if template file exists
	if (!wxFile::Exists(m_teCurrentTemplate))
	{
		m_templateEditor->SetValue(wxString::Format("Error: Template file not found: %s", m_teCurrentTemplate));
		return;
	}

	// Load the template file contents
	wxFileStream fileStream(m_teCurrentTemplate);
	if (!fileStream.IsOk())
	{
		m_templateEditor->SetValue(wxString::Format("Error: Failed to open template file: %s", m_teCurrentTemplate));
		return;
	}

	wxTextInputStream textStream(fileStream);
	wxString templateContents;

	// Read the entire file contents
	while (!fileStream.Eof())
	{
		templateContents += textStream.ReadLine() + "\n";
	}

	std::cout << "Well it seems like the template file was read into wxString templateContents" << std::endl;
	std::cout << "templateContents:\n\n"
			  << templateContents << std::endl;
	// Set the loaded content into m_templateEditor
	m_templateEditor->SetValue(templateContents);
}

void FlipTemplateEditor::OnTemplateEditorAutoSaveTimeout(wxTimerEvent &event)
{
	if (!m_teCurrentTemplate.IsEmpty())
	{
		wxString statusMessage = wxEmptyString;
		wxString content = m_templateEditor->GetValue();
		wxStringTokenizer tokenizer(content, "\n");			// Split content by lines
		wxString regexPattern = "^\\s*(.+)\\s*=>\\s*(.+)$"; // Pattern for "regex => replacement"
		wxString commentPattern = "^\\s*//.*$|^\\s*#.*$";	// Pattern for comments
		wxString blankPattern = "^\\s*$";					// Pattern for blank lines
		wxRegEx regexValidator(regexPattern);
		wxRegEx commentValidator(commentPattern);
		wxRegEx blankValidator(blankPattern);
		wxString line;
		bool isValid = true; // Assume the content is valid until proven otherwise

		while (tokenizer.HasMoreTokens())
		{
			line = tokenizer.GetNextToken().Trim(true).Trim(false); // Trim whitespace from both sides

			if (blankValidator.Matches(line) || commentValidator.Matches(line))
			{
				// Ignore blank lines or comments
				continue;
			}

			if (!regexValidator.Matches(line))
			{
				// If the line doesn't match the "regex => replacement" format
				wxString errorMessage = wxString::Format("Invalid line: '%s'", line);
				std::cout << errorMessage << std::endl;
				m_mainFrame->LogMessage(errorMessage);

				// Set the status bar text to indicate invalid regex
				statusMessage << "Template regexes invalid - please check your regexes";

				isValid = false; // Mark as invalid
				break;			 // Stop further checks
			}

			// If we reach here, the line is valid.
			wxString leftSide = regexValidator.GetMatch(line, 1);
			wxString rightSide = regexValidator.GetMatch(line, 2);

			// Further processing with leftSide and rightSide can be done here
			std::cout << "Valid regex: " << leftSide << " => " << rightSide << std::endl;
		}

		if (isValid)
		{
			// Set the status bar text to indicate valid regex
			statusMessage << "Template regexes valid";

			// Save the content to the file (after validation)
			wxFileOutputStream fileStream(m_teCurrentTemplate);
			if (!fileStream.IsOk())
			{
				std::cout << "Failed to open template file " << m_teCurrentTemplate << " for writing." << std::endl;
				m_mainFrame->LogMessage("Failed to open template file " + m_teCurrentTemplate + " for writing.");
				return;
			}
			wxTextOutputStream textStream(fileStream);
			textStream.WriteString(content);
			std::cout << "Wrote template editor content to file on disk." << std::endl;
			m_mainFrame->LogMessage("Wrote template editor content to file on disk.");
			statusMessage << " | Template content saved to disk";
		}
		m_templateEditorStatusBar->SetStatusText(statusMessage);
	}
	m_teAutoSaveTimer.Stop(); // Stop the timer after saving
}

void FlipTemplateEditor::OnTemplateEditorTextChanged(wxCommandEvent &event)
{
	m_templateEditorStatusBar->SetStatusText("Template content modified");
	// if the auto save timer is running, reset it
	if (m_teAutoSaveTimer.IsRunning())
	{
		m_teAutoSaveTimer.Start(-1);
	}
	else
	{
		m_teAutoSaveTimer.Start(m_teAutoSaveDelay, true);
	}
	// dev-note: when m_teAutoSaveTimer expires (after m_teAutoSaveDelay milliseconds of the user not typing)
	// the timer's event handler OnTemplateEditorAutoSaveTimeout will save the T.E. content to disk.
}

void FlipTemplateEditor::OnTemplateListUpdated(wxCommandEvent &event)
{
	std::cout << "Hello the OnTemplateListUpdated event was triggered!" << std::endl;
	// Update m_existingTemplates to reflect the current state of m_useTemplate in FlipMain
	m_templatesExisting->Clear();
	for (size_t i = 0; i < m_wxChoice_Templates->GetCount(); ++i)
	{
		m_templatesExisting->Append(m_wxChoice_Templates->GetString(i));
	}
}
