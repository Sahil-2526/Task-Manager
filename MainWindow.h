#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QScrollArea>
#include <QLabel>
#include <QFrame>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QTimer>       // Essential for auto-save
#include <QCloseEvent>  // Essential for save-on-exit
#include "Scheduler.h"

// --- Custom Dialog ---
class TaskDialog : public QDialog {
    Q_OBJECT
public:
    TaskDialog(QWidget *parent = nullptr);
    void setTaskData(const TaskManager& task);
    TaskManager getTaskData(int id = 0) const;

private:
    QLineEdit *nameEdit;
    QSpinBox *importanceSpin;
    QDateTimeEdit *estTimeEdit;
    QDateTimeEdit *deadlineEdit;
    QComboBox *statusCombo;
    QDoubleSpinBox *progressSpin; 
};

// --- Main Application Window ---
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    // Triggers when the user closes the app
    void closeEvent(QCloseEvent *event) override; 

private slots:
    void handleAddTask();
    void handleEditTask(int id);
    void handleRemoveTask(int id);
    void handleNextTask();
    void handleSave();
    void handleLoad();
    void refreshTaskList();
    void handleShowRecycleBin(); 
    void handleShowTodayTasks();
    void performAutoSave(); // Background save slot

private:
    void setupUI();
    QFrame* createTaskCard(const TaskManager& t); 
    
    Scheduler scheduler;
    
    QWidget *taskContainerWidget;
    QVBoxLayout *taskListLayout;
    QComboBox *filterCombo;
    QComboBox *sortCombo;
    QLabel *headerLabel;
};

#endif