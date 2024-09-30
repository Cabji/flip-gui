#include "FlipDataViewer.h"

FlipDataViewer::FlipDataViewer(FlipMain *parent)
	: DataViewer(parent)
{
	m_mainFrame = parent;
	// set unordered map to make relation between spinButton widgets and textCtrl widgets (before & after)
	m_uomap_SpinToTextCtrl[m_spinBefore] = m_dataBefore;
	m_uomap_SpinToTextCtrl[m_spinAfter] = m_dataAfter;
	// event binds
	m_spinBefore->Bind(wxEVT_SPIN_UP, &FlipDataViewer::OnSpinUp, this);
	Bind(wxEVT_CLOSE_WINDOW, &FlipDataViewer::OnClose, this);
}

void FlipDataViewer::OnClose(wxEvent &event)
{
	Hide();
}

void FlipDataViewer::OnSpinUp(wxEvent &event)
{
	wxSpinButton *cause = dynamic_cast<wxSpinButton *>(event.GetEventObject());
	m_mainFrame->LogMessage("Spin Up Event occurred. Value: " + wxString::Format("%i", cause->GetValue()));
	m_uomap_SpinToTextCtrl[cause]->SetValue("This is a test");
}