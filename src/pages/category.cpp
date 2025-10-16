#include <optional>

#include <QVector>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "../main_window.hpp"
#include "../models/category.hpp"
#include "category.hpp"
#include "add_category.hpp"

namespace pages
{
enum Data {
	Id = 255,
};

CategoryPage::CategoryPage(QVector<models::Category> const& categories, MainWindow* parent) : QWidget(parent) {
	add_window = new AddCategoryWindow(categories, this);
	add_window->hide();
	add_window->resize(300, 300);

	auto* addCategoryBtn = new QPushButton("Добавить");
	connect(addCategoryBtn, &QPushButton::clicked, add_window, &QWidget::show);
	auto* removeCategoryBtn = new QPushButton("Удалить");
	removeCategoryBtn->setEnabled(false);
	auto* editCategoryBtn = new QPushButton("Редактировать");
	editCategoryBtn->setEnabled(false);

	treeCategories = new QTreeWidget(this);
	treeCategories->setMinimumHeight(300);
	treeCategories->setMinimumWidth(500);
	treeCategories->setColumnCount(1);

	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(addCategoryBtn);
	mainLayout->addWidget(removeCategoryBtn);
	mainLayout->addWidget(editCategoryBtn);
	mainLayout->addWidget(treeCategories);
	
	QList<QTreeWidgetItem*> items;
	for(const auto& category : categories) {
		auto* cat = new QTreeWidgetItem(treeCategories); 
		cat->setText(0, category.name);
		cat->setData(0, Data::Id, QVariant::fromValue(category.id));
		for(const auto& subcategory : category.subcategories) {
			auto* subcat = new QTreeWidgetItem(cat); 
			subcat->setText(0, subcategory.name);
			subcat->setData(0, Data::Id, QVariant::fromValue(subcategory.id));
		}
		items.append(cat);
	}
	treeCategories->insertTopLevelItems(0, items);

	connect(parent, &MainWindow::created_category, this, &CategoryPage::handle_created_category);
	connect(parent, &MainWindow::created_subcategory, this, &CategoryPage::handle_created_subcategory);
	connect(this, &CategoryPage::handled_created_category, add_window, &AddCategoryWindow::add_category_after_created);
}

void CategoryPage::handle_create_category(size_t parent_id, QString name) {
	std::optional<size_t> parid;
	if (parent_id != 0) {
		parid = parent_id; 
	}
	models::db::Category category{
		.id = 0,
		.name = name,
		.parent_id = parid
	};
	emit throw_create_category(category);	
}

void CategoryPage::handle_created_category(models::Category category) {
	auto* treeCategory = new QTreeWidgetItem(treeCategories);
	treeCategory->setText(0, category.name);
	treeCategory->setData(0, Data::Id, QVariant::fromValue(category.id));
	treeCategories->insertTopLevelItem(0, treeCategory);
	emit handled_created_category(category);
}

void CategoryPage::handle_created_subcategory(models::Subcategory subcategory, size_t parent_id) {
	QTreeWidgetItem* parent = nullptr;
	for(uint32_t i = 0; i < treeCategories->topLevelItemCount(); ++i) {
		auto* item = treeCategories->topLevelItem(i);
		if (item->data(0, Data::Id).toInt() == parent_id) {
			auto* treeSubcategory = new QTreeWidgetItem(item);
			treeSubcategory->setText(0, subcategory.name);
			treeSubcategory->setData(0, Data::Id, QVariant::fromValue(subcategory.id));
			item->addChild(treeSubcategory);
		}
	}
}
}


