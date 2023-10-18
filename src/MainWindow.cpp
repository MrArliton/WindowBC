
bool MainApp::OnInit()
{
    MainFrame* frame = new MainFrame("BloodCells Clasterizator", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;

}

IMPLEMENT_APP(MainApp)

