#include "FlipMain.h"
#include "FlipTemplateEditor.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

FlipTemplateEditor::FlipTemplateEditor(wxWindow *parent)
	: TemplateEditor(parent),
	  m_parent(dynamic_cast<FlipMain *>(parent)),
	  m_existingTemplates(m_parent ? m_parent->m_useTemplate : nullptr)
{
	// dev-note: m_existingTemplates points to -> FlipMain::m_useTemplate which is
	//			 inherited from -> Flip base class. m_useTemplates is defined as a
	//			 public member of Flip class in wxFormBuilder so it is accessible
	//			 beyond direct child classes!
	//			 m_existingTemplates hides the wxChoice member in the base class and is
	//			 a pointer in FlipTemplateEditor.

	// check that we got the parent object on construction
	// if (!m_parent)
	// {
	// 	// 	// output error msg to console (use --console when running program to see console messages)
	// 	std::cout << "Parent is not FlipMain, casting failed." << std::endl;
	// 	// 	return;
	// }

	// bind event handlers
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
	// m_existingTemplates->Bind(wxEVT_CHOICE, &FlipTemplateEditor::OnTemplateChanged, this);
	// m_parent->Bind(EVT_TEMPLATE_LIST_UPDATED, &FlipTemplateEditor::OnTemplateListUpdated, this);

	// f0rce an OnTemplateChange event to update the m_existingTemplates object
	// wxCommandEvent event;
	// OnTemplateChanged(event);
}

void FlipTemplateEditor::OnTemplateListUpdated(wxCommandEvent &event)
{
	// Update m_existingTemplates to reflect the current state of m_useTemplate in FlipMain
	// m_existingTemplates->Clear();
	// for (size_t i = 0; i < m_parent->m_useTemplate->GetCount(); ++i)
	// {
	// 	m_existingTemplates->Append(m_parent->m_useTemplate->GetString(i));
	// }
}

void FlipTemplateEditor::OnClose(wxCloseEvent &event)
{
	std::cout << "Hiding Template Editor window" << std::endl;
	Hide(); // Hide the frame instead of closing it
}

void FlipTemplateEditor::OnTemplateChanged(wxCommandEvent &event)
{
	// get the selectionIndex of the wxChoice widget
	// int selectionIndex = m_existingTemplates->GetSelection();
	// if (selectionIndex == wxNOT_FOUND)
	// {
	// 	// No valid selection, so exit
	// 	return;
	// }

	// // Get the selected template name
	// wxString selectedTemplateName = m_existingTemplates->GetString(selectionIndex);

	// // Look up the corresponding file path in the m_tmap_userTemplates map
	// wxString selectedTemplatePath = ((FlipMain *)m_parent)->m_tmap_userTemplates[selectedTemplateName];

	// // Check if the file exists
	// if (!wxFile::Exists(selectedTemplatePath))
	// {
	// 	m_templateEditor->SetValue(wxString::Format("Error: Template file not found: %s", selectedTemplatePath));
	// 	return;
	// }

	// // Load the template file contents
	// wxFileInputStream fileStream(selectedTemplatePath);
	// if (!fileStream.IsOk())
	// {
	// 	m_templateEditor->SetValue(wxString::Format("Error: Failed to open template file: %s", selectedTemplatePath));
	// 	return;
	// }

	// wxTextInputStream textStream(fileStream);
	// wxString templateContents;

	// // Read the entire file contents
	// while (!fileStream.Eof())
	// {
	// 	templateContents += textStream.ReadLine() + "\n";
	// }

	// // Set the loaded content into m_templateEditor
	// m_templateEditor->SetValue(templateContents);
}
