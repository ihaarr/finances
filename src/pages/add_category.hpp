#pragma once

#include <QWidget>

class QComboBox;
class QTextEdit;

namespace models
{
class Category;
}
namespace pages
{
class CategoryPage;

class AddCategoryWindow : public QWidget {
	Q_OBJECT
public:	
	AddCategoryWindow(QVector<models::Category> const& categories, CategoryPage* parent = nullptr);
private slots:
	void handle_create_clicked();
signals:
	void create_category(size_t parent_id, QString name);
private:
	QComboBox* categoriesCombo;
	QTextEdit* edit;
};
}
