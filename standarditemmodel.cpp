#include "standarditemmodel.h"
#include "qsettings-xml.hpp"
StandardItemModel::StandardItemModel(const QString &name, const QString &root, QObject *parent) : QStandardItemModel(parent), m_name(name)
{
    m_root=root;
}

//StandardItemModel::StandardItemModel(const StandardItemModel& other) : m_name(other.m_name), m_root(other.m_root)
//{
//    setParent(other.parent());
//    for(int row=0; row<other.rowCount(); ++row){
//        for(int column=0; column<other.columnCount(); ++column){
//              QStandardItemModel::appendRow(new StandardItem(*other.item(row, column)));
//        }
//    }
//    for(int row=0; row<other.rowCount(); ++row){
//        setHeaderData(row, Qt::Vertical,other.headerData(row, Qt::Vertical));
//    }
//    for(int column=0; column<other.columnCount(); ++column){
//        setHeaderData(column, Qt::Horizontal,other.headerData(column, Qt::Horizontal));
//    }
//    for(auto section:other.m_sections){
//        m_sections.push_back(new StandardItemModel(*section));
//    }
//}

QHash<int, QByteArray> StandardItemModel::roleNames() const
{
    QHash<int, QByteArray> roleNames = QStandardItemModel::roleNames();
    roleNames.insert(Qt::DisplayRole, "display");
    roleNames.insert(NameRole, "name");
    roleNames.insert(QmlFileRole, "qmlFile");
    roleNames.insert(MaxRole, "max");
    roleNames.insert(MinRole, "min");
    roleNames.insert(UnitRole, "unit");
    roleNames.insert(IconRole, "icon");
    roleNames.insert(DescriptionRole, "description");
    roleNames.insert(FlagRole, "flags");
    roleNames.insert(ChoiceListRole, "choiceList");
    roleNames.insert(ComplexityRole, "complexity");
    roleNames.insert(ErrorRole, "error");
    roleNames.insert(ErrorStringRole, "errorString");
    return roleNames;
}

void StandardItemModel::appendRow(StandardItem *item)
{
    QStandardItemModel::appendRow(item);
    setHeaderData(rowCount()-1,Qt::Vertical,item->data(NameRole));
}

void StandardItemModel::appendRows(QList<StandardItem*> items)
{
    for(auto item:items){
        appendRow(item);
    }
}

void StandardItemModel::save(QSettings &settings)
{
    if(m_name==""){
        settings.beginGroup("Settings");
    }
    else{
        settings.beginGroup(clean(m_name));

    }


    for(int column=0; column<columnCount();++column){
        auto str=clean(headerData(column, Qt::Horizontal).toString());
        if(str!=""){
            settings.beginGroup(clean(str));
        }
        else if(columnCount()>1){
            str="Column"+QString::number(column);settings.beginGroup(str);
        }
        for(int row=0; row<rowCount();++row){
            auto header=clean(headerData(row, Qt::Vertical).toString());
            if(header==""){
                header="Row"+QString::number(row);
            }
            settings.setValue(header,data(index(row,column)));

        }
        if(str!=""){
            settings.endGroup();
        }
    }

    for(auto section:qAsConst(m_sections)){
        section->save(settings);
    }

    settings.endGroup();
}

QString StandardItemModel::findPath() const
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    if(m_root!=""){
        dir.mkdir(m_root);
        dir.cd(m_root);
    }
    return dir.path()+"/"+m_name+".xml";
}

QString StandardItemModel::findPathMeta() const
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    if(m_root!=""){
        dir.mkdir(m_root);
        dir.cd(m_root);
    }
    dir.mkdir("Meta");
    dir.cd("Meta");
    return dir.path()+"/"+m_name+"-meta.xml";
}
QString StandardItemModel::findPath(const QString &name, const QString &root)
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    if(root!=""){
        dir.mkdir(root);
        dir.cd(root);
    }
    return dir.path()+"/"+name+".xml";
}
void StandardItemModel::saveAsXml(const QString &path)
{
    if(path==""){
        QSettings settings(findPath(),xmlFormat);
        save(settings);
    }
    else{
        QSettings settings(path,xmlFormat);
        save(settings);
    }
}


void StandardItemModel::saveMetaInfo(const QString &path)
{
    if(path==""){
        QSettings settings(findPathMeta(),xmlFormat);
        qDebug()<<"Saving metainfo: "<<findPathMeta();
        saveMetaInfo(settings);
    }
    else{
        QSettings settings(path,xmlFormat);
        saveMetaInfo(settings);
    }
}

void StandardItemModel::saveMetaInfo(QSettings &settings)
{
    if(m_name==""){
        settings.beginGroup("Settings");
    }
    else{
        settings.beginGroup(clean(m_name));

    }


    for(int column=0; column<columnCount();++column){
        auto str=clean(headerData(column, Qt::Horizontal).toString());
        if(str!=""){
            settings.beginGroup(clean(str));
        }
        else if(columnCount()>1){
            str="Column"+QString::number(column);settings.beginGroup(str);
        }
        for(int row=0; row<rowCount();++row){
            auto header=clean(headerData(row, Qt::Vertical).toString());
            if(header==""){
                header="Row"+QString::number(row);
            }
            settings.beginGroup(header);

            QHash<int, QByteArray> roleNames = this->roleNames();
            for(auto i=roleNames.begin(); i!=roleNames.end(); ++i){
                if(i->data()){
                    settings.setValue(i->data(),data(index(row,column),i.key()));
                }

            }
            settings.endGroup();

        }
        if(str!=""){
            settings.endGroup();
        }
    }

    for(auto section:qAsConst(m_sections)){
        section->saveMetaInfo(settings);
    }

    settings.endGroup();
}

void StandardItemModel::loadXml(const QString &path)
{

    if(path==""){
        QSettings settings(findPath(),xmlFormat);
        load(settings);
    }
    else{
        QSettings settings(path,xmlFormat);
        load(settings);
    }
}

void StandardItemModel::load(QSettings &settings)
{
    beginResetModel();
    if(m_name==""){
        settings.beginGroup("Settings");
    }
    else{
        settings.beginGroup(clean(m_name));
    }
    for(int column=0; column<columnCount();++column){
        auto str=clean(headerData(column, Qt::Horizontal).toString());
        if(str!=""){
            settings.beginGroup(clean(str));
        }

        for(int row=0; row<rowCount();++row){
            setData(index(row, column), settings.value(clean(headerData(row, Qt::Vertical).toString())),Qt::DisplayRole);

        }
        if(str!=""){
            settings.endGroup();
        }

    }
    for(auto section:qAsConst(m_sections)){
        section->load(settings);
    }
    settings.endGroup();

    endResetModel();
}

void StandardItemModel::loadMetaInfo(const QString &path)
{

    if(path==""){
        QSettings settings(findPathMeta(),xmlFormat);
        loadMetaInfo(settings);
    }
    else{
        QSettings settings(path,xmlFormat);
        loadMetaInfo(settings);
    }
}

void StandardItemModel::loadMetaInfo(QSettings &settings)
{
    beginResetModel();
    if(m_name==""){
        settings.beginGroup("Settings");
    }
    else{
        settings.beginGroup(clean(m_name));
    }
    for(int column=0; column<columnCount();++column){
//        auto str=clean(headerData(column, Qt::Horizontal).toString());
//        if(str!=""){
//            settings.beginGroup(clean(str));
//        }

//        for(int row=0; row<rowCount();++row){
//            auto header=clean(headerData(row, Qt::Vertical).toString());
//            settings.beginGroup(header);

//            QHash<int, QByteArray> roleNames = this->roleNames();
//            //for(auto i=roleNames.begin(); i!=roleNames.end(); ++i){
////                if(i->data()){
////                    settings.setValue(i->data(),data(index(row,column),i.key()));
////                }
//                //appendRow(sett, 0.01,"BoundNumberField.qml",0,tr("Height of each layer to print"),"mm",0.005,0.5,{}));

//            //}
//            settings.endGroup();
//        }
//        if(str!=""){
//            settings.endGroup();
//        }

    }
    for(auto section:qAsConst(m_sections)){
        section->loadMetaInfo(settings);
    }
    settings.endGroup();

    endResetModel();
}
