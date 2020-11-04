#include "standarditem.h"
#include "standarditemmodel.h"
#include <QDebug>
StandardItem::StandardItem(const QString& name, const QVariant& value, const QString& qmlFile, int complexity, const QString &description, const QString& unit, const QVariant &min, const QVariant &max, const QVariantList &choiceList) : QStandardItem()
{
    setData(qmlFile,StandardItemModel::QmlFileRole);
    setData(value,Qt::DisplayRole);
    setData(complexity, StandardItemModel::ComplexityRole);
    setData(unit, StandardItemModel::UnitRole);
    setData(Qt::ItemIsEnabled, StandardItemModel::FlagRole);
    setData(name, StandardItemModel::NameRole);
    setData(int(flags()), StandardItemModel::FlagRole);
    setData(description,StandardItemModel::DescriptionRole);
    setData(min, StandardItemModel::MinRole);
    setData(max, StandardItemModel::MaxRole);
    setData(choiceList, StandardItemModel::ChoiceListRole);
}


void StandardItem::setData(const QVariant &value, int role)
{
    QStandardItem::setData(value, role);
}


void StandardItem::setEnabled(bool enable)
{
    QStandardItem::setEnabled(enable);
    setData(int(flags()), StandardItemModel::FlagRole);
    model()->dataChanged(index(),index(),{StandardItemModel::FlagRole});
}


//StandardItem::StandardItem(const QStandardItem &other)
//{
//    setFlags(other.flags());
//    setFont(other.font());
//    setIcon(other.icon());
//    setForeground(other.foreground());
//    setSizeHint(other.sizeHint());
//    setStatusTip(other.statusTip());
//    setText(other.text());
//    setTextAlignment(other.textAlignment());
//    setToolTip(other.toolTip());
//    setUserTristate(other.isUserTristate());
//    setWhatsThis(other.whatsThis());
//    for(int role=StandardItemModel::NameRole; role<=StandardItemModel::ComplexityRole; ++role){
//        setData(other.data(role),role);
//    }
//    setData(other.data(Qt::DisplayRole), Qt::DisplayRole);
//}
