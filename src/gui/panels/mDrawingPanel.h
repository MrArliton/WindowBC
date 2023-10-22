
#define OneColorMode true
#define MultiColorMode false 
class mDrawingPanel : public wxPanel
{
private:
    std::vector<claster>* clasters = nullptr;
    std::vector<wxColour> clasters_colours;
    bool colorMode = true;    

    void render(wxDC& dc)
    {
        dc.Clear();

        if(clasters && clasters->size() > 0)
        {

            const auto margin = FromDIP(2);    
            const auto margin_for_axis = FromDIP(10);    
            

            const auto win_size = this->GetSize();
            const auto drawing_area_size = std::min(win_size.GetWidth(), win_size.GetHeight()) - margin_for_axis*2 - margin*3;
            const auto start_x = margin + (win_size.GetWidth() - drawing_area_size)/2l + margin_for_axis;
            const auto start_y = win_size.GetHeight()-(win_size.GetHeight() - drawing_area_size)/2l - margin_for_axis - margin;

            dc.SetPen(wxPen( DefaultColorForDrawingAreaBorder, DefaultWidthForDrawingAreaBorder ));

            // Draw Box
            dc.DrawLine(start_x, start_y, start_x, start_y - drawing_area_size);    
            dc.DrawLine(start_x, start_y, start_x + drawing_area_size, start_y);
            dc.DrawLine(start_x, start_y - drawing_area_size, start_x + drawing_area_size , start_y - drawing_area_size);
            dc.DrawLine(start_x + drawing_area_size ,start_y, start_x + drawing_area_size, start_y - drawing_area_size);
            /// Draw a values on graph
            const auto margin_axis = FromDIP(5);
            const auto max_sized_text = wxString::Format(wxT("%.0f"), static_cast<float>(GraphWidth)); 
            dc.SetFont(wxFont((drawing_area_size/(AmountOfAxisValues))/max_sized_text.Length(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            const auto text_max_width = dc.GetTextExtent(max_sized_text).GetWidth();
            //Horizontal
            for(size_t i = 0;i < AmountOfAxisValues;i++)
            {
                auto text = wxString::Format(wxT("%.0f"), static_cast<float>(i*(GraphWidth/(AmountOfAxisValues-1))));  

                dc.DrawText(text, start_x + i*(drawing_area_size/(AmountOfAxisValues-1)) - margin_for_axis, start_y + margin_for_axis);
                dc.SetPen(wxPen( wxColor(100,0,0), DefaultWidthForDrawingAreaBorder ));
                dc.DrawLine(start_x + i*(drawing_area_size/(AmountOfAxisValues-1)), start_y - FromDIP(1), start_x + i*(drawing_area_size/(AmountOfAxisValues-1)), start_y + FromDIP(1));
                dc.SetPen(wxPen( DefaultColorForDrawingAreaBorder, DefaultWidthForDrawingAreaBorder ));
            }
            dc.SetFont(wxFont(std::min(start_x/max_sized_text.Length(),static_cast<size_t>(dc.GetFont().GetPointSize())), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            //Vertical
            for(size_t i = 1;i < AmountOfAxisValues;i++)
            {
                auto text = wxString::Format(wxT("%.0f"), static_cast<float>(i*(GraphHeight/(AmountOfAxisValues-1))));  
                
                dc.DrawText(text, start_x - dc.GetTextExtent(text).GetWidth() - margin , start_y - i*(drawing_area_size/(AmountOfAxisValues-1)));
                dc.SetPen(wxPen( wxColor(100,0,0), DefaultWidthForDrawingAreaBorder ));
                dc.DrawLine(start_x - FromDIP(1), start_y  - i*(drawing_area_size/(AmountOfAxisValues-1)), start_x + FromDIP(1), start_y  - i*(drawing_area_size/(AmountOfAxisValues-1)));
                dc.SetPen(wxPen( DefaultColorForDrawingAreaBorder, DefaultWidthForDrawingAreaBorder ));
            }   
            //
            //Draw points
            if(colorMode)
            {
                dc.SetPen(wxPen( DefaultColorForDrawingPoint, PointsWidth ));
            }else{
                if(clasters_colours.size() < clasters->size())
                {
                    clasters_colours.clear();
                    clasters_colours.reserve(clasters->size());
                    for(size_t i = 0;i < clasters->size();i++)
                    {
                        clasters_colours.push_back(wxColour(std::rand()%255, std::rand()%255, std::rand()%255));
                    }
                }
            }
            auto cls_id = 0;
            for(auto cls:(*clasters))
            {
                if(!colorMode)
                {
                    dc.SetPen(wxPen( clasters_colours[cls_id], PointsWidth ));
                }
                for(auto pnt:cls.points)
                {
                    const auto s_x = pnt[0] / GraphWidth;
                    const auto s_y = pnt[1] / GraphHeight;
                    if(s_x < 1 && s_y < 1)
                    {
                        dc.DrawCircle(start_x + drawing_area_size * s_x,
                        start_y - drawing_area_size * s_y, PointsWidth);
                    }
                }
                cls_id++;
            }    
            
        }
    }

    void OnPaint( wxPaintEvent &evt) 
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

    void SetColorMode(bool mode)
    {
        colorMode = mode;
    }

    void SetClasters(std::vector<claster>* clasters)
    {
        this->clasters = clasters;
    }

    const std::vector<claster>& GetClasters()
    {
        return *clasters;
    }
    
    void Paint()
    { //  
        wxClientDC dc(this);
        render(dc);
    }

    
};