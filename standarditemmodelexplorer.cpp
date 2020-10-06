#include "standarditemmodelexplorer.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
StandardItemModelExplorer::StandardItemModelExplorer(StandardItemModel *prototype, QObject* parent):QAbstractListModel(parent),m_rootPath(prototype->root()),m_prototype(prototype)
{
    m_prototype->setParent(this);
    m_activeModel=new StandardItemModel(*m_prototype);
    m_currentModel=new StandardItemModel(*m_prototype);

    getModelList();
    if(m_modelNames.size()==0){
        m_activeModel->saveAsXml();
        getModelList();
    }


}

void StandardItemModelExplorer::setActiveSelection(int row, bool save)
{
    if(row<0 || row>=rowCount() || row==m_activeSelection || m_activeModel==nullptr)
    {
        return;
    }
    m_activeSelection=row;
    if(save){
        m_activeModel->saveAsXml();
    }

    m_activeModel->setName(m_modelNames[m_activeSelection]);
    m_activeModel->loadXml();
    emit activeSelectionChanged();
}

void StandardItemModelExplorer::setCurrentSelection(int row, bool save)
{
    if(row<0 || row>=rowCount() || row==m_currentSelection || m_currentModel==nullptr)
    {
        return;
    }
    m_currentSelection=row;
    if(save){
        m_currentModel->saveAsXml();
    }

    m_currentModel->setName(m_modelNames[m_currentSelection]);
    m_currentModel->loadXml();
    emit currentSelectionChanged();
}

void StandardItemModelExplorer::setActiveSelection(const QString& name)
{
    int index=m_modelNames.indexOf(name);
    if(index>=0){
        setActiveSelection(index);
    }
}


void StandardItemModelExplorer::setCurrentSelection(const QString& name)
{
    int index=m_modelNames.indexOf(name);
    if(index>=0){
        setCurrentSelection(index);
    }
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
    setCurrentSelection(0, false);
    setActiveSelection(0, false);
}

void StandardItemModelExplorer::addNew(const QString& name, bool setCurrentSelection, bool setActiveSelection)
{
    if(m_prototype==nullptr){
        qWarning()<<"You need to set a prototype first";
        return;
    }

    StandardItemModel newModel=StandardItemModel(*m_prototype);

    if(name!=""){
        newModel.setName(name);
    }

    newModel.setRoot(m_rootPath);
    newModel.saveAsXml();
    getModelList();
    int row=m_modelNames.indexOf(newModel.name());
    if(setCurrentSelection){
       StandardItemModelExplorer::setCurrentSelection(row);
    }
    if(setActiveSelection){
        StandardItemModelExplorer::setActiveSelection(row);
    }
    return;
}

void StandardItemModelExplorer::deleteCurrentSelection()
{
      QFile::remove(m_currentModel->findPath());
      m_currentSelection=-1;
      m_activeSelection=-1;
      getModelList();
}
