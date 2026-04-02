#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString styleSheet = R"(
        /* --- GLOBAL BACKGROUND --- */
        QMainWindow, QDialog {
            background-color: #09090b; 
            font-family: 'Segoe UI', -apple-system, sans-serif;
            color: #e4e4e7;
        }

        /* --- SIDEBAR --- */
        #sidebar {
            background-color: #18181b;
            border-right: 1px solid #27272a;
        }
        #logoLabel {
            font-size: 20px;
            font-weight: 800;
            color: #f4f4f5;
            letter-spacing: 1px;
        }
        
        #sidebar QPushButton {
            text-align: left;
            padding: 12px 16px;
            background-color: transparent;
            color: #a1a1aa;
            border-radius: 8px;
            font-size: 14px;
            font-weight: 600;
            border: none;
        }
        #sidebar QPushButton:hover {
            background-color: #27272a;
            color: #f4f4f5;
        }
        #sidebar QPushButton#primaryButton {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #8b5cf6, stop:1 #6d28d9);
            color: white;
            text-align: center;
            margin-bottom: 20px;
        }
        #sidebar QPushButton#primaryButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #a78bfa, stop:1 #7c3aed);
        }

        /* --- CONTENT AREA --- */
        #contentArea {
            background-color: #09090b;
        }
        #headerLabel {
            font-size: 28px;
            font-weight: 800;
            color: #ffffff;
        }

        /* --- TASK CARDS --- */
        #taskContainer {
            background-color: #09090b;
        }
        #taskCard {
            background-color: #18181b;
            border: 1px solid #27272a;
            border-radius: 12px;
        }
        #taskCard:hover {
            border: 1px solid #3f3f46;
            background-color: #1f1f22;
        }
        #cardTitle {
            font-size: 18px;
            font-weight: 700;
            color: #ffffff;
        }
        #cardInfo, #cardDate {
            color: #a1a1aa;
            font-size: 13px;
        }
        #statusBadge {
            border-radius: 6px;
            font-size: 12px;
            font-weight: 800;
            padding: 6px 12px;
            text-transform: uppercase;
        }

        #actionButton, #deleteButton {
            background-color: #27272a;
            color: #d4d4d8;
            border: none;
            border-radius: 6px;
            padding: 8px;
            font-weight: 600;
        }
        #actionButton:hover { background-color: #3f3f46; color: white; }
        #deleteButton:hover { background-color: #ef4444; color: white; }

        /* --- INPUTS & DROPDOWNS --- */
        QComboBox, QLineEdit, QSpinBox, QDoubleSpinBox, QDateTimeEdit {
            background-color: #18181b;
            color: #f4f4f5;
            border: 1px solid #27272a;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 13px;
        }
        QComboBox:focus, QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #8b5cf6;
        }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView {
            background-color: #18181b;
            border: 1px solid #27272a;
            selection-background-color: #8b5cf6;
        }

        /* --- PROGRESS BAR --- */
        #taskProgressBar {
            background-color: #27272a;
            border: none;
            border-radius: 3px;
        }
        #taskProgressBar::chunk {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8b5cf6, stop:1 #6d28d9);
            border-radius: 3px;
        }

        /* --- SCROLLBAR --- */
        QScrollBar:vertical {
            background: #09090b;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #3f3f46;
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover { background: #52525b; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
    )";

    app.setStyleSheet(styleSheet);

    MainWindow window;
    window.show();
    return app.exec();
}