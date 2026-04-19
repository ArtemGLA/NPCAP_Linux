#include "mainwindow.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Parametter Editor");
    resize(1000, 700);

    setupUI();
    setupMenuBar();
}

void MainWindow::setupUI()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    auto* mainLayout = new QVBoxLayout(centralWidget);
    
    // Поисковая строка
    auto* searchLayout = new QHBoxLayout();
    m_searchBox = new QLineEdit();
    m_searchBox->setPlaceholderText("Search parameters...");
    searchLayout->addWidget(m_searchBox);
    
    auto* resetBtn = new QPushButton("Reset All");
    searchLayout->addWidget(resetBtn);
    
    mainLayout->addLayout(searchLayout);
    
    // Основной сплиттер
    auto* splitter = new QSplitter(Qt::Horizontal);
    
    // Дерево категорий
    m_categoryTree = new QTreeView();
    m_categoryModel = new QStandardItemModel(this);
    m_categoryTree->setModel(m_categoryModel);
    m_categoryTree->setHeaderHidden(true);
    m_categoryTree->setMinimumWidth(200);
    splitter->addWidget(m_categoryTree);
    
    // Правая панель
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    
    // Таблица параметров
    m_parameterTable = new QTableView();
    // TODO: Создайте ParameterModel и установите его
    // m_parameterModel = new ParameterModel(this);
    // m_filterModel = new QSortFilterProxyModel(this);
    // m_filterModel->setSourceModel(m_parameterModel);
    // m_parameterTable->setModel(m_filterModel);
    
    // TODO: Создайте ParameterDelegate
    // m_delegate = new ParameterDelegate(this);
    // m_parameterTable->setItemDelegate(m_delegate);
    
    rightLayout->addWidget(m_parameterTable, 3);
    
    // Описание параметра
    auto* descGroup = new QGroupBox("Description");
    auto* descLayout = new QVBoxLayout(descGroup);
    m_descriptionView = new QTextEdit();
    m_descriptionView->setReadOnly(true);
    m_descriptionView->setMaximumHeight(100);
    descLayout->addWidget(m_descriptionView);
    rightLayout->addWidget(descGroup);
    
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    
    mainLayout->addWidget(splitter);
}

    /// Создание меню.
void MainWindow::setupMenuBar()
{
    auto* fileMenu = menuBar()->addMenu("&File");

    auto* loadSchemaAction = fileMenu->addAction("Load Schema...");
    connect(loadSchemaAction, &QAction::triggered, this, &MainWindow::loadSchema);

    auto* loadProfileAction = fileMenu->addAction("Load Profile...");

    auto* saveProfileAction = fileMenu->addAction("Save Profile");

    fileMenu->addSeparator();

    auto* exitAction = fileMenu->addAction("Exit");

    auto* editMenu = menuBar()->addMenu("&Edit");

    auto* resetAllToDefaultAction = editMenu->addAction("Reset All to Default");
    auto* resetSelectedToDefaultAction = editMenu->addAction("Reset Selected to Default");
    
    auto* helpMenu = menuBar()->addMenu("&Help");

    auto* aboutAction = helpMenu->addAction("About");
}

void MainWindow::loadSchema()
{
    // TODO: Реализуйте загрузку схемы
    //
    // 1. Открыть диалог выбора файла
    // 2. Загрузить JSON схему
    // 3. Заполнить дерево категорий
    // 4. Инициализировать значения по умолчанию
    
    QString filepath = QFileDialog::getOpenFileName(
        this, "Open Schema", "", "JSON Files (*.json)"
    );
    
    if (filepath.isEmpty()) return;
    
    m_schema = ParameterSchema::load(filepath);
    populateCategories();
    
    statusBar()->showMessage("Schema loaded: " + filepath);
}

void MainWindow::populateCategories()
{
    // TODO: Заполнить дерево категорий из m_schema
    //
    // Для каждой группы создать QStandardItem
    // Добавить подгруппы как дочерние элементы
    
    m_categoryModel->clear();
    auto* groupItem = new QStandardItem("DSDS");
    m_categoryModel->appendRow(groupItem);
    
    // for (const auto& group : m_schema.groups) {
    //     auto* groupItem = new QStandardItem(group.name);
    //     for (const auto& subgroup : group.subgroups) {
    //         auto* subItem = new QStandardItem(subgroup.name);
    //         groupItem->appendRow(subItem);
    //     }
    //     m_categoryModel->appendRow(groupItem);
    // }
}