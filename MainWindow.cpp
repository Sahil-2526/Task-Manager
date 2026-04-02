#include "MainWindow.h"
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGraphicsDropShadowEffect>
#include <algorithm>
#include <map>
#include <functional> 
#include <memory>     

// ==========================================
// TASK DIALOG IMPLEMENTATION
// ==========================================
TaskDialog::TaskDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Task Details");
    setMinimumWidth(400);

    QFormLayout *layout = new QFormLayout(this);

    nameEdit = new QLineEdit(this);
    importanceSpin = new QSpinBox(this);
    importanceSpin->setRange(1, 100); 

    estTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    estTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    estTimeEdit->setCalendarPopup(true); 
    
    deadlineEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(1), this);
    deadlineEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    deadlineEdit->setCalendarPopup(true); 

    progressSpin = new QDoubleSpinBox(this);
    progressSpin->setRange(0.0, 100.0);
    progressSpin->setSuffix("%");
    progressSpin->setDecimals(1);

    statusCombo = new QComboBox(this);
    statusCombo->addItems({"Pending", "In Progress", "Completed"});

    layout->addRow("Task Name:", nameEdit);
    layout->addRow("Importance (1-100):", importanceSpin); 
    layout->addRow("Estimated Time:", estTimeEdit);
    layout->addRow("Deadline:", deadlineEdit);
    layout->addRow("Progress:", progressSpin);
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
    
    progressSpin->setValue(task.getProgress()); 
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
    t.setProgess(progressSpin->value()); 
    t.setId(id);
    return t;
}

// ==========================================
// MAIN WINDOW IMPLEMENTATION
// ==========================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Task Scheduler Dashboard");
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
    QPushButton *btnRecycle = new QPushButton("🗑 Recycle Bin", sidebar); 
    QPushButton *btnSave = new QPushButton("Save Data", sidebar);
    QPushButton *btnLoad = new QPushButton("Load Data", sidebar);

    sidebarLayout->addWidget(btnAdd);
    sidebarLayout->addWidget(btnNext);
    sidebarLayout->addWidget(btnRecycle); 
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

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::handleAddTask);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::handleSave);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::handleLoad);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::handleNextTask);
    connect(btnRecycle, &QPushButton::clicked, this, &MainWindow::handleShowRecycleBin); 
    connect(filterCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
    connect(sortCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
}

// --- CREATING THE VISUAL TASK CARD ---
QFrame* MainWindow::createTaskCard(const TaskManager& t) {
    QFrame *card = new QFrame(taskContainerWidget);
    card->setObjectName("taskCard");
    card->setFixedHeight(120);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));
    card->setGraphicsEffect(shadow);

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(20, 15, 20, 15);

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

    // --- VIEW-ONLY PROGRESS BAR ---
    QHBoxLayout *progressLayout = new QHBoxLayout();
    QLabel *progressLabel = new QLabel(QString::number(t.getProgress(), 'f', 1) + "%", card);
    progressLabel->setStyleSheet("color: #a1a1aa; font-size: 12px; font-weight: bold; width: 40px;");
    
    QProgressBar *progressBar = new QProgressBar(card);
    progressBar->setRange(0, 100);
    progressBar->setValue(static_cast<int>(t.getProgress()));
    progressBar->setTextVisible(false); // Text is handled by progressLabel
    progressBar->setFixedHeight(6);
    progressBar->setObjectName("taskProgressBar");

    progressLayout->addWidget(progressBar);
    progressLayout->addWidget(progressLabel);

    detailsLayout->addWidget(titleLabel);
    detailsLayout->addWidget(infoLabel);
    detailsLayout->addWidget(dateLabel);
    detailsLayout->addLayout(progressLayout); 

    // Middle Column: Status Badge
    QLabel *statusBadge = new QLabel(QString::fromStdString(t.getStatus()), card);
    statusBadge->setObjectName("statusBadge");
    statusBadge->setAlignment(Qt::AlignCenter);
    
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

    cardLayout->addLayout(detailsLayout, 3); 
    cardLayout->addWidget(statusBadge, 1, Qt::AlignCenter); 
    cardLayout->addLayout(actionsLayout, 1);

    int currentId = t.getId();
    connect(btnEdit, &QPushButton::clicked, [this, currentId]() { handleEditTask(currentId); });
    connect(btnDelete, &QPushButton::clicked, [this, currentId]() { handleRemoveTask(currentId); });

    return card;
}

void MainWindow::refreshTaskList() {
    QLayoutItem *child;
    while ((child = taskListLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    std::vector<TaskManager> currentTasks = scheduler.getTasks();

    QString filter = filterCombo->currentText();
    if (filter != "All") {
        std::vector<TaskManager> filtered;
        for (const auto& t : currentTasks) {
            if (QString::fromStdString(t.getStatus()) == filter) filtered.push_back(t);
        }
        currentTasks = filtered;
    }

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
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm", "Move this task to the Recycle Bin?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        scheduler.removeTask(id); 
        refreshTaskList();
    }
}

void MainWindow::handleShowRecycleBin() {
    QDialog *binDialog = new QDialog(this);
    binDialog->setWindowTitle("Recycle Bin");
    binDialog->resize(500, 400);
    binDialog->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *mainLayout = new QVBoxLayout(binDialog);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("<b>Deleted Tasks</b>", binDialog);
    QPushButton *btnEmpty = new QPushButton("Empty Recycle Bin", binDialog);
    btnEmpty->setStyleSheet("background-color: #ef4444; color: white; padding: 5px; font-weight: bold; border-radius: 4px;");
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(btnEmpty);
    mainLayout->addLayout(headerLayout);

    QScrollArea *scrollArea = new QScrollArea(binDialog);
    scrollArea->setWidgetResizable(true);
    QWidget *container = new QWidget(scrollArea);
    QVBoxLayout *listLayout = new QVBoxLayout(container);
    listLayout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    auto refreshBinList = std::make_shared<std::function<void()>>();
    
    *refreshBinList = [this, listLayout, binDialog, refreshBinList]() {
        QLayoutItem *child;
        while ((child = listLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        const auto& recycled = scheduler.getRecycledTasks();
        if(recycled.empty()) {
            QLabel* emptyLabel = new QLabel("Recycle Bin is empty.");
            emptyLabel->setAlignment(Qt::AlignCenter);
            emptyLabel->setStyleSheet("color: gray; margin-top: 20px;");
            listLayout->addWidget(emptyLabel);
            return;
        }

        for (const auto& t : recycled) {
            QFrame *card = new QFrame();
            card->setFrameShape(QFrame::StyledPanel);
            card->setStyleSheet("background-color: #f3f4f6; border-radius: 5px; margin: 5px;");
            QHBoxLayout *cardLayout = new QHBoxLayout(card);

            QLabel *nameLabel = new QLabel(QString("<b>%1</b> (ID: %2)").arg(QString::fromStdString(t.getTask())).arg(t.getId()));
            nameLabel->setStyleSheet("color: #374151;"); 

            QPushButton *btnRestore = new QPushButton("Restore");
            QPushButton *btnDelete = new QPushButton("Delete");
            
            btnRestore->setStyleSheet("background-color: #10b981; color: white; padding: 4px 8px; border-radius: 3px;");
            btnDelete->setStyleSheet("background-color: #ef4444; color: white; padding: 4px 8px; border-radius: 3px;");

            cardLayout->addWidget(nameLabel);
            cardLayout->addStretch();
            cardLayout->addWidget(btnRestore);
            cardLayout->addWidget(btnDelete);

            int tId = t.getId();
            
            connect(btnRestore, &QPushButton::clicked, [this, tId, refreshBinList]() {
                scheduler.restoreTask(tId);
                (*refreshBinList)(); 
                refreshTaskList();   
            });
            
            connect(btnDelete, &QPushButton::clicked, [this, tId, refreshBinList]() {
                scheduler.permanentlyRemoveTask(tId);
                (*refreshBinList)(); 
            });

            listLayout->addWidget(card);
        }
    };

    connect(btnEmpty, &QPushButton::clicked, [this, refreshBinList]() {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Empty", "Are you sure you want to permanently delete all tasks in the Recycle Bin?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            scheduler.emptyRecycleBin();
            (*refreshBinList)();
        }
    });

    (*refreshBinList)();
    binDialog->exec();
}

void MainWindow::handleNextTask() {
    try {
        TaskManager bestTask = scheduler.nextTask();
        QMessageBox::information(this, "Focus Priority", QString("You should work on:\n\n%1").arg(QString::fromStdString(bestTask.getTask())));
    } catch (...) {}
}

void MainWindow::handleSave() {
    scheduler.saveToFile("tasks.txt");
    QMessageBox::information(this, "Save Successful", "Your tasks and recycle bin have been saved.");
}

void MainWindow::handleLoad() {
    scheduler.loadFromFile("tasks.txt");
    refreshTaskList();
    QMessageBox::information(this, "Load Successful", "Your tasks have been loaded.");
}