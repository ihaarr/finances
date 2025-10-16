#include <QVector>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "../models/category.hpp"
#include "category.hpp"
#include "add_category.hpp"

namespace pages
{
enum Data {
	Id = 255,
};

CategoryPage::CategoryPage(QVector<models::Category> cats, QWidget* parent) : Page(parent), categories(cats) {
	add_window = new AddCategoryWindow(categories, this);
	add_window->hide();
	add_window->resize(300, 300);

	auto* addCategoryBtn = new QPushButton("Добавить");
	connect(addCategoryBtn, &QPushButton::clicked, add_window, &QWidget::show);
	auto* removeCategoryBtn = new QPushButton("Удалить");
	removeCategoryBtn->setEnabled(false);
	auto* editCategoryBtn = new QPushButton("Редактировать");
	editCategoryBtn->setEnabled(false);

	auto* treeCategories = new QTreeWidget(this);
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
}

void CategoryPage::handle_create_category(size_t parent_id, QString name) {
	qDebug() << parent_id << name;	
}
}


