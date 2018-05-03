#ifndef COLLECTION_POOL_HPP
#define COLLECTION_POOL_HPP

#include "Collection.hpp"
#include "CollectionIterator.hpp"
#include <vector>
#include <iterator>

template<typename T, bool is_const = false>
class PoolIterator;

template<typename T>
class CollectionPool : public Collection<T>
{
    public:
        CollectionPool() = default;
        CollectionPool(size_type n, const value_type& v = value_type{}) : pool_(n, v) {}
        CollectionPool(std::initializer_list<value_type> list) : pool_{list} {}
        
    
    private:
        std::vector<value_type> pool_;
};
#endif // !COLLECTION_POOL_HPP
