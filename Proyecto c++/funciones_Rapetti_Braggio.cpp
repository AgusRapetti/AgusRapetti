#include <iostream>
#include <string>
using namespace std;
#define TEXTO_MAX 22

#include <sstream> // Incluye la librería para stringstream

typedef std:: string Cadena;

struct archivo{

    Cadena nombreArchivo;
    Cadena extensionArchivo;
    archivo * sig{};
    archivo * ant{};
    Cadena contenido;
    bool escritura = true;
    bool lectura = true;
    bool cont = false;

};
typedef archivo * Arch;

struct directorio{
    Cadena nombre;
    directorio *pH{};
    directorio *sH{};
    archivo *archivos{};
    //directorio *directorios{};
    int cantArchivos{};
    int cantDirectorios{};
    directorio *padre{};
    string ruta; // Variable para almacenar la ruta actual
};
typedef directorio * dir;

struct _sistema{
    directorio raiz;
    directorio *RAIZ{};
    directorio *ACTUAL{};
};
typedef _sistema * Sistema;

enum _retorno{
    OK, ERROR, NO_IMPLEMENTADA
};
typedef enum _retorno TipoRet;

//----------------------------------------------------------------

void eliminarDirectorioYContenidos(dir directorio);

//----------------------------------------------------------------

TipoRet CREARSISTEMA(Sistema &s){

    s = new _sistema;
    s->raiz.nombre = "RAIZ";
    s->raiz.pH = NULL;
    s->raiz.sH = NULL;
    s->RAIZ = &s->raiz;
    s->ACTUAL = &s->raiz;
    return OK;
}

//----------------------------------------------------------------

TipoRet DESTRUIRSISTEMA(Sistema &s){

    dir DestSistema = s->RAIZ->pH;
    eliminarDirectorioYContenidos(DestSistema);
    eliminarDirectorioYContenidos(DestSistema->sH);
    delete s;
    cout << "Se elimino el sistema correctamente" << endl;
    return OK;
}

//----------------------------------------------------------------

TipoRet CREATEFILE (Sistema& s, Cadena nombreArchivo) {

    Arch nuevo = new archivo;
    nuevo->escritura = true;
    nuevo->lectura = true;

    size_t lastDotPos = nombreArchivo.find_last_of(".");
    Cadena extension = nombreArchivo.substr(lastDotPos + 1); 

    if (lastDotPos == Cadena::npos) { // Verifica si hay una extension
        cout << "Error: No extension" << endl << endl;
        return ERROR;
    }

    if (extension.length() <= 3 && extension.length() > 0) { // Verifica si la extension cumple con el requisito de longitud

      Arch a = s->ACTUAL->archivos;

      while (a != NULL) {
          if (a->nombreArchivo == nombreArchivo) {
              cout<< "Error: nombre de archivo repetido" << endl << endl;
              return ERROR;
          }
          if (a->sig == NULL) {
              a->sig = nuevo;
              nuevo->ant = a;
              nuevo->nombreArchivo = nombreArchivo;
              nuevo->sig = NULL;
              s->ACTUAL->cantArchivos++;
              cout << "Archivo agregado exitosamente" << endl << endl;
              return OK;
          }
          a = a->sig;
      }

      s->ACTUAL->archivos = nuevo;
      nuevo->nombreArchivo = nombreArchivo;
      nuevo->ant = NULL;
      nuevo->sig = NULL;
      s->ACTUAL->cantArchivos = 1;
      cout << "Archivo agregado exitosamente" << endl << endl;
      return OK;

    }

    else {
        cout << "Error el archivo no pudo ser agregado, verifique que todo sea correcto" << endl << endl;
        return ERROR;
    }
    return ERROR;
}


//----------------------------------------------------------------

TipoRet DELETE (Sistema &s, Cadena nombreArchivo){

  if (s->ACTUAL->archivos == NULL){
    cout<< "DELETE: No existen archivos" << endl << endl;
    return ERROR;
  }

  if (s->ACTUAL->archivos->nombreArchivo == nombreArchivo && s->ACTUAL->archivos -> escritura == true){
      Arch borrar = s->ACTUAL->archivos;
      delete borrar;
      s->ACTUAL->cantArchivos--;
      cout << "El archivo se elimino correctamente" << endl << endl;
      return OK;
    }

    Arch aux = s->ACTUAL->archivos;

    while (aux->nombreArchivo != nombreArchivo && aux->nombreArchivo.length() == nombreArchivo.length())
    {
        aux = aux->sig;

        if (aux == NULL)
        {
            cout << "DELETE: No existe un archivo con ese nombre" << endl << endl;
            return ERROR;
        }
    }

    if (aux -> nombreArchivo == nombreArchivo && aux -> escritura == true){
      Arch borrar = aux -> sig;
      aux -> sig = borrar -> sig;
      delete borrar;
      s->ACTUAL->cantArchivos--;
      cout << "El archivo se elimino correctamente" << endl << endl;
      return OK;
    }else if (aux -> nombreArchivo == nombreArchivo && aux -> escritura == false){
      cout<<"El archivo es de solo lectura, no puede ser eliminado" << endl << endl;
      return ERROR;
    }
    return NO_IMPLEMENTADA;
  }

//----------------------------------------------------------------

TipoRet ATTRIB (Sistema &s, Cadena nombreArchivo, Cadena parametro){

  if(s->ACTUAL->archivos == NULL){
    cout << "ATTRIB: No existen archivos" << endl << endl;
    return ERROR;
  }

  Arch aux = s->ACTUAL->archivos;

    while (aux -> nombreArchivo != nombreArchivo){
      aux = aux -> sig;

      if (aux == NULL){
        cout << "ATTRIB: No existe un archivo con ese nombre" << endl << endl;
        return ERROR;
      }
    }

  if (aux -> nombreArchivo == nombreArchivo){
    if (parametro == "+w" || parametro == "+W"){
      aux -> escritura = true;
      cout << "El permiso se agrego correctamente" << endl << endl;
      return OK;
    }

    if (parametro == "-w" || parametro == "-W"){
      aux -> escritura = false;
      cout << "El permiso se elimino correctamente" << endl << endl;
      return OK;
    }
  }
  return NO_IMPLEMENTADA;
}

//----------------------------------------------------------------

TipoRet IF (Sistema &s, Cadena nombreArchivo, Cadena texto){

  if (s->ACTUAL->archivos == NULL){
    cout << "IF: No existen archivos" << endl << endl;
    return ERROR;
  }

  Arch aux = s->ACTUAL->archivos;

    while (aux -> nombreArchivo != nombreArchivo){
      aux = aux -> sig;

      if (aux == NULL){
        cout<<"IF: No existe un archivo con ese nombre" << endl << endl;
        return ERROR;
      }
    }

  if (aux -> escritura == false){
    cout << "IF: Esta accion requiere permisos de escritura" << endl << endl;
    return ERROR;
  }

  if (aux -> nombreArchivo == nombreArchivo && aux-> escritura == true && aux->cont == false){

    aux->contenido = texto;
    aux -> cont = true;
    cout << "El contenido se agrego correctamente" << endl << endl;
    return OK;
  }

  if (aux -> nombreArchivo == nombreArchivo && aux-> escritura == true && aux->cont == true){

    aux->contenido.insert(0, texto + " ");
    cout << "El contenido se actualizo correctamente" << endl << endl;
    return OK;
  }
  return NO_IMPLEMENTADA;
}

//----------------------------------------------------------------

TipoRet DF (Sistema &s, Cadena nombreArchivo, int k){

  if (s->ACTUAL->archivos == NULL){
    cout << "DF: No existen archivos" << endl << endl;
    return ERROR;
  }

  Arch aux = s->ACTUAL->archivos;

  while (aux -> nombreArchivo != nombreArchivo){
      aux = aux -> sig;

      if (aux == NULL){
        cout << "DF: No existe un archivo con ese nombre" << endl << endl;
        return ERROR;
      }
    }

  if (aux -> escritura == false){
    cout << "DF: Esta accion requiere permisos de escritura" << endl << endl;
    return ERROR;
  }

  if(aux -> nombreArchivo == nombreArchivo){ 

    if (aux->contenido.length() < k){
      aux -> cont = false;
    }

    int pos = 0; // Posición desde donde deseas eliminar caracteres

    // Comprobación para asegurarse de que pos está dentro de los límites del texto
    if (pos >= 0 && pos < aux->contenido.length() && k > 0) {
        for (int i = 0; i < k; i++) {
            // Elimina el carácter en la posición 'pos'
            aux->contenido.erase(pos, 1);
        }
    }
      cout << "Se elimino el contenido exitosamente" << endl << endl;
      return OK;
    }
    return NO_IMPLEMENTADA;
  }

//----------------------------------------------------------------

TipoRet TYPE (Sistema &s, Cadena nombreArchivo){

  if (s->ACTUAL->archivos == NULL){
    cout << "TYPE: No existen archivos" << endl << endl;
    return ERROR;
  }

  Arch aux = s->ACTUAL->archivos;

  while (aux -> nombreArchivo != nombreArchivo){
    aux = aux -> sig;

    if (aux == NULL){
      cout<<"TYPE: No existe un archivo con ese nombre" << endl << endl;
      return ERROR;
    }
  }

  if (aux -> nombreArchivo == nombreArchivo){

    if (aux -> cont == true){
      for (int i = 0; i < aux -> contenido.length(); i += TEXTO_MAX) {
          // Imprimir cada segmento de 22 caracteres o menos
          for (int j = 0; j < TEXTO_MAX && i + j < aux -> contenido.length(); j++) {
              cout << aux -> contenido[i + j];
          }
          cout << endl;
    }
    cout << endl;
    return OK;
  }

    if (aux -> cont == false){
      cout << "El archivo no posee contenido" << endl << endl;
      return OK;
    }
  }
  return NO_IMPLEMENTADA;
}

//------------------------------------------------------------------------------------------------------

void imprimirDirectorioRecursivo(dir nodo, const string& rutaActual = "") {
    if (nodo) {
        string rutaNodo = rutaActual + "/" + nodo->nombre;
        cout << rutaNodo << endl;
      
      if (nodo->cantArchivos > 0){
        Arch aux = nodo->archivos;
        while (aux != NULL){
          cout << rutaNodo + "/" + aux->nombreArchivo << endl;
          aux = aux -> sig;
        }
      }
        
      if (nodo->cantDirectorios > 0){
        dir hijo = nodo->pH;
        while (hijo) {
            imprimirDirectorioRecursivo(hijo, rutaNodo);
            hijo = hijo->sH;
        }
      }
    }
}

TipoRet DIR(Sistema& s, Cadena parametro) {

    cout << endl;

    if (parametro != "/S") {


        if (s->RAIZ->ruta.empty()) {
            cout << "ROOT" << endl;
        }

        // cout << s->RAIZ->ruta << endl;

        if (s->ACTUAL->cantArchivos > 0) {

            // Bubble Sort para ordenar archivos por nombreArchivo
            Arch actual = s->RAIZ->archivos;
            while (actual != NULL) {
                Arch siguiente = actual->sig;
                while (siguiente != NULL) {
                    if (actual->nombreArchivo > siguiente->nombreArchivo) {
                        // Intercambiar archivos si están en el orden incorrecto
                        Cadena tempNombre = actual->nombreArchivo;
                        actual->nombreArchivo = siguiente->nombreArchivo;
                        siguiente->nombreArchivo = tempNombre;
                    }
                    siguiente = siguiente->sig;
                }
                actual = actual->sig;
            }

            // Imprimir archivos ordenados
            Arch tempArch = s->ACTUAL->archivos;  // Usar un puntero temporal

            while (tempArch != NULL) {
                cout << tempArch->nombreArchivo << "     " << "Lectura";
                if (tempArch->escritura == true) {
                    cout << "/Escritura";
                }
                cout << endl;
                tempArch = tempArch->sig;
            }
        }

        //-----------------------------------------------------------------------------------------
        if (s->ACTUAL->cantDirectorios > 0) {

            // Bubble Sort para ordenar directorios por nombre

            dir DirActual = s->ACTUAL->pH;

            while (DirActual->sH != NULL) {
                dir DirSiguiente = DirActual->sH;
                while (DirSiguiente != NULL) {
                    if (DirActual->nombre > DirSiguiente->nombre) {
                        // Intercambiar directorios si están en el orden incorrecto
                        Cadena tempNombre = DirActual->nombre;
                        DirActual->nombre = DirSiguiente->nombre;
                        DirSiguiente->nombre = tempNombre;
                    }
                    DirSiguiente = DirSiguiente->sH;
                }
                DirActual = DirActual->sH;
            }

            // Imprimir directorios ordenados

            DirActual = s->ACTUAL->pH;

            while (DirActual != NULL) { //IMPRIME DIRECTORIOS HIJO DE MI ACTUAL
                cout << "   " << DirActual->nombre;
                cout << endl;
                DirActual = DirActual->sH;
            }
        }

        if (s->ACTUAL->cantDirectorios == 0) {
            cout << "No hay subdirectorios en el directorio actual" << endl << endl;
        }
        cout << endl;
        return OK;
    }

    if (parametro == "/S") {

        // Bubble Sort para ordenar archivos por nombreArchivo
        Arch actual = s->RAIZ->archivos;
        while (actual != NULL) {
            Arch siguiente = actual->sig;
            while (siguiente != NULL) {
                if (actual->nombreArchivo > siguiente->nombreArchivo) {
                    // Intercambiar archivos si están en el orden incorrecto
                    Cadena tempNombre = actual->nombreArchivo;
                    actual->nombreArchivo = siguiente->nombreArchivo;
                    siguiente->nombreArchivo = tempNombre;

                    bool tempEscritura = actual->escritura;
                    actual->escritura = siguiente->escritura;
                    siguiente->escritura = tempEscritura;
                }
                siguiente = siguiente->sig;
            }
            actual = actual->sig;
        }

        // Bubble Sort para ordenar directorios por nombre
        dir DirActual = s->RAIZ->pH;
        while (DirActual != NULL) {
            dir DirSiguiente = DirActual->sH;
            while (DirSiguiente != NULL) {
                if (DirActual->nombre > DirSiguiente->nombre) {
                    // Intercambiar directorios si están en el orden incorrecto
                    dir tempNombre = DirActual;
                    DirActual = DirSiguiente;
                    DirSiguiente = tempNombre;
                }
                DirSiguiente = DirSiguiente->sH;
            }
            DirActual = DirActual->sH;
        }
        imprimirDirectorioRecursivo(s->RAIZ); // Llama a la función recursiva para imprimir la estructura
    }
    return OK;
}

//------------------------------------------------------------------------------------------------------

TipoRet MKDIR (Sistema &s, Cadena nombreDirectorio){

  if(nombreDirectorio == "RAIZ" || nombreDirectorio == "raiz" || nombreDirectorio == "Raiz"){
     cout << "Ese nombre no esta permitido" << endl;   
     return ERROR;
  }

  dir nuevo = new directorio;
  nuevo->nombre = nombreDirectorio;
  nuevo->padre = s->ACTUAL;

  if (s->ACTUAL->pH == NULL){
    s->ACTUAL -> pH = nuevo;
    dir aux = s->ACTUAL->pH;
    aux -> pH = NULL;
    aux -> sH = NULL;
    s->ACTUAL->cantDirectorios++;
    cout<<"Directorio creado correctamente"<<endl;
    return OK;
  }

  if (s->ACTUAL->pH != NULL){
    dir aux = s->ACTUAL->pH;

    while (aux->sH != NULL){
      if (aux->nombre == nombreDirectorio){
        cout << "Ya existe un directorio con ese nombre" << endl;
        return ERROR;  
      }
      aux = aux->sH;
    }
    if (aux->nombre == nombreDirectorio){
      cout << "Ya existe un directorio con ese nombre" << endl;
      return ERROR;  
    }

    aux->sH = nuevo;
    aux -> sH -> sH = NULL;
    s->ACTUAL->cantDirectorios++;
    cout<<"Directorio creado correctamente"<<endl;
    return OK;
  }

  return NO_IMPLEMENTADA;
}

//------------------------------------------------------------------------------------------------------

// Función para eliminar directorio y sus contenidos de forma recursiva
void eliminarDirectorioYContenidos(dir directorio) {

    if (directorio == nullptr) {
        return;
    }

    // Eliminar todos los archivos del directorio actual
    while (directorio->cantArchivos > 0) {
        // Suponemos que los archivos son dinámicamente asignados y deben liberarse

        Arch aux = directorio->archivos;
        while (aux -> sig != NULL){
          Arch borrar = aux;
          aux = aux -> sig;
          delete borrar;
        }
        delete aux;
        directorio->archivos = nullptr;
        directorio->cantArchivos = 0;
    }

    dir hijo = directorio->pH;
    while (hijo != nullptr) {
        eliminarDirectorioYContenidos(hijo);
        dir temp = hijo;
        hijo = hijo->sH;
        delete temp;
        directorio->cantDirectorios--;
    }
}

TipoRet RMDIR(Sistema &s, Cadena nombreDirectorio) {
    if (nombreDirectorio == "RAIZ" || nombreDirectorio == "raiz" || nombreDirectorio == "Raiz") {
        cout << "Ese directorio no está permitido eliminar" << endl;
        return ERROR;
    }

    if (s->ACTUAL->cantDirectorios == 0){
      cout << "No existe un directorio con ese nombre en el directorio actual" << endl;
      return ERROR;
    }

    dir aux = s->ACTUAL->pH;

    if (aux->nombre == nombreDirectorio) {
        // Llama a la función para eliminar directorio y contenidos de forma recursiva
        eliminarDirectorioYContenidos(aux);
        s->ACTUAL->pH = aux->sH;
        s->ACTUAL->cantDirectorios--;
        delete aux;
        cout << "Directorio eliminado correctamente" << endl;
        return OK;
    }

    while (aux->sH != nullptr) {
        if (aux->sH->nombre == nombreDirectorio) {
            // Llama a la función para eliminar directorio y contenidos de forma recursiva
            eliminarDirectorioYContenidos(aux->sH);
            cout << "Directorio eliminado" << endl;
            dir temp = aux->sH;
            aux->sH = aux->sH->sH;
            s->ACTUAL->cantDirectorios--;
            delete temp;
            return OK;
        }
        aux = aux->sH;
    }

    cout << "No existe un directorio con ese nombre en el directorio actual" << endl;
    return ERROR;
}

//------------------------------------------------------------------------------------------------------

TipoRet CD (Sistema &s, Cadena nombreDirectorio){

  if (s == NULL){
    return ERROR;
  }

  Cadena rutaCompleta;
// SI ME PASAN UNA RUTA

  if (nombreDirectorio[0] == '/') {

      string rutaParcial;
      dir aux = s->RAIZ;
      for (int i = 1; i < nombreDirectorio.length(); i++) { 
          if (nombreDirectorio[i] != '/') {
              rutaParcial.push_back(nombreDirectorio[i]);
          } else {
              // Actualizar la ruta completa cuando encuentro una barra '/'
              rutaCompleta = rutaCompleta + '/' + rutaParcial;
              // Continuar con la búsqueda del directorio
              bool encontrado = false;
              while (aux != nullptr) {
                  if (aux->nombre == rutaParcial) {
                      encontrado = true;
                      break;
                  }
                  aux = aux->sH;
              }
              // Restaurar la ruta parcial para el siguiente nombre de directorio
              rutaParcial.clear();
              if (!encontrado) {
                  cout << "Directorio no encontrado: " << rutaCompleta << endl;
                  return ERROR;
              }
              if (aux->pH != nullptr) {
                  aux = aux->pH;
              }
          }
      }
      s->ACTUAL = aux;
      s->RAIZ->ruta = rutaCompleta;
      return OK;
  }

  if (nombreDirectorio == "RAIZ"){
    s->ACTUAL = s->RAIZ;
    s->RAIZ->ruta = s->ACTUAL->nombre;
    return OK;
  }

  if (nombreDirectorio == ".."){

    if (s->ACTUAL == s->RAIZ){
      cout << "ERROR NO PUEDE RETROCEDER SI ESTAS EN RAIZ" << endl;
      return ERROR;
    }

    // Obtener la última barra '/' en la ruta actual
        size_t lastSlash = s->RAIZ->ruta.find_last_of('/');

        if (lastSlash != string::npos) {
            // Construir la nueva ruta excluyendo el último componente
            s->RAIZ->ruta = s->RAIZ->ruta.substr(0, lastSlash);
            s->ACTUAL = s->ACTUAL->padre;
            return OK;
        }
    }


  

  if (nombreDirectorio != "RAIZ") {
      dir aux = s->ACTUAL->pH;

      rutaCompleta = rutaCompleta + s->RAIZ->ruta + '/' + nombreDirectorio;
      while (aux != NULL) {
          if (aux->nombre == nombreDirectorio) {
              s->RAIZ->ruta = rutaCompleta;//guarda la ruta
              s->ACTUAL = aux;
              return OK;
          }
          aux = aux->sH;
      }
    cout << "No existe un directorio con ese nombre" << endl;
    return ERROR;
  }
  return NO_IMPLEMENTADA;
}

//------------------------------------------------------------------------------------------------------

TipoRet MOVE (Sistema &s, Cadena nombre, Cadena directorioDestino){

  //-------- MOVE ARCHIVOS --------// //ArchPtr es el puntero que queda apuntando al archivo que quiero mover
  bool MoveArch = false;
  Arch ArchPtr = new archivo;
  Arch borrar = nullptr;

  if (s->ACTUAL->cantArchivos > 0){

    borrar = s->ACTUAL->archivos;

    if (s->ACTUAL->cantArchivos == 1){
      if (borrar->sig == NULL){
        if (borrar->nombreArchivo == nombre){
          MoveArch = true;
          ArchPtr->nombreArchivo = borrar->nombreArchivo; //Hago una ciopia del archivo al que apunta borrar si es el que busco
          ArchPtr->contenido = borrar->contenido;
          if (borrar->escritura == true){
            ArchPtr->escritura = true;
          }
          if (borrar->cont == true){
            ArchPtr->cont = true;
          }
        }
      }
    }
      while (borrar != NULL && MoveArch == false){ // Caundo hay mas de un archivo

        if (borrar->nombreArchivo == nombre){
          cout << borrar->nombreArchivo << endl;
          Arch aux4 = borrar;
          if (borrar->ant != NULL){
            aux4 = borrar->ant;
          }
          aux4->sig = borrar->sig;
          ArchPtr->nombreArchivo = borrar->nombreArchivo; //Hago una ciopia del archivo al que apunta borrar si es el que busco
          ArchPtr->contenido = borrar->contenido;
          if (borrar->escritura == true){
            ArchPtr->escritura = true;
          }
          if (borrar->cont == true){
            ArchPtr->cont = true;
          }
          MoveArch = true;
        }
          if (borrar->nombreArchivo != nombre && borrar->sig == NULL){
            cout << "No existe un archivo con ese nombre" << endl;
          }
          borrar = borrar -> sig;
      }
  }

  if (s->ACTUAL->cantArchivos == 0 && MoveArch == true){
    cout << "No hay archivos en el directorio actual" << endl;
  }
  //-------------------------------//


  //-------- MOVE DIRECTORIO --------// // BUSCO EL DIRECTORIO QUE ME PIDEN MOVER
  dir aux = nullptr;
  bool encontrado = false;

  if (s->ACTUAL->cantDirectorios > 0){

    if (!MoveArch){

      if (s->ACTUAL->cantDirectorios == 1){
        aux = s->ACTUAL->pH;
          if (aux->nombre == nombre){
            aux -> padre = nullptr;
            encontrado = true;
          }
        if (aux->nombre != nombre && aux->sH == NULL){
          cout << "No existe un directorio con ese nombre" << endl;
          return ERROR;
        }
      }
      else{

        aux = s->ACTUAL;

        if(aux->pH->nombre == nombre){
          aux = aux->pH;
          s->ACTUAL->pH = aux->sH;
          encontrado = true;
        }

        if (encontrado == false){
           aux = aux->pH;
        }
        
        while (aux->sH != NULL && encontrado == false){
            
          if (aux->sH->nombre == nombre){
            encontrado = true;
            dir enganche = aux;
            aux = aux->sH;
            enganche->sH = aux->sH;
          }

          
          if (encontrado == false){
            aux = aux->sH;
          }
        }
        if (aux->nombre != nombre && aux->sH == NULL && encontrado == false){
          cout << "No existe un directorio con ese nombre" << endl;
          return ERROR;
        }
      }
    }
  }
  if (s->ACTUAL->cantDirectorios == 0 && MoveArch == false){
    cout << "No hay subdirectorios en el directorio actual (MOVE)" << endl;
    return ERROR;
  }
  //---------------------------------//

  Cadena rutaCompleta_MOVE; // ANALIZO LA RUTA QUE ME PASAN
  string rutaParcial_MOVE;
  dir aux2 = s->RAIZ;
  for (int i = 1; i < directorioDestino.length(); i++) { 
      if (directorioDestino[i] != '/') {
            rutaParcial_MOVE.push_back(directorioDestino[i]);
      } else {
          // Actualizar la ruta completa cuando encuentres una barra '/'
            rutaCompleta_MOVE = rutaCompleta_MOVE + '/' + rutaParcial_MOVE;
          // Continuar con la búsqueda del directorio
          bool rutaEncontrada = false;
          while (aux2 != nullptr) {
              if (aux2->nombre == rutaParcial_MOVE) {
                  rutaEncontrada = true;
                  break;
              }
              aux2 = aux2->sH;
          }
          // Restaurar la ruta parcial para el siguiente nombre de directorio
            rutaParcial_MOVE.clear();
          if (!rutaEncontrada) {
              // Directorio no encontrado, maneja el error si es necesario
              cout << "Directorio no encontrado: " << rutaCompleta_MOVE << endl;
              return ERROR;
          }
          if (aux2->pH != nullptr) {
              aux2 = aux2->pH;
          }
      }
  }

  //-------- MOVE ARCHIVOS --------// //aux2 Es el que esta en la ruta
  if (MoveArch){

    if (aux2->cantArchivos == 0){
      aux2->archivos = ArchPtr;
      ArchPtr->sig = NULL;
      aux2->cantArchivos++;
      delete borrar;
      s->ACTUAL->cantArchivos--;
      return OK;
    }

    Arch aux5 = aux2->archivos;

    while(aux5 -> sig != NULL){

      if (aux5->nombreArchivo == ArchPtr->nombreArchivo){
        aux5 = ArchPtr;
        return OK;
      }
      aux5 = aux5 -> sig;
    }
    aux5 -> sig = ArchPtr;
    ArchPtr->sig = NULL;
    delete borrar;
    s->ACTUAL->cantArchivos--;
    return OK;
  }

  //-------- MOVE DIRECTORIO --------//

  if (MoveArch == false){ //COLOCAR EN EL DESTINO


    if (aux2->cantDirectorios == 0) {

            aux2->pH = aux;
            aux -> padre = aux2;
            aux->sH = nullptr; // Asegura que el nodo movido no tenga hermanos en el destino
            s->ACTUAL->cantDirectorios--;
            aux2->cantDirectorios++;
            return OK;
        }

    if (aux2 ->cantDirectorios == 1){

          dir ultimoDir = aux2->pH;
          cout << ultimoDir->nombre << endl;

          ultimoDir->sH = aux;
          aux->sH = nullptr; // Asegura que el nodo movido no tenga hermanos en el destino
          aux2->sH = aux;
          aux->padre = aux2->padre;
          return OK;
    }

    dir ultimoDir = aux2->pH;
    cout << ultimoDir->nombre << endl;

    while (ultimoDir->sH != NULL) {
        ultimoDir = ultimoDir->sH;
    }

    ultimoDir->sH = aux;

    aux->sH = nullptr; // Asegura que el nodo movido no tenga hermanos en el destino
    aux->padre = aux2;
    s->ACTUAL->cantDirectorios--;
    return OK;

  }
  //---------------------------------//
  return NO_IMPLEMENTADA;
}


int main() {

    Sistema s;
    CREARSISTEMA(s);
    string input;
    string comando, argumento, tercerArg, texto;
    int k;
    while (true) {
        //cout << "Ingrese un comando: ";
        if (s->RAIZ->ruta.empty()){
          cout<< "/RAIZ" << ": ";
        }
        else{
          cout << '/' + s->RAIZ->nombre << s->RAIZ->ruta << ": ";
        }
        getline(cin, input);
        size_t firstSpacePos = input.find(' ');
        if (firstSpacePos != string::npos) {
            comando = input.substr(0, firstSpacePos);
            size_t secondSpacePos = input.find(' ', firstSpacePos + 1);
            if (secondSpacePos != string::npos) {
                argumento = input.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
                tercerArg = input.substr(secondSpacePos + 1);
            }
            else {
                argumento = input.substr(firstSpacePos + 1);

            }
        }
        else {
            comando = input;
        }

        if (comando == "RMDIR") {
            RMDIR(s, argumento); continue;
        }
        if (comando == "rmdir") {
            RMDIR(s, argumento); continue;
        }
        if (comando == "MKDIR") {
            MKDIR(s, argumento); continue;
        }
        if (comando == "mkdir") {
            MKDIR(s, argumento); continue;
        }
        if (comando == "CD") {
            CD(s, argumento); continue;
        }
        if (comando == "cd") {
            CD(s, argumento); continue;
        }
        if (comando == "MOVE") {
            MOVE(s, argumento, tercerArg); continue;
        }
        if (comando == "move") {
            MOVE(s, argumento, tercerArg); continue;
        }
        if (comando == "DIR") {
            DIR(s, argumento); continue;
        }
        if (comando == "dir") {
            DIR(s, argumento); continue;
        }
        if (comando == "TYPE") {
            TYPE(s, argumento); continue;
        }
        if (comando == "DF") {
            k = stoi(tercerArg);
            DF(s, argumento, k); continue;
        }
        if (comando == "IF") {
            IF(s, argumento, tercerArg); continue;
        }
        if (comando == "ATTRIB") {
            ATTRIB(s, argumento, tercerArg); continue;
        }
        if (comando == "DELETE") {
            DELETE(s, argumento); continue;
        }
        if (comando == "CREATEFILE") {
            CREATEFILE(s, argumento); continue;
        }
        if (comando == "createfile") {
            CREATEFILE(s, argumento); continue;
        }
        if (comando == "DESTRUIRSISTEMA") {
          DESTRUIRSISTEMA(s);return 0;
        }
        else {
            cout << "Comando invalido" << endl;
            continue;
        }
    }
}

