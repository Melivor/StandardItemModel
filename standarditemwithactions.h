#ifndef STANDARDITEMWITHACTIONS_H
#define STANDARDITEMWITHACTIONS_H
#include "standarditem.h"

class StandardItemWithActions: public StandardItem
{
public:
    StandardItemWithActions(const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", int complexity=0, const QString& description="", const QString& unit="", const QVariant& min=QVariant(),const QVariant& max=QVariant(), const QVariantList& choiceList=QVariantList()):StandardItem(name, value, qmlFile, complexity,description,unit, min,max,choiceList){};
    inline void addAction(std::function<void(const QVariant& value, int role)> action){m_actions.push_back(action);}
    void performActions(const QVariant &value, int role);
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
private :
    QList<std::function<void(const QVariant& value, int role)>> m_actions;
};

#endif // STANDARDITEMWITHACTIONS_H
