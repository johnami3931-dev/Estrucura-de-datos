#include <stdio.h> // La única biblioteca que necesitamos para imprimir (printf) y leer (scanf)

#define TAMANO 10 // Para no escribir '10' por todos lados. Si el profe pide cambiarlo, solo lo cambio aquí.

// --- Prototipos: Un índice de las funciones que vamos a usar ---
void mostrarMenu();
void llenarArray(int numeros[], int n);
void mostrarArray(int numeros[], int n, const char* mensaje);
void ordenarConSelection(int numeros[], int n);
void ordenarConInsertion(int numeros[], int n);

// --- La función principal donde todo comienza ---c
int main() {
    int tiemposOriginales[TAMANO];
    int tiemposCopia[TAMANO]; // Un segundo arreglo para no dañar el original [cite: 23]
    int opcion;
    int datosListos = 0; // Una "banderita" para saber si ya ingresaron los números

    do {
        mostrarMenu(); // Mostramos el menú [cite: 22]
        scanf("%d", &opcion);

        // Dependiendo de lo que elija el usuario...
        switch (opcion) {
            case 1:
                llenarArray(tiemposOriginales, TAMANO); // Llamamos a la función para llenar el arreglo
                datosListos = 1; // Levantamos la bandera
                break;

            case 2:
                if (datosListos == 1) {
                    mostrarArray(tiemposOriginales, TAMANO, "Arreglo Original");
                } else {
                    printf("\n>> Primero tienes que ingresar los tiempos en la opcion 1.\n");
                }
                break;

            case 3: // Ordenar con Selection Sort
                if (datosListos == 1) {
                    // 1. Copiamos los números originales a la copia para no perderlos
                    for (int i = 0; i < TAMANO; i++) {
                        tiemposCopia[i] = tiemposOriginales[i];
                    }
                    // 2. Ahora sí, ordenamos la copia
                    ordenarConSelection(tiemposCopia, TAMANO);
                } else {
                    printf("\n>> Primero tienes que ingresar los tiempos en la opcion 1.\n");
                }
                break;

            case 4: // Ordenar con Insertion Sort
                if (datosListos == 1) {
                    // 1. Hacemos lo mismo: copiar para no dañar el original
                    for (int i = 0; i < TAMANO; i++) {
                        tiemposCopia[i] = tiemposOriginales[i];
                    }
                    // 2. Ordenamos la copia
                    ordenarConInsertion(tiemposCopia, TAMANO);
                } else {
                    printf("\n>> Primero tienes que ingresar los tiempos en la opcion 1.\n");
                }
                break;

            case 5:
                printf("\n¡Nos vemos!\n");
                break;

            default:
                printf("\n>> Esa opcion no existe. Intenta de nuevo.\n");
        }

    } while (opcion != 5); // El menú se repite hasta que elijan 5 para salir

    return 0;
}

// --- Implementación de las funciones ---

// Función para mostrar el menú, nada complicado
void mostrarMenu() {
    printf("\n\n------ TALLER 1: TIEMPOS DE SOPORTE ------\n");
    printf("1. Ingresar 10 tiempos\n");
    printf("2. Mostrar tiempos originales\n");
    printf("3. Ordenar de menor a mayor (Selection Sort)\n");
    printf("4. Ordenar de mayor a menor (Insertion Sort)\n");
    printf("5. Salir\n");
    printf("Elige una opcion: ");
}

// Función para pedir los 10 números [cite: 13]
void llenarArray(int numeros[], int n) {
    printf("\n--- Ingresa los 10 tiempos ---\n");
    for (int i = 0; i < n; i++) {
        int num;
        do {
            printf("Tiempo #%d: ", i + 1);
            scanf("%d", &num);
            if (num <= 0) {
                // Si el número no es positivo, regañamos y lo pedimos otra vez [cite: 14]
                printf("  >> Error: El tiempo debe ser positivo. Intenta de nuevo.\n");
            }
        } while (num <= 0); // El bucle se repite si el número es inválido
        numeros[i] = num;
    }
    printf("\n¡Tiempos guardados!\n");
}

// Una función simple para mostrar cualquier arreglo
void mostrarArray(int numeros[], int n, const char* mensaje) {
    printf("\n--- %s ---\n[ ", mensaje);
    for (int i = 0; i < n; i++) {
        printf("%d ", numeros[i]);
    }
    printf("]\n");
}

// Función que ordena de menor a mayor con Selection Sort [cite: 8, 16]
void ordenarConSelection(int numeros[], int n) {
    printf("\n--- Proceso Selection Sort (menor a mayor) ---\n");
    int paso = 1;

    for (int i = 0; i < n - 1; i++) {
        // Buscamos el número más pequeño del resto del arreglo
        int indiceDelMinimo = i;
        for (int j = i + 1; j < n; j++) {
            if (numeros[j] < numeros[indiceDelMinimo]) {
                indiceDelMinimo = j;
            }
        }

        // Si encontramos uno más pequeño, lo intercambiamos
        if (indiceDelMinimo != i) {
            int temp = numeros[i];
            numeros[i] = numeros[indiceDelMinimo];
            numeros[indiceDelMinimo] = temp;
        }

        // Mostramos cómo quedó el arreglo después del intercambio, como pide el profe [cite: 17]
        printf("Paso %d: [ ", paso++);
        for(int k=0; k<n; k++) { printf("%d ", numeros[k]); }
        printf("]\n");
    }
    mostrarArray(numeros, n, "Resultado Final con Selection Sort");
}

// Función que ordena de mayor a menor con Insertion Sort [cite: 9, 19]
void ordenarConInsertion(int numeros[], int n) {
    printf("\n--- Proceso Insertion Sort (mayor a menor) ---\n");
    int paso = 1;

    // Empezamos desde el segundo elemento
    for (int i = 1; i < n; i++) {
        int llave = numeros[i]; // La "carta" que queremos ordenar
        int j = i - 1;

        // Movemos los elementos que son más pequeños que nuestra "carta"
        // hacia la derecha para hacerle espacio.
        while (j >= 0 && numeros[j] < llave) {
            numeros[j + 1] = numeros[j];
            j = j - 1;
        }
        numeros[j + 1] = llave; // Ponemos la "carta" en su lugar

        // Mostramos cómo quedó el arreglo después de la inserción [cite: 20]
        printf("Paso %d: [ ", paso++);
        for(int k=0; k<n; k++) { printf("%d ", numeros[k]); }
        printf("]\n");
    }
    mostrarArray(numeros, n, "Resultado Final con Insertion Sort");
}