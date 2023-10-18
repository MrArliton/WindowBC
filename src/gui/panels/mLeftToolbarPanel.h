
class mLeftToolbarPanel : public wxPanel
{
private:
    wxStaticText* progressText;
    wxGauge* progressBar;

public:
    mLeftToolbarPanel(wxWindow* parent,  wxWindowID id) :  wxPanel(parent, id)
    {
        this->SetFont(this->GetFont().MakeLarger());

        auto margin = FromDIP(5); 

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        
        progressText =  new wxStaticText(this, wxID_ANY, " Progress ");

        progressBar = new wxGauge(this, wxID_ANY, 100);

        sizer->Add(progressText, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        sizer->AddSpacer(margin);

        sizer->Add(progressBar, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        this->SetSizer(sizer);
      
    }
   
    void SetProgress(lfloat progress)
    {
        progressBar->SetValue(static_cast<int>(progress*100));
    }



};