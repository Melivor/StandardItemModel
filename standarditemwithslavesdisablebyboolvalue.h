#ifndef STANDARDITEMWITHSLAVESDISABLEBYBOOLVALUE_H
#define STANDARDITEMWITHSLAVESDISABLEBYBOOLVALUE_H
#include "standarditemwithslaves.h"

class StandardItemWithSlavesDisableByBoolValue : public StandardItemWithSlaves
{
public:
   StandardItemWithSlavesDisableByBoolValue(const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", QList<QStandardItem*> slaves={},int complexity=0, const QString& description="",const QString& unit="", const QVariant& min=QVariant(),const QVariant& max=QVariant(), const QVariantList& choiceList=QVariantList()):StandardItemWithSlaves(name, value,qmlFile,slaves, complexity,description,unit,min,max,choiceList){updateSlave();}
    //virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
   virtual void updateSlave(int role=Qt::DisplayRole) override;
};

#endif // STANDARDITEMWITHSLAVESDISABLEBYBOOLVALUE_H
