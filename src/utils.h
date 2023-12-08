
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

}