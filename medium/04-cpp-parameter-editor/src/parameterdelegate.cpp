#include "parameterdelegate.h"
#include "parametermodel.h"
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>

ParameterDelegate::ParameterDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* ParameterDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    if (index.column() != ParameterModel::ValueColumn) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    // Получаем параметр через модель
    auto* model = const_cast<QAbstractItemModel*>(index.model());
    auto* paramModel = qobject_cast<ParameterModel*>(model);
    if (!paramModel) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    const Parameter* param = paramModel->getParameter(index.row());
    if (!param) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    // Создаём редактор в зависимости от типа
    switch (param->type) {
    case ParameterType::Float:
        return createFloatEditor(parent, *param);
    case ParameterType::Int:
        return createIntEditor(parent, *param);
    case ParameterType::Bool:
        return createBoolEditor(parent);
    case ParameterType::Enum:
        return createEnumEditor(parent, *param);
    default:
        return createStringEditor(parent);
    }
}

QWidget* ParameterDelegate::createFloatEditor(QWidget* parent, const Parameter& param) const
{
    auto* spinBox = new QDoubleSpinBox(parent);
    spinBox->setDecimals(4);

    if (param.minValue.isValid()) {
        spinBox->setMinimum(param.minValue.toDouble());
    } else {
        spinBox->setMinimum(-1e9);
    }

    if (param.maxValue.isValid()) {
        spinBox->setMaximum(param.maxValue.toDouble());
    } else {
        spinBox->setMaximum(1e9);
    }

    if (param.increment > 0) {
        spinBox->setSingleStep(param.increment);
    } else {
        spinBox->setSingleStep(0.001);
    }

    return spinBox;
}

QWidget* ParameterDelegate::createIntEditor(QWidget* parent, const Parameter& param) const
{
    auto* spinBox = new QSpinBox(parent);

    if (param.minValue.isValid()) {
        spinBox->setMinimum(param.minValue.toInt());
    } else {
        spinBox->setMinimum(-1e9);
    }

    if (param.maxValue.isValid()) {
        spinBox->setMaximum(param.maxValue.toInt());
    } else {
        spinBox->setMaximum(1e9);
    }

    if (param.increment > 0) {
        spinBox->setSingleStep(static_cast<int>(param.increment));
    }

    return spinBox;
}

QWidget* ParameterDelegate::createBoolEditor(QWidget* parent) const
{
    auto* checkBox = new QCheckBox(parent);
    checkBox->setText("Enabled");
    return checkBox;
}

QWidget* ParameterDelegate::createEnumEditor(QWidget* parent, const Parameter& param) const
{
    auto* comboBox = new QComboBox(parent);

    for (const auto& enumVal : param.enumValues) {
        comboBox->addItem(enumVal.name, enumVal.value);
    }

    return comboBox;
}

QWidget* ParameterDelegate::createStringEditor(QWidget* parent) const
{
    return new QLineEdit(parent);
}

void ParameterDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QVariant value = index.data(Qt::EditRole);

    if (auto* spinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        spinBox->setValue(value.toDouble());
    }
    else if (auto* spinBox = qobject_cast<QSpinBox*>(editor)) {
        spinBox->setValue(value.toInt());
    }
    else if (auto* checkBox = qobject_cast<QCheckBox*>(editor)) {
        checkBox->setChecked(value.toBool());
    }
    else if (auto* comboBox = qobject_cast<QComboBox*>(editor)) {
        int idx = comboBox->findData(value);
        if (idx >= 0) comboBox->setCurrentIndex(idx);
    }
    else if (auto* lineEdit = qobject_cast<QLineEdit*>(editor)) {
        lineEdit->setText(value.toString());
    }
    else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ParameterDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                     const QModelIndex& index) const
{
    QVariant value;

    if (auto* spinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        value = spinBox->value();
    }
    else if (auto* spinBox = qobject_cast<QSpinBox*>(editor)) {
        value = spinBox->value();
    }
    else if (auto* checkBox = qobject_cast<QCheckBox*>(editor)) {
        value = checkBox->isChecked();
    }
    else if (auto* comboBox = qobject_cast<QComboBox*>(editor)) {
        value = comboBox->currentData();
    }
    else if (auto* lineEdit = qobject_cast<QLineEdit*>(editor)) {
        value = lineEdit->text();
    }
    else {
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }

    model->setData(index, value);
}

void ParameterDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
