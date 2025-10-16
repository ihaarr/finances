#pragma once

#include <QWidget>

#include "storage.hpp"
#include "models/category.hpp"

class QHBoxLayout;

namespace pages
{
enum class Type;
class CategoryPage;
class OperationPage;
class AnalyticPage;
}

namespace storage
{
class Storage;
}

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
signals:
	void created_category(models::Category);
	void created_subcategory(models::Subcategory, size_t parent_id);
public slots:
	void create_category_handler(models::db::Category);
private slots:
	void set_page(pages::Type page);
private:
	QWidget* current_page;
	pages::CategoryPage* category_page;
	pages::OperationPage* operation_page;
	pages::AnalyticPage* analytic_page;
	QHBoxLayout* layout;
	storage::Storage storage;
	QVector<models::Category> categories;
};
