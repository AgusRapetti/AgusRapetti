#include <iostream>
#include <cstring>

// Función que provoca una fuga de memoria
void fugaDeMemoria() {
    int* ptr = new int[10];  // Asignación de memoria sin liberar
}

// Función que usa una variable no inicializada
void variableNoInicializada() {
    int x;  // Variable no inicializada
    std::cout << "Valor de x: " << x << std::endl;  // Uso de variable no inicializada
}

// Función insegura que copia cadenas
void copiaInsegura() {
    char fuente[10] = "Hola";
    char destino[5];  // El destino es más pequeño que la fuente
    std::strcpy(destino, fuente);  // Posible desbordamiento de buffer
}

// Función con código duplicado
void codigoDuplicado() {
    int a = 5;
    int b = 10;
    std::cout << "La suma es: " << a + b << std::endl;

    int c = 5;
    int d = 10;
    std::cout << "La suma es: " << c + d << std::endl;  // Código duplicado
}

int main() {
    fugaDeMemoria();
    variableNoInicializada();
    copiaInsegura();
    codigoDuplicado();
    
    return 0;
}
