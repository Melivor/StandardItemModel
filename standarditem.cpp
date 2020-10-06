#include "standarditem.h"
#include "standarditemmodel.h"
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
