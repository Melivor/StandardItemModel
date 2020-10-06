#ifndef STANDARDITEM_H
#define STANDARDITEM_H
#include <QStandardItem>

class StandardItem : public QStandardItem
{
public:
    StandardItem(const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", int complexity=0, const QString& description="", const QString& unit="",const QVariant& min=QVariant(),const QVariant& max=QVariant(), const QVariantList& choiceList=QVariantList());
    virtual QVariant data(int role = Qt::UserRole + 1) const{return QStandardItem::data(role);}
};

#endif // STANDARDITEM_H
