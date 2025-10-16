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
std::expected<Storage, QSqlError> Storage::connect() {
	const QString INIT_TABLE_CATEGORIES = QStringLiteral(
	"CREATE TABLE IF NOT EXISTS Categories ("
	"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
	"name TEXT NOT NULL,"
	"arent_id INTEGER,"
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
	db.setDatabaseName("/Users/ihaarr/test.db");
	if (!db.open()) {
		return std::unexpected{db.lastError()};	
	}

	QSqlQuery query(db);
	if (!query.exec("PRAGMA foreign_keys = ON")) {
		return std::unexpected{query.lastError()};	
	}
	if (!query.exec(INIT_TABLE_CATEGORIES)) {
		return std::unexpected{query.lastError()};	
	}	
	if (!query.exec(INIT_TABLE_OPERATIONS)) {
		return std::unexpected{query.lastError()};
	}
	
	return Storage{db}; 
}

Storage::Storage(const QSqlDatabase& db) : conn(db) {

}

std::expected<QVector<models::Category>, QSqlError> Storage::get_categories() {
	QSqlQuery query(conn);
	query.prepare("SELECT id, name, parent_id FROM Categories ORDER BY id");
	if (!query.exec()) {
		throw std::runtime_error(query.lastError().text().toStdString());
		return std::unexpected{query.lastError()};
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
}
