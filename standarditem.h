#ifndef STANDARDITEM_H
#define STANDARDITEM_H
#include <QStandardItem>
//#include "standarditemmodel.h"
class Linker;
//class StandardItemModel;
class StandardItem : public QStandardItem
{
    //Q_OBJECT
public:
    StandardItem(const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", int complexity=0, const QString& description="", const QString& unit="",const QVariant& min=QVariant(),const QVariant& max=QVariant(), const QVariantList& choiceList=QVariantList());
   // StandardItem(const QStandardItem& other);
    //virtual QVariant data(int role = Qt::UserRole + 1) const override {return QStandardItem::data(role);}
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
    virtual void setEnabled(bool enable);

private:
    //QList<Linker*> m_linkers;
  //  void dataChanged(const QVariant &value, int role);
    friend class StandardItemModel;
};

#endif // STANDARDITEM_H
