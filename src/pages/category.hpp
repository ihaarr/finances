#pragma once

#include "page.hpp"

namespace models
{
struct Category;
}

namespace pages
{
class CategoryPage : public Page {
	Q_OBJECT
public:
	CategoryPage(QVector<models::Category> const& cats, QWidget* parent = nullptr);
};
}

