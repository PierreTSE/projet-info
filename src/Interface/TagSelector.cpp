#include "TagSelector.hpp"
#include "../Utilities/Utilities.hpp"

TagSelector::TagSelector(TagList possibleTags, long long width, long long minHeight) :
	possibleTags_{ possibleTags },
    width_{width},
    minHeight_{minHeight}
{
    
}

Widget::img TagSelector::actualRender() const
{
	const auto nb_line = possibleTags_.size();
	const auto height = std::max<long long>(nb_line*lineHeight, minHeight_);

	img render(width_, height, 1, 3, 255);

	int i = 0;
	for (auto& tag : possibleTags_)
	{
		int state;
		if (selectedTags_.find(tag) != selectedTags_.end())
			state = 2;
		else
			state = 0;

		img line(width_, lineHeight, 1, 3, 255);
		unsigned char white[] = { 255, 255, 255 }, black[] = { 0, 0, 0 };

		line.draw_rectangle(5, 5, lineHeight - 5, lineHeight - 5, black);
		if (state != 2)
			line.draw_rectangle(6, 6, lineHeight - 6, lineHeight - 6, white);

		line.draw_text(33, (lineHeight - 23) / 2 + 1, UTF8toISO8859_1(tag).c_str(), black, white, 1, 23);

		render.draw_image(0, lineHeight*i, 0, 0, line);
		i++;
	}

	return render;
}

void TagSelector::actualResize(const dim_t& size)
{
	width_ = size.x;
	minHeight_ = size.y;
	callRedraw();
}

dim_t TagSelector::actualSize() const
{
	return dim_t{ width_, std::max<long long>(possibleTags_.size()*lineHeight, minHeight_) };
}

bool TagSelector::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<ClickEvent>(event.event))
	{
		const auto& ce = std::get<ClickEvent>(event.event);
		if (ce.type == ClickEvent::LEFT)
		{
			const int pos = event.pos.y / lineHeight;
			auto it = possibleTags_.begin();
			bool valid = true;

			for (int i = 0; i < pos && it != possibleTags_.end(); ++i)
			{
				if (++it == possibleTags_.end())
					valid = false;
			}

			if (valid)
			{
				const Tag& tag = *it;
				if (selectedTags_.find(tag) != selectedTags_.end())
					selectedTags_.insert(tag);
				else
					selectedTags_.erase(tag);
			}
			callBack(ce, this, selectedTags_);
			callRedraw();
		}
		return true;
	}

	return false;
}
