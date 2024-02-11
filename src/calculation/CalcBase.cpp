#include "CalcBase.h"
//--------------------------------------------
void CalculationThread::Progress(lfloat prog)
{
    wxCommandEvent evt(AnalyseSystemEvent, aUpdateViewEvtID);
    evt.SetClientData(new lfloat(prog)); // --- Don't forget free!!!!
    wxPostEvent(parent, evt);
}
