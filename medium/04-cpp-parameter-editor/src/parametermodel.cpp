#include "parametermodel.h"
#include <QFont>

ParameterModel::ParameterModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void ParameterModel::setParameters(const QList<Parameter>& params)
{
    beginResetModel();
    m_parameters = params;
    endResetModel();
}

void ParameterModel::setValues(const QMap<QString, QVariant>& values)
{
    m_values = values;
    
    // Обновляем все отображаемые значения
    if (!m_parameters.isEmpty()) {
        emit dataChanged(index(0, ValueColumn), 
                        index(rowCount() - 1, ValueColumn));
    }
}

QVariant ParameterModel::getValue(const QString& name) const
{
    return m_values.value(name);
}

QMap<QString, QVariant> ParameterModel::getAllValues() const
{
    return m_values;
}

const Parameter* ParameterModel::getParameter(int row) const
{
    if (row < 0 || row >= m_parameters.size()) {
        return nullptr;
    }
    return &m_parameters[row];
}

int ParameterModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_parameters.size();
}

int ParameterModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return ColumnCount;
}

QVariant ParameterModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_parameters.size()) {
        return QVariant();
    }
    
    const Parameter& param = m_parameters[index.row()];
    
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case NameColumn:
                return param.displayName.isEmpty() ? param.name : param.displayName;
            case ValueColumn:
                return param.formatValue(m_values.value(param.name, param.defaultValue));
            case DefaultColumn:
                return param.formatValue(param.defaultValue);
            case UnitsColumn:
                return param.units;
        }
    }
    else if (role == Qt::EditRole && index.column() == ValueColumn) {
        return m_values.value(param.name, param.defaultValue);
    }
    else if (role == Qt::UserRole) {
        return param.name;  // Скрытое хранение имени
    }
    else if (role == Qt::FontRole) {
        // Подсветка изменённых значений
        QVariant currentVal = m_values.value(param.name);
        if (currentVal.isValid() && currentVal != param.defaultValue) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    else if (role == Qt::ForegroundRole && index.column() == ValueColumn) {
        // Изменённые значения показываем синим
        QVariant currentVal = m_values.value(param.name);
        if (currentVal.isValid() && currentVal != param.defaultValue) {
            return QColor(Qt::blue);
        }
    }
    
    return QVariant();
}

bool ParameterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.column() != ValueColumn || role != Qt::EditRole) {
        return false;
    }
    
    const Parameter& param = m_parameters[index.row()];
    
    if (!param.validate(value)) {
        return false;
    }
    
    m_values[param.name] = value;
    emit dataChanged(index, index);
    emit parameterChanged(param.name, value);
    
    return true;
}

Qt::ItemFlags ParameterModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    
    if (index.column() == ValueColumn) {
        flags |= Qt::ItemIsEditable;
    }
    
    return flags;
}

QVariant ParameterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }
    
    switch (section) {
        case NameColumn: return "Parameter";
        case ValueColumn: return "Value";
        case DefaultColumn: return "Default";
        case UnitsColumn: return "Units";
        default: return QVariant();
    }
}