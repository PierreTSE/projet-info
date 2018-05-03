#ifndef FILTERED_COLLECTION_HPP
#define FILTERED_COLLECTION_HPP

#include "Collection.hpp"
#include <vector>
#include <functional>

template<typename T>
class FilteredIterator;

template<typename T>
class FilteredCollection : public Collection<T>
{
    friend class FilteredIterator<T>;

    public:
        using value_type = typename Collection<T>::value_type;
        using allocator_type = typename Collection<T>::allocator_type;
        using reference = typename Collection<T>::reference;
        using const_reference = typename Collection<T>::const_reference;
        using pointer = typename Collection<T>::pointer;
        using const_pointer = typename Collection<T>::const_pointer;
        using iterator = typename Collection<T>::iterator;
        //using const_iterator = typename Collection<T>::const_iterator;
        using reverse_iterator = typename Collection<T>::reverse_iterator;
        //using const_reverse_iterator = typename Collection<T>::const_reverse_iterator;
        using difference_type = typename Collection<T>::difference_type;
        using size_type = typename Collection<T>::size_type;

    private:
        using filtre_t = std::function<bool(const_reference)>;

    public:
        FilteredCollection(Collection<T>& c, const filtre_t& f) : collection_{ c }, filtre_{ f } {};

        iterator begin() {
            auto first = collection_.begin();
            auto last = collection_.end();
            // Gestion du cas vide
            if(first == last)
                return end();
            
            CollectionIterator<T> it(new FilteredIterator<T>(*this, first)); 
            // si le premier élément passe le filtre, on le retourne
            if(filtre_(*it))
                return it;
            // sinon on envoie le suivant (qui passe le filtre)
            return ++it;
        }
        iterator end()
        {
            return iterator(new FilteredIterator<T>(*this, collection_.end()));
        }

    private:
        Collection<T>& collection_;
        filtre_t filtre_;
};

template<typename T>
class FilteredIterator : public IteratorBase<T>
{
    public:
        FilteredIterator(FilteredCollection<T>& f, CollectionIterator<T> itr, bool valid = true) : 
            filter_reference_{ f }, itr_{ itr }, valid_{valid} {}
    
        T & operator*() override //dereference operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator*() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_.end())
                throw std::out_of_range("Error: call operator*() on an iterator past the end of his internal container");
            return *itr_;
        }
    
        const T& operator*() const override //const dereference operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator*() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_.end())
                throw std::out_of_range("Error: call operator*() on an iterator past the end of his internal container");
            return *itr_;
        }
    
        T* operator->() override //arrow operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator->() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_.end())
                throw std::out_of_range("Error: call operator->() on an iterator past the end of his internal container");
            return itr_.operator->();
        }
    
        const T* operator->() const override //const arrow operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator->() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_.end())
                throw std::out_of_range("Error: call operator->() on an iterator past the end of his internal container");
            return itr_.operator->();
        }
    
        void operator++() override //pre-increment operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator++() on an iterator in invalid state");
            while(itr_ != filter_reference_.collection_.end())
            {
                ++itr_;
                if(itr_ == filter_reference_.collection_.end() or filter_reference_.filtre_(*itr_)) 
                    return;
            }
            valid_ = false;
        }
    
        void operator--() override //pre-decrement operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator--() on an iterator in invalid state");
            while(itr_ != filter_reference_.collection_.begin())
            {
                --itr_;
                if(filter_reference_.filtre_(*itr_))
                    return;
            }
            valid_ = false;
        }
    
        IteratorBase<T>* clone() const override
        {
            return new FilteredIterator(filter_reference_, itr_, valid_);
        }
    
        bool equal(const IteratorBase<T>& rhs) const override
        {
            auto& rhs_ref = dynamic_cast<const FilteredIterator&>(rhs);
            if(!valid_ and !rhs_ref.valid_)
                return true;
            if(!valid_ or !rhs_ref.valid_)
                return false;
            return itr_ == rhs_ref.itr_;
        }
    
    private:
        FilteredCollection<T>& filter_reference_;
        CollectionIterator<T> itr_;
        bool valid_;
};


#endif // !FILTERED_COLLECTION_HPP

