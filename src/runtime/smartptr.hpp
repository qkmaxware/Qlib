#ifndef _SMART_POINTER_H
#define _SMART_POINTER_H

/// <Summary>
/// Simple smart pointer class
/// </Summary>
template <class t>
class ptr {
    private:
        t* ref;
    
    public:
        ptr(t* value) : ref(value) {}
        ~ptr(){
            delete ref;
        }

        t& operator * () {
            return *ref;
        }

        t* operator -> (){
            return ref;
        }
};

#endif