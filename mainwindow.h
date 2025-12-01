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
#include <QMessageBox>
#include <vector>
#include "segment_tree.h"
#include "treewidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    segment_tree<int, int>* segTree;
    TreeWidget* treeWidget;
    QLabel* arrayLabel;
    QLabel* resultLabel;
    QLineEdit* initArrayInput;
    QSpinBox* posSpinBox;
    QSpinBox* valSpinBox;
    QSpinBox* leftSpinBox;
    QSpinBox* rightSpinBox;

    void updateDisplay() {
        treeWidget->setTree(segTree->getTree(), segTree->getOriginalSize());

        // Actualizar visualización del array
        auto currentArray = segTree->getOriginalArray();
        QString arrayText = "Array: [";
        for (size_t i = 0; i < currentArray.size(); i++) {
            arrayText += QString::number(currentArray[i]);
            if (i < currentArray.size() - 1) arrayText += ", ";
        }
        arrayText += "]";
        arrayLabel->setText(arrayText);

        // Actualizar límites de los spinboxes
        int maxIndex = segTree->getOriginalSize() - 1;
        posSpinBox->setMaximum(maxIndex);
        leftSpinBox->setMaximum(maxIndex);
        rightSpinBox->setMaximum(maxIndex);
    }

    vector<int> parseArrayInput(const QString& input) {
        vector<int> result;
        QStringList parts = input.split(',');

        for (const QString& part : parts) {
            bool ok;
            int num = part.trimmed().toInt(&ok);
            if (ok) {
                result.push_back(num);
            }
        }

        return result;
    }

private slots:
    void onInitArray() {
        QString input = initArrayInput->text().trimmed();

        if (input.isEmpty()) {
            QMessageBox::warning(this, "Error", "Por favor ingresa una lista de números");
            return;
        }

        vector<int> nums = parseArrayInput(input);

        if (nums.empty()) {
            QMessageBox::warning(this, "Error", "No se pudo parsear la lista. Usa formato: 1,2,3,4,5");
            return;
        }

        delete segTree;
        segTree = new segment_tree<int, int>(nums);

        resultLabel->setText(QString("Árbol inicializado con %1 elementos").arg(nums.size()));
        updateDisplay();
    }

    void onUpdate() {
        int pos = posSpinBox->value();
        int val = valSpinBox->value();

        if (pos >= 0 && pos < segTree->getOriginalSize()) {
            segTree->update(pos, val);
            resultLabel->setText(QString("Actualizado: posición %1 = %2")
                               .arg(pos).arg(val));
            updateDisplay();
        }
    }

    void onQuery() {
        int left = leftSpinBox->value();
        int right = rightSpinBox->value();

        if (left <= right && left >= 0 && right < segTree->getOriginalSize()) {
            int result = segTree->query(left, right);
            resultLabel->setText(QString("Suma [%1, %2] = %3")
                               .arg(left).arg(right).arg(result));
        } else {
            resultLabel->setText("Rango inválido");
        }
    }

    void onEliminate() {
        int pos = posSpinBox->value();

        if (pos >= 0 && pos < segTree->getOriginalSize()) {
            segTree->eliminate(pos);
            resultLabel->setText(QString("Eliminado: posición %1").arg(pos));
            updateDisplay();
        }
    }

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        // Inicializar con array por defecto
        vector<int> defaultArray = {1, 6, 4, 7, 2, 8};
        segTree = new segment_tree<int, int>(defaultArray);

        setWindowTitle("Visualizador de Segment Tree");
        resize(950, 750);

        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

        // Widget del árbol
        treeWidget = new TreeWidget(this);
        mainLayout->addWidget(treeWidget, 1);

        // Label del array
        arrayLabel = new QLabel("Array: []", this);
        arrayLabel->setStyleSheet("font-size: 14pt; padding: 10px; background-color: #f0f0f0; border-radius: 5px;");
        mainLayout->addWidget(arrayLabel);

        // Panel de controles
        QGroupBox* controlGroup = new QGroupBox("Controles", this);
        QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

        // Inicializar con input personalizado
        QHBoxLayout* initLayout = new QHBoxLayout();
        initLayout->addWidget(new QLabel("Inicializar Array:", this));
        initArrayInput = new QLineEdit(this);
        initArrayInput->setPlaceholderText("Ejemplo: 1,2,3,4,5,6,7,8,9,10");
        initArrayInput->setText("1,6,4,7,2,8");
        initLayout->addWidget(initArrayInput);
        QPushButton* initBtn = new QPushButton("Crear Árbol", this);
        connect(initBtn, &QPushButton::clicked, this, &MainWindow::onInitArray);
        initLayout->addWidget(initBtn);
        controlLayout->addLayout(initLayout);

        // Separador
        QFrame* line1 = new QFrame();
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
        controlLayout->addWidget(line1);

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
        eliminateBtn->setStyleSheet("background-color: #f44336; color: white;");
        connect(eliminateBtn, &QPushButton::clicked, this, &MainWindow::onEliminate);
        eliminateLayout->addWidget(eliminateBtn);
        controlLayout->addLayout(eliminateLayout);

        mainLayout->addWidget(controlGroup);

        // Label de resultado
        resultLabel = new QLabel("Árbol inicializado con valores por defecto [1,6,4,7,2,8]", this);
        resultLabel->setStyleSheet("font-size: 12pt; color: blue; padding: 8px; background-color: #e3f2fd; border-radius: 5px;");
        resultLabel->setWordWrap(true);
        mainLayout->addWidget(resultLabel);

        // Inicializar display
        updateDisplay();
    }

    ~MainWindow() {
        delete segTree;
    }
};


#endif