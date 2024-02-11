
class MainFrame : public wxFrame
{
    MainPanel* panel;
    wxBoxSizer* mainSizer;

    std::unique_ptr<AnalyseSystem> analyse;
    // --- Commands ---
    void loadPoints();    

    // --- Command event ---
    void OnCommand(wxCommandEvent & evt);
    void ExecuteCommand(AEventHandle handle);

    public:
        MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        ~MainFrame();    
};