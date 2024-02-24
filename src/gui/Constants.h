// Events ids 
enum 
{
    aDefaultEvent = 0,
    aStartCalculateEvent,   
    aRevertCalculateEvent,
    aLoadPointsInStepEvent,
    aEndCalculationEvent
};
//
#define AFindColour(str) (wxColourDatabase::Find(str))
#define GoldenValue  0.618033988749895

#define BackgroundMainPanelColor wxColour(50,50,50)
#define BackgroundPanelDefaultColor wxColour(170,170,170)

#define ToolbarPanelSize wxSize(FromDIP(100),FromDIP(30))
#define DrawingPanelSize wxSize(FromDIP(250),FromDIP(250))
#define ToolbarLeftPanelSize wxSize(FromDIP(140),FromDIP(200))
#define ToolbarRightPanelSize wxSize(FromDIP(140),FromDIP(200))
#define ConsolePanelSize wxSize(FromDIP(100),FromDIP(75))


// --- Graph ---
#define ColorGeneratorShiftRed 13
#define ColorGeneratorShiftGreen 21
#define ColorGeneratorShiftBlue 41
#define ColorForDrawingPoint wxColor(40,40,40)
#define ColorForDrawingAXIS wxColor(255,255,255)
#define ColorForDrawingAXISMarker wxColor(255,0,255)
#define ThickForDrawingAxis FromDIP(2)
#define SizeOfFontAxis FromDIP(8)
#define SizeOfArrowAxis FromDIP(3)
#define PointsWidth FromDIP(1)
#define GraphWidth 280000l
#define GraphHeight 280000l
#define AmountOfAxisValues 10 
// --- Process of clasterization ---
#define MaxPointsForLoading 300000
