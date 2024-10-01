#include "FlipDataViewer.h"

FlipDataViewer::FlipDataViewer(FlipMain *parent)
	: DataViewer(parent)
{
	m_mainFrame = parent;
	// set unordered map to make relation between spinButton widgets and textCtrl widgets (before & after)
	m_spinPages->SetValue(0);
	// set continue processing button to disabled if -dbp is not checked
	if (!m_mainFrame->GetSwitchValue("dbp"))
	{
		m_btnContinueProcessing->Disable();
	}
	// event binds
	Bind(wxEVT_SPIN_UP, &FlipDataViewer::OnSpinUp, this);
	Bind(wxEVT_CLOSE_WINDOW, &FlipDataViewer::OnClose, this);
	Bind(EVT_FLIPMAIN_LAUNCH_CLICKED, &FlipDataViewer::OnFlipMainLaunchClicked, this);
}

void FlipDataViewer::OnClose(wxEvent &event)
{
	Hide();
}

void FlipDataViewer::OnFlipMainLaunchClicked(wxEvent &event)
{
	// when FlipMain::OnBtnLaunch is clicked and its handler runs, it will trigger an event to be handled in here
	// this is primarily so we can adjust the widgets in FlipDataViewer to suit what FlipMain is doing
	m_mainFrame->LogMessage("we are handling a LAUNCH button click in FlipDataViewer!");
}

void FlipDataViewer::OnSpinUp(wxEvent &event)
{
	// set current page number in m_lblSpinPages
	m_lblSpinPages->SetLabel("Page " + wxString::Format("%i", m_spinPages->GetValue()) + " of " + wxString::Format("%i", m_mainFrame->GetPDFPageTotal()));
	// get before processing page data and display in before textctrl
	m_dataBefore->SetValue(m_mainFrame->GetPDFPageText(m_spinPages->GetValue()));

	// get after processing page data and display in after textctrl (NOTE: this line will need to be updated later!)
	m_dataAfter->SetValue(m_mainFrame->GetPDFPageText(m_spinPages->GetValue()));

	// log info
	m_mainFrame->LogMessage("Spin Up Event occurred. SpinButton Value: " + wxString::Format("%i", m_spinPages->GetValue()));
}