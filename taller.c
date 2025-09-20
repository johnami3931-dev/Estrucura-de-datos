#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// --- ¡Hola! Aquí vamos a pedirte los números ---
// Esta función, 'llenarArray', se encarga de que ingreses 10 números enteros.
// Es un poco estricta: si no pones un número, te pedirá que lo hagas de nuevo hasta que lo logres.
void llenarArray(int arr[], int n) {
    cout << "¡Hola! Por favor, dame " << n << " numeros enteros para que podamos jugar con ellos." << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Vamos con el numero " << i + 1 << ": ";
        // Este bucle es para asegurarnos de que solo ingreses numeros validos.
        while (!(cin >> arr[i])) {
            cout << "¡Oops! Eso no parece un numero. Intenta de nuevo: ";
            // Limpiamos lo que pusiste mal.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// --- ¡El primer truco de magia! Selection Sort (orden ascendente) ---
// Con esta funcion, 'ordenarSelectionAscendente', vamos a ordenar los numeros de mas pequeno a mas grande.
// Lo hace buscando el numero mas pequeno y poniendolo al principio, luego el siguiente mas pequeno, y asi sucesivamente.
// ¡Y te mostramos como va el proceso despues de cada cambio!
void ordenarSelectionAscendente(int arr[], int n) {
    cout << "\n¡Mira el proceso de Selection Sort! Estamos ordenando de menor a mayor." << endl;
    for (int i = 0; i < n - 1; ++i) {
        // Encontramos el numero mas pequeno que queda.
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // ¡Lo ponemos en su lugar!
        swap(arr[min_idx], arr[i]);
        
        cout << "Despues del paso " << i + 1 << " el arreglo se ve asi: ";
        for (int k = 0; k < n; ++k) {
            cout << arr[k] << " ";
        }
        cout << endl;
    }
}

// --- ¡Ahora el segundo truco! Insertion Sort (orden descendente) ---
// 'ordenarInsertionDescendente' hace lo contrario: ordena los numeros de mas grande a mas pequeno.
// Es como ordenar cartas en la mano: tomas una nueva y la pones en el lugar correcto entre las que ya tienes.
// Tambien te mostramos como va quedando todo.
void ordenarInsertionDescendente(int arr[], int n) {
    cout << "\n¡Ahora, el truco de Insertion Sort! Vamos a ordenar de mayor a menor." << endl;
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        
        // Movemos los numeros que son mas pequenos para hacerle espacio al que estamos ordenando.
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;

        cout << "Paso " << i << ": ";
        for (int k = 0; k < n; ++k) {
            cout << arr[k] << " ";
        }
        cout << endl;
    }
}

// --- ¡El menu, tu centro de control! ---
// Esta funcion es el cerebro del programa. Aqui eliges que quieres hacer.
// Y no te preocupes, siempre guardamos una copia de tus numeros originales para que puedas jugar con ellos cuantas veces quieras sin perderlos.
void mostrarMenu(int arr[], int n) {
    int opcion;
    do {
        cout << "\n--- ¿Que quieres hacer ahora? ---" << endl;
        cout << "1. Ver mis numeros como los ingrese" << endl;
        cout << "2. Ver la magia de Selection Sort (ascendente)" << endl;
        cout << "3. Ver el arte de Insertion Sort (descendente)" << endl;
        cout << "4. Ya no quiero jugar mas :(" << endl;
        cout << "Escribe tu eleccion: ";
        cin >> opcion;

        // Hacemos una copia para no dañar los numeros originales.
        int tempArr[n];
        copy(arr, arr + n, tempArr);

        switch (opcion) {
            case 1:
                cout << "Tus numeros son: ";
                for (int i = 0; i < n; ++i) {
                    cout << arr[i] << " ";
                }
                cout << endl;
                break;
            case 2:
                ordenarSelectionAscendente(tempArr, n);
                cout << "¡Y asi queda al final! (Selection Sort): ";
                for (int i = 0; i < n; ++i) {
                    cout << tempArr[i] << " ";
                }
                cout << endl;
                break;
            case 3:
                ordenarInsertionDescendente(tempArr, n);
                cout << "¡Y asi queda al final! (Insertion Sort): ";
                for (int i = 0; i < n; ++i) {
                    cout << tempArr[i] << " ";
                }
                cout << endl;
                break;
            case 4:
                cout << "¡Adios! Gracias por jugar conmigo." << endl;
                break;
            default:
                cout << "Esa opcion no existe. Elige una de la lista." << endl;
                break;
        }
    } while (opcion != 4);
}

// --- ¡La funcion principal! Aqui empieza todo ---
int main() {
    const int TAMANO = 10;
    int arr[TAMANO];

    // Primero, te pido los numeros.
    llenarArray(arr, TAMANO);
    // Luego, te muestro el menu para que elijas que hacer.
    mostrarMenu(arr, TAMANO);

    return 0;
}
//KIARA MARCELA GARCIA CAICEDO,JHON JAIRO MINA GARCES