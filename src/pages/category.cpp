#include <optional>

#include <QVector>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLabel>

#include "../main_window.hpp"
#include "../models/category.hpp"
#include "category.hpp"
#include "add_category.hpp"

namespace pages
{
enum Data {
	Id = 255,
	ParentId,
};

CategoryPage::CategoryPage(QVector<models::Category> const& categories, MainWindow* parent) : QWidget(parent) {
	add_window = new AddCategoryWindow(categories, this);
	add_window->hide();
	add_window->resize(300, 300);

	auto* addCategoryBtn = new QPushButton("Добавить");
	connect(addCategoryBtn, &QPushButton::clicked, add_window, &QWidget::show);
	removeCategoryBtn = new QPushButton("Удалить");
	removeCategoryBtn->setEnabled(false);
	editCategoryBtn = new QPushButton("Редактировать");
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
			subcat->setData(0, Data::ParentId, QVariant::fromValue(category.id));
		}
		items.append(cat);
	}
	treeCategories->insertTopLevelItems(0, items);

	connect(parent, &MainWindow::created_category, this, &CategoryPage::handle_created_category);
	connect(parent, &MainWindow::created_subcategory, this, &CategoryPage::handle_created_subcategory);
	connect(this, &CategoryPage::handled_created_category, add_window, &AddCategoryWindow::add_category_after_created);
	connect(treeCategories, &QTreeWidget::itemClicked, this, &CategoryPage::category_changed);
	connect(removeCategoryBtn, &QPushButton::clicked, this, &CategoryPage::try_remove_category);
	connect(this, &CategoryPage::throw_remove_category, parent, &MainWindow::remove_category_handler);
	connect(parent, &MainWindow::result_remove_category, this, &CategoryPage::receive_answer_remove_category);
	connect(this, &CategoryPage::category_removed_successful, add_window, &AddCategoryWindow::handle_remove_category);
}

void CategoryPage::receive_answer_remove_category(bool success, QString err) {
	QDialog m;
	m.setModal(true);
	m.resize(100, 100);
	auto* layout = new QVBoxLayout(&m);
	auto* label = new QLabel(QString("Категория удалена"), &m);
	auto* okBtn = new QPushButton(QString("OK"), &m);
	layout->addWidget(label);
	layout->addWidget(okBtn);
	connect(okBtn, &QPushButton::clicked, &m, &QDialog::accept);
	auto id = currentCategoryItem->data(0, Data::Id).toInt();

	if (success) {
		if (!currentCategoryItem->data(0, Data::ParentId).toInt()) {
			qDebug() << "remove parent";
			delete treeCategories->takeTopLevelItem(treeCategories->indexOfTopLevelItem(currentCategoryItem));
			qDebug() << "removed parent";
		} else {
			auto parent_id = currentCategoryItem->data(0, Data::ParentId).toInt();
			qDebug() << "remove child with parent_id" << parent_id;
			for(uint32_t i = 0; i < treeCategories->topLevelItemCount(); ++i) {
				auto* parent = treeCategories->topLevelItem(i);
				if (parent->data(0, Data::Id).toInt() == parent_id) {
					parent->removeChild(currentCategoryItem);
					delete currentCategoryItem;
					qDebug() << "removed child";
				}
			}
		}
		currentCategoryItem = nullptr;
		treeCategories->setCurrentItem(nullptr);
		removeCategoryBtn->setEnabled(false);
		editCategoryBtn->setEnabled(false);
		emit category_removed_successful(id);
	} else {
		label->setText(QString("Не удалось удалить категорию: %1").arg(err));
	}

	m.exec();
}

void CategoryPage::try_remove_category() {
	qDebug() << "received try remove cateogry";
	auto id = currentCategoryItem->data(0, Data::Id).toInt();
	qDebug() << "trying remove category with id" << id;
	emit throw_remove_category(id);
}

void CategoryPage::category_changed(QTreeWidgetItem* current, int) {
	qDebug() << "category changed from" << currentCategoryItem << "to" << current;
	currentCategoryItem = current;
	removeCategoryBtn->setEnabled(true);
	editCategoryBtn->setEnabled(true);
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
			treeSubcategory->setData(0, Data::ParentId, QVariant::fromValue(parent_id));
			item->addChild(treeSubcategory);
		}
	}
}
}


