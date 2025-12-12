#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras de Datos

// Nodo de la Cola FIFO (Pedidos)
typedef struct NodoCola {
    char destino[50];
    int cantidad_solicitada;
    struct NodoCola *siguiente;
} NodoCola;

// Nodo del Árbol AVL (Inventario/Lotes)
typedef struct NodoAVL {
    long fecha_vencimiento;         // Clave (ID): AAAAMMDD
    int stock_total;                // Cantidad disponible
    char nombre_producto[50];       // Nombre producto
    NodoCola *cabeza_pedidos;       // Puntero al inicio de la cola
    int altura;                     // Para balanceo
    struct NodoAVL *izquierda;
    struct NodoAVL *derecha;
} NodoAVL;

// --- Funciones de Utilidad y AVL (Abreviadas) ---

// Auxiliar: Altura y Factor de Balanceo
int max(int a, int b) { return (a > b) ? a : b; }
int altura(NodoAVL *N) { return (N == NULL) ? 0 : N->altura; }
int factor_balanceo(NodoAVL *N) { return (N == NULL) ? 0 : altura(N->izquierda) - altura(N->derecha); }
void actualizar_altura(NodoAVL *N) { N->altura = max(altura(N->izquierda), altura(N->derecha)) + 1; }

// Creación de Nodos
NodoAVL *nuevo_nodo_avl(long fecha, int cantidad, const char *producto) {
    NodoAVL *nodo = (NodoAVL *)malloc(sizeof(NodoAVL));
    if (!nodo) { perror("Error en malloc AVL"); exit(EXIT_FAILURE); }
    nodo->fecha_vencimiento = fecha;
    nodo->stock_total = cantidad;
    strcpy(nodo->nombre_producto, producto);
    nodo->cabeza_pedidos = NULL;
    nodo->izquierda = nodo->derecha = NULL;
    nodo->altura = 1;
    return nodo;
}

// Rotaciones y Balanceo
NodoAVL *rotacion_derecha(NodoAVL *y) {
    NodoAVL *x = y->izquierda;
    y->izquierda = x->derecha;
    x->derecha = y;
    actualizar_altura(y);
    actualizar_altura(x);
    return x;
}

NodoAVL *rotacion_izquierda(NodoAVL *x) {
    NodoAVL *y = x->derecha;
    x->derecha = y->izquierda;
    y->izquierda = x;
    actualizar_altura(x);
    actualizar_altura(y);
    return y;
}

NodoAVL *balancear(NodoAVL *nodo) {
    if (!nodo) return nodo;
    actualizar_altura(nodo);
    int balance = factor_balanceo(nodo);

    if (balance > 1) { // Desbalanceo Izquierda
        if (factor_balanceo(nodo->izquierda) < 0)
            nodo->izquierda = rotacion_izquierda(nodo->izquierda); // Caso LR
        return rotacion_derecha(nodo); // Caso LL
    }
    if (balance < -1) { // Desbalanceo Derecha
        if (factor_balanceo(nodo->derecha) > 0)
            nodo->derecha = rotacion_derecha(nodo->derecha); // Caso RL
        return rotacion_izquierda(nodo); // Caso RR
    }
    return nodo;
}

// Inserción en AVL
NodoAVL *insertar_avl(NodoAVL *nodo, long fecha, int cantidad, const char *producto, int *exito) {
    if (nodo == NULL) {
        *exito = 1; // Éxito
        return nuevo_nodo_avl(fecha, cantidad, producto);
    }
    if (fecha == nodo->fecha_vencimiento) {
        *exito = -1; // Ya existe
        return nodo;
    }

    if (fecha < nodo->fecha_vencimiento)
        nodo->izquierda = insertar_avl(nodo->izquierda, fecha, cantidad, producto, exito);
    else
        nodo->derecha = insertar_avl(nodo->derecha, fecha, cantidad, producto, exito);

    return balancear(nodo);
}

// Auxiliar para Eliminación y Despacho
NodoAVL *encontrar_minimo(NodoAVL *nodo) {
    NodoAVL *actual = nodo;
    while (actual && actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}

// Prototipos de Cola (para uso en AVL)
void liberar_cola(NodoCola *cabeza);
int contar_pedidos(NodoCola *cabeza);

// Eliminación en AVL (Baja de Producto)
NodoAVL *eliminar_avl(NodoAVL *raiz, long clave, int *exito) {
    if (raiz == NULL) { *exito = 0; return NULL; }

    if (clave < raiz->fecha_vencimiento)
        raiz->izquierda = eliminar_avl(raiz->izquierda, clave, exito);
    else if (clave > raiz->fecha_vencimiento)
        raiz->derecha = eliminar_avl(raiz->derecha, clave, exito);
    else { // Nodo encontrado
        *exito = 1;
        liberar_cola(raiz->cabeza_pedidos); // Liberar memoria de cola

        if (!raiz->izquierda || !raiz->derecha) {
            NodoAVL *temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;
            if (temp == NULL) { temp = raiz; raiz = NULL; }
            else { *raiz = *temp; }
            free(temp);
        } else {
            NodoAVL *temp = encontrar_minimo(raiz->derecha);
            raiz->fecha_vencimiento = temp->fecha_vencimiento;
            raiz->stock_total = temp->stock_total;
            strcpy(raiz->nombre_producto, temp->nombre_producto);
            // La cola no se copia, se queda en el nodo AVL
            raiz->derecha = eliminar_avl(raiz->derecha, temp->fecha_vencimiento, exito);
        }
    }
    if (raiz == NULL) return raiz;
    return balancear(raiz); // Balancear
}

// --- Funciones de Cola (FIFO) ---

// Creación de Nodo de Cola
NodoCola *nuevo_nodo_cola(const char *destino, int cantidad) {
    NodoCola *nodo = (NodoCola *)malloc(sizeof(NodoCola));
    if (!nodo) { perror("Error en malloc Cola"); exit(EXIT_FAILURE); }
    strcpy(nodo->destino, destino);
    nodo->cantidad_solicitada = cantidad;
    nodo->siguiente = NULL;
    return nodo;
}

// Liberar la memoria de toda una cola
void liberar_cola(NodoCola *cabeza) {
    NodoCola *actual = cabeza;
    NodoCola *siguiente;
    while (actual) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

// Agregar un pedido al final de la cola (Encolar)
void encolar(NodoAVL *nodo_avl, const char *destino, int cantidad) {
    if (!nodo_avl) return;

    NodoCola *nuevo = nuevo_nodo_cola(destino, cantidad);
    nodo_avl->stock_total -= cantidad; // Descontar stock

    if (nodo_avl->cabeza_pedidos == NULL) {
        nodo_avl->cabeza_pedidos = nuevo;
        return;
    }

    NodoCola *actual = nodo_avl->cabeza_pedidos;
    while (actual->siguiente) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo; // Agregar al final
}

// Contar pedidos en la cola
int contar_pedidos(NodoCola *cabeza) {
    int contador = 0;
    NodoCola *actual = cabeza;
    while (actual) {
        contador++;
        actual = actual->siguiente;
    }
    return contador;
}

// Cancelar un pedido específico (Eliminación en la cola)
int cancelar_pedido(NodoAVL *nodo_avl, const char *destino_a_eliminar) {
    if (!nodo_avl || !nodo_avl->cabeza_pedidos) return 0;

    NodoCola *actual = nodo_avl->cabeza_pedidos;
    NodoCola *anterior = NULL;

    // Buscar y eliminar
    while (actual) {
        if (strcmp(actual->destino, destino_a_eliminar) == 0) {
            if (anterior == NULL) { // Es la cabeza
                nodo_avl->cabeza_pedidos = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            nodo_avl->stock_total += actual->cantidad_solicitada; // Restablecer stock
            free(actual); // Liberar nodo de la cola
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return 0; // No se encontró
}

// --- Requerimientos Funcionales (Lógica de Negocio) ---

// 1. Recepción de Mercancía
void recepcion_mercancia(NodoAVL **raiz) {
    long fecha; int cantidad; char producto[50]; int exito = 0;
    printf("\n--- 1. Recepción de Mercancía ---\n");
    printf("Fecha (AAAAMMDD): "); scanf("%ld", &fecha);
    printf("Cantidad: "); scanf("%d", &cantidad);
    printf("Producto: "); while (getchar() != '\n'); fgets(producto, 50, stdin); producto[strcspn(producto, "\n")] = 0;

    *raiz = insertar_avl(*raiz, fecha, cantidad, producto, &exito);

    if (exito == 1) printf("Mercancía insertada y árbol balanceado.\n");
    else if (exito == -1) printf("ERROR: La fecha %ld ya existe. Producto no procesado.\n", fecha);
    else printf("ERROR: No se pudo insertar.\n");
}

// 2. Registrar Pedido de Despacho
void registrar_pedido(NodoAVL *raiz) {
    if (!raiz) { printf("\nInventario vacío.\n"); return; }
    
    NodoAVL *prioritario = encontrar_minimo(raiz); // Lote más próximo a vencer
    if (!prioritario) { printf("Error al encontrar lote prioritario.\n"); return; }

    char destino[50]; int cantidad;
    printf("\n--- 2. Registrar Pedido de Despacho ---\n");
    printf("Lote prioritario: %ld (%s), Stock: %d\n", prioritario->fecha_vencimiento, prioritario->nombre_producto, prioritario->stock_total);
    
    printf("Destino: "); while (getchar() != '\n'); fgets(destino, 50, stdin); destino[strcspn(destino, "\n")] = 0;
    printf("Cantidad solicitada: "); scanf("%d", &cantidad);

    if (cantidad <= 0 || cantidad > prioritario->stock_total) {
        printf("ERROR: Cantidad inválida o insuficiente stock.\n");
        return;
    }

    encolar(prioritario, destino, cantidad); // Encolar y descontar stock
    printf("Pedido a %s por %d unidades registrado. Nuevo stock: %d.\n", destino, cantidad, prioritario->stock_total);
}

// 3.1. Baja de Producto (Eliminación en AVL)
void baja_producto(NodoAVL **raiz) {
    long clave; int exito = 0;
    printf("\n--- 3.1. Baja de Producto (Eliminar Lote) ---\n");
    printf("Fecha del lote a eliminar: "); scanf("%ld", &clave);

    *raiz = eliminar_avl(*raiz, clave, &exito); // Eliminar, liberar cola y balancear

    if (exito == 1) printf("Lote %ld eliminado y memoria liberada.\n", clave);
    else printf("ERROR: Lote %ld no encontrado.\n", clave);
}

// 3.2. Cancelar Pedido (Eliminación en la cola)
void cancelar_pedido_especifico(NodoAVL *raiz) {
    long clave; char destino[50]; int exito = 0;
    printf("\n--- 3.2. Cancelar Pedido Específico ---\n");
    printf("Fecha del lote: "); scanf("%ld", &clave);

    NodoAVL *lote = raiz; // Buscar el nodo AVL por fecha
    while (lote && lote->fecha_vencimiento != clave) {
        lote = (clave < lote->fecha_vencimiento) ? lote->izquierda : lote->derecha;
    }

    if (!lote) { printf("ERROR: Lote %ld no encontrado.\n", clave); return; }
    
    printf("Destino del pedido a cancelar: "); while (getchar() != '\n'); fgets(destino, 50, stdin); destino[strcspn(destino, "\n")] = 0;

    exito = cancelar_pedido(lote, destino); // Buscar, eliminar y restablecer stock

    if (exito) printf("Pedido a %s cancelado. Stock restablecido: %d.\n", destino, lote->stock_total);
    else printf("ERROR: Pedido a %s no encontrado en lote %ld.\n", destino, clave);
}

// 4. Reporte de Estado (Recorrido In-Order)
void in_order_recorrido(NodoAVL *raiz) {
    if (raiz != NULL) {
        in_order_recorrido(raiz->izquierda); // Izquierda: Fechas más antiguas/próximas a vencer
        printf("| %ld | Producto: %s | Stock: %d | Pedidos: %d\n",
               raiz->fecha_vencimiento, raiz->nombre_producto,
               raiz->stock_total, contar_pedidos(raiz->cabeza_pedidos));
        in_order_recorrido(raiz->derecha); // Derecha: Fechas futuras/frescas
    }
}

void reporte_estado(NodoAVL *raiz) {
    printf("\n--- 4. Reporte de Estado ---\n");
    printf("Listado ordenado (In-Order):\n");
    printf("-------------------------------------------------------------------\n");
    in_order_recorrido(raiz);
    printf("-------------------------------------------------------------------\n");
    if (!raiz) printf("El inventario está vacío.\n");
}

// Liberar la memoria total del árbol
void liberar_memoria_total(NodoAVL *raiz) {
    if (raiz == NULL) return;
    liberar_memoria_total(raiz->izquierda);
    liberar_memoria_total(raiz->derecha);
    liberar_cola(raiz->cabeza_pedidos); // Liberar cola primero
    free(raiz); // Luego liberar el nodo AVL
}

// Función Principal (Menú)
int main() {
    NodoAVL *raiz = NULL;
    int opcion;

    printf("Sistema Logístico de Buenaventura (AVL + FIFO)\n");

    do {
        printf("\n--- MENÚ ---\n");
        printf("1. Recepción de Mercancía (Insertar)\n");
        printf("2. Registrar Pedido de Despacho (Encolar)\n");
        printf("3. Cancelación (Submenú)\n");
        printf("4. Reporte de Estado (In-Order)\n");
        printf("0. Salir\n");
        printf("Opción: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1: recepcion_mercancia(&raiz); break;
            case 2: registrar_pedido(raiz); break;
            case 3: {
                int sub_opcion;
                printf("  3.1. Baja de Producto (Eliminar Lote)\n");
                printf("  3.2. Cancelar Pedido (Eliminar en Cola)\n");
                printf("  Opción 3: ");
                if (scanf("%d", &sub_opcion) == 1) {
                    if (sub_opcion == 1) baja_producto(&raiz);
                    else if (sub_opcion == 2) cancelar_pedido_especifico(raiz);
                    else printf("Opción inválida.\n");
                } else { printf("Entrada inválida.\n"); while (getchar() != '\n'); }
                break;
            }
            case 4: reporte_estado(raiz); break;
            case 0: printf("Saliendo. Liberando memoria...\n"); break;
            default: printf("Opción inválida.\n");
        }
    } while (opcion != 0);

    liberar_memoria_total(raiz);
    printf("Memoria liberada. Finalizado.\n");

    return 0;
}