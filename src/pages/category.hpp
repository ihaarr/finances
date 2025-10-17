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
class QPushButton;
class QTreeWidgetItem;

namespace pages
{
class AddCategoryWindow;

class CategoryPage : public QWidget {
	Q_OBJECT
public:
	CategoryPage(QVector<models::Category> const& cats, MainWindow* parent = nullptr);

signals:
	void throw_create_category(models::db::Category);
	void throw_remove_category(size_t);
	void handled_created_category(models::Category);
	void category_removed_successful(size_t);
public slots:
	void handle_create_category(size_t parent_id, QString name);
	void handle_created_category(models::Category);
	void handle_created_subcategory(models::Subcategory, size_t parent_id);
	void receive_answer_remove_category(bool, QString);
private slots:
	void category_changed(QTreeWidgetItem*, int);
	void try_remove_category();
private:
	QTreeWidget* treeCategories;
	AddCategoryWindow* add_window;
	QPushButton* editCategoryBtn;
	QPushButton* removeCategoryBtn;
	QTreeWidgetItem* currentCategoryItem;
};
}

