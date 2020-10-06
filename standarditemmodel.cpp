#include "standarditemmodel.h"
#include "qsettings-xml.hpp"
StandardItemModel::StandardItemModel(const QString &name, const QString &root, QObject *parent) : QStandardItemModel(parent), m_name(name)
{
    m_root=root;
}

StandardItemModel::StandardItemModel(const StandardItemModel& other) : m_name(other.m_name), m_root(other.m_root), m_sections(other.m_sections)
{
    setParent(other.parent());
    for(int row=0; row<other.rowCount(); ++row){
        for(int column=0; column<other.columnCount(); ++column){
              QStandardItemModel::appendRow(other.item(row, column));
        }
    }
    for(int row=0; row<other.rowCount(); ++row){
        setHeaderData(row, Qt::Vertical,other.headerData(row, Qt::Vertical));
    }
    for(int column=0; column<other.columnCount(); ++column){
        setHeaderData(column, Qt::Horizontal,other.headerData(column, Qt::Horizontal));
    }
}

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
    return roleNames;
}

void StandardItemModel::appendRow(StandardItem *item)
{
    QStandardItemModel::appendRow(item);
    setHeaderData(rowCount()-1,Qt::Vertical,item->data(NameRole));
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

    for(auto section:m_sections){
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
    for(auto section:m_sections){
        section->load(settings);
    }
    settings.endGroup();

    endResetModel();
}
