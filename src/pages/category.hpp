#pragma once

#include <vector>
#include <QtWidgets/QTreeWidget>
#include "page.hpp"

class CategoryPage : public Page {
	Q_OBJECT
public:
	CategoryPage(std::vector<size_t> const& cats, QWidget* parent = nullptr);
};
