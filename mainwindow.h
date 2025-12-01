#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <vector>
#include "segment_tree.h"
#include "treewidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    segment_tree* segTree;
    std::vector<int> currentArray;
    TreeWidget* treeWidget;
    QLabel* arrayLabel;
    QLabel* resultLabel;
    QSpinBox* posSpinBox;
    QSpinBox* valSpinBox;
    QSpinBox* leftSpinBox;
    QSpinBox* rightSpinBox;
    
    void updateDisplay() {
        treeWidget->setTree(segTree->getTree(), segTree->getOriginalSize());
        
        // Actualizar visualización del array
        QString arrayText = "Array: [";
        for (size_t i = 0; i < currentArray.size(); i++) {
            arrayText += QString::number(currentArray[i]);
            if (i < currentArray.size() - 1) arrayText += ", ";
        }
        arrayText += "]";
        arrayLabel->setText(arrayText);
    }
    
private slots:
    void onInitArray() {
        currentArray = {1, 6, 4, 7, 2, 8};
        delete segTree;
        segTree = new segment_tree(currentArray);
        
        posSpinBox->setMaximum(currentArray.size() - 1);
        leftSpinBox->setMaximum(currentArray.size() - 1);
        rightSpinBox->setMaximum(currentArray.size() - 1);
        
        resultLabel->setText("Árbol inicializado");
        updateDisplay();
    }
    
    void onUpdate() {
        int pos = posSpinBox->value();
        int val = valSpinBox->value();
        
        if (pos >= 0 && pos < currentArray.size()) {
            currentArray[pos] = val;
            segTree->update(pos, val);
            resultLabel->setText(QString("Actualizado: posición %1 = %2")
                               .arg(pos).arg(val));
            updateDisplay();
        }
    }
    
    void onQuery() {
        int left = leftSpinBox->value();
        int right = rightSpinBox->value();
        
        if (left <= right && left >= 0 && right < currentArray.size()) {
            int result = segTree->query(left, right);
            resultLabel->setText(QString("Suma [%1, %2] = %3")
                               .arg(left).arg(right).arg(result));
        } else {
            resultLabel->setText("Rango inválido");
        }
    }
    
    void onEliminate() {
        int pos = posSpinBox->value();
        
        if (pos >= 0 && pos < currentArray.size()) {
            currentArray[pos] = 0;
            segTree->eliminate(pos);
            resultLabel->setText(QString("Eliminado: posición %1").arg(pos));
            updateDisplay();
        }
    }
    
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        segTree = nullptr;
        
        setWindowTitle("Visualizador de Segment Tree");
        resize(900, 700);
        
        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        
        // Widget del árbol
        treeWidget = new TreeWidget(this);
        mainLayout->addWidget(treeWidget, 1);
        
        // Label del array
        arrayLabel = new QLabel("Array: []", this);
        arrayLabel->setStyleSheet("font-size: 14pt; padding: 10px;");
        mainLayout->addWidget(arrayLabel);
        
        // Panel de controles
        QGroupBox* controlGroup = new QGroupBox("Controles", this);
        QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
        
        // Botón inicializar
        QPushButton* initBtn = new QPushButton("Inicializar Array [1,6,4,7,2,8]", this);
        connect(initBtn, &QPushButton::clicked, this, &MainWindow::onInitArray);
        controlLayout->addWidget(initBtn);
        
        // Actualizar
        QHBoxLayout* updateLayout = new QHBoxLayout();
        updateLayout->addWidget(new QLabel("Actualizar - Pos:", this));
        posSpinBox = new QSpinBox(this);
        posSpinBox->setMinimum(0);
        updateLayout->addWidget(posSpinBox);
        updateLayout->addWidget(new QLabel("Valor:", this));
        valSpinBox = new QSpinBox(this);
        valSpinBox->setMinimum(-1000);
        valSpinBox->setMaximum(1000);
        updateLayout->addWidget(valSpinBox);
        QPushButton* updateBtn = new QPushButton("Actualizar", this);
        connect(updateBtn, &QPushButton::clicked, this, &MainWindow::onUpdate);
        updateLayout->addWidget(updateBtn);
        controlLayout->addLayout(updateLayout);
        
        // Query
        QHBoxLayout* queryLayout = new QHBoxLayout();
        queryLayout->addWidget(new QLabel("Consulta - Izq:", this));
        leftSpinBox = new QSpinBox(this);
        leftSpinBox->setMinimum(0);
        queryLayout->addWidget(leftSpinBox);
        queryLayout->addWidget(new QLabel("Der:", this));
        rightSpinBox = new QSpinBox(this);
        rightSpinBox->setMinimum(0);
        queryLayout->addWidget(rightSpinBox);
        QPushButton* queryBtn = new QPushButton("Consultar", this);
        connect(queryBtn, &QPushButton::clicked, this, &MainWindow::onQuery);
        queryLayout->addWidget(queryBtn);
        controlLayout->addLayout(queryLayout);
        
        // Eliminar
        QHBoxLayout* eliminateLayout = new QHBoxLayout();
        QPushButton* eliminateBtn = new QPushButton("Eliminar (pos actual)", this);
        connect(eliminateBtn, &QPushButton::clicked, this, &MainWindow::onEliminate);
        eliminateLayout->addWidget(eliminateBtn);
        controlLayout->addLayout(eliminateLayout);
        
        mainLayout->addWidget(controlGroup);
        
        // Label de resultado
        resultLabel = new QLabel("Listo para comenzar", this);
        resultLabel->setStyleSheet("font-size: 12pt; color: blue; padding: 5px;");
        mainLayout->addWidget(resultLabel);
    }
    
    ~MainWindow() {
        delete segTree;
    }
};

#endif