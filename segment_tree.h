#ifndef PRUEBA2_SEGMENT_TREE_H
#define PRUEBA2_SEGMENT_TREE_H

#include <functional>
#include <vector>
using namespace std;

//clase del segment_tree
template <typename treetype = int, typename T = int>
class segment_tree {
    //vector que representa el arbol
    vector<treetype> tree;
    //el tamanio original de arreglo y el tamanio actual del arbol
    int size_og = 1, size = 1;
    /*
     * En un segment tree se pueden hacer consultas en rango, estas consultas pueden ser sobre totales, maximos o hasta minimos.
     * Es por ello que se usara una funcion lambda para determinar que funcion tendra el arbol
     */
    const function< treetype(const treetype&, const treetype&) > operation;
    //valor por default del arbol, es decir, es valor neutro que no afecta a las operaciones del segment tree
    treetype DEFAULT;

    // Array original para tracking
    vector<T> original_array;

    //funcion recursiva "build" para construir el arbol
    void build(
        const vector<T>& nums, //arreglo original, ej: {8, 2 ,4, 1}
        int idx, //indice actual de la iteracion
        int lx, //limite izquiero del arreglo original, cambia en cada iteracion
        int rx //limite derecho del arreglo original, cambia en cada iteracion
        ) {
        if(lx == rx) { //si los limites son iguales
            tree[idx] = nums[lx]; // inicia asignando las hojas a su respectivo valor del array original
            return; //termina la iteracion (caso base)
        }
        int mid = (lx + rx)/2; //calcula la mitad para dividir lado izquierdo y derecho
        build(nums, idx*2 + 1, lx, mid); //inicia recursividad por la izquierda
        build(nums, idx*2 + 2, mid + 1, rx); //va recursivamente por la derecha
        tree[idx] = operation(tree[idx*2 + 1], tree[idx*2+2]); //calcula el padre de los  2 hijos (izquierda y derecha) con la operacion determinada
    }

    //funcion recursiva "query" para hacer consultas en rango
    treetype query(
        int l, //limite izquierdo de la consulta, ej: 2.
        int r, //limite derecho de la consulta, ej: 4. O sea en este caso la consulta seria [2:4]
        int idx, //el indice actual de la iteracion
        int lx, //limite izquierdo del arreglo original, cambia en cada iteracion
        int rx //limite derecho del arreglo original, cambia en cada iteracion
        ) {
        if(rx < l || r < lx){return DEFAULT;} // si el rango en el que esta buscando actualmente no tiene nada que ver, retorna DEFAULT para ignorarlo
        if(l <= lx && rx <= r){return tree[idx];} //si esta dentro del rango que se esta buscando, retorna el indice
        int mid = (lx + rx)/2; //calcula la mitad para dividir lado izquierdo y derecho
        treetype left = query(l, r, idx*2 + 1, lx, mid); //busca recursivamente por la izquierda
        treetype right = query(l, r, idx*2 + 2, mid + 1, rx); //busca recursivamente por la derecha
        return operation(left, right); //retorna la operacion de la busqueda por izquierda y derecha (para los casos de solapamiento)
    }

    //funcion recursiva para actualizar (y eliminar) elementos del array
    void update(
        int pos, //la posicion del elemento en el arreglo original que quieres actualizar
        T val, //valor al cual se quiere actualizar
        int idx, //el indice actual de la iteracion
        int lx, //limite izquierdo del arreglo original, cambia en cada iteracion
        int rx //limite derecho del arreglo original, cambia en cada iteracion
        ) {
        if(lx == rx) {tree[idx] = treetype(val);} //parecido a build, si los limites coinciden entonces se actualiza el valor
        else {
            int mid = (lx + rx)/2; // calcula la mitad para dividir lado izquierdo y derecho
            if(pos <= mid){update(pos, val, idx*2 + 1, lx, mid);} //si la posicion que se quiere actulizar es menor o igual a la mitad, sigue buscando por la izuqierda
            else {update(pos, val, idx*2 + 2, mid + 1, rx);} //caso contrario busca por la derecha
            tree[idx] = operation(tree[idx*2 + 1], tree[idx*2 + 2]); //de ahi actualiza el padre del dato actualizado y asi con todos los nodos que deberian que usan su valor para definirse
        }
    }

public:
    //construccion
    //constructor explicito
    explicit segment_tree(
        const vector<T>& nums, // arreglo original, ej: {1, 3, 5, 2}
        const function < treetype(const treetype&, const treetype&) >& op = [](const treetype& a, const treetype& b) { return a + b; }, //operacion que determinara el funcionamineto del arbol, por defecto es suma
        treetype def = 0 //valor default dependiendo del tipo de segment tree, por defecto es 0 (segment tree de sumas en rango)
        ) : operation(op), DEFAULT(def) { //asigna operation y DEFAULT
        original_array = nums;
        size_og = nums.size(); //tamanio original del arreglo
        while (size < nums.size()) size *= 2; //calculo para obtener el tamanio del arbol
        tree.resize(2*size - 1, 0); //se redimensiona el arbol con el tamanio calculado * 2 - 1 (cantidad total de posibles nodos)
        build(nums, 0, 0, size_og - 1); // llama a la funcion recursiva "build" y le manda el arreglo original, el idx 0 y los limites izquierdo y derecho del arreglo original
    }

    //consulta en rango
    treetype query(
        int l, // limite izquierdo de la consulta
        int r // limite derecho de la consulta
        //por ejemplo, aqui estaria pasando [3, 5]
        ) {
        return query(l, r, 0, 0, size_og - 1); //llama a la funcion recursiva "query" que esta en private y le manda los parametros
    }

    //actualizacion
    void update(
        int pos, //posicion del valor que se quiere actualizar
        T val //valor al cual se quiere actualizar
        ) {
        if(pos >= 0 && pos < size_og) {
            original_array[pos] = val;
            update(pos, val, 0, 0, size_og - 1); //llama a la funcion recursiva "update" que esta en private y le manda los parametros
        }
    }

    //eliminacion
    /*
     * Para realizar una eliminacion en un segment tree se utiliza la actualizacion y se cambia el valor del elemento a 0.
     * Esto es asi para evitar tener que reconstruir el arbol y mantener la eficiencia en las consultas.
     */
    void eliminate(
        int pos //posicion del elemento que se quiere eliminar
        ) {
        if(pos >= 0 && pos < size_og) {
            original_array[pos] = DEFAULT;
            update(pos, DEFAULT, 0, 0, size_og - 1); //llama a la funcion recursiva "update" que esta en private y le manda los parametros. El valor a actualizar es el DEFAULT
        }
    }

    // Añadir nuevo valor al final
    void push_back(T val) {
        original_array.push_back(val);
        rebuild();
    }

    // Reconstruir el árbol
    void rebuild() {
        size_og = original_array.size();
        size = 1;
        while (size < original_array.size()) size *= 2;
        tree.clear();
        tree.resize(2*size - 1, DEFAULT);
        build(original_array, 0, 0, size_og - 1);
    }

    const vector<treetype>& getTree() const { return tree; }
    int getOriginalSize() const { return size_og; }
    const vector<T>& getOriginalArray() const { return original_array; }
};

#endif //PRUEBA2_SEGMENT_TREE_H