#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parameter.h"

#include <QMainWindow>
#include <QTreeView>
#include <QTableView>
#include <QTextEdit>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    /// Загрузить схему параметров.
    void loadSchema();

private:
    /// Настройка UI элементов.
    void setupUI();

    /// Создание тулбара.
    void setupMenuBar();

    /// Заполнение дерева категорий.
    void populateCategories();

    // Data
    ParameterSchema m_schema;
    QMap<QString, QVariant> m_currentValues;
    QMap<QString, QVariant> m_modifiedValues;
    QString m_currentProfilePath;
    
    // UI elements
    QTreeView* m_categoryTree;
    QStandardItemModel* m_categoryModel;
    
    QTableView* m_parameterTable;
    QSortFilterProxyModel* m_filterModel;
    
    QTextEdit* m_descriptionView;
    QLineEdit* m_searchBox;
};
#endif
