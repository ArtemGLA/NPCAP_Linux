/**
 * MainWindow - реализация главного окна редактора параметров.
 */

#include "mainwindow.h"
#include "parametermodel.h"
#include "parameterdelegate.h"
#include <QDateTime>
#include <QFileInfo>
#include <QCloseEvent>
#include <QApplication>

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
#include <QLabel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_categoryTree(nullptr)
    , m_categoryModel(nullptr)
    , m_parameterTable(nullptr)
    , m_parameterModel(nullptr)
    , m_filterModel(nullptr)
    , m_delegate(nullptr)
    , m_descriptionView(nullptr)
    , m_searchBox(nullptr)
{
    setWindowTitle("Parameter Editor");
    resize(1000, 700);

    setupUI();
    setupMenuBar();
    setupToolBar();

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Панель поиска и кнопок
    auto* topWidget = new QWidget();
    auto* topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto* searchLabel = new QLabel("🔍 Search:", this);
    topLayout->addWidget(searchLabel);

    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("Parameter name or value...");
    m_searchBox->setClearButtonEnabled(true);
    topLayout->addWidget(m_searchBox, 1);

    auto* loadProfileBtn = new QPushButton("Load Profile", this);
    topLayout->addWidget(loadProfileBtn);

    auto* saveProfileBtn = new QPushButton("Save Profile", this);
    topLayout->addWidget(saveProfileBtn);

    auto* resetBtn = new QPushButton("Reset All", this);
    resetBtn->setStyleSheet("QPushButton { color: red; }");
    topLayout->addWidget(resetBtn);

    mainLayout->addWidget(topWidget);

    // Основной сплиттер (дерево категорий | таблица параметров + описание)
    auto* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);

    // Левая панель - дерево категорий
    auto* leftWidget = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    auto* categoriesLabel = new QLabel("<b>Categories</b>", this);
    leftLayout->addWidget(categoriesLabel);

    m_categoryTree = new QTreeView(this);
    m_categoryModel = new QStandardItemModel(this);
    m_categoryTree->setModel(m_categoryModel);
    m_categoryTree->setHeaderHidden(true);
    m_categoryTree->setIndentation(15);
    m_categoryTree->setMinimumWidth(200);
    m_categoryTree->setMaximumWidth(300);
    m_categoryTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_categoryTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftLayout->addWidget(m_categoryTree);

    splitter->addWidget(leftWidget);

    // Правая панель - таблица параметров + описание
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    auto* paramsLabel = new QLabel("<b>Parameters</b>", this);
    rightLayout->addWidget(paramsLabel);

    // Таблица параметров
    m_parameterTable = new QTableView(this);
    m_parameterTable->setAlternatingRowColors(true);
    m_parameterTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_parameterTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_parameterTable->setSortingEnabled(true);
    m_parameterTable->horizontalHeader()->setStretchLastSection(true);
    m_parameterTable->verticalHeader()->setVisible(false);

    // Создаём модели
    m_parameterModel = new ParameterModel(this);
    m_filterModel = new QSortFilterProxyModel(this);
    m_filterModel->setSourceModel(m_parameterModel);
    m_filterModel->setFilterKeyColumn(ParameterModel::NameColumn);
    m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_parameterTable->setModel(m_filterModel);

    // Создаём делегат
    m_delegate = new ParameterDelegate(this);
    m_parameterTable->setItemDelegate(m_delegate);

    rightLayout->addWidget(m_parameterTable, 3);

    // Панель описания параметра
    auto* descGroup = new QGroupBox("Parameter Description", this);
    auto* descLayout = new QVBoxLayout(descGroup);
    m_descriptionView = new QTextEdit(this);
    m_descriptionView->setReadOnly(true);
    m_descriptionView->setMaximumHeight(120);
    m_descriptionView->setMinimumHeight(100);
    m_descriptionView->setStyleSheet("QTextEdit { background-color: #f8f8f8; }");
    descLayout->addWidget(m_descriptionView);
    rightLayout->addWidget(descGroup);

    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    mainLayout->addWidget(splitter);

    // Подключение сигналов
    connect(m_searchBox, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
    connect(m_categoryTree->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onCategorySelected);
    connect(resetBtn, &QPushButton::clicked,
            this, &MainWindow::resetAllToDefault);
    connect(loadProfileBtn, &QPushButton::clicked,
            this, &MainWindow::loadProfile);
    connect(saveProfileBtn, &QPushButton::clicked,
            this, &MainWindow::saveProfile);
    connect(m_parameterTable->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onParameterSelected);
    connect(m_parameterModel, &ParameterModel::parameterChanged,
            this, &MainWindow::onParameterChanged);
}

void MainWindow::setupMenuBar()
{
    // Меню File
    auto* fileMenu = menuBar()->addMenu("&File");

    auto* loadSchemaAction = fileMenu->addAction("&Load Schema...");
    loadSchemaAction->setShortcut(QKeySequence::Open);
    connect(loadSchemaAction, &QAction::triggered, this, &MainWindow::loadSchema);

    fileMenu->addSeparator();

    auto* loadProfileAction = fileMenu->addAction("&Load Profile...");
    loadProfileAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(loadProfileAction, &QAction::triggered, this, &MainWindow::loadProfile);

    auto* saveProfileAction = fileMenu->addAction("&Save Profile");
    saveProfileAction->setShortcut(QKeySequence::Save);
    connect(saveProfileAction, &QAction::triggered, this, &MainWindow::saveProfile);

    auto* saveProfileAsAction = fileMenu->addAction("Save Profile &As...");
    saveProfileAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveProfileAsAction, &QAction::triggered, this, [this]() {
        m_currentProfilePath.clear();
        saveProfile();
    });

    fileMenu->addSeparator();

    auto* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Меню Edit
    auto* editMenu = menuBar()->addMenu("&Edit");

    auto* resetAllAction = editMenu->addAction("Reset &All to Default");
    resetAllAction->setShortcut(QKeySequence("Ctrl+Shift+R"));
    connect(resetAllAction, &QAction::triggered, this, &MainWindow::resetAllToDefault);

    auto* resetSelectedAction = editMenu->addAction("Reset &Selected to Default");
    resetSelectedAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(resetSelectedAction, &QAction::triggered, this, &MainWindow::resetSelectedToDefault);

    // Меню Help
    auto* helpMenu = menuBar()->addMenu("&Help");

    auto* aboutAction = helpMenu->addAction("&About");
    aboutAction->setShortcut(QKeySequence::HelpContents);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    auto* aboutQtAction = helpMenu->addAction("About &Qt");
    connect(aboutQtAction, &QAction::triggered, this, &QApplication::aboutQt);
}

void MainWindow::setupToolBar()
{
    auto* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(20, 20));

    // Создаём действия для тулбара
    auto* loadSchemaAction = new QAction("Load Schema", this);
    auto* loadProfileAction = new QAction("Load Profile", this);
    auto* saveProfileAction = new QAction("Save Profile", this);
    auto* resetAction = new QAction("Reset All", this);

    connect(loadSchemaAction, &QAction::triggered, this, &MainWindow::loadSchema);
    connect(loadProfileAction, &QAction::triggered, this, &MainWindow::loadProfile);
    connect(saveProfileAction, &QAction::triggered, this, &MainWindow::saveProfile);
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetAllToDefault);

    toolbar->addAction(loadSchemaAction);
    toolbar->addSeparator();
    toolbar->addAction(loadProfileAction);
    toolbar->addAction(saveProfileAction);
    toolbar->addSeparator();
    toolbar->addAction(resetAction);
}

void MainWindow::loadSchema()
{
    QString filepath = QFileDialog::getOpenFileName(
        this, "Open Schema", "", "JSON Files (*.json);;All Files (*)"
        );

    if (filepath.isEmpty()) return;
    loadSchemaFromPath(filepath);
}

void MainWindow::loadSchemaFromPath(const QString& path)
{
    m_schema = ParameterSchema::load(path);

    if (m_schema.groups.isEmpty()) {
        QMessageBox::warning(this, "Load Error",
                             "Failed to load schema from:\n" + path +
                                 "\n\nCheck that the file is valid JSON.");
        return;
    }

    // Инициализируем значения по умолчанию
    m_currentValues.clear();
    m_modifiedValues.clear();
    m_currentProfilePath.clear();

    auto allParams = m_schema.allParameters();
    for (const auto& param : allParams) {
        m_currentValues[param.name] = param.defaultValue;
    }

    // Устанавливаем параметры в модель
    m_parameterModel->setParameters(allParams);
    m_parameterModel->setValues(m_currentValues);

    // Заполняем дерево категорий
    populateCategories();

    setWindowTitle("Parameter Editor - " + QFileInfo(path).fileName());
    statusBar()->showMessage("Schema loaded: " + path, 3000);
}

void MainWindow::saveProfile()
{
    if (m_schema.groups.isEmpty()) {
        QMessageBox::warning(this, "No Schema",
                             "Please load a schema first before saving a profile.");
        return;
    }

    if (m_currentProfilePath.isEmpty()) {
        QString filepath = QFileDialog::getSaveFileName(
            this, "Save Profile", "", "JSON Files (*.json);;All Files (*)"
            );
        if (filepath.isEmpty()) return;
        m_currentProfilePath = filepath;
    }

    // Создаём JSON для сохранения
    QJsonObject root;
    root["version"] = "1.0";
    root["vehicle"] = m_schema.vehicle;
    root["profile_name"] = QFileInfo(m_currentProfilePath).baseName();
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject parameters;
    for (auto it = m_currentValues.begin(); it != m_currentValues.end(); ++it) {
        QVariant val = it.value();
        if (val.type() == QVariant::Double) {
            parameters[it.key()] = val.toDouble();
        } else if (val.type() == QVariant::Int) {
            parameters[it.key()] = val.toInt();
        } else if (val.type() == QVariant::Bool) {
            parameters[it.key()] = val.toBool();
        } else {
            parameters[it.key()] = val.toString();
        }
    }
    root["parameters"] = parameters;

    // Сохраняем изменённые параметры отдельно для быстрого доступа
    QJsonObject modified;
    for (auto it = m_modifiedValues.begin(); it != m_modifiedValues.end(); ++it) {
        QVariant val = it.value();
        if (val.type() == QVariant::Double) {
            modified[it.key()] = val.toDouble();
        } else if (val.type() == QVariant::Int) {
            modified[it.key()] = val.toInt();
        } else if (val.type() == QVariant::Bool) {
            modified[it.key()] = val.toBool();
        } else {
            modified[it.key()] = val.toString();
        }
    }
    root["modified_parameters"] = modified;

    QFile file(m_currentProfilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        statusBar()->showMessage("Profile saved: " + m_currentProfilePath, 3000);
    } else {
        QMessageBox::warning(this, "Save Error",
                             "Could not save profile to:\n" + m_currentProfilePath);
    }
}

void MainWindow::loadProfile()
{
    if (m_schema.groups.isEmpty()) {
        QMessageBox::warning(this, "No Schema",
                             "Please load a schema first before loading a profile.");
        return;
    }

    QString filepath = QFileDialog::getOpenFileName(
        this, "Open Profile", "", "JSON Files (*.json);;All Files (*)"
        );

    if (filepath.isEmpty()) return;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Load Error", "Cannot open file:\n" + filepath);
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, "Load Error", "Invalid JSON in file:\n" + filepath);
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject parameters = root["parameters"].toObject();

    // Валидируем и загружаем значения
    int loadedCount = 0;
    int invalidCount = 0;

    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        QString name = it.key();
        QVariant val = it.value().toVariant();

        Parameter* param = m_schema.findParameter(name);
        if (param && param->validate(val)) {
            m_currentValues[name] = val;
            if (val != param->defaultValue) {
                m_modifiedValues[name] = val;
            } else {
                m_modifiedValues.remove(name);
            }
            loadedCount++;
        } else {
            invalidCount++;
        }
    }

    m_parameterModel->setValues(m_currentValues);
    m_currentProfilePath = filepath;

    setWindowTitle("Parameter Editor - " + QFileInfo(filepath).fileName());
    statusBar()->showMessage(QString("Profile loaded: %1 (%2 parameters, %3 invalid)")
                                 .arg(filepath).arg(loadedCount).arg(invalidCount), 3000);

    if (invalidCount > 0) {
        QMessageBox::warning(this, "Partial Load",
                             QString("Loaded %1 parameters.\n%2 parameters were invalid and skipped.")
                                 .arg(loadedCount).arg(invalidCount));
    }
}

void MainWindow::resetAllToDefault()
{
    if (m_schema.groups.isEmpty()) {
        QMessageBox::warning(this, "No Schema", "No schema loaded.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Reset All",
        "Reset all parameters to their default values?\n\n"
        "This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        auto allParams = m_schema.allParameters();
        m_currentValues.clear();
        m_modifiedValues.clear();

        for (const auto& param : allParams) {
            m_currentValues[param.name] = param.defaultValue;
        }

        m_parameterModel->setValues(m_currentValues);
        statusBar()->showMessage("All parameters reset to default", 2000);
    }
}

void MainWindow::resetSelectedToDefault()
{
    if (m_schema.groups.isEmpty()) return;

    QModelIndex current = m_parameterTable->currentIndex();
    if (!current.isValid()) return;

    QModelIndex sourceIndex = m_filterModel->mapToSource(current);
    const Parameter* param = m_parameterModel->getParameter(sourceIndex.row());

    if (param) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Reset Parameter",
            QString("Reset '%1' to its default value (%2)?")
                .arg(param->displayName.isEmpty() ? param->name : param->displayName)
                .arg(param->formatValue(param->defaultValue)),
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            m_currentValues[param->name] = param->defaultValue;
            m_modifiedValues.remove(param->name);
            m_parameterModel->setValues(m_currentValues);
            statusBar()->showMessage("Reset: " + param->name, 2000);
        }
    }
}

void MainWindow::onParameterChanged(const QString& name, const QVariant& value)
{
    m_currentValues[name] = value;

    Parameter* param = m_schema.findParameter(name);
    if (param) {
        if (value != param->defaultValue) {
            m_modifiedValues[name] = value;
        } else {
            m_modifiedValues.remove(name);
        }
    }

    // Обновляем статус-бар с информацией о количестве изменений
    statusBar()->showMessage(QString("Modified: %1 = %2 (%3 changes total)")
                                 .arg(name)
                                 .arg(param ? param->formatValue(value) : value.toString())
                                 .arg(m_modifiedValues.size()), 2000);
}

void MainWindow::onCategorySelected(const QModelIndex& index)
{
    if (!index.isValid()) return;

    QStandardItem* item = m_categoryModel->itemFromIndex(index);
    if (!item) return;

    // Определяем группу и подгруппу
    QString subgroupName = item->text();
    QString groupName;

    QStandardItem* parent = item->parent();
    if (parent) {
        groupName = parent->text();
    } else {
        groupName = item->text();
        subgroupName.clear();
    }

    showParametersForCategory(groupName, subgroupName);
}

void MainWindow::showParametersForCategory(const QString& group, const QString& subgroup)
{
    QList<Parameter> filtered;

    for (const auto& param : m_schema.allParameters()) {
        if (param.group == group) {
            if (subgroup.isEmpty() || param.subgroup == subgroup) {
                filtered.append(param);
            }
        }
    }

    m_parameterModel->setParameters(filtered);
    m_parameterModel->setValues(m_currentValues);

    // Сбрасываем фильтр поиска при смене категории
    if (!m_searchBox->text().isEmpty()) {
        m_searchBox->clear();
    }
}

void MainWindow::onSearchTextChanged(const QString& text)
{
    if (text.isEmpty()) {
        m_filterModel->setFilterRegularExpression(QRegularExpression());
    } else {
        // Ищем по имени параметра, отображаемому имени и описанию
        // Для простоты ищем только по имени
        m_filterModel->setFilterRegularExpression(
            QRegularExpression(text, QRegularExpression::CaseInsensitiveOption)
            );
    }
}

void MainWindow::onParameterSelected(const QModelIndex& index)
{
    if (!index.isValid()) return;

    QModelIndex sourceIndex = m_filterModel->mapToSource(index);
    const Parameter* param = m_parameterModel->getParameter(sourceIndex.row());

    if (param) {
        updateDescription(*param);
    }
}

void MainWindow::populateCategories()
{
    m_categoryModel->clear();
    m_categoryModel->setHorizontalHeaderLabels(QStringList() << "Categories");

    for (const auto& group : m_schema.groups) {
        auto* groupItem = new QStandardItem(group.name);
        groupItem->setEditable(false);
        groupItem->setToolTip(group.description);

        // Устанавливаем жирный шрифт для групп
        QFont boldFont = groupItem->font();
        boldFont.setBold(true);
        groupItem->setFont(boldFont);

        for (const auto& subgroup : group.subgroups) {
            auto* subItem = new QStandardItem(subgroup.name);
            subItem->setEditable(false);
            subItem->setToolTip(subgroup.description.isEmpty() ? group.description : subgroup.description);
            groupItem->appendRow(subItem);
        }

        m_categoryModel->appendRow(groupItem);
    }

    // Разворачиваем все категории
    m_categoryTree->expandAll();

    // Выбираем первую категорию по умолчанию
    if (m_categoryModel->rowCount() > 0) {
        QModelIndex firstIndex = m_categoryModel->index(0, 0);
        m_categoryTree->setCurrentIndex(firstIndex);
        onCategorySelected(firstIndex);
    }
}

void MainWindow::updateDescription(const Parameter& param)
{
    QString typeStr;
    switch (param.type) {
    case ParameterType::Float: typeStr = "Floating Point"; break;
    case ParameterType::Int: typeStr = "Integer"; break;
    case ParameterType::Bool: typeStr = "Boolean"; break;
    case ParameterType::Enum: typeStr = "Enumeration"; break;
    case ParameterType::String: typeStr = "String"; break;
    }

    QString html = QString(
                       "<html>"
                       "<head><style>"
                       "body { font-family: Arial, sans-serif; margin: 5px; }"
                       ".param-name { font-size: 14px; font-weight: bold; color: #2c3e50; }"
                       ".param-id { font-size: 11px; color: #7f8c8d; font-family: monospace; }"
                       ".param-desc { margin-top: 8px; color: #34495e; }"
                       ".param-details { margin-top: 10px; font-size: 12px; }"
                       ".detail-label { font-weight: bold; color: #2980b9; }"
                       ".detail-value { color: #27ae60; font-family: monospace; }"
                       ".modified { color: #e74c3c; font-weight: bold; }"
                       "</style></head>"
                       "<body>"
                       "<div class='param-name'>%1</div>"
                       "<div class='param-id'>%2</div>"
                       "<div class='param-desc'>%3</div>"
                       "<hr>"
                       "<table class='param-details' width='100%%'>"
                       "<tr><td class='detail-label'>Type:</td><td class='detail-value'>%4</td></tr>"
                       "<tr><td class='detail-label'>Range:</td><td class='detail-value'>%5</td></tr>"
                       "<tr><td class='detail-label'>Default:</td><td class='detail-value'>%6</td></tr>"
                       "<tr><td class='detail-label'>Units:</td><td class='detail-value'>%7</td></tr>"
                       )
                       .arg(param.displayName.isEmpty() ? param.name : param.displayName)
                       .arg(param.name)
                       .arg(param.description.isEmpty() ? "No description available." : param.description)
                       .arg(typeStr)
                       .arg((param.minValue.isValid() ? param.formatValue(param.minValue) : "—") +
                            " to " +
                            (param.maxValue.isValid() ? param.formatValue(param.maxValue) : "—"))
                       .arg(param.formatValue(param.defaultValue))
                       .arg(param.units.isEmpty() ? "—" : param.units);

    // Добавляем информацию об изменении
    if (m_modifiedValues.contains(param.name)) {
        QVariant currentVal = m_currentValues.value(param.name);
        html += QString(
                    "<tr><td class='detail-label'>Current:</td>"
                    "<td class='detail-value modified'>%1 (modified)</td></tr>")
                    .arg(param.formatValue(currentVal));
    }

    // Для enum добавляем список возможных значений
    if (param.type == ParameterType::Enum && !param.enumValues.isEmpty()) {
        html += "<tr><td class='detail-label' valign='top'>Values:</td><td>";
        for (const auto& ev : param.enumValues) {
            html += QString("%1: %2<br>").arg(ev.value).arg(ev.name);
        }
        html += "</td></tr>";
    }

    html += "</table></body></html>";

    m_descriptionView->setHtml(html);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About Parameter Editor",
                       "<b>Parameter Editor</b><br>"
                       "Version 1.0<br><br>"
                       "A professional tool for editing drone parameters.<br>"
                       "Supports ArduPilot and PX4 parameter schemas.<br><br>"
                       "Features:<br>"
                       "• Hierarchical parameter organization<br>"
                       "• Real-time validation<br>"
                       "• Profile save/load<br>"
                       "• Search and filter<br>"
                       "• Modified parameter highlighting<br><br>"
                       "Built with Qt Framework<br>"
                       "© 2024 NPCAP");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!m_modifiedValues.isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Unsaved Changes",
            "You have unsaved changes. Do you want to save them before exiting?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Save) {
            saveProfile();
            event->accept();
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
