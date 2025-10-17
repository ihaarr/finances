#include <expected>
#include <optional>
#include <vector>
#include <algorithm>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "storage.hpp"
#include "models/category.hpp"

namespace storage
{
std::expected<Storage, QString> Storage::connect() {
	const QString INIT_TABLE_CATEGORIES = QStringLiteral(
	"CREATE TABLE IF NOT EXISTS Categories ("
	"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
	"name TEXT NOT NULL,"
	"parent_id INTEGER,"
	"FOREIGN KEY(parent_id) REFERENCES Categories(id)"
	");"
	);

	const QString INIT_TABLE_OPERATIONS = QStringLiteral(
	"CREATE TABLE IF NOT EXISTS Operations ("
	"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
	"category_id INTEGER NOT NULL,"
	"value INTERGER NOT NULL,"
	"date TEXT NOT NULL,"
	"FOREIGN KEY(category_id) REFERENCES Categories(id)"
	");"
	);

	auto db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("/Users/ihaarr/empty.db");
	if (!db.open()) {
		return std::unexpected{db.lastError().text()};	
	}

	QSqlQuery query(db);
	if (!query.exec("PRAGMA foreign_keys = ON")) {
		return std::unexpected{query.lastError().text()};	
	}
	if (!query.exec(INIT_TABLE_CATEGORIES)) {
		return std::unexpected{query.lastError().text()};	
	}	
	if (!query.exec(INIT_TABLE_OPERATIONS)) {
		return std::unexpected{query.lastError().text()};
	}
	
	return Storage{db}; 
}

Storage::Storage(const QSqlDatabase& db) : conn(db) {

}

std::expected<QVector<models::Category>, QString> Storage::get_categories() {
	QSqlQuery query(conn);
	query.prepare("SELECT id, name, parent_id FROM Categories ORDER BY id");
	if (!query.exec()) {
		return std::unexpected{query.lastError().text()};
	}
	struct CategoryDB {
		size_t id;
		QString name;
		std::optional<size_t> parent_id;
	};
	QVector<CategoryDB> categories;
	while(query.next()) {
		std::optional<size_t> parent_id;
		if (!query.value(2).isNull()) {
			parent_id = query.value(2).toInt();
		}
		categories.emplace_back(query.value(0).toInt(), query.value(1).toString(), std::move(parent_id));
	}
	std::unordered_map<size_t, std::vector<size_t>> hs;
	for(const auto& category : categories) {
		if (!category.parent_id.has_value()) {
			hs.insert({category.id, {}});
		}
	}
	for(const auto& category : categories) {
		if (category.parent_id.has_value()) {
			hs[category.parent_id.value()].emplace_back(category.id);
		}
	}
	QVector<models::Category> res;
	for(const auto& [parent_id, children] : hs) {
		models::Category category;
		category.id = parent_id;
		category.name = std::ranges::find(categories, parent_id, &CategoryDB::id)->name;
		for(const auto& cat : categories) {
			if (cat.parent_id == parent_id) {
				category.subcategories.emplace_back(cat.id, cat.name);
			}
		}
		res.emplace_back(category);
	}
	return res;
}

std::expected<size_t, QString> Storage::create_category(models::db::Category category) {
	QSqlQuery query(conn);
	query.prepare("INSERT INTO Categories (name, parent_id) VALUES (:name, :parent_id) RETURNING id");
	query.bindValue(":name", QVariant::fromValue(category.name));
	if (category.parent_id.has_value()) {
		query.bindValue(QString(":parent_id"), QVariant::fromValue(category.parent_id.value()));
	} else {
		query.bindValue(QString(":parent_id"), QVariant());
	}
	if (!query.exec()) {
		return std::unexpected{query.lastError().text()};
	}
	query.next();
	return query.value(0).toInt();
}

std::expected<void, QString> Storage::remove_category(size_t id) {
	QSqlQuery query(conn);
	query.prepare("DELETE FROM Categories WHERE parent_id = :id");
	query.bindValue(QString(":id"), QVariant::fromValue(id));
	if (!query.exec()) {
		return std::unexpected{query.lastError().text()};
	}
	query.prepare("DELETE FROM Categories WHERE id = :id");
	query.bindValue(QString(":id"), QVariant::fromValue(id));
	if (!query.exec()) {
		return std::unexpected{query.lastError().text()};
	}
	return {};
}
}
