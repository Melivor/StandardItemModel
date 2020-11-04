#ifndef STANDARDITEMWITHSLAVES_H
#define STANDARDITEMWITHSLAVES_H
#include "standarditem.h"
#include <QDebug>
class StandardItemWithSlaves : public StandardItem
{
public:
    StandardItemWithSlaves(const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="",  QList<QStandardItem*> slaves={}, int complexity=0, const QString& description="", const QString& unit="", const QVariant& min=QVariant(),const QVariant& max=QVariant(), const QVariantList& choiceList=QVariantList()):StandardItem(name, value, qmlFile, complexity,description,unit, min,max,choiceList){m_slaves.append(slaves);}
    inline void appendSlave(QStandardItem* slave){m_slaves.append(slave);updateSlave();}
    inline void appendSlaves(QList<QStandardItem*> slave){m_slaves.append(slave);updateSlave();}
    virtual void updateSlave(int role=Qt::DisplayRole);
     virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
    //inline void addSlave(std::function<void(const QVariant& value, int role)> action, StandardItem* slave){m_slaves.append(slave); m_actions.append(action);}
protected:
    QList<QStandardItem*> m_slaves;

};

#endif // STANDARDITEMWITHSLAVES_H
