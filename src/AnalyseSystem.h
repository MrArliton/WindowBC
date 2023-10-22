
// Takes events from window parts and work with data, make events for visualizations.
wxDECLARE_EVENT(AnalyseSystemEvent, wxCommandEvent);
wxDEFINE_EVENT(AnalyseSystemEvent, wxCommandEvent);

enum
{
    aDefaultAnalyseEvtID = 0,
    aStartLoadOfPointsEvtID,
    aStartClasterizationEvtID,
    aStartRevertClasterizationEvtID,
    aEndLoadOfPointsEvtID, 
    aEndClasterizationEvtID,
    aEndRevertClasterizationEvtID,
    aUpdateViewEvtID,
};

namespace a_sys
{
    struct clast_info
    {
        //
        std::string pathToPoints;
        // clasters    
        std::vector<claster> clasters;
    };
}

class CalculateClasterizationThread : public wxThread
{
private:
    a_sys::clast_info& cInfo;
    lfloat attraction_coef;
    lfloat trend_coef;
    wxWindow* parent;
public:
    CalculateClasterizationThread(wxWindow* nParent ,a_sys::clast_info& nCInfo, lfloat nAttractionCoef, lfloat nTrendCoef) :
     parent(nParent), cInfo(nCInfo), attraction_coef(nAttractionCoef), trend_coef(nTrendCoef) {}
    
    void Progress(lfloat);

    virtual void *Entry();
    
    
};

class AnalyseSystem
{ 
    private:
        a_sys::clast_info cInfo;
        wxWindow* parent; // For sending events
        CalculateClasterizationThread* thread = nullptr;

        std::vector<claster> MakeClastersFromPoints(const std::vector<point> points); 
    public: 
        AnalyseSystem(wxWindow* parent) : parent(parent) {};

        AnalyseSystem(const AnalyseSystem &) = delete;
        AnalyseSystem(AnalyseSystem &&) = delete;
        AnalyseSystem& operator=(const AnalyseSystem &) = delete;
        AnalyseSystem& operator=(AnalyseSystem &&) = delete;
        ~AnalyseSystem() = default;

        void LoadPointsFromFileCSV(const std::string& path);
        
        void StartClasterization(lfloat attraction_coef, lfloat trend_coef);
        void RevertClasterization();

        void updateProgress(lfloat progress); // From 0 to 1 -- float
        void endCommand(); // Says system that thread is terminated succesfully 
};


