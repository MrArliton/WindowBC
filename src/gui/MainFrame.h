

class MainFrame : public wxFrame
{
    MainPanel* panel;
    wxBoxSizer* mainSizer;
    a_anl::AnalyseSystem analyse;
    // --- Commands ---
    void loadPoints();    

    // --- Command event ---
    void OnCommand(wxCommandEvent & evt);
    void ExecuteCommand(a_util::AEventHandle handle);

    public:
        MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        ~MainFrame();    
};