#include "FlipDataViewer.h"

// define custom events
wxDEFINE_EVENT(EVT_FLIPDATAVIEWER_CONTPROCESSING_CLICKED, wxCommandEvent);

FlipDataViewer::FlipDataViewer(FlipMain *parent)
	: DataViewer(parent)
{
	m_mainFrame = parent;
	// set unordered map to make relation between spinButton widgets and textCtrl widgets (before & after)
	m_spinPages->SetValue(0);
	m_btnContinueProcessing->SetLabel(wxString::FromUTF8("Continue processing ↻"));
	// event binds
	Bind(wxEVT_SPIN, &FlipDataViewer::OnSpin, this);
	Bind(wxEVT_CLOSE_WINDOW, &FlipDataViewer::OnClose, this);
	Bind(EVT_FLIPMAIN_LAUNCH_CLICKED, &FlipDataViewer::OnFlipMainLaunchClicked, this);
	m_btnContinueProcessing->Bind(wxEVT_BUTTON, &FlipDataViewer::OnBtnContinueProcessing, this);
}

void FlipDataViewer::OnBtnContinueProcessing(wxEvent &event)
{
	// trip event to tell FlipMain that the continue processing button was clicked
	wxCommandEvent tripEvent(EVT_FLIPDATAVIEWER_CONTPROCESSING_CLICKED);
	wxPostEvent(m_mainFrame, tripEvent);
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
	// check if m_btnContinueProcessing widget should be shown/hidden
	if (m_mainFrame->GetSwitchValue("dbp"))
	{
		m_btnContinueProcessing->Show();
	}
	else
	{
		m_btnContinueProcessing->Hide();
	}
	// one off call of OnSpin() to make the page data load into the data viewer
	wxCommandEvent tripEvent = wxCommandEvent(wxEVT_NULL);
	OnSpin(tripEvent);
	m_dataviewerSizer->Layout();
	this->Fit();
}

void FlipDataViewer::OnSpin(wxEvent &event)
{
	if (m_spinPages->GetValue() <= m_mainFrame->GetPDFPageTotal())
	{
		// set current page number in m_lblSpinPages
		m_lblSpinPages->SetLabel("Page " + wxString::Format("%i", m_spinPages->GetValue() + 1) + " of " + wxString::Format("%i", m_mainFrame->GetPDFPageTotal()));
		// get before processing page data and display in before textctrl
		m_dataBefore->SetValue(m_mainFrame->GetPDFPageText(m_spinPages->GetValue()));

		// get after processing page data and display in after textctrl (NOTE: this line will need to be updated later!)
		m_dataAfter->SetValue(m_mainFrame->GetProcessedPDFPageText(m_spinPages->GetValue()));

		// log info
	}
	else
	{
		// prevent spin value from exceeding the maximum number of pages
		m_spinPages->SetValue(m_spinPages->GetValue() - 1);
	}
	m_mainFrame->LogMessage("Spin Event occurred. SpinButton Value: " + wxString::Format("%i", m_spinPages->GetValue()));
}