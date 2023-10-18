
class mToolbarPanel : public wxPanel
{
private:
    wxButton* loadFileButton;
public:
    mToolbarPanel(wxWindow* parent,  wxWindowID id) :  wxPanel(parent, id)
    {
        this->SetFont(this->GetFont().MakeLarger());

        auto margin = FromDIP(5); 

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        loadFileButton = new wxButton(this, aStartLoadOfPointsEvtID, "Load file");

        sizer->Add(loadFileButton, wxALIGN_LEFT, margin);

        this->SetSizer(sizer);
    }
    template< typename Class, typename EventHandler>
    void SetEventFunction(void(Class::*func)(wxCommandEvent & evt), EventHandler* handler)
    {
        loadFileButton->Bind(wxEVT_BUTTON, func, handler); 
    }

};