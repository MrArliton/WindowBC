
void MainFrame::loadPoints()
{

    wxFileDialog* openFileDialog = new wxFileDialog(NULL,  _("Open CSV file"), "", "",
        "CSV files (*.csv)|*.csv|FCS files (*.fcs)|*.fcs", wxFD_OPEN|wxFD_FILE_MUST_EXIST);


	if (openFileDialog->ShowModal() == wxID_OK) 
	{
      std::string path = openFileDialog->GetPath().ToStdString();
      if(path.back() == 'v')
      {
         analyse->LoadPointsFromFileCSV(path);
      }else // --- Open fcs files - using python script for convert fcs to csv, and read csv.  ---
      {
         path = '\"'+path+'\"'; // --- For correct reading path argument in python script --- 
         try{
         a_util::execPythonScript("./scripts/fcs_loader.py", {path,  '\"'+a_util::getCurrentDirectory()+"/temp/temp.csv\""});
         std::system("md temp");
         analyse->LoadPointsFromFileCSV("./temp/temp.csv");
         } 
      }
  }else
  {
      wxMessageDialog *dial = new wxMessageDialog(NULL, 
      wxT("File isn't loaded"), wxT("Info"), wxOK);
      dial->ShowModal();
  }

	// --- Clean up after ourselves ---
	openFileDialog->Destroy();
}
void MainFrame::OnCommand(wxCommandEvent & evt)
{
  /// --- Log ---
  if(evt.GetId() != aUpdateViewEvtID)  
    wxLogGeneric(wxLOG_Message, a_util::string_format("Command: %d", evt.GetId()).c_str()); 
 
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
      analyse->endCommand();
      break;
    case aEndClasterizationEvtID:
      panel->UpdateProgressBar(0);
      panel->UpdateDrawingPanel(static_cast<std::vector<claster>*>(evt.GetClientData()), MultiColorMode);
      analyse->endCommand();
      break;
    case aEndRevertClasterizationEvtID:
      panel->UpdateProgressBar(0);
      panel->UpdateDrawingPanel(static_cast<std::vector<claster>*>(evt.GetClientData()), OneColorMode);
      analyse->endCommand();
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

    // --- Sizers and MainPanel ---
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