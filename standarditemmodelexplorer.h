#ifndef STANDARDITEMMODELEXPLORER_H
#define STANDARDITEMMODELEXPLORER_H
#include "standarditemmodel.h"
#include "QDebug"
class StandardItemModelExplorer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(StandardItemModel* activeModel READ activeModel CONSTANT )
   // Q_PROPERTY(StandardItemModel* currentModel READ currentModel CONSTANT)
    //Q_PROPERTY(int currentSelection READ currentSelection WRITE setCurrentSelection NOTIFY currentSelectionChanged)
    Q_PROPERTY(int activeSelection READ activeSelection WRITE setActiveSelection NOTIFY activeSelectionChanged)
public:
    StandardItemModelExplorer(StandardItemModel* prototype, QObject* parent=nullptr);
    ~StandardItemModelExplorer(){if(m_prototype)m_prototype->saveAsXml();}
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent);return m_modelNames.size();}
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {Q_UNUSED(role); return m_modelNames[index.row()];}
    //Q_INVOKABLE void setCurrentSelection(int row, bool save=true);
    Q_INVOKABLE void setActiveSelection(int row, bool save=true);
    //Q_INVOKABLE void setCurrentSelection(const QString& name);
    Q_INVOKABLE void setActiveSelection(const QString& name);
    Q_INVOKABLE void copyProfilSettings(const QString& name);
    Q_INVOKABLE void saveActiveModelIndex();
    Q_INVOKABLE void loadSavedModelIndex();
    Q_INVOKABLE void switchSavedAndActiveModelIndex();
    Q_INVOKABLE QString path(){return m_prototype->findPath();}

   // Q_INVOKABLE QString defaultName(){return m_defaultName;}
    //Q_INVOKABLE void deleteCurrentSelection();
    void getModelList();
    Q_INVOKABLE void addNew(const QString& name="", bool setActiveSelection=false);
    //StandardItemModel* currentModel() const ;
    StandardItemModel* activeModel() const ;
    //int currentSelection(){return m_currentSelection;}
    int activeSelection(){return m_activeSelection;}
    //QVariant  getCurrentData(int row, int column=0, int section=-1, int role=Qt::DisplayRole) const;
    void setActiveData(const QVariant& value, int row, int column=0, int section=-1, int role=Qt::DisplayRole);
    void setActiveData(const QString& dataName, const QVariant& value){if(m_dataRow.count(dataName)==0||m_dataSection.count(dataName)==0){qWarning()<<"StandardItemModelExplorer: "<<dataName<<" No such data";return;} else {setActiveData(value,m_dataRow.at(dataName),0,m_dataSection.at(dataName));}}
    QVariant  getActiveData(int row, int column=0, int section=-1, int role=Qt::DisplayRole) const;
    QVariant getActiveData(const QString& dataName){ if(m_dataRow.count(dataName)==0||m_dataSection.count(dataName)==0){qWarning()<<"StandardItemModelExplorer: "<<dataName<<" No such data";return QVariant();} else{ return getActiveData(m_dataRow.at(dataName), 0, m_dataSection.at(dataName));} }
private :
    std::map<QString, int> m_dataSection;
    std::map<QString, int> m_dataRow;
    void setEffectiveSelection(int row);
    void deleteModel(const QString& name);
    QVariant getData(StandardItemModel* model, int row, int column, int section, int role) const;
    void unselect();
    QString m_rootPath;
    QString m_defaultName;
    QStringList m_modelNames;
    int m_activeSelection=-1;
    int m_savedIndex=0;
    StandardItemModel* m_prototype=nullptr;
signals:
    void activeSelectionChanged();
};

#endif // STANDARDITEMMODELEXPLORER_H
