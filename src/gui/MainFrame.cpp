
void MainFrame::LoadPoints()
{

    wxFileDialog* openFileDialog = new wxFileDialog(NULL,  _("Open CSV file"), "", "",
        "CSV files (*.csv)|*.csv|FCS files (*.fcs)|*.fcs", wxFD_OPEN|wxFD_FILE_MUST_EXIST);


	if (openFileDialog->ShowModal() == wxID_OK) 
	{
        std::string path = openFileDialog->GetPath().ToStdString();
        if(path.back() == 'v')
        {
                if(auto points = convert_csv_to_points(path))
                {
                    std::cout << (*points).size() << "\n"; 
                    analyse.CreateInStepPoints(*points);
                }
        }
        else // --- Open fcs files - using python script for convert fcs to csv, and read csv.  ---
        {
            path = '\"'+path+'\"'; // --- For correct reading path argument in python script --- 
            std::system("md temp");
            a_util::execPythonScript("./scripts/fcs_loader.py", {path,  '\"'+a_util::getCurrentDirectory()+"/temp/temp.csv\""});
            if(auto points = convert_csv_to_points("./temp/temp.csv"))
            {
                std::cout << (*points).size() << "\n"; 
                analyse.CreateInStepPoints(*points);
            }    
        }
  }
  else
  {
      wxMessageDialog *dial = new wxMessageDialog(NULL, 
      wxT("File isn't loaded"), wxT("Info"), wxOK);
      dial->ShowModal();
  }

	// --- Clean up after ourselves ---
	openFileDialog->Destroy();
}
void MainFrame::OnCommand(wxCommandEvent& evt)
{
  /// --- Log ---
  wxLogGeneric(wxLOG_Message, a_util::string_format("Execute command: %d", evt.GetId()).c_str()); 
  // Avoid manual free data by using AEventHandle 
  ExecuteCommand(a_util::AEventHandle(evt.GetId(), static_cast<std::map<std::string, lfloat>*>(evt.GetClientData()), evt.GetInt()));  
}

void MainFrame::ExecuteCommand(a_util::AEventHandle handle)
{
    switch(handle.getId())
    {
        case aStartCalculateEvent:
            {
                auto options = panel->GetClasterizationOptions();
                if(!options)
                {
                    analyse.CalculateMethod(a_anl::CalculationMethodUPGMC, {});
                }
                analyse.CalculateMethod(a_anl::CalculationMethodUPGMC, *options);
            }
        break;
        case aRevertCalculateEvent:
        break;
        case aLoadPointsInStepEvent:
            LoadPoints();
            panel->UpdateDrawingPanel();   
        case aEndCalculationEvent:
            panel->UpdateDrawingPanel();   
        case aUpdateProgress: // Update progress bar
            {
                auto progress = handle.getValue("progress");
                if(progress)
                {
                    panel->UpdateProgressBar(*progress);      
                }
            }
 
        break;
    }      
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
     : wxFrame(nullptr, wxID_ANY, title, pos, size)
{   
    // --- Sizers and MainPanel ---
    const auto margin = FromDIP(1);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    panel = new MainPanel(this, wxID_ANY);

    panel->SetBackgroundColour(BackgroundMainPanelColor);
    panel->SetEventFunction(&MainFrame::OnCommand, this);

    this->SetBackgroundColour(panel->GetBackgroundColour());

    mainSizer->Add(panel, 1, wxEXPAND | wxALL, margin);
    this->SetSizerAndFit(mainSizer);  
    // --- Events
    analyse.AttachEventHandler(dynamic_cast<wxEvtHandler*>(this));
    Bind(A_ANALYSE_EVT, &MainFrame::OnCommand, this); 
}

MainFrame::~MainFrame()
{
    analyse.DettachEventHanddler(dynamic_cast<wxEvtHandler*>(this));
}