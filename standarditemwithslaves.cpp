#include "standarditemwithslaves.h"


void StandardItemWithSlaves::setData(const QVariant &value, int role)
{

    QStandardItem::setData(value, role);
    updateSlave(role);

}


void StandardItemWithSlaves::updateSlave(int role)
{

}
