#include <QLabel>
#include "operation.hpp"

namespace pages
{
OperationPage::OperationPage(QWidget* parent) : Page(parent) {
	new QLabel(QString("IOEPARA"), this);
}
}

