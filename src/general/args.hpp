#ifndef _QLIB_GEN_ARGS_H
#define _QLIB_GEN_ARGS_H

#include <vector>

using namespace std;

template <class t>
class args {
    private:
        vector<t> list;
    public:
        args(){}
        args(vector<t> values): list(values){}

        size_t count(){
            return list.size();
        }

        t& operator[] (size_t idx){
            return list[idx];
        }

        args& operator << (t value){
            list.push_back(value);
            return *this;
        }
};

#endif