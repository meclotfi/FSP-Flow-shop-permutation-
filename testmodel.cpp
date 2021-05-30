#include "testmodel.h"
#include "QAbstractTableModel"

TestModel::TestModel(QObject *parent) : QAbstractTableModel(parent)
{
}

// Create a method to populate the model with data:
void TestModel::populateData(const QList<QString> &Name,const QList<QString> &bench,const QList<QString> &Cmax,const QList<QString> &time)
{



    tm_methode_name.clear();
    tm_methode_name = Name;
    tm_benchmark.clear();
    tm_benchmark = bench;
    tm_Cmax.clear();
    tm_Cmax = Cmax;
    tm_Time.clear();
    tm_Time = time;
    return;
}
void TestModel::Add_row(QString Name,QString bench,QString Cmax,QString time)
{
tm_methode_name.emplaceBack(Name);
tm_benchmark.emplaceBack(bench);
tm_Cmax.emplaceBack(Cmax);
tm_Time.emplaceBack(time);
beginResetModel();
endResetModel();

return;
}
void TestModel::Clear()
{
tm_methode_name.clear();
tm_benchmark.clear();
tm_Cmax.clear();
tm_Time.clear();
beginResetModel();
endResetModel();

return;
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return tm_methode_name.length();
}

int TestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return index.row();
    }else if (index.column() == 1) {
        return tm_methode_name[index.row()];
    } else if (index.column() == 2) {
        return tm_benchmark[index.row()];
    }else if (index.column() == 3) {
        return tm_Cmax[index.row()];
    }else if (index.column() == 4) {
        return tm_Time[index.row()];
    }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Index");
        }
        else if (section == 1) {
            return QString("Methode");
        } else if (section == 2) {
            return QString("Benchmark");
        }else if (section == 3) {
            return QString("CMAX");
        }else if (section == 4) {
            return QString("Time(s)");
        }
    }
    return QVariant();
}
