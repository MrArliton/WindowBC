// Generic lincage - Centroid Clasterization (We use Euclidian distance) 

// --- Binary heap ---
struct binary_heap
{
    private:

    lfloat* A;
    size_t* I; // --- Find for default index in A   ---
    size_t* R; 
    size_t size = 0; 

    public:
    binary_heap() = delete;
    binary_heap(binary_heap const &) = delete;
    binary_heap(binary_heap const &&) = delete;
    binary_heap & operator=(binary_heap const &) = delete;

    binary_heap(std::vector<lfloat> distances): size(distances.size())
    {
        A = new lfloat[size];
        I = new size_t[size];
        R = new size_t[size];
        for(int i = 0;i < size;i++){
            A[i] = distances[i];
            I[i] = R[i] = i;
        }
    }

    size_t argmin()
    {
        return I[0];
    }

    void update(size_t i, lfloat val){
        
        if(A[i] < val){
            A[i] = val;
            heap_sort(R[i]);
        }else{
            A[i] = val;
            inverse_heap_sort(R[i]);
        }
    }

    void pop(){
        size--;
        I[0] = I[size];
        R[I[0]] = 0;
        heap_sort(0);
    }

    void remove(size_t idx)
    {
        // --- Remove an element from the heap. ---
        --size;
        R[I[size]] = R[idx];
        I[R[idx]] = I[size];
        if ( H(size)<=A[idx] )
        {
            inverse_heap_sort(R[idx]);
        }
        else
        {
            heap_sort(R[idx]);
        }
    }

    void heapify()
    {
        
        for(size_t i = size>>1;i > 0;)
        {
            --i;
            heap_sort(i);
        }

    }

    ~binary_heap()
    {
        delete[] A;
        delete[] I;
        delete[] R;
    }

    void out_heap(void)
    {
        int i = 0;
        int k = 1;
        while(i < size)
        {
            while((i < k) && (i < size))
            {
            std::cout << H(i) << " ";
            i++;
            }
            std::cout << std::endl;
            k = k * 2 + 1;
        }
    }
    
    private:
    void inverse_heap_sort(size_t i)
    {
        size_t j;
        for ( ; (i>0) && ( H(i)<H(j=(i-1)>>1) ); i=j)

        swap_heap(i,j);
    }

    void heap_sort(size_t i)
    {
        size_t j;
        for(;(j = (i*2+1))<size; i=j)
        {
            if(H(j) > H(i))
            {
                j++;
                if(j >= size || H(j)>=H(i)) break;
            }
            else if(j+1 < size && H(j+1) < H(j)) j++;

            swap_heap(i,j);
        }

    }

    void swap_heap(size_t idx1, size_t idx2)
    {
        size_t tmp  = I[idx1] ;       
        I[idx1] = I[idx2];
        I[idx2] = tmp;

        R[I[idx1]] = idx1;
        R[I[idx2]] = idx2;

    }
    inline lfloat H(const size_t i) const 
    {
        return A[I[i]];
    }
};

ldouble u_calculate_disimilarity(const point& point1,const point& point2, ldouble attraction_coef) {
    assert(point1.size() == point2.size());

    ldouble otv = 0, buff;

    for(int i = 0; i < point1.size();i++){
        buff = point1[i]-point2[i];
        otv += buff*buff;
    }

    return std::sqrt(otv) - attraction_coef;
}

dissimilarities u_calculate_start_dissimilarities(const std::vector<point>& points, ldouble attraction_coef){
    size_t width = points.size();
     
    assert(width > 0);

    dissimilarities dmat(width);
    for(int i = 0;i < width;i++){
        dmat[i].resize(width);
        for(int j = 0;j < i;j++){
            dmat[j][i] = dmat[i][j] = u_calculate_disimilarity(points[i],points[j], attraction_coef);
        }
    }
    return dmat;
}


bool u_stopingCriteria(const std::vector<ldouble>& distances, ldouble trend_coef){ // Calculating criteria of stopping upgmc method 
    const std::vector<ldouble>& last_min_distances = distances;

    if(last_min_distances.size() < 5){
        return false;    
    }
    
    size_t i = last_min_distances.size()-1;
    ldouble y_3 = last_min_distances[i];
    i--;
    for(;last_min_distances[i] > y_3;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_2 = last_min_distances[i]; 
    i--;
    for(;last_min_distances[i] > y_2;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_1 = last_min_distances[i];
    i--;
    for(;last_min_distances[i] > y_1;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_0 = last_min_distances[i];
    
    y_3 = y_3 - y_0 + (i+3)*trend_coef;
    y_2 = y_2 - y_0 + (i+2)*trend_coef;
    y_1 = y_1 - y_0 + (i+1)*trend_coef;

    ldouble criteria = 1.0/245.0 * (19.0 * (y_1 * y_1) - 11.0 * (y_2 * y_2) + 41.0 * (y_3 * y_3) + 12.0 * y_1 * y_2 - 64.0 * y_1 * y_3 - 46.0 * y_2 * y_3);

    if(criteria <= 0){
        return false;
    }
    return true;      
}

template<class u_thread>
std::vector<std::tuple<size_t, size_t, ldouble>> u_generic_linkage(std::vector<point>& points, ldouble attraction_coef, u_thread* thread){
    std::vector<std::tuple<size_t, size_t, ldouble>> dendrogram;
    std::vector<size_t> sizes(points.size(),1);
    GEN_OUT("Calculate dissimilarities");
    dissimilarities diss = u_calculate_start_dissimilarities(points, attraction_coef);
    GEN_OUT("End calculate dissimilarities");
    // S - array of available claster's indexes
    std::vector<size_t> indexes;
    indexes.reserve(points.size());
    for(size_t i = 0;i < points.size();i++){
        indexes.push_back(i);
    } 
    ////
    std::vector<size_t> n_nghbr;
    n_nghbr.resize(points.size()-1);
    std::vector<ldouble> mindist;
    mindist.resize(points.size()-1); 
    GEN_OUT("Init n_nghbr, mindist");
    // Initialization of n_nghbr, mindist
    for(auto i:indexes){
        if(i!=indexes.back()){
            ldouble mn_val = std::numeric_limits<ldouble>::max();
            size_t index = i+1;  
            for(int j = index; j < diss.size();j++){
                if(mn_val > diss[i][j]){
                    mn_val = diss[i][j];
                    index = j;
                }
            } 
            n_nghbr[i] = index; 
            mindist[i] = mn_val; 
        }
    }
    GEN_OUT("End init n_nghbr, mindist");
    ////
    GEN_OUT("Init priority_queue");
    // Initialization priority_queue of indices in S \ {N − 1}, keys are in mindist
    binary_heap p_q(mindist);
    p_q.heapify();
    GEN_OUT("End init priority_queue");

    std::vector<ldouble> last_min_distances;
    last_min_distances.reserve(points.size());

    size_t iteration = 0;
    const size_t s_amount = clasters.size()/100 + 1; 
    while(indexes.size() > 1){
        // --- For Thread working with wxWidgets ---
        if(thread->TestDestroy())
        {
            return {};
        }
        if(iteration%s_amount == 1)
        {
            thread->Progress(static_cast<lfloat>(iteration)/static_cast<lfloat>(clasters.size()));
        }
        //
        size_t a = p_q.argmin();
        size_t b = n_nghbr[a];
        ldouble mn_d = mindist[a];

        while(mn_d != diss[a][b]){
            // Find new n_nghbr
            mn_d = std::numeric_limits<ldouble>::max();
            for(size_t i:indexes){
                if(i > a && mn_d > diss[a][i]){
                    mn_d = diss[a][i];
                    b = i;
                }            
            }
            n_nghbr[a] = b;
            mindist[a] = mn_d;
            p_q.update(a,mn_d);
            // Update a,b
            a = p_q.argmin();  
            b = n_nghbr[a];
            mn_d = mindist[a];
            ////
        }
        last_min_distances.push_back(mn_d);
        p_q.pop();
        // Saving step in dendrogram
        dendrogram.push_back(std::make_tuple(a, // Index of first claster
        b, // Index of second claster 
        mn_d));// Dissimilarity between clasters
        
        indexes.erase(std::lower_bound(indexes.begin(),indexes.end(),a)); // Erase a

        // Update dissimilarities
        const size_t sz_a = sizes[a];
        const size_t sz_b = sizes[b];
        const size_t buff = sz_a+sz_b;
        for(size_t i:indexes){
            if(i!=b){
                diss[b][i] = diss[i][b] = // WMA Formula for update distances
                std::sqrt(((sz_a * diss[a][i]*diss[a][i] + sz_b * diss[b][i]*diss[b][i])/
                (buff))
                 + 
                ((sz_a*sz_b*diss[a][b]*diss[a][b])
                /
                (buff*buff))) - attraction_coef*sz_a*sz_b;
            }
        }
        sizes[b] += sizes[a];
        ////
        //
        for(size_t i:indexes){
            if(i < a){
                if(n_nghbr[i] == a){
                    n_nghbr[i] = b;
                }
            }else{
                break;
            }
        }
        //
        for(size_t i:indexes){
            if(i < b){
                if(diss[i][b] < mindist[i]){
                     n_nghbr[i] = b;
                     p_q.update(i,diss[i][b]);
                     mindist[i] = diss[i][b];   
                }        
            }else{
                break;
            }
        }
        if(b != indexes.back()){
            auto iter = std::lower_bound(indexes.begin(), indexes.end(), b);
            size_t index = *std::next(iter);
            mn_d = std::numeric_limits<ldouble>::max(); 
            for(iter++;iter!=indexes.end();iter++){
                if(mn_d > diss[b][*iter]){
                    index = *iter;
                    mn_d = diss[b][index];
                }
            }
            n_nghbr[b] = index;
            mindist[b] = mn_d;
            p_q.update(b,mn_d);
        }
    }
    // --- For Thread working with wxWidgets ---
    thread->Progress(1);
    //
    return dendrogram;
}

std::vector<size_t> make_markers_using_markov_stopping(std::vector<point>& points, std::vector<std::tuple<size_t, size_t, ldouble>> dendrogram, ldouble trend_coef)  
{
    std::vector<ldouble> distances;
    distances.reserve(dendrogram.size());
    std::vector<size_t> markers(points.size());
    for(int i = 1;i < markers.size();i++)
    {
        markers[i - 1] = i;
    }

    auto iter = dendrogram.begin(); 
    while(iter != dendrogram.end() && !u_stopingCriteria(distances, trend_coef))
    {
        size_t first = std::get<0>(*iter);
        size_t second =  std::get<1>(*iter);
        distances.push_back(std::get<2>(*iter));
        markers[first] = markers[second];
        iter = std::next(iter);
    }

    return markers;
}

