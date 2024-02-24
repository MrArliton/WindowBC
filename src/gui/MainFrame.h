

class MainFrame : public wxFrame
{
    MainPanel* panel;
    wxBoxSizer* mainSizer;
    a_anl::AnalyseSystem& analyse = a_anl::AnalyseSystem::getInstance();
    // --- Commands ---
    void LoadPoints();    

    // --- Command event ---
    void OnCommand(wxCommandEvent & evt);
    void ExecuteCommand(a_util::AEventHandle handle);

    public:
        MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        ~MainFrame();    
};