#ifndef KEYWORD_LIST_HPP
#define KEYWORD_LIST_HPP

#include <string>
#include <unordered_set>

using Tag = std::string;
using TagList = std::unordered_set<Tag>; //avec le set, chaque Tag est unique

#endif // !KEYWORD_LIST_HPP
