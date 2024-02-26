
namespace a_util{

    std::string getCurrentDirectory()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        
        return std::string(buffer).substr(0, pos);
    }

    template<typename ... Args>
    std::string string_format( const std::string& format, Args ... args )
    {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // --- Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // --- We don't want the '\0' inside
    }

    int execPythonScript(const std::string& path,const std::vector<std::string>& args){ // --- Args: path to python script, args - list of script's arguments
        std::string command = "python " + path;
        for(size_t i = 0;i < args.size();i++)
        {
            command+=" ";
            command+=args[i]; 
        } 
        wxLogGeneric(wxLOG_Message, string_format("Execute system command: %s",command.c_str()).c_str());
        return std::system(command.c_str());
    }
    // --- Utils for event handling
    enum
    {
        A_LIVE_MODE  = 0, 
        A_DESTROY_MODE = 1
    };

    void setDataForHandle(wxCommandEvent& evt, void* data, int mode)
    {
        evt.SetInt(mode); // SetMode for EventHandler
        evt.SetClientData(data);  
    }

    class AEventHandle // Avoid free data
    {
    private:
        int id; 
        std::map<std::string, lfloat>* data = nullptr;
        int mode = A_DESTROY_MODE; 
    public:
        AEventHandle(const AEventHandle&) = delete;
        AEventHandle(const AEventHandle&&) = delete;
        AEventHandle& operator =(const AEventHandle&) = delete; 
        AEventHandle(int _id, std::map<std::string, lfloat>* _data, int _mode = A_DESTROY_MODE) : id(_id), data(_data), mode(_mode) {}
        ~AEventHandle() { if(data != nullptr && mode == A_DESTROY_MODE) delete data; };

        std::optional<lfloat> getValue(const std::string& key)
        {
            if(data != nullptr && (*data).find(key) != (*data).end())
            {
                return (*data)[key];
            }
            return std::nullopt;
        } 
        int getId() { return id; }
    };

}

namespace a_anl_util
{
    lfloat GetMaxValueFromAllAxis(const std::vector<point>& points)
    {
        lfloat mx = std::numeric_limits<lfloat>().min();
        for(auto& point : points)
        {
            for(auto value : point)
            {
                if(value > mx)
                {
                    mx = value;
                }
            }
        }
        return mx;
    }
    lfloat GetMinValueFromAllAxis(const std::vector<point>& points)
    {
        lfloat mn = std::numeric_limits<lfloat>().max();
        for(auto& point : points)
        {
            for(auto value : point)
            {
                if(value < mn)
                {
                    mn = value;
                }
            }
        }
        return mn;
    }

}
 
namespace a_draw_util
{

std::pair<std::string, lfloat> getTextFromNumberForAxis(size_t index, size_t amount, lfloat minValue, lfloat maxValue, size_t lengthNumber = 3)  // Index from 0 to amount - 1
{    
    assert(index < amount && index >= 0);
    size_t length = 0;
    lfloat value = minValue + std::round(index * (maxValue - minValue) / (amount - 1.f));
    lfloat buff = value;        
    while((value /= 10) >= 1) 
    {
        length++; 
    }
    value *= 10;

    if(length < lengthNumber) // If a number correct return it
    {
        return {std::to_string(value).substr(0, lengthNumber + 1) ,value};
    }


    return {std::to_string(value).substr(0, lengthNumber + 1) + "*10^" + std::to_string(length), buff}; // "LengthNumber + 1" == String have dot.  

}

}