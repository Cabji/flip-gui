#include "FlipMain.h"
#include "FlipTemplateEditor.h"

FlipTemplateEditor::FlipTemplateEditor(wxWindow *parent)
	: TemplateEditor(parent), m_ptr_UseTemplates(((FlipMain *)parent)->m_useTemplate)
{
	// dev-note: m_ptr_UseTemplates points to-> FlipMain::m_useTemplate which is
	//			 inherited from-> Flip base class. m_useTemplates is defined as a
	//			 public member of Flip class in wxFormBuilder so it is accessible
	//			 beyond direct child classes!

	// bind event handlers
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
}

void FlipTemplateEditor::OnClose(wxCloseEvent &event)
{
	std::cout << "Hiding Template Editor window" << std::endl;
	Hide(); // Hide the frame instead of closing it
}