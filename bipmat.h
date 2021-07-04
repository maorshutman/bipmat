//
//  bipmat.h
//  bipmat
//
//  Created by Maor Shutman on 04/07/2021.
//  Copyright © 2021 Maor Shutman. All rights reserved.
//

#ifndef bipmat_h
#define bipmat_h

#include <vector>


namespace bm { class BipartiteMatcher {
    private:
        std::vector<std::vector<float>> costs;
        int n = 0;
        int m = 0;
    
    public:
        BipartiteMatcher(std::string input_path);
        ~BipartiteMatcher();
        
        void match_max();
        void match_min();
    
        void print_costs();
        int get_n() { return n; }
        int get_m() { return m; }
    
};

}

#endif /* bipmat_h */
