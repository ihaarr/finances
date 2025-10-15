#include <QHBoxLayout>
#include "main_window.hpp"
#include "pages/category.hpp"
#include "pages/analytic.hpp"
#include "pages/operation.hpp"
#include "sidebar.hpp"

MainWindow::MainWindow() : QWidget(nullptr) {
	layout = new QHBoxLayout(this);
	auto* sidebar = new Sidebar();
	connect(sidebar, &Sidebar::page_changed, this, &MainWindow::set_page);
	category_page = new pages::CategoryPage({0,1,2,3,4});
	operation_page = new pages::OperationPage();
	analytic_page = new pages::AnalyticPage();
	layout->addWidget(sidebar);
	layout->addWidget(category_page);
	current_page = category_page;
}

void MainWindow::set_page(pages::Type page) {
	auto* prev = current_page;
	switch(page) {
		case pages::Type::Category:
			layout->replaceWidget(current_page, category_page);
			current_page = category_page;
			break;
		case pages::Type::Operation:
			layout->replaceWidget(current_page, operation_page);
			current_page = operation_page;
			break;
		case pages::Type::Analytic:
			layout->replaceWidget(current_page, analytic_page);
			current_page = analytic_page;
			break;
	}
	if (prev != current_page) {
		prev->hide();
		current_page->show();
	}
}
