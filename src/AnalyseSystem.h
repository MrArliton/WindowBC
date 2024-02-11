


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




class AnalyseSystem
{ 
    private:
        std::vector<point> points; // == points for analyzes
        wxWindow* parent; // --- For sending events
        CalculationThread* thread = nullptr;
        void openThread();
        void closeThread();
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

        void updateProgress(lfloat progress); // --- From 0 to 1 -- float
        void endCommand(); // --- Says system that thread is terminated succesfully 
};


