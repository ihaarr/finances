#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "pages/page.hpp"
#include "pages/type.hpp"

class MainWindow : public QWidget {
	Q_OBJECT

public:
	MainWindow();
private slots:
	void set_page(pages::Type page);
private:
	pages::Page* current_page;
	pages::Page* category_page;
	pages::Page* operation_page;
	pages::Page* analytic_page;
	QHBoxLayout* layout;
};
