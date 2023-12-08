
class MainPanel: public wxPanel 
{
private:    
     mDrawingPanel* drawingPanel;
     mRightToolbarPanel* toolbarRightPanel;
     mLeftToolbarPanel* toolbarLeftPanel;
     mToolbarPanel* toolbarPanel;
     wxPanel* consolePanel;
public:        
    MainPanel(wxWindow* parent, wxWindowID id) : wxPanel(parent,id)
    {

        const auto margin = FromDIP(1);

        auto sizer = new wxGridBagSizer(margin, margin);    
        //---------------------- 

        // --- General panels --- 
        // --- Drawing
        drawingPanel = new mDrawingPanel(this, wxID_ANY);
        drawingPanel->SetBackgroundColour(BackgroundPanelDefaultColor);

        // ---  Top Toolbar
        toolbarPanel = new mToolbarPanel(this, wxID_ANY);
        toolbarPanel->SetBackgroundColour(BackgroundPanelDefaultColor);
               
        // --- Left Toolbar
        toolbarLeftPanel = new mLeftToolbarPanel(this, wxID_ANY);
        toolbarLeftPanel->SetBackgroundColour(BackgroundPanelDefaultColor);

        // --- Right Toolbar
        toolbarRightPanel = new mRightToolbarPanel(this, wxID_ANY);
        toolbarRightPanel->SetBackgroundColour(BackgroundPanelDefaultColor);
        
        // --- Bottom console 
        consolePanel = new wxPanel(this, wxID_ANY);
        consolePanel->SetMinSize(ConsolePanelSize);
        consolePanel->SetBackgroundColour(BackgroundPanelDefaultColor);

        auto toolbarSize = ToolbarPanelSize;
        toolbarPanel->SetMinSize(toolbarSize);
        toolbarSize.SetWidth(wxGetDisplaySize().GetWidth());
        toolbarPanel->SetMaxSize(toolbarSize);
        sizer->Add(toolbarPanel,{0,0}, {1,3}, wxEXPAND);
        
        drawingPanel->SetMinSize(DrawingPanelSize);
        toolbarLeftPanel->SetMinSize(ToolbarLeftPanelSize);
        toolbarRightPanel->SetMinSize(ToolbarRightPanelSize);
        


        // --- Add to sizer
        sizer->Add(drawingPanel,{1,1}, {1,1}, wxEXPAND);    
        sizer->Add(toolbarLeftPanel,{1,0}, {2,1}, wxEXPAND | wxALIGN_LEFT);
        sizer->Add(toolbarRightPanel,{1,2}, {2,1}, wxEXPAND | wxALIGN_RIGHT);
        sizer->Add(consolePanel,{2,1}, {1,1}, wxEXPAND);

        sizer->AddGrowableCol(1);
        sizer->AddGrowableRow(1);
        //

        this->SetSizer(sizer);

    } 
    template<typename Class,typename EventHandler>
    void SetEventFunction(void(Class::*func)(wxCommandEvent & evt), EventHandler* handler)
    {
        toolbarRightPanel->SetEventFunction(func, handler);
        toolbarPanel->SetEventFunction(func, handler);
    }
    void UpdateDrawingPanel(std::vector<claster>* clasters, bool colorMode = OneColorMode)
    {
        drawingPanel->SetClasters(clasters);
        drawingPanel->SetColorMode(colorMode);
        drawingPanel->Paint();
    }
    void UpdateProgressBar(lfloat progress)
    {
        toolbarLeftPanel->SetProgress(progress);
    }


    std::optional<std::map<std::string, lfloat>> GetClasterizationOptions()
    {
        std::map<std::string, lfloat> mp;
        auto a_coef = toolbarRightPanel->GetAttractionCoeficient();
        auto t_coef = toolbarRightPanel->GetTrendCoeficient();
        auto size = toolbarRightPanel->GetSizeOfDataset();
        if(!a_coef || !t_coef || !size)
        {
            std::cerr << "Error: Attraction or Trend Coefficients are incorrect.";
            return std::nullopt;
        }
        mp["attraction_coef"] = a_coef.value();
        mp["trend_coef"] = t_coef.value();
        mp["size"] = t_coef.value(); 
        return mp;
    }
};