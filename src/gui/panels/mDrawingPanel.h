
#define OneColorMode true
#define MultiColorMode false 
class mDrawingPanel : public wxPanel
{
private:
    wxColour getColour(size_t index)
    {
        if(index == 0)
        {
            return wxColour(0,0,0);
        }
        float hue = GoldenValue * index; // Use principle of the golden ratio
        auto rgb = wxImage::HSVtoRGB(wxImage::HSVValue(hue, 0.5,0.95)); 
        return wxColour(rgb.red, rgb.green, rgb.blue);         
    } 

    void render(wxDC& dc)
    {
        size_t amount_of_axis_numbers = 10;  

        lfloat minValue = 0;
        lfloat maxValue = 100000;

        const auto margin = FromDIP(2);          
        const auto winSize = this->GetSize();

        dc.Clear();

        dc.SetPen(wxPen( ColorForDrawingAXIS, ThickForDrawingAxis ));
        
        wxSize maxAxisTextLength = dc.GetTextExtent(wxString(a_draw_util::getTextFromNumberForAxis(amount_of_axis_numbers - 1,amount_of_axis_numbers, minValue, maxValue, 2).first));
        // Boxes sizes
        int boxSize = std::min(winSize.GetHeight(), winSize.GetWidth()); 
        int drawBoxSize = boxSize - margin * 3 - std::max(maxAxisTextLength.GetHeight(), maxAxisTextLength.GetWidth()) - SizeOfArrowAxis;
        int drawBoxSizeForAxis =  drawBoxSize + SizeOfArrowAxis; // Some bigger then drawBox
        // Axis coordinates        
        int startXAxis = std::max((winSize.GetWidth() - boxSize) / 2 + margin, margin);
        int startYAxis = winSize.GetHeight() - margin;
        // Drawing zone coordinates 
        int startXDraw = startXAxis + margin + maxAxisTextLength.GetWidth();
        int startYDraw = startYAxis - margin - maxAxisTextLength.GetHeight();

        // Draw axis
        // Vertical
        dc.DrawLine(startXDraw, startYDraw, startXDraw, startYDraw - drawBoxSizeForAxis);
        dc.DrawLine(startXDraw, startYDraw - drawBoxSizeForAxis, startXDraw - SizeOfArrowAxis, startYDraw - drawBoxSizeForAxis + SizeOfArrowAxis);
        dc.DrawLine(startXDraw, startYDraw - drawBoxSizeForAxis, startXDraw + SizeOfArrowAxis, startYDraw - drawBoxSizeForAxis + SizeOfArrowAxis);
        // Horizontal
        dc.DrawLine(startXDraw, startYDraw, startXDraw + drawBoxSizeForAxis, startYDraw);
        dc.DrawLine(startXDraw + drawBoxSizeForAxis, startYDraw, startXDraw + drawBoxSizeForAxis - SizeOfArrowAxis, startYDraw - SizeOfArrowAxis);
        dc.DrawLine(startXDraw + drawBoxSizeForAxis, startYDraw, startXDraw + drawBoxSizeForAxis - SizeOfArrowAxis, startYDraw + SizeOfArrowAxis);
        // Draw Axis text
        // Vertical
        for(int i = 0;i < amount_of_axis_numbers;i++)
        {
            dc.SetPen(wxPen( ColorForDrawingAXIS, SizeOfFontAxis ));
            auto value = a_draw_util::getTextFromNumberForAxis(i,amount_of_axis_numbers, minValue, maxValue); // pair<string, float> string --- a string number format --- float is value in the string. 
            int y = startYDraw - (drawBoxSize * (value.second / (maxValue - minValue)));
            dc.DrawText(wxString(value.first), startXDraw - margin - dc.GetTextExtent(wxString(value.first)).GetWidth(), y);
            dc.SetPen(wxPen( ColorForDrawingAXISMarker, ThickForDrawingAxis ));
            dc.DrawLine(startXDraw - 1, y, startXDraw + 1, y);
        }
        // Horizontal
        for(int i = 1;i < amount_of_axis_numbers;i++)
        {
            dc.SetPen(wxPen( ColorForDrawingAXIS, SizeOfFontAxis ));
            auto value = a_draw_util::getTextFromNumberForAxis(i,amount_of_axis_numbers, minValue, maxValue, 2); // pair<string, float> string --- a string number format --- float is value in the string. 
            int x = startXDraw + (drawBoxSize * (value.second / (maxValue - minValue)));
            dc.DrawText(wxString(value.first), x , startYDraw + margin);
            dc.SetPen(wxPen( ColorForDrawingAXISMarker, ThickForDrawingAxis ));
            dc.DrawLine(x, startYDraw + 1, x, startYDraw - 1);
        }

    }

    void OnPaint(wxPaintEvent &evt) 
    {
        wxPaintDC dc(this);
        render(dc);        
    } 
public:
    mDrawingPanel(wxWindow* parent,  wxWindowID id) :  wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    {
        this->SetBackgroundStyle(wxBG_STYLE_PAINT);
        this->Bind(wxEVT_PAINT, &mDrawingPanel::OnPaint, this);
    }

   
    void Paint()
    { //  
        wxClientDC dc(this);
        render(dc);
    }

    
};