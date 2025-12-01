#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <vector>

class TreeWidget : public QWidget {
    Q_OBJECT
    
private:
    std::vector<int> tree;
    int originalSize;
    
    struct NodeInfo {
        int x, y;
        int value;
        int index;
    };
    
    void calculatePositions(int idx, int depth, int& position, 
                          std::vector<NodeInfo>& nodes) {
        if (idx >= tree.size()) return;
        
        // Visitar hijo izquierdo
        int leftChild = idx * 2 + 1;
        if (leftChild < tree.size()) {
            calculatePositions(leftChild, depth + 1, position, nodes);
        }
        
        // Posición actual
        NodeInfo node;
        node.x = position;
        node.y = depth;
        node.value = tree[idx];
        node.index = idx;
        nodes.push_back(node);
        position++;
        
        // Visitar hijo derecho
        int rightChild = idx * 2 + 2;
        if (rightChild < tree.size()) {
            calculatePositions(rightChild, depth + 1, position, nodes);
        }
    }
    
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        if (tree.empty()) return;
        
        // Calcular posiciones
        std::vector<NodeInfo> nodes;
        int position = 0;
        calculatePositions(0, 0, position, nodes);
        
        // Calcular dimensiones
        int maxDepth = 0;
        int maxPos = 0;
        for (const auto& node : nodes) {
            if (node.y > maxDepth) maxDepth = node.y;
            if (node.x > maxPos) maxPos = node.x;
        }
        
        int nodeRadius = 30;
        int horizontalSpacing = width() / (maxPos + 2);
        int verticalSpacing = (height() - 100) / (maxDepth + 2);
        
        // Dibujar conexiones primero
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        for (const auto& node : nodes) {
            int parentIdx = (node.index - 1) / 2;
            if (node.index > 0 && parentIdx >= 0) {
                // Buscar posición del padre
                for (const auto& parent : nodes) {
                    if (parent.index == parentIdx) {
                        int x1 = parent.x * horizontalSpacing + horizontalSpacing;
                        int y1 = parent.y * verticalSpacing + verticalSpacing;
                        int x2 = node.x * horizontalSpacing + horizontalSpacing;
                        int y2 = node.y * verticalSpacing + verticalSpacing;
                        painter.drawLine(x1, y1, x2, y2);
                        break;
                    }
                }
            }
        }
        
        // Dibujar nodos
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        for (const auto& node : nodes) {
            int x = node.x * horizontalSpacing + horizontalSpacing;
            int y = node.y * verticalSpacing + verticalSpacing;
            
            // Círculo del nodo
            painter.setBrush(QColor(70, 130, 180));
            painter.setPen(QPen(QColor(25, 25, 112), 2));
            painter.drawEllipse(QPoint(x, y), nodeRadius, nodeRadius);
            
            // Valor
            painter.setPen(Qt::white);
            QString text = QString::number(node.value);
            QRect textRect(x - nodeRadius, y - nodeRadius, 
                          nodeRadius * 2, nodeRadius * 2);
            painter.drawText(textRect, Qt::AlignCenter, text);
        }
    }
    
public:
    explicit TreeWidget(QWidget* parent = nullptr) 
        : QWidget(parent), originalSize(0) {
        setMinimumSize(600, 400);
    }
    
    void setTree(const std::vector<int>& t, int origSize) {
        tree = t;
        originalSize = origSize;
        update();
    }
};

#endif