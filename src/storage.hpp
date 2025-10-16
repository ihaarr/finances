#pragma once

#include <expected>

#include <QSqlDatabase>

class QSqlError;
namespace models
{
struct Category;
}

namespace storage
{
class Storage {
public:
	Storage() = default;
	Storage(const QSqlDatabase& db);
	static std::expected<Storage, QSqlError> connect();

	std::expected<QVector<models::Category>, QSqlError> get_categories();
private:
	QSqlDatabase conn;
};
}
