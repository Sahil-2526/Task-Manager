#include "MainWindow.h"
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGraphicsDropShadowEffect>
#include <algorithm>
#include <map>

// --- Qt Charts Includes ---
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// ==========================================
// TASK DIALOG IMPLEMENTATION
// ==========================================
TaskDialog::TaskDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Task Details");
    setMinimumWidth(400);

    QFormLayout *layout = new QFormLayout(this);

    nameEdit = new QLineEdit(this);
    
    // Updated range from 1-10 to 1-100
    importanceSpin = new QSpinBox(this);
    importanceSpin->setRange(1, 100); 

    // Enabled Calendar Popup for Estimated Time
    estTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    estTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    estTimeEdit->setCalendarPopup(true); 
    
    // Enabled Calendar Popup for Deadline
    deadlineEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(1), this);
    deadlineEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    deadlineEdit->setCalendarPopup(true); 

    statusCombo = new QComboBox(this);
    statusCombo->addItems({"Pending", "In Progress", "Completed"});

    layout->addRow("Task Name:", nameEdit);
    layout->addRow("Importance (1-100):", importanceSpin); 
    layout->addRow("Estimated Time:", estTimeEdit);
    layout->addRow("Deadline:", deadlineEdit);
    layout->addRow("Status:", statusCombo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void TaskDialog::setTaskData(const TaskManager& task) {
    nameEdit->setText(QString::fromStdString(task.getTask()));
    importanceSpin->setValue(task.getImportanceLvL());
    QDateTime est = QDateTime::fromString(QString::fromStdString(task.getEstimatedTime()), "yyyy-MM-dd HH:mm:ss");
    estTimeEdit->setDateTime(est.isValid() ? est : QDateTime::currentDateTime());
    QDateTime dead = QDateTime::fromString(QString::fromStdString(task.getDeadline()), "yyyy-MM-dd HH:mm:ss");
    deadlineEdit->setDateTime(dead.isValid() ? dead : QDateTime::currentDateTime());
    statusCombo->setCurrentText(QString::fromStdString(task.getStatus()));
}

TaskManager TaskDialog::getTaskData(int id) const {
    std::string name = nameEdit->text().toStdString();
    int imp = importanceSpin->value();
    std::string est = estTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    std::string dead = deadlineEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    Status stat = Status::Pending;
    if(statusCombo->currentText() == "In Progress") stat = Status::InProgress;
    if(statusCombo->currentText() == "Completed") stat = Status::Completed;

    TaskManager t(name, imp, est, dead, stat);
    t.setId(id);
    return t;
}

// ==========================================
// MAIN WINDOW IMPLEMENTATION
// ==========================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("AI Task Scheduler Dashboard");
    resize(1000, 700);
    setupUI();
    scheduler.loadFromFile("tasks.txt");
    refreshTaskList();
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- SIDEBAR ---
    QFrame *sidebar = new QFrame(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);
    sidebarLayout->setSpacing(15);

    QLabel *logoLabel = new QLabel("Task Scheduler", sidebar);
    logoLabel->setObjectName("logoLabel");
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addSpacing(30);

    QPushButton *btnAdd = new QPushButton("+ Create Task", sidebar);
    btnAdd->setObjectName("primaryButton");
    
    QPushButton *btnNext = new QPushButton("Suggest Next Task", sidebar);
    QPushButton *btnStats = new QPushButton("Statistics", sidebar);
    QPushButton *btnSave = new QPushButton("Save Data", sidebar);
    QPushButton *btnLoad = new QPushButton("Load Data", sidebar);

    sidebarLayout->addWidget(btnAdd);
    sidebarLayout->addWidget(btnNext);
    sidebarLayout->addWidget(btnStats);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnSave);
    sidebarLayout->addWidget(btnLoad);

    mainLayout->addWidget(sidebar);

    // --- MAIN CONTENT AREA ---
    QWidget *contentArea = new QWidget(this);
    contentArea->setObjectName("contentArea");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(20);

    // Header Controls (Filter/Sort)
    QHBoxLayout *headerControlsLayout = new QHBoxLayout();
    headerLabel = new QLabel("Your Tasks", contentArea);
    headerLabel->setObjectName("headerLabel");
    
    filterCombo = new QComboBox(contentArea);
    filterCombo->addItems({"All", "Pending", "In Progress", "Completed"});
    
    sortCombo = new QComboBox(contentArea);
    sortCombo->addItems({"Sort by ID", "Sort by Importance", "Sort by Deadline", "Sort by Est Time"});

    headerControlsLayout->addWidget(headerLabel);
    headerControlsLayout->addStretch();
    headerControlsLayout->addWidget(new QLabel("Filter:", contentArea));
    headerControlsLayout->addWidget(filterCombo);
    headerControlsLayout->addWidget(new QLabel("Sort:", contentArea));
    headerControlsLayout->addWidget(sortCombo);

    contentLayout->addLayout(headerControlsLayout);

    // Scroll Area for Tasks
    QScrollArea *scrollArea = new QScrollArea(contentArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("taskScrollArea");
    scrollArea->setFrameShape(QFrame::NoFrame);

    taskContainerWidget = new QWidget(scrollArea);
    taskContainerWidget->setObjectName("taskContainer");
    taskListLayout = new QVBoxLayout(taskContainerWidget);
    taskListLayout->setContentsMargins(0, 10, 0, 10);
    taskListLayout->setSpacing(15);
    taskListLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(taskContainerWidget);
    contentLayout->addWidget(scrollArea);

    mainLayout->addWidget(contentArea);
    setCentralWidget(centralWidget);

    // --- Connections ---
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::handleAddTask);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::handleSave);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::handleLoad);
    connect(btnStats, &QPushButton::clicked, this, &MainWindow::handleShowStats);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::handleNextTask);
    connect(filterCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
    connect(sortCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
}

// --- CREATING THE VISUAL TASK CARD ---
QFrame* MainWindow::createTaskCard(const TaskManager& t) {
    QFrame *card = new QFrame(taskContainerWidget);
    card->setObjectName("taskCard");
    card->setFixedHeight(120);

    // Shadow Effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));
    card->setGraphicsEffect(shadow);

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(20, 15, 20, 15);

    // Left Column: Details
    QVBoxLayout *detailsLayout = new QVBoxLayout();
    QLabel *titleLabel = new QLabel(QString::fromStdString(t.getTask()), card);
    titleLabel->setObjectName("cardTitle");
    
    QString infoText = QString("ID: %1 • Importance: %2 • Score: %3")
                       .arg(t.getId())
                       .arg(t.getImportanceLvL())
                       .arg(t.getScore(), 0, 'f', 2);
    QLabel *infoLabel = new QLabel(infoText, card);
    infoLabel->setObjectName("cardInfo");

    QLabel *dateLabel = new QLabel("⏱ Deadline: " + QString::fromStdString(t.getDeadline()), card);
    dateLabel->setObjectName("cardDate");

    detailsLayout->addWidget(titleLabel);
    detailsLayout->addWidget(infoLabel);
    detailsLayout->addWidget(dateLabel);

    // Middle Column: Status Badge
    QLabel *statusBadge = new QLabel(QString::fromStdString(t.getStatus()), card);
    statusBadge->setObjectName("statusBadge");
    statusBadge->setAlignment(Qt::AlignCenter);
    
    // Dynamic coloring for status badge
    if(t.getStatus() == "Pending") statusBadge->setStyleSheet("background-color: rgba(245, 158, 11, 0.2); color: #fcd34d;");
    else if(t.getStatus() == "In Progress") statusBadge->setStyleSheet("background-color: rgba(59, 130, 246, 0.2); color: #93c5fd;");
    else statusBadge->setStyleSheet("background-color: rgba(16, 185, 129, 0.2); color: #6ee7b7;");

    // Right Column: Action Buttons
    QVBoxLayout *actionsLayout = new QVBoxLayout();
    QPushButton *btnEdit = new QPushButton("Edit", card);
    QPushButton *btnDelete = new QPushButton("Delete", card);
    btnEdit->setObjectName("actionButton");
    btnDelete->setObjectName("deleteButton");

    actionsLayout->addWidget(btnEdit);
    actionsLayout->addWidget(btnDelete);

    // Put it all together
    cardLayout->addLayout(detailsLayout, 3); // 3 parts width
    cardLayout->addWidget(statusBadge, 1, Qt::AlignCenter); // 1 part width
    cardLayout->addLayout(actionsLayout, 1);

    // Connect buttons to their specific task ID using Lambdas
    int currentId = t.getId();
    connect(btnEdit, &QPushButton::clicked, [this, currentId]() { handleEditTask(currentId); });
    connect(btnDelete, &QPushButton::clicked, [this, currentId]() { handleRemoveTask(currentId); });

    return card;
}

void MainWindow::refreshTaskList() {
    // Clear existing cards
    QLayoutItem *child;
    while ((child = taskListLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    std::vector<TaskManager> currentTasks = scheduler.getTasks();

    // Filtering
    QString filter = filterCombo->currentText();
    if (filter != "All") {
        std::vector<TaskManager> filtered;
        for (const auto& t : currentTasks) {
            if (QString::fromStdString(t.getStatus()) == filter) filtered.push_back(t);
        }
        currentTasks = filtered;
    }

    // Sorting
    QString sortMode = sortCombo->currentText();
    if (sortMode == "Sort by Importance") {
        std::sort(currentTasks.begin(), currentTasks.end(), [](const TaskManager& a, const TaskManager& b) {
            return a.getImportanceLvL() > b.getImportanceLvL();
        });
    } else if (sortMode == "Sort by Deadline") {
        std::sort(currentTasks.begin(), currentTasks.end(), [](const TaskManager& a, const TaskManager& b) {
            return a.getDeadline() < b.getDeadline(); 
        });
    }

    // Add new cards to the layout
    for (const auto& t : currentTasks) {
        taskListLayout->addWidget(createTaskCard(t));
    }
    
    headerLabel->setText(QString("Your Tasks (%1)").arg(currentTasks.size()));
}

void MainWindow::handleAddTask() {
    TaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        scheduler.addTask(dialog.getTaskData());
        refreshTaskList();
    }
}

void MainWindow::handleEditTask(int id) {
    try {
        TaskManager currentTask = scheduler.findTaskById(id);
        TaskDialog dialog(this);
        dialog.setTaskData(currentTask);
        if (dialog.exec() == QDialog::Accepted) {
            TaskManager updatedTask = dialog.getTaskData(id);
            scheduler.updateTask(id, updatedTask);
            refreshTaskList();
        }
    } catch (...) {
        QMessageBox::critical(this, "Error", "Task not found.");
    }
}

void MainWindow::handleRemoveTask(int id) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm", "Are you sure you want to delete this task?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        scheduler.removeTask(id);
        refreshTaskList();
    }
}

// --- NEW STATISTICS IMPLEMENTATION WITH GRAPH ---
void MainWindow::handleShowStats() {
    std::vector<TaskManager> currentTasks = scheduler.getTasks();
    if (currentTasks.empty()) {
        QMessageBox::information(this, "Statistics", "No tasks available to analyze.");
        return;
    }

    // 1. Aggregating Data (Tasks per Day based on Deadline)
    std::map<QString, int> tasksPerDay;
    for (const auto& t : currentTasks) {
        QString dateStr = QString::fromStdString(t.getDeadline()).left(10); 
        tasksPerDay[dateStr]++;
    }

    // 2. Building the Chart Series
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Tasks Due");

    QStringList categories;
    int maxTasks = 0;

    for (auto const& [date, count] : tasksPerDay) {
        categories << date;       
        *set << count;            
        if (count > maxTasks) {
            maxTasks = count;
        }
    }
    
    set->setColor(QColor(59, 130, 246)); 
    series->append(set);

    // 3. Setting up the Chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Task Workload (Deadlines per Day)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    // X-Axis
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y-Axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxTasks + 1); 
    axisY->setTickCount(maxTasks + 2); 
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // 4. Creating the View and Dialog
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Task Statistics Dashboard");
    statsDialog->resize(700, 500); 
    
    QVBoxLayout *layout = new QVBoxLayout(statsDialog);
    layout->addWidget(chartView);

    QString avgStats = QString("<b>Overall Average Task Importance:</b> %1 / 100")
                        .arg(scheduler.avgImportanceLevel(), 0, 'f', 2);
    QLabel *avgLabel = new QLabel(avgStats, statsDialog);
    avgLabel->setAlignment(Qt::AlignCenter);
    avgLabel->setStyleSheet("font-size: 14px; padding: 10px;");
    layout->addWidget(avgLabel);

    statsDialog->exec();
}

void MainWindow::handleNextTask() {
    try {
        TaskManager bestTask = scheduler.nextTask();
        QMessageBox::information(this, "Focus Priority", QString("You should work on:\n\n%1").arg(QString::fromStdString(bestTask.getTask())));
    } catch (...) {}
}

void MainWindow::handleSave() {
    scheduler.saveToFile("tasks.txt");
}

void MainWindow::handleLoad() {
    scheduler.loadFromFile("tasks.txt");
    refreshTaskList();
}

