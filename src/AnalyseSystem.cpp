// Event of Analyse system
wxDEFINE_EVENT(A_ANALYSE_EVT, wxCommandEvent);

namespace a_anl
{
// AnalyseSystem class
namespace // Calculate functions
{
    std::vector<std::tuple<size_t, size_t, lfloat>> data_upgmc; // REmake info
    int CalculateUPGMC(CalculateThread* thread, const std::map<std::string, lfloat>& params)
    {        
        if(data_upgmc.empty())
        {
            data_upgmc = u_generic_linkage<CalculateThread>(thread->GetPoints(), params.at("attraction_coef"), thread);
        }
        auto markers = make_markers_using_markov_stopping(thread->GetPoints(), data_upgmc, params.at("trend_coef"));
        thread->SetMarkers(markers);    
        return 1;
    }

}
// Start calculation thread with some method
void AnalyseSystem::CalculateMethod(Method method,const std::map<std::string, lfloat>& params)
{
    switch(method)
    {
        case(CalculationMethodUPGMC):
            TerminateCalculation();
            if(!GetCurrentStepCondition().points.empty())
            {
                calcThread = new CalculateThread(*this, CalculateUPGMC, params);
                if ( calcThread->Run() != wxTHREAD_NO_ERROR )
                {
                    wxLogError("Can't create the thread!");
                    delete calcThread;
                    calcThread = nullptr;
                }
            }
            else
            {
                wxLogGeneric(wxLOG_Message, "Points isn't load, clasterization cannot start"); 
            }
        break;
    }
}
// Close the calculation thread
void AnalyseSystem::TerminateCalculation()
{
        {
            wxCriticalSectionLocker enter(calcThreadCS);
            if(calcThread)
            {
                calcThread->Delete();        
            }
        }
        while (true)
        {
            { 
                wxCriticalSectionLocker enter(calcThreadCS);
                if (!calcThread) break;
            }
            // wait for thread completion
            wxThread::This()->Sleep(1);
        }
}
// Load points into step
void AnalyseSystem::CreateInStepPoints(const std::vector<point>& points)
{
    TerminateCalculation();
    
    steps.at(step).points = points;       
    
}  

void AnalyseSystem::CreateInStepPoints(std::vector<point>&& points)
{
    {
        wxCriticalSectionLocker enter(calcThreadCS);
        if(calcThread)
        {
            calcThread->Delete();        
        }
        while (true)
        {
            { 
                wxCriticalSectionLocker enter(calcThreadCS);
                if (!calcThread) break;
            }
            // wait for thread completion
            wxThread::This()->Sleep(1);
        }
    }
    std::swap(steps.at(step).points, points);
} 
// Get info about current step
condition& AnalyseSystem::GetCurrentStepCondition()
{ 
    {
        wxCriticalSectionLocker enter(calcThreadCS);
        if(!calcThread) // When calcutions is being, we can't take
        {
            return steps.at(step);
        }
        static condition cond; // Special condition, because required condition not available
        cond.info["unavailable"] = 1.f; 
        return cond;
    }
}

// CalculateThread Class

void CalculateThread::Progress(lfloat progress)
{
    if(handler.eventHandler != nullptr)
    {
        wxCommandEvent event(A_ANALYSE_EVT, aUpdateProgress);
        event.SetClientData(new std::map<std::string, lfloat>({{"progress",progress}}));
        event.SetInt(a_util::A_DESTROY_MODE);
        handler.eventHandler->AddPendingEvent(event);
    }
}

// --- Block of data controll  
// Getters
std::vector<point>& CalculateThread::GetPoints()
{
    wxCriticalSectionLocker enter(handler.calcThreadCS);
    return handler.steps[handler.GetStepIndex()].points;
}

void CalculateThread::SetPoints(const std::vector<point>& points)
{
    wxCriticalSectionLocker enter(handler.calcThreadCS);
    handler.steps[handler.GetStepIndex()].points = points;    
}
// Setters
void CalculateThread::SetPoints(std::vector<point>&& points)
{
    wxCriticalSectionLocker enter(handler.calcThreadCS);
    handler.steps[handler.GetStepIndex()].points = std::move(points);
}

void CalculateThread::SetMarkers(const std::vector<size_t>& markers)
{
    wxCriticalSectionLocker enter(handler.calcThreadCS);
    handler.steps[handler.GetStepIndex()].markers = markers;

}

void CalculateThread::SetMarkers(std::vector<size_t>&& markers)
{
    wxCriticalSectionLocker enter(handler.calcThreadCS);
    handler.steps[handler.GetStepIndex()].markers = std::move(markers);
} 

CalculateThread::~CalculateThread()
{
    wxCriticalSectionLocker enter(handler.calcThreadCS); // Lock
    // Thread being destroyed 
    handler.calcThread = nullptr;
    // Send event
    if(handler.eventHandler != nullptr)
    {
        wxCommandEvent event(A_ANALYSE_EVT, aEndCalculationEvent);
        handler.eventHandler->AddPendingEvent(event);
    }
}  

} // namespace