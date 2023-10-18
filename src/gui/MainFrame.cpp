
void MainFrame::loadPoints()
{

    wxFileDialog* openFileDialog = new wxFileDialog(NULL,  _("Open CSV file"), "", "",
        "CSV files (*.csv)|*.csv", wxFD_OPEN|wxFD_FILE_MUST_EXIST);


	if (openFileDialog->ShowModal() == wxID_OK) 
	{
      analyse->LoadPointsFromFileCSV(openFileDialog->GetPath().ToStdString());
  }else
  {
      wxMessageDialog *dial = new wxMessageDialog(NULL, 
      wxT("File isn't loaded"), wxT("Info"), wxOK);
      dial->ShowModal();
  }

	// Clean up after ourselves
	openFileDialog->Destroy();
}/*
void MainFrame::clasterizeFunctor::operator() (wxCommandEvent & evt)
{   
  //  lfloat a_coef = parent->toolbarRightPanel->GetAttractionCoeficient().value();
  //  lfloat t_coef = parent->toolbarRightPanel->GetTrendCoeficient().value();
  //  if(!parent->drawingPanel->GetClasters().empty()){
 //       parent->drawingPanel->SetClasters(u_generic_linkage(parent->drawingPanel->GetClasters(), a_coef, t_coef));
 //       parent->drawingPanel->SetColorMode(MultiColorMode);
 //       parent->drawingPanel->Paint();
  //  }
}
void MainFrame::revertFunctor::operator() (wxCommandEvent & evt)
{
  //  parent->drawingPanel->SetClasters(parent->originalClasters);
  //  parent->drawingPanel->Paint();
}
*/
void MainFrame::OnCommand(wxCommandEvent & evt)
{
   switch(evt.GetId()){
    case aStartClasterizationEvtID:
      {
        auto options = panel->GetClasterizationOptions().value();
        analyse->StartClasterization(options["attraction_coef"], options["trend_coef"]);
      }
      break; 
    case aStartRevertClasterizationEvtID:
      analyse->RevertClasterization();
      break; 
    case aStartLoadOfPointsEvtID:
      loadPoints();
      break;  
    case aEndLoadOfPointsEvtID:
      panel->UpdateDrawingPanel(static_cast<std::vector<claster>*>(evt.GetClientData()), OneColorMode);
      break;
    case aEndClasterizationEvtID:
      panel->UpdateProgressBar(0);
      panel->UpdateDrawingPanel(static_cast<std::vector<claster>*>(evt.GetClientData()), MultiColorMode);
      break;
    case aEndRevertClasterizationEvtID:
      panel->UpdateProgressBar(0);
      panel->UpdateDrawingPanel(static_cast<std::vector<claster>*>(evt.GetClientData()), OneColorMode);
      break;    
    case aUpdateViewEvtID:
      {
        lfloat progress = *static_cast<lfloat*>(evt.GetClientData());
        panel->UpdateProgressBar(progress);
        delete static_cast<lfloat*>(evt.GetClientData()); // Clean
      }
      break;  
    default:
      break;  
   }

}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
     : wxFrame(nullptr, wxID_ANY, title, pos, size)
{   
    analyse = std::unique_ptr<AnalyseSystem>(new AnalyseSystem(this));

    // Sizers and MainPanel
    const auto margin = FromDIP(1);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    panel = new MainPanel(this, wxID_ANY);

    panel->SetBackgroundColour(BackgroundMainPanelColor);
    panel->SetEventFunction(&MainFrame::OnCommand, this);

    this->SetBackgroundColour(panel->GetBackgroundColour());

    mainSizer->Add(panel, 1, wxEXPAND | wxALL, margin);
    this->SetSizerAndFit(mainSizer);  
    //---------------------- 

    this->Bind(AnalyseSystemEvent, &MainFrame::OnCommand, this);
}

MainFrame::~MainFrame()
{
}