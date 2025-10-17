#pragma once

#include <expected>
#include <QSqlDatabase>

namespace models
{
struct Category;
namespace db
{
struct Category;
}
}

namespace storage
{
class Storage {
public:
	Storage() = default;
	Storage(const QSqlDatabase& db);
	static std::expected<Storage, QString> connect();

	std::expected<QVector<models::Category>, QString> get_categories();
	std::expected<size_t, QString> create_category(models::db::Category);
	std::expected<void, QString> remove_category(size_t id);
private:
	QSqlDatabase conn;
};
}
