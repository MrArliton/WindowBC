// --- Takes events from window parts and work with data, make events for visualizations. --- 
wxDECLARE_EVENT(CalculationEvent, wxCommandEvent);
wxDEFINE_EVENT(CalculationEvent, wxCommandEvent);

@template<typename INFO>
class CalculationThread : public wxThread
{
private:
    INFO& cInfo;
    wxWindow* parent;
public:
    CalculateClasterizationThread(wxWindow* nParent ,INFO& nCInfo, lfloat nAttractionCoef) :
     parent(nParent), cInfo(nCInfo), attraction_coef(nAttractionCoef) {}
    
    void Progress(lfloat);

    virtual void *Entry() = 0;
};