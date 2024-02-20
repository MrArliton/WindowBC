
namespace a_anl
{

    struct condition
    {
        std::vector<point> points; // Points for working
        std::map<std::string, lfloat> info; // Info of condition 
        std::vector<size_t> markers; // Clasters markers
    };

    class AnalyseSystem
    {
    private:
        std::vector<condition> steps;
        int step = 0; // -1 Not a steps       
    public:
        AnalyseSystem(const AnalyseSystem&) = delete;
        AnalyseSystem(AnalyseSystem&&) = delete;
        AnalyseSystem& operator=(const AnalyseSystem&) = delete; 

        AnalyseSystem() { steps.emplace_back(); }
        ~AnalyseSystem() {}

        //void calculateMethod(const std::string& method,const std::map<std::string, ldouble>& options);

        void nextStep() { step++; steps.emplace_back(); }
        void backStep() { if(step > 0) { step--; steps.pop_back(); }}   

        void createInStepPoints(const std::vector<point>& points) { steps.at(step).points = points; }  // Copyied
        void createInStepPoints(std::vector<point>&& points) { std::swap(steps.at(step).points, points); } // Moved
        
        int getStepIndex() { return step; }

        condition& getCurrentStepCondition() // We can make some calculation with it
        { 
            return steps.at(step);
        }    
    };

}