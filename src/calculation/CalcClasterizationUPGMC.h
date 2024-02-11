
namespace a_info
{
    struct clast_info
    {
        std::string pathToPoints;
        std::vector<std::tuple<size_t, size_t, ldouble>> dendrogram;
        std::vector<point> points;
    };
}