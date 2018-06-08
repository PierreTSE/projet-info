#ifndef KEYWORD_LIST_HPP
#define KEYWORD_LIST_HPP

#include <string>
#include <set>
#include <cctype>


using Tag = std::string;

struct TagCaseInsensitiveComparator
{
    bool operator()(const Tag& a, const Tag& b) const
    {
        size_t i = 0;
        for(; i < a.size() && i < b.size(); i++)
        {
            if(tolower(a[i]) != tolower(b[i]))
                return tolower(a[i]) < tolower(b[i]);
        }
        return i >= a.size() && i < b.size();
    }
};
using TagList = std::set<Tag, TagCaseInsensitiveComparator>; //avec le set, chaque Tag est unique

#endif // !KEYWORD_LIST_HPP
