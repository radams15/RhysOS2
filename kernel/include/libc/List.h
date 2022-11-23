//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_LIST_H
#define RHYSOS_LIST_H

#include "Memory/Memory.h"
#include "Types.h"
#include "Memory/malloc.h"
#include "Panic.h"

template<typename T>
class List {
public:
    typedef int8 (*Comparator)(T, T);

    List(uint32 maxSize, Comparator itemComparator);

    List(void* addr, uint32 maxSize, Comparator itemComparator);

    void destroy();
    void insert(T item);
    T get(uint32 i);
    T remove(uint32 i);
    int length();

private:
    int8 defaultComparator(T a, T b);

    T* arry;
    uint32 size;
    uint32 maxSize;
    Comparator compare;
};

template<typename T>
List<T>::List(unsigned int maxSize, Comparator itemComparator) {
    arry = (T*) Memory::kmalloc(maxSize*sizeof(T));

    memset((uint8*) arry, 0, maxSize*sizeof(T));

    size = 0;
    this->maxSize = maxSize;
    compare = itemComparator;
}

template<typename T>
List<T>::List(void *addr, unsigned int maxSize, Comparator itemComparator) {
    arry = (T*) addr;

    memset((uint8*) arry, 0, maxSize*sizeof(T));
    size = 0;
    this->maxSize = maxSize;
    compare = itemComparator;
}

template<typename T>
void List<T>::destroy() {

}

template<typename T>
void List<T>::insert(T item) {
    uint32 i = 0;
    while(i < size && compare(arry[i], item)){
        i++;
    }

    if(i == size){
        arry[size++] = item;
    }else{
        T temp = arry[i];
        arry[i] = item;

        while(i<size){
            i++;
            T temp2 = arry[i];
            arry[i] = temp;
            temp = temp2;
        }
        size++;
    }
}

template<typename T>
T List<T>::get(unsigned int i) {
    if(i < size) {
        return arry[i];
    }else{
        PANIC("Error, list index greater than list length!");
        return NULL;
    }
}

template<typename T>
T List<T>::remove(unsigned int i) {
    while(i<size){
        arry[i] = arry[i+1];
        i++;
    }

    size--;
}

template<typename T>
int8 List<T>::defaultComparator(T a, T b) {
    return a<b;
}

template<typename T>
int List<T>::length() {
    return size;
}


#endif //RHYSOS_LIST_H
