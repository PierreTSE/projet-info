#ifndef KEYWORD_LIST_HPP
#define KEYWORD_LIST_HPP

#include <string>
#include <vector>

struct Tag
{
	std::wstring str;
};

class TagList
{
private:
	std::vector<Tag> list_;
};

#endif // !KEYWORD_LIST_HPP
