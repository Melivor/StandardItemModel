#include "standarditemwithslavesdisablebyboolvalue.h"
#include <QDebug>
#include "standarditemmodel.h"
//StandardItemWithSlavesDisableByBoolValue::StandardItemWithSlavesDisableByBoolValue()
//{

//}


void StandardItemWithSlavesDisableByBoolValue::updateSlave(int role)
{
    // QStandardItem::setData(value, role);

    if(role==Qt::DisplayRole){
        QVariant value=data(Qt::DisplayRole);
        for(auto slave:m_slaves){
            slave->setEnabled(value.toBool());
            slave->setData(int(slave->flags()),StandardItemModel::FlagRole);
            if(slave->model()){
                emit slave->model()->dataChanged(slave->index(),slave->index(),{StandardItemModel::FlagRole});
            }
            else{
                qWarning()<<"You must add slave in a model";
            }
        }
    }


}
