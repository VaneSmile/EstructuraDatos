#include <stdio.h>
#include <stdlib.h>

//Estructura para representar un cliente
typedef struct {
    int numeroCuenta;
    int tipoCuenta; // 1 para preferencial, 2 para tradicional
    int turno; // Número de turno asignado
} Cliente;

//Nodo de la cola
typedef struct Nodo {
    Cliente cliente;
    struct Nodo* siguiente;
} Nodo;

//Cola para clientes tradicionales
Nodo* colaTradicional = NULL;

//Cola para clientes preferenciales
Nodo* colaPreferencial = NULL;

//Funcion para encolar un cliente
void in(Nodo** cola, Cliente cliente) {
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->cliente = cliente;
    nuevoNodo->siguiente = NULL;

    if (*cola == NULL) {
        *cola = nuevoNodo;
    } else {
        Nodo* ultimo = *cola;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = nuevoNodo;
    }
}

// Funcion para desencolar un cliente
Cliente out(Nodo** cola) {
    if (*cola == NULL) {
        Cliente clienteVacio = {0, 0, 0};
        return clienteVacio;
    }

    Nodo* primerNodo = *cola;
    Cliente cliente = primerNodo->cliente;
    *cola = primerNodo->siguiente;
    free(primerNodo);
    return cliente;
}

// Funcion para transferir clientes según la condición
void TransCliente() {
    int numClientesTradicional = 0;
    int numClientesPreferencial = 0;

    Nodo* temp = colaTradicional;
    while (temp != NULL) {
        numClientesTradicional++;
        temp = temp->siguiente;
    }

    temp = colaPreferencial;
    while (temp != NULL) {
        numClientesPreferencial++;
        temp = temp->siguiente;
    }

    if (numClientesTradicional >= 5 && numClientesPreferencial <= 2) {
        // Mover el tercer cliente de la fila tradicional a la fila preferencial
        if (numClientesTradicional >= 3) {
            Cliente clienteMovido = out(&colaTradicional);
            in(&colaPreferencial, clienteMovido);
        }
    }
}

// Funcion para eliminar un cliente de la fila tradicional
void eliminarClienteTradicional(int numeroCuenta) {
    Nodo* temp = colaTradicional;
    Nodo* prev = NULL;

    while (temp != NULL) {
        if (temp->cliente.numeroCuenta == numeroCuenta) {
            if (prev == NULL) {
                colaTradicional = temp->siguiente;
            } else {
                prev->siguiente = temp->siguiente;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->siguiente;
    }
}

// Funcion para atender al cliente
void atenderCliente(Nodo** cola) {
    Cliente clienteAtendido = out(cola);
    if (clienteAtendido.numeroCuenta != 0) {
        printf("Cliente atendido: Cuenta %d (Tipo %d, Turno %d)\n", clienteAtendido.numeroCuenta, clienteAtendido.tipoCuenta, clienteAtendido.turno);
        // Reasignar turnos
        Nodo* temp = *cola;
        int nuevoTurno = 1;
        while (temp != NULL) {
            temp->cliente.turno = nuevoTurno;
            temp = temp->siguiente;
            nuevoTurno++;
        }
    } else {
        printf("No hay clientes en la fila.\n");
    }
}

// Funcion para visualizar los clientes en una fila
void visualizarFila(Nodo* cola) {
    Nodo* temp = cola;
    int turno = 1;
    while (temp != NULL) {
        printf("Turno %d: Cuenta %d (Tipo %d)\n", turno, temp->cliente.numeroCuenta, temp->cliente.tipoCuenta);
        temp = temp->siguiente;
        turno++;
    }
    
}

int main() {
    int opcion;
    Cliente nuevoCliente;

    do {
        printf("\n--- Menú ---\n");
        printf("1. Registrar cliente\n");
        printf("2. Visualizar filas\n");
        printf("3. Salir\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Número de cuenta: ");
                scanf("%d", &nuevoCliente.numeroCuenta);
                printf("Tipo de cuenta (1 para preferencial, 2 para tradicional): ");
                scanf("%d", &nuevoCliente.tipoCuenta);
                if (nuevoCliente.tipoCuenta == 1) {
                    in(&colaPreferencial, nuevoCliente);
                } else if (nuevoCliente.tipoCuenta == 2) {
                    in(&colaTradicional, nuevoCliente);
                }
                TransCliente();

                break;
            case 2:
                printf("\nFila Preferencial:\n");
                visualizarFila(colaPreferencial);
                printf("\nFila Tradicional:\n");
                visualizarFila(colaTradicional);
                break;
            case 3:
                printf("¡Hasta luego!\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 3);

    return 0;
}
