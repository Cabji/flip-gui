#include "FlipDataViewer.h"

FlipDataViewer::FlipDataViewer(FlipMain *parent)
	: DataViewer(parent)
{
	m_mainFrame = parent;
	// event binds
	m_spinBefore->Bind(wxEVT_SPIN_UP, &FlipDataViewer::OnSpinUp, this);
}

void FlipDataViewer::OnSpinUp(wxEvent &event)
{
	m_mainFrame->LogMessage("Spin Up Event occurred.");
}