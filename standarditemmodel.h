#ifndef STANDARDITEMMODEL_H
#define STANDARDITEMMODEL_H
#include <QStandardItemModel>
#include <QSettings>
#include <QCoreApplication>
#include <standarditem.h>
class StandardItemModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(QList<StandardItemModel*> sections READ sections CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString root READ root CONSTANT)

public:
    StandardItemModel(const QString& name="", const QString& root="", QObject* parent=nullptr);
    //StandardItemModel(const StandardItemModel& other);
    enum Roles {
        QmlFileRole = Qt::UserRole + 500,
        NameRole,
        MaxRole,
        MinRole,
        UnitRole,
        IconRole,
        DescriptionRole,
        FlagRole,
        ChoiceListRole,
        ComplexityRole,
        ErrorRole,
        ErrorStringRole
    };
    QList<StandardItemModel*> sections(){return m_sections;};
    virtual QHash<int,QByteArray> roleNames() const override;
    void save(QSettings& settings);
    Q_INVOKABLE void saveAsXml(const QString& path="");
    void load(QSettings& settings);
    void loadXml(const QString& path="");

   // void setSection(const QString& name, int start, int end);
    void appendRow(StandardItem *item);
    void appendRows(QList<StandardItem *> items);
    inline void appendSection(StandardItemModel* section){m_sections.append(section);section->setParent(this);}
    inline void setName(const QString& name){m_name=name;emit nameChanged();}
    inline void setRoot(const QString& root){m_root=root;}
    inline const QString& name(){return m_name;emit nameChanged();}
    inline const QString& root(){return m_root;}
    QString findPath() const;
    static QString findPath(const QString& name, const QString& root);
private :
    QList<StandardItemModel*> m_sections;
    QString m_name;

    QString m_root;


signals:
    void nameChanged();
};

#endif // STANDARDITEMMODEL_H
