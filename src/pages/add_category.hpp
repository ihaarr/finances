#pragma once

#include <QWidget>

#include "../models/category.hpp"

class QComboBox;
class QTextEdit;

namespace pages
{
class CategoryPage;

class AddCategoryWindow : public QWidget {
	Q_OBJECT
public:	
	AddCategoryWindow(QVector<models::Category> const& categories, CategoryPage* parent = nullptr);
private slots:
	void handle_create_clicked();
public slots:
	void add_category_after_created(models::Category);
	void handle_remove_category(size_t);
signals:
	void create_category(size_t parent_id, QString name);
private:
	QComboBox* categoriesCombo;
	QTextEdit* edit;
};
}
