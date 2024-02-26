
// Events
wxDECLARE_EVENT(A_ANALYSE_EVT, wxCommandEvent);

enum AnalyseEvtID
{
    AEndCalculationEvtID = 1
};

namespace a_anl
{
    class AnalyseSystem;
    class CalculateThread;

    enum Method
    {
        CalculationMethodUPGMC = 1
    };

    struct condition
    {
        std::vector<point> points; // Points for working
        std::map<std::string, lfloat> info; // Info of condition 
        std::vector<size_t> markers; // Clasters markers
    };

    class CalculateThread : public wxThread
    {
    protected:
        AnalyseSystem& handler;
        std::map<std::string, lfloat> params;
        int(*calcFunction)(CalculateThread* thread,const std::map<std::string, lfloat>& params);
    public:    
        CalculateThread(AnalyseSystem& _handler, int(*func)(CalculateThread* thread,const std::map<std::string, lfloat>& __params),const std::map<std::string, lfloat>& _params)
        : wxThread(wxTHREAD_DETACHED),
         handler(_handler),
          calcFunction(func),
           params(_params) {}

        std::vector<point>& GetPoints();

        void SetPoints(const std::vector<point>& points);
        void SetPoints(std::vector<point>&& points);

        void SetMarkers(const std::vector<size_t>& markers);
        void SetMarkers(std::vector<size_t>&& markers);    

        void Progress(lfloat progress);
        ~CalculateThread();

    protected:
       virtual ExitCode Entry()
       {
            return reinterpret_cast<wxThread::ExitCode>(calcFunction(this, params));
       };
    };

    class AnalyseSystem // Singleton
    {
    friend class CalculateThread;    
    protected:
        wxEvtHandler* eventHandler;
        // Realise synchronize with analyse
        wxCriticalSection calcThreadCS;
        CalculateThread* calcThread;
        //
        std::vector<condition> steps;
        int step = 0; // -1 Not a steps        
   
        AnalyseSystem() { steps.emplace_back(); } 

    public:
        AnalyseSystem(const AnalyseSystem&) = delete;
        AnalyseSystem(AnalyseSystem&&) = delete;
        AnalyseSystem& operator=(const AnalyseSystem&) = delete; 

        static AnalyseSystem& getInstance()
        {
            static AnalyseSystem analyse; // Singleton
            return analyse;
        }

        ~AnalyseSystem() { TerminateCalculation(); }

        void CalculateMethod(Method method,const std::map<std::string, lfloat>& params); // Start a calculationThread with some method
        void TerminateCalculation();

        void NextStep() { step++; steps.emplace_back(); }
        void BackStep() { if(step > 0) { step--; steps.pop_back(); }}   

        void CreateInStepPoints(const std::vector<point>& points);
        void CreateInStepPoints(std::vector<point>&& points); 
        
        int GetStepIndex() { return step; }

        void AttachEventHandler(wxEvtHandler* _eventHandler) // Detach handler if you will destroy it
        { eventHandler =  _eventHandler;}

        bool DettachEventHanddler(wxEvtHandler* _eventHandler) // Detach if input handler == in analyse handler
        {
            if(_eventHandler == eventHandler)
            {
                eventHandler = nullptr;
                return true;
            }
            return false;
        }     

        condition& GetCurrentStepCondition(); // We can make some easy calculation with it

    };



}