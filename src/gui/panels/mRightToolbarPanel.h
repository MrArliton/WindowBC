

class mRightToolbarPanel : public wxPanel {
private:
    wxStaticText* attractionCoefText;
    wxStaticText* trendCoefText;
    wxStaticText* amountOfPointsText;
    wxStaticText* sizeOfDatasetText;
    wxTextCtrl* amountOfPointsValue;

    wxTextCtrl* setSizeOfDataset;
    wxTextCtrl* setAttractionCoef;
    wxTextCtrl* setTrendCoef;

    wxButton* startClasterization;
    wxButton* revertClasterization;

public:
    mRightToolbarPanel(wxWindow* parent,  wxWindowID id) :  wxPanel(parent, id)
    {
        this->SetFont(this->GetFont().MakeLarger());

        auto margin = FromDIP(5); 

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        attractionCoefText = new wxStaticText(this, wxID_ANY, " Attraction coeficient ");

        trendCoefText = new wxStaticText(this, wxID_ANY, " Trend coeficient ");

        amountOfPointsText = new wxStaticText(this, wxID_ANY, " Amount of points ");
        amountOfPointsValue = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxTextValidator(wxFILTER_NUMERIC));
        amountOfPointsValue->SetBackgroundColour(wxColour(200,200,200));
        
        sizeOfDatasetText = new wxStaticText(this, wxID_ANY, " Size of dataset ");

        
        setSizeOfDataset = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
        
        setAttractionCoef = new wxTextCtrl(this, wxID_ANY, "0.0001", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));

        setTrendCoef = new wxTextCtrl(this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));

        startClasterization = new wxButton(this, aStartClasterizationEvtID, " Clasterize ");
        revertClasterization = new wxButton(this, aStartRevertClasterizationEvtID, " Revert ");
 
        sizer->Add(attractionCoefText, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        sizer->Add(setAttractionCoef, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        
        sizer->Add(trendCoefText, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        sizer->Add(setTrendCoef, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        sizer->Add(amountOfPointsText, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        sizer->Add(amountOfPointsValue, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        sizer->AddSpacer(margin);   
        sizer->Add(sizeOfDatasetText, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        sizer->Add(setSizeOfDataset, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        sizer->AddSpacer(margin);   
        sizer->Add(startClasterization, 0, wxALIGN_CENTER_HORIZONTAL, margin);
        sizer->AddSpacer(margin);
        sizer->Add(revertClasterization, 0, wxALIGN_CENTER_HORIZONTAL, margin);

        this->SetSizer(sizer);
    }
    template< typename Class, typename EventHandler>
    void SetEventFunction(void(Class::*func)(wxCommandEvent & evt), EventHandler* handler){
        startClasterization->Bind(wxEVT_BUTTON, func, handler); 
        revertClasterization->Bind(wxEVT_BUTTON, func, handler); 
    }


    std::optional<lfloat> GetAttractionCoeficient() const
    {
        double value;
        if(!setAttractionCoef->GetLineText(0).ToDouble(&value)){
            return std::nullopt;
        }
        return value;
    }
    std::optional<lfloat> GetTrendCoeficient() const
    {
        double value;
        if(!setTrendCoef->GetLineText(0).ToDouble(&value)){
            return std::nullopt;
        }
        return value;
    }
    std::optional<lfloat> GetSizeOfDataset() const
    {
        double value;
        if(!setSizeOfDataset->GetLineText(0).ToDouble(&value)){
            return std::nullopt;
        }
        return value;
    }



};