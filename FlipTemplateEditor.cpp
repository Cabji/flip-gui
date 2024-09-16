#include "FlipMain.h"
#include "FlipTemplateEditor.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

FlipTemplateEditor::FlipTemplateEditor(FlipMain *parent)
	: TemplateEditor(parent)
{
	// dev-note: m_existingTemplates points to -> FlipMain::m_useTemplate which is
	//			 inherited from -> Flip base class. m_useTemplates is defined as a
	//			 public member of Flip class in wxFormBuilder so it is accessible
	//			 beyond direct child classes!
	//			 m_existingTemplates hides the wxChoice member in the base class and is
	//			 a pointer in FlipTemplateEditor.

	m_wxChoice_Templates = parent->m_useTemplate;
	// bind event handlers
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
	Bind(wxEVT_BUTTON, &FlipTemplateEditor::OnBtnAddTemplate, this);
	m_templatesExisting->Bind(wxEVT_CHOICE, &FlipTemplateEditor::OnTemplateChoiceChanged, this);
	parent->Bind(EVT_TEMPLATE_LIST_UPDATED, &FlipTemplateEditor::OnTemplateListUpdated, this);

	// f0rce an OnTemplateChange event to update the m_existingTemplates object
	wxCommandEvent event;
	// OnTemplateChanged(event);
}

void FlipTemplateEditor::OnBtnAddTemplate(wxCommandEvent &event)
{
	// get filename info from m_filePickerAddNew
	wxString target = m_filePickerAddNew->GetTextCtrlValue();
	if (!target)
	{
		// we didn't get anything from the file picker control
	}
	std::cout << "Well it seems like the file picker wants to make a file at: " << target << std::endl;
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
	wxString selectedTemplateName = m_templatesExisting->GetString(selectionIndex);

	// Look up the corresponding file path in the m_tmap_userTemplates map
	wxString selectedTemplatePath = ((FlipMain *)m_parent)->m_tmap_userTemplates[selectedTemplateName];
	std::cout << "Select template path: " << selectedTemplatePath << std::endl;

	// // Check if template file exists
	if (!wxFile::Exists(selectedTemplatePath))
	{
		m_templateEditor->SetValue(wxString::Format("Error: Template file not found: %s", selectedTemplatePath));
		return;
	}

	// Load the template file contents
	wxFileInputStream fileStream(selectedTemplatePath);
	if (!fileStream.IsOk())
	{
		m_templateEditor->SetValue(wxString::Format("Error: Failed to open template file: %s", selectedTemplatePath));
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
