//
//  hash.hpp
//  HashMAp
//
//  Created by Akhil Vinta on 3/6/20.
//  Copyright © 2020 Akhil Vinta. All rights reserved.
//

#ifndef ExapandableHashMap_h
#define ExapandableHashMap_h

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

// Hash.h
using namespace std;
const int BUCKETS = 8;

template<typename KeyType, typename ValueType>

class ExapandableHashMap{
public:
    ExapandableHashMap(double maximumLoadFactor = 0.5);
    ~ExapandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    const ValueType* find(const KeyType& key) const;
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExapandableHashMap*>(this)->find(key));
    }

    ExapandableHashMap(const ExapandableHashMap&) = delete;
    ExapandableHashMap& operator=(const ExapandableHashMap&) = delete;
    
    
    
    
private:
    struct Pair {
       
        Pair(KeyType m_key, ValueType m_value){
            key = m_key;
            value = m_value;
        }
        KeyType key;
        ValueType value;
        
    };
    
    
    int m_maxItems;
    int m_size;
    int m_bucketCount;
    double m_loadFactor;
    bool calledFromResize;
    vector<Pair*>* hashTable;
    
    void resize();
    unsigned int hashThis(const KeyType& key) const;

};

#endif /* ExapandableHashMap_h */


template<typename KeyType, typename ValueType>
ExapandableHashMap<KeyType, ValueType> :: ExapandableHashMap(double loadFactor){
    
    this -> m_loadFactor = (loadFactor <= 0) ? 0.5 : loadFactor;
    
    m_bucketCount = 8;
    hashTable = new vector<Pair*>[m_bucketCount];
    m_size = 0;
    m_maxItems = m_bucketCount * this -> m_loadFactor;
    calledFromResize = false;
}

template<typename KeyType, typename ValueType>
ExapandableHashMap<KeyType, ValueType> :: ~ExapandableHashMap(){
   
    int n = (calledFromResize) ? 2 : 1;
    
    for(int i = 0; i < m_bucketCount/n; i++){
        for(int j = 0; j < hashTable[i].size(); j++){
            Pair* temp = hashTable[i][j];
            hashTable[i].erase(hashTable[i].begin() + j);
            delete temp;
        }
    }
    delete [] hashTable;
}


template<typename KeyType, typename ValueType>
void ExapandableHashMap<KeyType, ValueType> :: associate(const KeyType& key, const ValueType& value) {

    m_size++;

    if(m_size > m_maxItems)
        this->resize();
    
    int bucketNum = hashThis(key);
    
    for(int i = 0; i < hashTable[bucketNum].size(); i++){
        if(hashTable[bucketNum][i]->key == key){
            hashTable[bucketNum][i]->value = value;
            m_size--;
            return;
        }
    }
    
    hashTable[bucketNum].push_back(new Pair(key,value));
    
}


template<typename KeyType, typename ValueType>
void ExapandableHashMap<KeyType, ValueType> :: resize(){

    m_bucketCount *= 2;
    m_maxItems = m_bucketCount * m_loadFactor;
    vector<Pair*>* tempHash = new vector<Pair*>[m_bucketCount];
    
    for(int i = 0; i < m_bucketCount/2 ; i++){
        for(int j = 0; j < hashTable[i].size(); j++){
            Pair* temp = hashTable[i][j];
            int bucketNum = hashThis(temp->key);
            tempHash[bucketNum].push_back(new Pair(temp->key,temp->value));
        }
    }
    calledFromResize = true;
    this -> ~ExapandableHashMap();
    calledFromResize = false;
    hashTable = tempHash;
        
}


template<typename KeyType, typename ValueType>
void ExapandableHashMap<KeyType, ValueType> :: reset(){
    ~ExapandableHashMap();
    hashTable = new vector<Pair*>[BUCKETS];
    m_size = 0;
    m_bucketCount = 8;
    m_maxItems = m_bucketCount * m_loadFactor;
}



template<typename KeyType, typename ValueType>
const ValueType* ExapandableHashMap<KeyType, ValueType>::find(const KeyType& key) const {
   
    int numBucket = hashThis(key);
    
    if(hashTable[numBucket].empty())
        return nullptr;

    for(int i = 0; i < hashTable[numBucket].size(); i++){
        if(hashTable[numBucket][i] -> key == key)
            return &(hashTable[numBucket][i] -> value);
    }
    
    return nullptr;
}


template<typename KeyType, typename ValueType>
int ExapandableHashMap<KeyType, ValueType> :: size() const{
    return m_size;
}

template<typename KeyType, typename ValueType>
unsigned int ExapandableHashMap<KeyType, ValueType> :: hashThis(const KeyType& key) const{
    
    unsigned int hasher(const KeyType & key);
    
    unsigned int h = hasher(key) % m_bucketCount;
    return h;
    
}
