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
#include "Scheduler.h"

// --- Custom Dialog (Keep this mostly the same) ---
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
};

// --- Main Application Window ---
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleAddTask();
    void handleEditTask(int id);
    void handleRemoveTask(int id);
    void handleShowStats();
    void handleNextTask();
    void handleSave();
    void handleLoad();
    void refreshTaskList();

private:
    void setupUI();
    QFrame* createTaskCard(const TaskManager& t); // Creates a beautiful UI card for a task
    
    Scheduler scheduler;
    
    // UI Elements
    QWidget *taskContainerWidget;
    QVBoxLayout *taskListLayout;
    QComboBox *filterCombo;
    QComboBox *sortCombo;
    QLabel *headerLabel;
};

#endif