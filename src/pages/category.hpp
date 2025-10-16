#pragma once

#include "page.hpp"
#include "../models/category.hpp"

namespace pages
{
class AddCategoryWindow;

class CategoryPage : public Page {
	Q_OBJECT
public slots:
	void handle_create_category(size_t parent_id, QString name);
public:
	CategoryPage(QVector<models::Category> cats, QWidget* parent = nullptr);
private:
	QVector<models::Category> categories;
	AddCategoryWindow* add_window;
};
}

