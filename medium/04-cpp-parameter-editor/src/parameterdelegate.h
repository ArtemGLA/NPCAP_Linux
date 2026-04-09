#pragma once

#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "parameter.h"

/**
 * Делегат для редактирования параметров.
 * Создаёт разные виджеты в зависимости от типа параметра.
 */
class ParameterDelegate : public QStyledItemDelegate {
    Q_OBJECT
    
public:
    explicit ParameterDelegate(QObject *parent = nullptr);
    
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                         const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                     const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                             const QModelIndex& index) const override;
    
private:
    QWidget* createFloatEditor(QWidget* parent, const Parameter& param) const;
    QWidget* createIntEditor(QWidget* parent, const Parameter& param) const;
    QWidget* createBoolEditor(QWidget* parent) const;
    QWidget* createEnumEditor(QWidget* parent, const Parameter& param) const;
    QWidget* createStringEditor(QWidget* parent) const;
};
