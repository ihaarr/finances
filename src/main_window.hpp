#pragma once

#include <QWidget>

#include "storage.hpp"

class QHBoxLayout;
namespace models
{
struct Category;
}

namespace pages
{
class Page;
enum class Type;
}

namespace storage
{
class Storage;
}

class MainWindow : public QWidget {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
private slots:
	void set_page(pages::Type page);
private:
	pages::Page* current_page;
	pages::Page* category_page;
	pages::Page* operation_page;
	pages::Page* analytic_page;
	QHBoxLayout* layout;
	storage::Storage storage;
};
