#ifndef TESTMODEL_H
#define TESTMODEL_H
#include "QAbstractTableModel"

class TestModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TestModel(QObject *parent = 0);

   void populateData(const QList<QString> &Name,const QList<QString> &bench,const QList<QString> &Cmax,const QList<QString> &time);
   void Add_row(QString Name,QString bench,QString Cmax,QString time);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    void Clear();
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> tm_methode_name;
    QList<QString> tm_benchmark;
    QList<QString> tm_Cmax;
    QList<QString> tm_Time;

};


#endif // TESTMODEL_H
