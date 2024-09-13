#include "FlipTemplateEditor.h"

FlipTemplateEditor::FlipTemplateEditor(wxWindow *parent)
	: TemplateEditor(parent)
{
	Bind(wxEVT_CLOSE_WINDOW, &FlipTemplateEditor::OnClose, this);
}

void FlipTemplateEditor::OnClose(wxCloseEvent &event)
{
	std::cout << "Hiding Template Editor window" << std::endl;
	Hide(); // Hide the frame instead of closing it
}