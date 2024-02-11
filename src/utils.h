
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
        evt.setInt(mode); // SetMode for EventHandler
        evt.SetClientData(data);  
    }

    class AEventHandle // Avoid free data
    {
    private:
        int id; 
        void* data = nullptr;
        int mode = A_DESTROY_MODE; 
    public:
        EventHandle(const EventHandle&) = delete;
        EventHandle(const EventHandle&&) = delete;
        EventHandle& operator =(const EventHandle&) = delete; 
        EventHandle(int _id, void* _data, int _mode = A_DESTROY_MODE) : id(_id), data(data_), mode(_mode) {}
        ~EventHandle() { (data != nullptr && mode == A_DESTROY_MODE) ? {delete data;} };

        template<typename TYPE>
        TYPE& getValue() { return *(static_cast<TYPE*>(data)); } 
        int getId() { return id; }
    }
    // --- Utils for drawing
    struct DrawingData
    {
        std::vector<point>& points; 
        std::vector<size_t> markers; // For coloring  
    } 

}