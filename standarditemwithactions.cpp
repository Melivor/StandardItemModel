#include "standarditemwithactions.h"
#include <QDebug>

void StandardItemWithActions::setData(const QVariant &value, int role)
{

   StandardItem::setData(value, role);
   performActions(value, role);

}

void StandardItemWithActions::performActions(const QVariant &value, int role)
{
    for(auto&& action:m_actions){
        action(value, role);
    }
}
