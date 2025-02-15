#include "FlipDataViewer.h"

// define custom events
wxDEFINE_EVENT(EVT_FLIPDATAVIEWER_BTNSAVE_CLICKED, wxCommandEvent);
wxDEFINE_EVENT(EVT_FLIPDATAVIEWER_CONTINUEPROCESSING_CLICKED, wxCommandEvent);
wxDEFINE_EVENT(EVT_FLIPDATAVIEWER_FINISHPROCESSING_CLICKED, wxCommandEvent);

FlipDataViewer::FlipDataViewer(FlipMain *parent)
	: DataViewer(parent)
{
	// initialize members
	m_mainFrame = parent;
	m_spinPages->SetValue(0);
	// by default dataViewer buttons and m_saveFormat are disabled on construction
	m_btnContinueProcessing->SetLabel(wxString::FromUTF8("♻️ Continue processing"));
	m_btnContinueProcessing->Disable();
	m_btnFinishProcessing->SetLabel(wxString::FromUTF8("🏁 Finish processing"));
	m_btnFinishProcessing->Disable();
	m_btnSave->SetLabel(wxString::FromUTF8("💾 Save"));
	m_btnSave->Disable();
	m_saveFormat->Disable();
	// event binds
	Bind(wxEVT_SPIN, &FlipDataViewer::OnSpin, this);
	Bind(wxEVT_CLOSE_WINDOW, &FlipDataViewer::OnClose, this);
	Bind(EVT_FLIPMAIN_LAUNCH_CLICKED, &FlipDataViewer::OnFlipMainLaunchClicked, this);
	m_btnFinishProcessing->Bind(wxEVT_BUTTON, &FlipDataViewer::OnBtnFinishProcessing, this);
	m_btnContinueProcessing->Bind(wxEVT_BUTTON, &FlipDataViewer::OnBtnContinueProcessing, this);
	m_btnSave->Bind(wxEVT_BUTTON, &FlipDataViewer::OnBtnSave, this);
}

bool FlipDataViewer::GetBtnContinueProcessingAbility()
{
	return m_btnContinueProcessing->IsEnabled();
}

bool FlipDataViewer::GetBtnFinishProcessingAbility()
{
	return m_btnFinishProcessing->IsEnabled();
}

bool FlipDataViewer::GetBtnSaveAbility()
{
	// we should always ensure the wxChoice widget for the save format's status matches the btnSave widget
	return m_btnSave->IsEnabled();
}

void FlipDataViewer::ToggleBtnContinueProcessingAbility()
{
	// toggle enable/disable state of Continue processing button
	m_btnContinueProcessing->Enable(!m_btnContinueProcessing->IsEnabled());
}

void FlipDataViewer::ToggleBtnFinishProcessingAbility()
{
	// toggle enable/disable state of Finish processing button
	m_btnFinishProcessing->Enable(!m_btnFinishProcessing->IsEnabled());
}

void FlipDataViewer::ToggleBtnSaveAbility()
{
	// toggle enable/disable state of Save button & wxChocie widgets
	m_saveFormat->Enable(!m_saveFormat->IsEnabled());
	m_btnSave->Enable(!m_btnSave->IsEnabled());
}

void FlipDataViewer::OnBtnContinueProcessing(wxEvent &event)
{
	// trip event to tell FlipMain that the continue processing button was clicked
	wxCommandEvent tripEvent(EVT_FLIPDATAVIEWER_CONTINUEPROCESSING_CLICKED);
	wxPostEvent(m_mainFrame, tripEvent);
}

void FlipDataViewer::OnBtnFinishProcessing(wxEvent &event)
{
	// trip event to tell FlipMain that the continue processing button was clicked
	wxCommandEvent tripEvent(EVT_FLIPDATAVIEWER_FINISHPROCESSING_CLICKED);
	wxPostEvent(m_mainFrame, tripEvent);
}

void FlipDataViewer::OnBtnSave(wxEvent &event)
{
	// trip event for FlipMain to handle the save process
	wxCommandEvent tripEvent(EVT_FLIPDATAVIEWER_BTNSAVE_CLICKED);
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
	// check if m_btnContinueProcessing widget should be shown/hidden
	if (m_mainFrame->GetSwitchValue("dbp"))
	{
		m_btnContinueProcessing->Show();
	}
	else
	{
		m_btnContinueProcessing->Hide();
	}
	// reset data viewer user-viewable data
	m_dataBefore->ChangeValue(wxEmptyString);
	m_dataAfter->ChangeValue(wxEmptyString);
	m_spinPages->SetValue(0);
	// set current page number in m_lblSpinPages
	m_lblSpinPages->SetLabel("Page 0 of " + wxString::Format("%i", m_mainFrame->GetPDFPageTotal()));
	m_btnSave->Disable();

	// one off call of OnSpin() to make the page data load into the data viewer
	wxCommandEvent tripEvent = wxCommandEvent(wxEVT_NULL);
	OnSpin(tripEvent);
	m_dataviewerSizer->Layout();
}

void FlipDataViewer::OnSpin(wxEvent &event)
{
	// check requested page number is valid
	if (m_spinPages->GetValue() <= m_mainFrame->GetPDFPageTotal() - 1)
	{
		int currentPage = m_spinPages->GetValue();
		int currentRegex = m_mainFrame->GetRegexCurrentIndex();
		int totalPages = m_mainFrame->GetPDFPageTotal();
		int totalRegexes = m_mainFrame->GetRegexTotal();
		// set current page number in m_lblSpinPages
		m_lblSpinPages->SetLabel(wxString::Format("Page %i of %i (Regex %i of %i used)", currentPage + 1, totalPages, currentRegex, totalRegexes));
		// get before processing page data and display in before textctrl
		m_dataBefore->SetValue(m_mainFrame->GetPDFPageText(currentPage));

		// ensure that the spin value is within the bounds of the processed PDF page data vector
		if (m_spinPages->GetValue() <= m_mainFrame->GetProcessedPDFPageTotal() - 1)
		{
			// get after processing page data and display in after textctrl
			m_dataAfter->SetValue(m_mainFrame->GetProcessedPDFPageText(currentPage));
			// set widget background colour based on if this page was processed successfully or not
			if (m_mainFrame->GetIsPDFPageRekt(currentPage))
			{
				// page is rekt, set colour to red
				m_dataAfter->SetBackgroundColour(wxColour(220, 180, 180));
			}
			else
			{
				// page is good! set colour to white
				m_dataAfter->SetBackgroundColour(wxColour(255, 255, 255));
			}
		}
		// m_mainFrame->LogMessage("Spin Event occurred. SpinButton Value: " + wxString::Format("%i", currentPage));
	}
	else
	{
		// prevent spin value from exceeding the maximum number of pages
		m_spinPages->SetValue(m_spinPages->GetValue() - 1);
		// m_mainFrame->LogMessage("Spin value maximum reached.");
	}
}