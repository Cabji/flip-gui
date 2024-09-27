#include "FlipMain.h"
#include "FlipTemplateEditor.h"
#include <wx/msgdlg.h>
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
	m_wxChoicePtr_Templates = parent->m_useTemplate;

	// bind event handlers
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
	Bind(wxEVT_TIMER, &FlipTemplateEditor::OnTemplateEditorAutoSaveTimeout, this);
	m_btnAddTemplate->Bind(wxEVT_BUTTON, &FlipTemplateEditor::OnBtnAddTemplate, this);
	m_btnRemoveTemplate->Bind(wxEVT_BUTTON, &FlipTemplateEditor::OnBtnRemoveTemplate, this);
	m_templatesExisting->Bind(wxEVT_CHOICE, &FlipTemplateEditor::OnTemplateChoiceChanged, this);
	m_templateEditor->Bind(wxEVT_TEXT, FlipTemplateEditor::OnTemplateEditorTextChanged, this);
	parent->Bind(EVT_TEMPLATE_LIST_UPDATED, &FlipTemplateEditor::OnTemplateListUpdated, this);

	// set default values
	m_filePickerAddNew->SetInitialDirectory(m_mainFrame->FLIP_DEFAULT_CONFIG_PATH);
}

void FlipTemplateEditor::OnBtnAddTemplate(wxCommandEvent &event)
{
	// Get filename info from m_filePickerAddNew
	wxString target = m_filePickerAddNew->GetTextCtrlValue();
	if (target.IsEmpty())
	{
		// We didn't get anything from the file picker control
		m_mainFrame->LogMessage("TE::OnBtnAdd: target value was empty");
		return;
	}

	// Get filename & path separately
	wxFileName absoluteFilename(target);
	wxString path = absoluteFilename.GetPath();
	wxString filename = absoluteFilename.GetFullName();

	// Check path existence & create if required
	if (!wxDirExists(path) && !path.IsEmpty())
	{
		if (!wxFileName::Mkdir(path, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
		{
			m_mainFrame->LogMessage("TE::OnBtnAdd: Could not create directory for new template file: " + target);
			return;
		}
		m_mainFrame->LogMessage("TE::OnBtnAdd: Created directory: " + path + " for new template creation.");
	}

	// Create new template file
	wxFileOutputStream wxfs_Target(target);
	if (!wxfs_Target.IsOk())
	{
		m_mainFrame->LogMessage("TE::OnBtnAdd: Creation of new template file failed");
		return;
	}

	m_mainFrame->LogMessage("Template Editor created a new file at: " + target);

	// Add the new template file's details into m_wxChoice_templates and m_mainFrame->m_tmap_userTemplates
	m_mainFrame->m_tmap_userTemplates[filename] = target;

	// Update m_wxChoicePtr_Templates (FlipMain::wxChoice) widgets
	m_wxChoicePtr_Templates->Append(filename); // For main frame wxChoice

	// Select the new template in both wxChoice widgets
	int newIndex = m_templatesExisting->FindString(filename);
	m_templatesExisting->SetSelection(newIndex);
	m_wxChoicePtr_Templates->SetSelection(newIndex);

	// Trigger the event handler to load the new file into the template editor
	wxCommandEvent choiceEvent(wxEVT_CHOICE, m_templatesExisting->GetId());
	choiceEvent.SetInt(newIndex);
	OnTemplateChoiceChanged(choiceEvent);

	m_mainFrame->LogMessage("New template added, and editor updated.");
	m_templateEditorStatusBar->SetStatusText("New template created at " + target);
}

void FlipTemplateEditor::OnBtnRemoveTemplate(wxCommandEvent &event)
{
	// Check if a template is selected in m_templatesExisting
	int selectionIndex = m_templatesExisting->GetSelection();
	if (selectionIndex == wxNOT_FOUND)
	{
		wxMessageBox("Please select a template to remove.", "No Template Selected", wxOK | wxICON_WARNING);
		return;
	}

	// Get the selected template name and path
	wxString selectedTemplateRelativeFilename = m_templatesExisting->GetString(selectionIndex);
	wxString selectedTemplateAbsoluteFilename = m_mainFrame->m_tmap_userTemplates[selectedTemplateRelativeFilename];

	// Ask the user for confirmation before deleting the file
	wxString confirmationMessage = wxString::Format("Are you sure you want to permanently delete the template '%s'?\nThis action cannot be undone.", selectedTemplateAbsoluteFilename);
	int response = wxMessageBox(confirmationMessage, "Confirm Deletion", wxOK | wxCANCEL | wxICON_WARNING);

	if (response != wxOK)
	{
		// User canceled, so exit the function
		return;
	}

	// Attempt to delete the file
	if (wxRemoveFile(selectedTemplateAbsoluteFilename))
	{
		// Remove the deleted template from the map, update the wxChoice widget, clear the template editor textCtrl
		// m_mainFrame->m_tmap_userTemplates.erase(selectedTemplateRelativeFilename);
		m_mainFrame->m_tmap_userTemplates = m_mainFrame->ReadUserTemplates();
		// m_templatesExisting->Delete(selectionIndex);
		m_templatesExisting->SetSelection(wxNOT_FOUND);
		m_templateEditor->Clear();
		m_templateFileWasDeleted = true;
		// update status bar to show successful deletion
		m_templateEditorStatusBar->SetStatusText("Template file " + selectedTemplateAbsoluteFilename + " deleted");
		m_mainFrame->UpdateTemplateChoices();
	}
	else
	{
		wxMessageBox(wxString::Format("Failed to delete template '%s'.", selectedTemplateRelativeFilename), "Error", wxOK | wxICON_ERROR);
	}
}

void FlipTemplateEditor::OnClose(wxCloseEvent &event)
{
	m_mainFrame->LogMessage("Hiding Template Editor window");
	Hide(); // Hide the frame instead of closing it
}

void FlipTemplateEditor::OnTemplateChoiceChanged(wxCommandEvent &event)
{
	m_mainFrame->LogMessage("Template Choice in Template Editor changed");
	// get the selectionIndex of the wxChoice widget
	int selectionIndex = m_templatesExisting->GetSelection();
	if (selectionIndex == wxNOT_FOUND)
	{
		// No valid selection, so exit
		return;
	}

	// // Get the selected template name
	wxString selectedTemplateRelativeFilename = m_templatesExisting->GetString(selectionIndex);

	// Look up the corresponding file path in the m_tmap_userTemplates map
	// dev-note: m_teCurrentTemplate is the absolute path and filename to the currently selected template file in Template Editor
	m_teCurrentTemplate = m_mainFrame->m_tmap_userTemplates[selectedTemplateRelativeFilename];
	m_mainFrame->LogMessage("Selected template path: " + m_teCurrentTemplate + '\n' + "Selected template name: " + selectedTemplateRelativeFilename);

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

	// Remove the last newline character if it exists
	if (!templateContents.IsEmpty() && templateContents.Last() == '\n')
	{
		templateContents.RemoveLast();
	}

	m_mainFrame->LogMessage("Well it seems like the template file was read into wxString templateContents");
	m_mainFrame->LogMessage("templateContents:\n\n" + templateContents);
	// Set the loaded content into m_templateEditor
	m_templateEditor->SetValue(templateContents);
	m_templateFileWasLoaded = true;
	m_templateEditorStatusBar->SetStatusText(selectedTemplateRelativeFilename + " loaded");
}

void FlipTemplateEditor::OnTemplateEditorAutoSaveTimeout(wxTimerEvent &event)
{
	// pre-check. if templateFileWasDeleted or current teplate selection is blank, return out.
	if (m_templateFileWasDeleted || m_templateFileWasLoaded || m_templatesExisting->GetCurrentSelection() == wxNOT_FOUND)
	{
		// Reset flags and return without doing anything
		m_templateFileWasDeleted = false;
		m_templateFileWasLoaded = false;
		return;
	}

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
			m_mainFrame->LogMessage("Valid regex: " + leftSide + "=> " + rightSide);
		}

		if (isValid)
		{
			// Set the status bar text to indicate valid regex
			statusMessage << "Template regexes valid";

			// Save the content to the file (after validation)
			wxFileOutputStream fileStream(m_teCurrentTemplate);
			if (!fileStream.IsOk())
			{
				m_mainFrame->LogMessage("Failed to open template file " + m_teCurrentTemplate + " for writing.");
				return;
			}
			wxTextOutputStream textStream(fileStream);
			textStream.WriteString(content);
			m_mainFrame->LogMessage("Wrote template editor content to file on disk.");
			statusMessage << " | Template content saved to disk";
		}
		m_templateEditorStatusBar->SetStatusText(statusMessage);
	}

	m_teAutoSaveTimer.Stop(); // Stop the timer after saving
}

void FlipTemplateEditor::OnTemplateEditorTextChanged(wxCommandEvent &event)
{
	if (m_templateFileWasDeleted || m_templateFileWasLoaded || m_templatesExisting->GetSelection() == wxNOT_FOUND)
	{
		return;
	}
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
	m_mainFrame->LogMessage("\tFlipTemplateEditor::m_templatesExisting wxChoices widget updated");
	// Update m_existingTemplates to reflect the current state of m_useTemplate in FlipMain
	m_templatesExisting->Clear();
	for (size_t i = 0; i < m_wxChoicePtr_Templates->GetCount(); ++i)
	{
		m_templatesExisting->Append(m_wxChoicePtr_Templates->GetString(i));
	}
}
