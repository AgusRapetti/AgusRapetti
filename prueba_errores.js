// Función con variables no declaradas
function variablesNoDeclaradas() {
    a = 5;  // 'a' no está declarada
    b = 10;  // 'b' no está declarada
    console.log(a + b);
}

// Función que provoca errores por uso de 'eval'
function usoDeEval() {
    let userCode = "console.log('Este es un código inseguro')";
    eval(userCode);  // Uso de eval es inseguro y propenso a vulnerabilidades
}

// Función que tiene código duplicado
function codigoDuplicado() {
    let x = 5;
    let y = 10;
    console.log("La suma es: " + (x + y));
    
    let a = 5;
    let b = 10;
    console.log("La suma es: " + (a + b));  // Código duplicado
}

// Función con una complejidad ciclomática alta
function logicaCompleja(valor) {
    if (valor > 10) {
        if (valor < 20) {
            if (valor % 2 === 0) {
                console.log("Valor par y entre 10 y 20");
            } else {
                console.log("Valor impar y entre 10 y 20");
            }
        } else {
            console.log("Valor mayor o igual a 20");
        }
    } else {
        if (valor < 0) {
            console.log("Valor negativo");
        } else {
            console.log("Valor entre 0 y 10");
        }
    }
}

variablesNoDeclaradas();
usoDeEval();
codigoDuplicado();
logicaCompleja(15);
