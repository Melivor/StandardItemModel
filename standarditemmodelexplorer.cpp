#include "standarditemmodelexplorer.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
StandardItemModelExplorer::StandardItemModelExplorer(StandardItemModel *prototype, QObject* parent):QAbstractListModel(parent),m_rootPath(prototype->root()),m_prototype(prototype)
{
    m_prototype->setParent(this);
    getModelList();
    if(m_modelNames.size()==0){
        m_prototype->saveAsXml();
        getModelList();
    }
    m_filteredModel=new QSortFilterProxyModel(this);
    m_filteredModel->setSourceModel(this);

    for(int row=0; row<m_prototype->rowCount(); ++row){
        QString str=m_prototype->data(m_prototype->index(row, 0),StandardItemModel::NameRole).toString();
        m_dataRow[str]=row;
        m_dataSection[str]=-1;
    }



    int sectionNumber=0;
    for(auto&& section:m_prototype->sections()){
        for(int row=0; row<section->rowCount(); ++row){
            QString str=section->data(section->index(row, 0),StandardItemModel::NameRole).toString();
            m_dataRow[str]=row;
            m_dataSection[str]=sectionNumber;
        }
        ++sectionNumber;
    }



}

void StandardItemModelExplorer::setActiveSelection(int row, bool save)
{
    if(row<0 || row>=rowCount() || row==m_activeSelection || m_prototype==nullptr)
    {
        return;
    }

    if(save){
        m_prototype->saveAsXml();
    }
    m_activeSelection=row;
    //setEffectiveSelection(row);
    m_prototype->setName(m_modelNames[m_activeSelection]);
    m_prototype->loadXml();
    emit activeSelectionChanged();
}

//void StandardItemModelExplorer::setCurrentSelection(int row, bool save)
//{
//    if(row<0 || row>=rowCount() || row==m_currentSelection || m_prototype==nullptr)
//    {
//        return;
//    }

//    if(save){
//        m_prototype->saveAsXml();
//    }
//    m_currentSelection=row;
//    setEffectiveSelection(row);
//    qDebug()<<"Current selection is "<<row<<m_modelNames[row]<<"effective selection is: "<<m_effectiveSelection;
//    emit currentSelectionChanged();
//    //emit activeSelectionChanged();
//}

//void StandardItemModelExplorer::setEffectiveSelection(int row)
//{
//    if(m_effectiveSelection==row){
//        return;
//    }
//    m_effectiveSelection=row;
//    qDebug()<<"Active selection is "<<row<<m_modelNames[row]<<"effective selection is: "<<m_effectiveSelection;
//    m_prototype->setName(m_modelNames[m_effectiveSelection]);
//    m_prototype->loadXml();
//    qDebug()<<"Effectice selection is "<<row<<m_modelNames[row];
//}
void StandardItemModelExplorer::setActiveSelection(const QString& name)
{
    int index=m_modelNames.indexOf(name);
    if(index>=0){
        setActiveSelection(index);
    }
}

void StandardItemModelExplorer::copyProfilSettings(const QString &name)
{

    QString pname=m_prototype->name();
    setActiveSelection(name);
    m_prototype->setName(pname);

}

void StandardItemModelExplorer::deleteProfilSettings(const QString &name)
{

    if(QFile::remove(getProfilSettingsPath()+"/"+name+".xml")){
        beginResetModel();
        m_modelNames.removeAll(name);
        endResetModel();
    }


   // m_prototype->deleteXml(name);
}

QString StandardItemModelExplorer::getProfilSettingsPath()
{
     return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/"+m_rootPath;

}
void StandardItemModelExplorer::getModelList()
{

    beginResetModel();
    m_modelNames.clear();
    QString path=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    dir.cd(m_rootPath);
    auto list=dir.entryList({"*.xml"});
    for(auto modelPath:list){
        modelPath.chop(4);
        m_modelNames.append(modelPath);
    }
    endResetModel();
    if(m_modelNames.empty()){
        unselect();
        return;
    }
    //setCurrentSelection(0, false);
    setActiveSelection(0, false);
}

void StandardItemModelExplorer::addNew(const QString& name, bool setActiveSelection)
{
    if(m_prototype==nullptr){
        qWarning()<<"You need to set a prototype first";
        return;
    }
    unselect();
    m_prototype->setName(name);
    m_prototype->saveAsXml();
    getModelList();
    int row=m_modelNames.indexOf(name);
    //    if(setCurrentSelection){
    //       StandardItemModelExplorer::setCurrentSelection(row);
    //    }
    if(setActiveSelection){
        StandardItemModelExplorer::setActiveSelection(row);
    }
    return;
}

//void StandardItemModelExplorer::deleteCurrentSelection()
//{
//      QFile::remove(StandardItemModel::findPath(m_prototype->root(), m_modelNames[m_currentSelection]));
//      deleteModel(m_modelNames[m_currentSelection]);
//}

void StandardItemModelExplorer::deleteModel(const QString &name)
{
    QFile::remove(StandardItemModel::findPath(m_prototype->root(), name));
    unselect();
    getModelList();
}
QVariant StandardItemModelExplorer::getActiveData(int row, int column, int section, int role) const
{
    return getData(activeModel(), row, column, section, role);
}

//QVariant StandardItemModelExplorer::getCurrentData(int row, int column, int section, int role) const
//{
//    return getData(currentModel(), row, column, section, role);
//}
QVariant StandardItemModelExplorer::getData(StandardItemModel *model, int row, int column, int section, int role) const
{
    if(model==nullptr){
        return QVariant();
    }
    if(section>-1){
        if(section<model->sections().size()){
            auto&& subModel=model->sections()[section];
            if(row>=0 && row<subModel->rowCount() && column>=0 && column<subModel->columnCount()){
                return subModel->data(subModel->index(row, column),role);
            }
        }
        return QVariant();
    }
    if(row>=0 && row<model->rowCount() && column>=0 && column<model->columnCount()){
        return model->data(model->index(row, column),role);
    }
    return QVariant();
}

void StandardItemModelExplorer::setActiveData(const QVariant &value, int row, int column, int section, int role)
{
    auto model=activeModel();
    if(model==nullptr){
        return;
    }
    if(section>-1){
        if(section<model->sections().size()){
            auto&& subModel=model->sections().at(section);
            if(row>=0 && row<subModel->rowCount() && column>=0 && column<subModel->columnCount()){
                subModel->setData(subModel->index(row, column),value,role);
            }
        }
        return;
    }
    if(row>=0 && row<model->rowCount() && column>=0 && column<model->columnCount()){
        model->setData(model->index(row, column),value, role);
    }
    return;
}

StandardItem* StandardItemModelExplorer::item(int row, int column, int section)
{
    auto model=activeModel();
    if(model==nullptr){
        return nullptr;
    }
    if(section>-1){
        if(section<model->sections().size()){
            auto&& subModel=model->sections().at(section);
            if(row>=0 && row<subModel->rowCount() && column>=0 && column<subModel->columnCount()){
                return reinterpret_cast<StandardItem*>(subModel->item(row, column));
            }
        }
        return nullptr;
    }
    if(row>=0 && row<model->rowCount() && column>=0 && column<model->columnCount()){
        return reinterpret_cast<StandardItem*>(model->item(row, column));
    }
    return nullptr;
}

StandardItemModel* StandardItemModelExplorer::activeModel() const
{
    //    if(m_activeSelection<0||m_activeSelection>=m_modelNames.size()){
    //        return nullptr;
    //    }
    //    if(m_activeSelection!=m_effectiveSelection){
    //        m_prototype->setName(m_modelNames[m_activeSelection]);
    //        m_prototype->loadXml();
    //        m_effectiveSelection=m_activeSelection;
    //    }
    return m_prototype;
}

//StandardItemModel* StandardItemModelExplorer::currentModel() const
//{
//    qDebug()<<"Getting current model";
//    if(m_currentSelection<0||m_currentSelection>=m_modelNames.size()){
//        return nullptr;
//    }
//    if(m_currentSelection!=m_effectiveSelection){
//        m_prototype->setName(m_modelNames[m_currentSelection]);
//        m_prototype->loadXml();
//        m_effectiveSelection=m_currentSelection;
//    }
//    return m_prototype;
//}

void StandardItemModelExplorer::unselect()
{
    m_activeSelection=-1;
    //m_currentSelection=-1;
    // m_effectiveSelection=-1;
}

void StandardItemModelExplorer::loadSavedModelIndex()
{
    setActiveSelection(m_savedIndex);
}

void StandardItemModelExplorer::saveActiveModelIndex()
{
    m_savedIndex=m_activeSelection;
}

void StandardItemModelExplorer::switchSavedAndActiveModelIndex()
{
    int index=m_activeSelection;
    loadSavedModelIndex();
    m_savedIndex=index;
}

void StandardItemModelExplorer::setFilter(const QString &filter)
{
    //qDebug()<<"Setting filter";
    m_filteredModel->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive, QRegExp::FixedString));}
