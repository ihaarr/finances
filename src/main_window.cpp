#include <QHBoxLayout>
#include "main_window.hpp"
#include "pages/category.hpp"
#include "sidebar.hpp"

MainWindow::MainWindow() {
	auto* layout = new QHBoxLayout(this);
	auto* sidebar = new Sidebar();
	auto* page = new CategoryPage({0,1,2,3,4});
	layout->addWidget(sidebar);
	layout->addWidget(page);
}
