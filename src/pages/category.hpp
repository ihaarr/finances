#pragma once

#include <QWidget>

#include "../models/category.hpp"

/*
namespace models
{
class Category;
namespace db
{
class Category;
}
}
*/
class QTreeWidget;
class MainWindow;

namespace pages
{
class AddCategoryWindow;

class CategoryPage : public QWidget {
	Q_OBJECT
public:
	CategoryPage(QVector<models::Category> const& cats, MainWindow* parent = nullptr);

signals:
	void throw_create_category(models::db::Category);
	void handled_created_category(models::Category);
public slots:
	void handle_create_category(size_t parent_id, QString name);
	void handle_created_category(models::Category);
	void handle_created_subcategory(models::Subcategory, size_t parent_id);
private:
	QTreeWidget* treeCategories;
	AddCategoryWindow* add_window;
};
}

