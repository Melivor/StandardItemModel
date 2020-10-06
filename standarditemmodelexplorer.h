#ifndef STANDARDITEMMODELEXPLORER_H
#define STANDARDITEMMODELEXPLORER_H
#include "standarditemmodel.h"

class StandardItemModelExplorer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(StandardItemModel* activeModel READ activeModel CONSTANT)
    Q_PROPERTY(StandardItemModel* currentModel READ currentModel CONSTANT)
    Q_PROPERTY(int currentSelection READ currentSelection WRITE setCurrentSelection NOTIFY currentSelectionChanged)
    Q_PROPERTY(int activeSelection READ activeSelection WRITE setActiveSelection NOTIFY activeSelectionChanged)
public:
    StandardItemModelExplorer(StandardItemModel* prototype, QObject* parent=nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent);return m_modelNames.size();}
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {Q_UNUSED(role); return m_modelNames[index.row()];}
    Q_INVOKABLE void setCurrentSelection(int row, bool save=true);
    Q_INVOKABLE void setActiveSelection(int row, bool save=true);
    Q_INVOKABLE void setCurrentSelection(const QString& name);
    Q_INVOKABLE void setActiveSelection(const QString& name);
    Q_INVOKABLE void deleteCurrentSelection();
    void getModelList();
    Q_INVOKABLE void addNew(const QString& name="", bool setCurrentSelection=true, bool setActiveSelection=false);
    StandardItemModel* currentModel(){return m_currentModel;}
    StandardItemModel* activeModel(){return m_activeModel;}
    int currentSelection(){return m_currentSelection;}
    int activeSelection(){return m_activeSelection;}
private :

    QString m_rootPath;
    QStringList m_modelNames;
    int m_activeSelection=-1;
    int m_currentSelection=-1;
    StandardItemModel* m_activeModel=nullptr;
    StandardItemModel* m_currentModel=nullptr;
    StandardItemModel* m_prototype=nullptr;
signals:
    void currentSelectionChanged();
    void activeSelectionChanged();
};

#endif // STANDARDITEMMODELEXPLORER_H
