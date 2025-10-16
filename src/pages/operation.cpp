#include <QLabel>
#include "operation.hpp"

namespace pages
{
OperationPage::OperationPage(QWidget* parent) : QWidget(parent) {
	new QLabel(QString("IOEPARA"), this);
}
}

