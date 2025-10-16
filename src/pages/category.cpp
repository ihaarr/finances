#include <QVector>
#include <QTreeWidget>

#include "../models/category.hpp"
#include "category.hpp"

#include <iostream>
namespace pages
{
enum Data {
	Id = 255,
};

CategoryPage::CategoryPage(QVector<models::Category> const& cats, QWidget* parent) : Page(parent) {
	auto* categories = new QTreeWidget(this);
	categories->setColumnCount(1);
	QList<QTreeWidgetItem*> items;
	for(const auto& category : cats) {
		auto* cat = new QTreeWidgetItem(categories); 
		cat->setText(0, category.name);
		cat->setData(0, Data::Id, QVariant::fromValue(category.id));
		for(const auto& subcategory : category.subcategories) {
			auto* subcat = new QTreeWidgetItem(cat); 
			subcat->setText(0, subcategory.name);
			subcat->setData(0, Data::Id, QVariant::fromValue(subcategory.id));
		}
		items.append(cat);
	}
	categories->insertTopLevelItems(0, items);
}
}


