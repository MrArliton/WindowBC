

void AnalyseSystem::closeThread()
{
    if(thread)
    {
        thread->Delete();
        thread = nullptr;
    }
}

void AnalyseSystem::LoadPointsFromFileCSV(const std::string& path)
{
    if(thread)
    {
        thread->Delete();
        thread = nullptr;

        updateProgress(0);
    }

    points = std::move(convert_csv_to_points(path, MaxPointsForLoading).value());
    wxCommandEvent evt(AnalyseSystemEvent, aEndLoadOfPointsEvtID);
    
    a_utils::setDataForHandle(evt, &points, a_utils::A_LIVE_MODE);
    
    parent->ProcessWindowEvent(evt);
}



void AnalyseSystem::StartClasterization(lfloat attraction_coef, lfloat trend_coef)
{
    if(thread)
    {   
        thread->Delete();
        thread = nullptr;

        updateProgress(0);
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
    if(thread)
    {
        thread->Delete();
        thread = nullptr;

        updateProgress(0);
    }
    cInfo.clasters = std::move(convert_csv_to_clasters(cInfo.pathToPoints, MaxPointsForLoading).value());
    wxCommandEvent evt(AnalyseSystemEvent, aEndRevertClasterizationEvtID);
    evt.SetClientData(&cInfo.clasters);
    parent->ProcessWindowEvent(evt);         
}

void AnalyseSystem::endCommand()
{
    thread = nullptr;
}

void AnalyseSystem::updateProgress(lfloat progress)
{
    wxCommandEvent evt(AnalyseSystemEvent, aUpdateViewEvtID);
    evt.SetClientData(new lfloat(progress)); // --- Free lfloat* in CommandFunction
    parent->ProcessWindowEvent(evt);   
}

