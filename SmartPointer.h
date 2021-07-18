#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <exception>
#include <unordered_map>

using namespace std;

template<class T>
class SmartPointer{
public:

    SmartPointer(T *value=nullptr);
    SmartPointer(const SmartPointer &value);
    ~SmartPointer();

    T* operator->() const;
    T& operator*() const;
    void operator=(T *value);
    void operator=(const SmartPointer &value);
    bool operator==(const SmartPointer &value);

    T* pObject;
    inline static unordered_map<T*, int> map;
};

template<class T>
inline SmartPointer<T>::SmartPointer(T *value){

    pObject = value;
    auto search = map.find(pObject);

    if ( search != map.end() )
        ++search->second;
    else
        map.insert({pObject, 1});

}

template<class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer &value){

    pObject = value.pObject;
    auto search = map.find(pObject);

    if ( search != map.end() )
        ++search->second;
    else
        map.insert({pObject, 1});
}

template<class T>
inline SmartPointer<T>::~SmartPointer(){

    auto search = map.find(this->pObject);
    if ( search != map.end() ){

        search->second = --search->second;

        if ( search->second == 0 ){
            map.erase(search);
            delete pObject;
        }

    }else
        cerr << "destroying object that was not in the map.\n";
}

template<class T>
inline T *SmartPointer<T>::operator->() const {

    return pObject;
}

template<class T>
inline T &SmartPointer<T>::operator*() const {

    return *pObject;
}

template<class T>
inline void SmartPointer<T>::operator =(T *value) {

    /* reassigning to the same object shouldnt do anything */
    if ( value != pObject ){

        /* when reassigning a smartpointer we first have to dekrement
         * the counter of the pointer it was previously assigned to */
        auto search = map.find(pObject);
        if ( search != map.end() ){

            /* if this is the last smartpointer, we have to destroy the object */
            if(--search->second == 0){

                map.erase(search);
                delete pObject;
            }
        }

        pObject = value;

        search = map.find(value);
        if ( search != map.end() )
            search->second = ++search->second;
        else
            map.insert( {value, 1} );
    }
}

template<class T>
inline void SmartPointer<T>::operator =(const SmartPointer &value){

    /* reassigning to the same object shouldnt do anything */
    if ( pObject != value.pObject ){

        /* when reassigning a smartpointer we first have to decrement
         * the counter of the pointer it was previously assigned to */
        auto search = map.find(pObject);
        if ( search != map.end() ){

            /* if this is the last smartpointer, we have to destroy the object */
            if(--search->second == 0){

                map.erase(search);
                delete pObject;
            }
        }

        pObject = value.pObject;
        search = map.find(pObject);

        if ( search != map.end() )
            ++search->second;
        else
            throw runtime_error("assigning sp to sp failed.\n");
    }
}


template<class T>
inline bool SmartPointer<T>::operator==(const SmartPointer &value) {

    if ( pObject == value.pObject ){
        return true;
    }else{
        return false;
    }
}

#endif // SMARTPOINTER_H
