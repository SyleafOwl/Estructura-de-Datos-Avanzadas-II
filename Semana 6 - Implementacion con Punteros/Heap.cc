#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

// PASO 1: Estructura del NodoHeap
struct NodoHeap {
    int valor;
    NodoHeap* izq;
    NodoHeap* der;
    NodoHeap* padre; // Crucial para flotar valores hacia arriba

    // Constructor
    NodoHeap(int v) {
        valor = v;
        izq = nullptr;
        der = nullptr;
        padre = nullptr;
    }
};

// PASO 2: La Clase y la Navegacion
class MaxHeapPunteros {
private:
    NodoHeap* raiz;
    int cantidadNodos; // Mantiene el tamano del heap

    // Funcion clave para encontrar cualquier nodo en un Arbol Completo
    NodoHeap* obtenerNodo(int n) {
        vector<int> camino;
        // Construimos el camino desde el nodo hasta la raiz
        while (n > 1) {
            camino.push_back(n % 2);
            n = n / 2;
        }

        NodoHeap* actual = raiz;
        // Recorremos el camino al reves (desde la raiz hacia abajo)
        for (int i = camino.size() - 1; i >= 0; i--) {
            if (camino[i] == 0) actual = actual->izq;
            else actual = actual->der;
        }
        return actual;
    }

    // PASO 3: Sift Up (Flotar hacia arriba)
    void siftUp(NodoHeap* nodo) {
        // Mientras tenga padre y su valor sea mayor al de su padre
        while (nodo->padre != nullptr && nodo->valor > nodo->padre->valor) {
            swap(nodo->valor, nodo->padre->valor); // Intercambiamos solo valores
            nodo = nodo->padre; // Subimos al padre
        }
    }

    // PASO 4: Sift Down (Hundir hacia abajo)
    void siftDown(NodoHeap* nodo) {
        while (nodo->izq != nullptr) { // Mientras tenga al menos un hijo
            NodoHeap* mayor = nodo->izq;
            
            // Si tiene hijo derecho y es mayor que el izquierdo
            if (nodo->der != nullptr && nodo->der->valor > mayor->valor) { 
                mayor = nodo->der; 
            }
            
            // Si el nodo actual es mayor o igual al hijo mayor, terminamos
            if (nodo->valor >= mayor->valor) break;
            
            swap(nodo->valor, mayor->valor); // Hundimos el valor
            nodo = mayor;
        }
    }

    // PASO 5: Funcion recursiva para el Destructor
    void limpiarMemoria(NodoHeap* nodo) {
        if (nodo == nullptr) return;
        limpiarMemoria(nodo->izq);
        limpiarMemoria(nodo->der);
        delete nodo;
    }

public:
    MaxHeapPunteros() : raiz(nullptr), cantidadNodos(0) {}

    // PASO 3: Insercion
    void insertar(int valor) {
        NodoHeap* nuevo = new NodoHeap(valor);
        cantidadNodos++;

        if (cantidadNodos == 1) {
            raiz = nuevo;
            return;
        }

        // El padre del nuevo nodo estara en la posicion (cantidadNodos/2)
        NodoHeap* padre = obtenerNodo(cantidadNodos / 2);

        if (cantidadNodos % 2 == 0) padre->izq = nuevo; // Par = Hijo izquierdo
        else padre->der = nuevo; // Impar = Hijo derecho

        nuevo->padre = padre;
        siftUp(nuevo); // Restablecer propiedad de Max-Heap
    }

    // PASO 4: Extraccion del Maximo
    int extraerMax() {
        if (cantidadNodos == 0) throw runtime_error("Heap vacio");

        int valorMaximo = raiz->valor;

        if (cantidadNodos == 1) {
            delete raiz;
            raiz = nullptr;
            cantidadNodos--;
            return valorMaximo;
        }

        // Obtener el ultimo nodo
        NodoHeap* ultimo = obtenerNodo(cantidadNodos);

        // Mover el ultimo valor a la raiz
        swap(raiz->valor, ultimo->valor);

        // Desconectar el ultimo nodo de su padre
        if (ultimo->padre->izq == ultimo) ultimo->padre->izq = nullptr;
        else ultimo->padre->der = nullptr;

        delete ultimo; // Liberar memoria
        cantidadNodos--;

        siftDown(raiz); // Restablecer propiedad de Max-Heap
        return valorMaximo;
    }

    // PASO 5: Destructor
    ~MaxHeapPunteros() { 
        limpiarMemoria(raiz); 
    }
};

// PASO 5: Programa Principal
int main() {
    MaxHeapPunteros heap;
    
    cout << "Insertando: 15, 10, 40, 50, 30" << endl;
    heap.insertar(15);
    heap.insertar(10);
    heap.insertar(40);
    heap.insertar(50);
    heap.insertar(30);

    cout << "Extrayendo elementos (debe salir ordenado de mayor a menor):" << endl;
    for(int i = 0; i < 5; i++) {
        cout << heap.extraerMax() << " ";
    }
    cout << endl;
    
    return 0;
}