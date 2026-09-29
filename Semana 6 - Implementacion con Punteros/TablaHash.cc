#include <iostream>
#include <string>

using namespace std;

// Estructura para los nodos de la lista enlazada
struct NodoHash {
    string clave;
    int valor;
    NodoHash* siguiente; // Puntero al siguiente nodo en caso de colisión

    // Constructor del nodo
    NodoHash(string c, int v) {
        clave = c;
        valor = v;
        siguiente = nullptr;
    }
};

class TablaHash {
private:
    NodoHash** tabla; // Puntero doble: Un arreglo de punteros a NodoHash
    int capacidad;

    // Función hash privada
    int funcionHash(string clave) {
        int suma = 0;
        for (char c : clave) {
            suma += c;
        }
        return suma % capacidad; // Asegura que el índice esté en rango
    }

public:
    // Constructor
    TablaHash(int cap) {
        capacidad = cap;
        tabla = new NodoHash*[capacidad]; // Crear el arreglo de punteros

        // Inicializar todos los punteros a nullptr (vacíos)
        for (int i = 0; i < i < capacidad; i++) {
            tabla[i] = nullptr;
        }
    }

    // Método de Inserción
    void insertar(string clave, int valor) {
        int indice = funcionHash(clave);
        NodoHash* nuevoNodo = new NodoHash(clave, valor);

        // Si no hay colisión, es el primer elemento
        if (tabla[indice] == nullptr) {
            tabla[indice] = nuevoNodo;
        } else {
            // Manejo de colisión: Enlazar al inicio de la lista
            nuevoNodo->siguiente = tabla[indice];
            tabla[indice] = nuevoNodo;
        }
    }

    // Método de Búsqueda
    int buscar(string clave) {
        int indice = funcionHash(clave);
        NodoHash* actual = tabla[indice];

        // Recorrer la lista enlazada usando el puntero
        while (actual != nullptr) {
            if (actual->clave == clave) {
                return actual->valor; // Se encontró
            }
            actual = actual->siguiente; // Avanzar
        }
        return -1; // -1 indica que no se encontró
    }

    // Método de Eliminación
    void eliminar(string clave) {
        int indice = funcionHash(clave);
        NodoHash* actual = tabla[indice];
        NodoHash* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->clave == clave) {
                // Si el nodo a eliminar es el primero (cabeza de la lista)
                if (anterior == nullptr) {
                    tabla[indice] = actual->siguiente;
                } else {
                    // Si el nodo está en medio o al final
                    anterior->siguiente = actual->siguiente;
                }
                delete actual; // Liberar memoria del puntero
                cout << "Clave " << clave << " eliminada" << endl;
                return;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        cout << "Clave no encontrada para eliminar." << endl;
    }

    // Destructor (Prevención de Fugas de Memoria)
    ~TablaHash() {
        for (int i = 0; i < capacidad; i++) {
            NodoHash* actual = tabla[i];
            while (actual != nullptr) {
                NodoHash* temporal = actual;
                actual = actual->siguiente;
                delete temporal; // Liberar cada nodo
            }
        }
        delete[] tabla; // Liberar el arreglo dinámico de punteros
    }
};

int main() {
    TablaHash miTabla(10); // Tabla con capacidad de 10 buckets

    // 1. Insertar elementos
    miTabla.insertar("Ana", 25);
    miTabla.insertar("Pedro", 30);
    miTabla.insertar("Juan", 40);

    // Forzando colisiones si Ana y Naa generan el mismo hash
    miTabla.insertar("Naa", 50);

    // 2. Buscar elementos
    cout << "Edad de Juan: " << miTabla.buscar("Juan") << endl;
    cout << "Edad de Ana: " << miTabla.buscar("Ana") << endl;
    cout << "Edad de Carlos: " << miTabla.buscar("Carlos") << endl; // Retorna -1

    // 3. Eliminar elementos
    miTabla.eliminar("Ana");
    cout << "Busqueda post-eliminacion (Ana): " << miTabla.buscar("Ana") << endl;

    return 0; // El destructor se llama automáticamente aquí
}