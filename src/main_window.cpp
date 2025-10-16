#include <format>

#include <QHBoxLayout>
#include <QVector>
#include <QSqlError>

#include "main_window.hpp"
#include "pages/category.hpp"
#include "pages/analytic.hpp"
#include "pages/operation.hpp"
#include "models/category.hpp"
#include "sidebar.hpp"
#include "storage.hpp"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
	auto st = storage::Storage::connect();
	if (!st.has_value()) {
		throw std::runtime_error(std::format("Failed to connect db: {}", st.error().toStdString()));
	}
	storage = st.value();	
	auto cats_res = storage.get_categories();
	if (!cats_res.has_value()) {
		throw std::runtime_error(std::format(":Failed to get categories: {}", st.error().toStdString()));
	}
	categories = cats_res.value();
	layout = new QHBoxLayout(this);
	auto* sidebar = new Sidebar();
	connect(sidebar, &Sidebar::page_changed, this, &MainWindow::set_page);
	category_page = new pages::CategoryPage(categories, this);
	operation_page = new pages::OperationPage();
	analytic_page = new pages::AnalyticPage();
	layout->addWidget(sidebar);
	layout->addWidget(category_page);
	current_page = category_page;

	using pages::CategoryPage;
	connect(category_page, &CategoryPage::throw_create_category, this, &MainWindow::create_category_handler);
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

void MainWindow::create_category_handler(models::db::Category category) {
	auto result = storage.create_category(category);
	if (!result) {
		qDebug() << "Failed to create category:" << result.error();
		return;
	}
	size_t id = result.value();
	if (!category.parent_id) {
		qDebug() << "Created category with id =" << id << "and name" << category.name;
		models::Category treeCategory {
			.id = id,
			.name = category.name,	
		};
		categories.emplace_back(treeCategory);
		emit created_category(treeCategory);
	} else {
		size_t parent_id = category.parent_id.value();
		qDebug() << "Created subccategory with id =" << id << "and name" << category.name << "and parent_id" << parent_id;
		models::Subcategory treeSubcategory {
			.id = id,
			.name = category.name,	
		};
		for(auto& category : categories) {
			if (category.id == parent_id) {
				category.subcategories.emplace_back(treeSubcategory);
				break;
			}
		}
		emit created_subcategory(treeSubcategory, parent_id);
	}
}
