#ifndef SEPARATE_CHAINING_H
#define SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include "Employee.h"
#include "utils.h"

using namespace std;

template <typename HashedObj>
class ChainingHash
{
  public:
    explicit ChainingHash( int size = 101 ) : currentSize{ 0 }
      { theLists.resize( 101 ); }

    bool contains(const HashedObj& x) const {
        auto& tempList = theLists[myhash(x)];
        return find(tempList.begin(), tempList.end(), x) != tempList.end();
    }

    void makeEmpty() {
        for (auto& tempList : theLists) {
            tempList.clear();
        }
        currentSize = 0;
    }

    bool insert(const HashedObj& x) {
        auto& tempList = theLists[myhash(x)];
        if (find(tempList.begin(), tempList.end(), x) != tempList.end()) {
            return false;
        }

        tempList.push_back(x);
        currentSize++;

        if (loadFactor() >= 1.0) {
            rehash();
        }

        return true;
    }

    bool insert(HashedObj&& x) {
        auto& tempList = theLists[myhash(x)];
        auto itr = find(tempList.begin(), tempList.end(), x);

        if (itr != tempList.end()) {
            return false;
        }
        tempList.push_back(move(x));
        ++currentSize;

        if (loadFactor() >= 1.0) {
            rehash();
        }
        return true;
    }

    bool remove(const HashedObj& x) {
        auto& tempList = theLists[myhash(x)];
        auto itr = find(tempList.begin(), tempList.end(), x);

        if (itr == tempList.end()) {
            return false;
        }

        tempList.erase(itr);
        currentSize--;
        return true;
    }

    double readLoadFactor() 
    {
        return loadFactor();
    }

    double readCurrentSize() 
    {
        return currentSize;
    }

    double readArraySize() 
    {
        return theLists.size();
    }

  private:
    vector<list<HashedObj>> theLists;
    int currentSize;

    void rehash() {
        vector<list<HashedObj>> oldLists = theLists;

        theLists.resize(nextPrime(2 * theLists.size()));
        for (auto& tempList : theLists) {
            tempList.clear();
        }
        currentSize = 0;
        for (auto& tempList : oldLists) {
            for (auto& x : tempList) {
                insert(move(x));
            }
        }
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % theLists.size( );
    }

    double loadFactor() const {
        return static_cast<double>(currentSize) / theLists.size();
    }
};

#endif
