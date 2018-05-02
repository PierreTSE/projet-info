#ifndef OWNING_COLLECTION_HPP
#define OWNING_COLLECTION_HPP

template<typename T>
class OwningCollection : public Collection<T>
{
public:
};

class OwningIterator : public IteratorBase<T>

#endif // !OWNING_COLLECTION_HPP
