
bool MainApp::OnInit()
{

    wxLog* logger = new wxLogStream(&std::cout);
    wxLog::SetActiveTarget(logger);
    
    MainFrame* frame = new MainFrame("BloodCells Clasterizator", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;

}

IMPLEMENT_APP(MainApp)

