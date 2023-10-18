
#include <fstream>

std::optional<std::vector<claster>> convert_csv_to_clasters(std::string path, size_t amount_points = 100000000, char delimiter = ',', size_t arg_1 = 1, size_t arg_2 = 2){
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "CSV file isn't open: " << path << std::endl;
        return std::nullopt;
    }


    std::string header;

    std::getline(file,header);

    std::vector<claster> clasters;
    clasters.reserve(amount_points);    
    // Fill clasters
    size_t i = 0;
    for(std::string line; std::getline(file, line);){
        point pnt;
        
        size_t del = 0;
        //  Find arg_1
            for(int i = 0;i < arg_1;i++){
                del = line.find(delimiter,del);
                del++;
            }
        //
        std::string buff = line.substr(del,line.find(delimiter,del)-del);
        pnt.push_back(std::stold(buff));
        //  Find arg_2
            for(int i = 0;i < arg_2-arg_1;i++){
                del = line.find(delimiter,del);
                del++;
            }
        //
        buff = line.substr(del,line.find(delimiter,del)-del);
        pnt.push_back(std::stold(buff));

        clasters.emplace_back(std::move(pnt));

        i++;
        if(i >= amount_points) break;
    }
    //
    return clasters;
}
