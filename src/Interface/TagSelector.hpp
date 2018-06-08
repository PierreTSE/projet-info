#ifndef TAGSELECTOR_HPP
#define TAGSELECTOR_HPP

#include "../Collection/Collection.hpp"
#include "Widget.hpp"
#include "../Image/TagList.hpp"
#include <functional>


/**
 * \brief Gère une liste de Tags sélectionnables pour la recherche
 */
class TagSelector : public Widget
{
    public:
		TagSelector(TagList possibleTags, TagList selectedTags, long long width, long long minHeight);

		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override;
		bool actualPropagateEvent(const Event& event) override;

		void setCallBack(const std::function<bool(ClickEvent, TagSelector*, TagList)> f) { callBack_ = f; }

    private:
		TagList possibleTags_;
		TagList selectedTags_;

		bool is_selected_ = false;

		long long width_;
		long long minHeight_;
		static constexpr long long lineHeight = 30;

		std::function<bool(ClickEvent, TagSelector*, TagList)> callBack_;
		bool callBack(ClickEvent ce, TagSelector* tagSelector, TagList tags) { return callBack_(ce, tagSelector, tags); }


};
#endif // TAGSELECTOR_HPP
