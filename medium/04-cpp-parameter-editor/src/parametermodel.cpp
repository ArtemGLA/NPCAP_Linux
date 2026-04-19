#include "parametermodel.h"

ParameterModel::ParameterModel(QObject *parent) : QAbstractTableModel(parent) {
    // Заполнение данных
    m_data = {
        {"Tom", 39},
        {"Bob", 43},
        {"Sam", 28}
    };
}

int ParameterModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_data.size();
}

int ParameterModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : 2;
}

QVariant ParameterModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    
    if (index.row() >= m_data.size() || index.column() >= 2)
        return QVariant();
    
    if (index.column() == 0)
        return m_data[index.row()].first;
    else if (index.column() == 1)
        return m_data[index.row()].second;
    
    return QVariant();
}

QVariant ParameterModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    
    return section == 0 ? "Name" : "Age";
}
