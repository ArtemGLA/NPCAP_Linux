#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QVariant>
#include "parameter.h"

/**
 * Модель таблицы параметров.
 */
class ParameterModel : public QAbstractTableModel {
    Q_OBJECT
    
public:
    enum Column {
        NameColumn = 0,
        ValueColumn,
        DefaultColumn,
        UnitsColumn,
        ColumnCount
    };
    
    explicit ParameterModel(QObject *parent = nullptr);
    
    /// Установить список параметров.
    void setParameters(const QList<Parameter>& params);
    
    /// Установить текущие значения.
    void setValues(const QMap<QString, QVariant>& values);
    
    /// Получить текущее значение параметра.
    QVariant getValue(const QString& name) const;
    
    /// Получить все текущие значения.
    QMap<QString, QVariant> getAllValues() const;
    
    /// Получить параметр по индексу.
    const Parameter* getParameter(int row) const;
    
    // QAbstractTableModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    
signals:
    /// Сигнал об изменении параметра.
    void parameterChanged(const QString& name, const QVariant& value);
    
private:
    QList<Parameter> m_parameters;
    QMap<QString, QVariant> m_values;
};
