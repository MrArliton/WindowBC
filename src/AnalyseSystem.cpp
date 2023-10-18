
std::vector<claster> AnalyseSystem::MakeClastersFromPoints(const std::vector<point> points)
{
    std::vector<claster> clasters;
    clasters.reserve(points.size());
    for(auto pnt:points)
    {   
        clasters.emplace_back(pnt);
    }

    return clasters;
} 


void AnalyseSystem::LoadPointsFromFileCSV(const std::string& path)
{
    cInfo.pathToPoints = path;
    cInfo.clasters = std::move(convert_csv_to_clasters(path, MaxPointsForLoading).value());
    wxCommandEvent evt(AnalyseSystemEvent, aEndLoadOfPointsEvtID);
    evt.SetClientData(&cInfo.clasters);
    parent->ProcessWindowEvent(evt);
}

void AnalyseSystem::StartClasterization(lfloat attraction_coef, lfloat trend_coef)
{
    if(thread && !thread->IsAlive())
    {
        delete thread;
        thread = nullptr;
    }    
    if(!thread)
    {
        thread = new CalculateClasterizationThread(parent, cInfo, attraction_coef, trend_coef);
        if (thread->Create() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can’t create CalculationThread!"));
        }else
        {
            thread->Run();
        }
        
    }
}

void AnalyseSystem::RevertClasterization()
{
    if(thread && thread->IsRunning())
    {
        thread->Delete();
    }
    cInfo.clasters = std::move(convert_csv_to_clasters(cInfo.pathToPoints, MaxPointsForLoading).value());
    wxCommandEvent evt(AnalyseSystemEvent, aEndRevertClasterizationEvtID);
    evt.SetClientData(&cInfo.clasters);
    parent->ProcessWindowEvent(evt);         
}


void CalculateClasterizationThread::Progress(lfloat prog)
{
    wxCommandEvent evt(AnalyseSystemEvent, aUpdateViewEvtID);
    evt.SetClientData(new lfloat(prog)); // Don't forget free!!!!
    wxPostEvent(parent, evt);
}

void *CalculateClasterizationThread::Entry()
{   
    std::vector<claster> clasters = u_generic_linkage(cInfo.clasters, attraction_coef, trend_coef, this);
    if(!TestDestroy())
    {
        cInfo.clasters = std::move(clasters);
    }else{
        wxLogMessage("Clasterization process: terminated");
        return nullptr;
    }
    wxLogMessage(wxString("Clasterization process: complete with attraction coef-") << attraction_coef << ", trend coef-" << trend_coef);
    wxCommandEvent evt(AnalyseSystemEvent, aEndClasterizationEvtID);
    evt.SetClientData(&cInfo.clasters);
    wxPostEvent(parent, evt);
    return nullptr;
}
