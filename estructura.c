#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALERTA 20
#define max 50
#define MAX_COMPRAS 100

struct mercaditoLibre {
    struct NodoUsuario *usuario;
    struct NodoProductos *productos;
};

struct NodoUsuario {
    struct Usuario *refUsuario;
    struct NodoUsuario *sig;
};

struct Usuario {
    char *nombre;
    int id;
    char historialcompraNombre[MAX_COMPRAS][100];
    int historialcompraCantidad[MAX_COMPRAS];
    int nroCompras;
};


struct NodoProductos {
    struct producto *refProducto;
    struct NodoProductos *izq, *der;
};

struct producto {
    char *nombre;
    int cantidad;
    int id;
    int carga_inicial;
    int giro;
};

int buscarUsuario(struct NodoUsuario *usuarios, int id);
void imprimirUsuariosOrdenados(struct NodoUsuario *usuarios);
void eliminarUsuario(struct NodoUsuario **usuario, int id);
void alerta(struct NodoProductos *productos);
int buscarProducto(struct NodoProductos *productos, int id);
void imprimirProducto(struct NodoProductos *productos,int id);
void comprobarAlerta(struct NodoProductos *productos, int id);
void recargarProducto(struct NodoProductos **producto,int id,int recarga);
void cambiarNombreUsuario(struct NodoUsuario **usuarios, int id, char *nuevoNombre);
int enlazarProducto(struct NodoProductos **head, struct producto *nuevoProducto);
void ordenarPorID(struct NodoUsuario **head);
int descontarProducto(struct NodoProductos *producto, int idProducto, int cantidad);
void agregarHistorial(struct NodoUsuario *usuario, struct NodoProductos *producto, int idUsuario, int idProducto, int cantidad);



struct NodoUsuario *crearNodo(struct NodoUsuario *usuarios) {
    struct NodoUsuario *nuevo;
    char buffer[max];

    /*Se obtiene la memoria para el nuevo nodo y para la estructura Usuario*/
    nuevo = (struct NodoUsuario *)malloc(sizeof(struct NodoUsuario));
    nuevo->refUsuario = (struct Usuario *)malloc(sizeof(struct Usuario));

    /*Se completa con los datos*/
    printf("Ingrese id (solo numeros enteros): \n");
    do
    {
        scanf("%d", &nuevo->refUsuario->id);
        if(nuevo->refUsuario->id <= 0){
            printf("ingrese id mayor a 0");
        }
        if(buscarUsuario(usuarios,nuevo->refUsuario->id)!=0)
            printf("id ya usado, pruebe con otro id mayor a 0\n");
    }while(buscarUsuario(usuarios,nuevo->refUsuario->id)!=0 || nuevo->refUsuario->id <= 0);
    printf("Ingrese nombre Cliente:\n");
    scanf(" %[^\n]", buffer);
    nuevo->refUsuario->nombre = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(nuevo->refUsuario->nombre, buffer);

    nuevo->refUsuario->nroCompras=0;

    nuevo->sig = NULL;
    return nuevo;
}

void enlazarNodo(struct NodoUsuario **head, struct Usuario *nuevo)
{
    struct NodoUsuario *rec=*head;
    if(*head==NULL)
    {
        (*head)=nuevo;
        return;
    }
    while(rec!=NULL)
    {
        if(rec->sig==NULL)
        {
            rec->sig=nuevo;
            rec=nuevo;
        }
        rec=rec->sig;
    }
    ordenarPorID(head);
}


// Función de ordenamiento burbuja mejorado

void ordenarPorID(struct NodoUsuario **head) {
    int intercambiado;
    struct NodoUsuario *ptr1;
    struct NodoUsuario *lptr = NULL;

    if (*head == NULL)
        return;

    do {
        intercambiado = 0;
        ptr1 = *head;

        while (ptr1->sig != lptr) {
            if (ptr1->refUsuario->id > ptr1->sig->refUsuario->id) {
                // Intercambiar los valores de los nodos, incluyendo la cadena de nombre
                int tempID = ptr1->refUsuario->id;
                ptr1->refUsuario->id = ptr1->sig->refUsuario->id;
                ptr1->sig->refUsuario->id = tempID;

                char tempNombre[max];
                strcpy(tempNombre, ptr1->refUsuario->nombre);
                strcpy(ptr1->refUsuario->nombre, ptr1->sig->refUsuario->nombre);
                strcpy(ptr1->sig->refUsuario->nombre, tempNombre);

                intercambiado = 1;
            }
            ptr1 = ptr1->sig;
        }
        lptr = ptr1;
    } while (intercambiado);
}

// Función de búsqueda binaria
struct NodoUsuario *busquedaBinaria(struct NodoUsuario **head, int id) {
    struct NodoUsuario *inicio = *head;  // Usar *head para obtener el valor apuntado por head
    struct NodoUsuario *fin = NULL;


    while (inicio != fin) {
        struct NodoUsuario *mitad = inicio;
        int elementosEnMitad = 0;

        // Encontrar el elemento en la mitad de la lista
        while (mitad != fin) {
            mitad = mitad->sig;
            elementosEnMitad++;
        }

        mitad = inicio;
        for (int i = 0; i < elementosEnMitad / 2; i++) {
            mitad = mitad->sig;
        }

        // Verificar si el elemento está en la mitad
        if (mitad->refUsuario->id == id) {
            return mitad;
        } else if (mitad->refUsuario->id < id) {
            inicio = mitad->sig;
        } else {
            fin = mitad;
        }
    }
    return NULL;
}

void imprimirUsuario(struct NodoUsuario *usuario) {
    if (usuario != NULL) {
        printf("Usuario encontrado:\n");
        printf("ID: %d\n", usuario->refUsuario->id);
        printf("Nombre: %s\n", usuario->refUsuario->nombre);
        printf("Número de compras: %d\n", usuario->refUsuario->nroCompras);
    } else {
        printf("No se encontró un usuario con ese ID.\n");
    }
}


int buscarUsuario(struct NodoUsuario *usuarios, int id) {
    struct NodoUsuario *rec = usuarios;

    if(usuarios!=NULL)
    {
        while (rec != NULL) {
            if (rec->refUsuario->id == id) {
                return 1;  // Encontrado
            } else if (rec->refUsuario->id < id) {
                rec = rec->sig;
            } else {
                return 0;
            }
        }
    }

    return 0;  // No encontrado
}

void cambiarNombreUsuario(struct NodoUsuario **usuarios, int id, char *nuevoNombre){
    struct NodoUsuario *rec = *usuarios;

    while (rec != NULL && rec->refUsuario->id < id) {
        rec = rec->sig;
    }

    if (rec != id) {
        printf("Usuario con ID %d no encontrado.\n", id);
        return;
    } else {
        // Libera la memoria del nombre anterior antes de asignar el nuevo nombre
        free(rec->refUsuario->nombre);
        // Asigna el nuevo nombre utilizando memoria dinámica
        rec->refUsuario->nombre = (char *)malloc((strlen(nuevoNombre) + 1) * sizeof(char));
        strcpy(rec->refUsuario->nombre, nuevoNombre);
        printf("Nuevo Nombre: %s\n", rec->refUsuario->nombre);
        return;
    }
}


void imprimirUsuariosOrdenados(struct NodoUsuario *usuarios) {
    struct NodoUsuario *rec = usuarios;
    int numero=1;
    if (usuarios == NULL) {
        printf("NULL\n");
        return;
    }

    while (rec != NULL) {
        printf("Usuario %d\n",numero);
        printf("Nombre: %s\n",rec->refUsuario->nombre);
        printf("ID: %d \n", rec->refUsuario->id);
        printf("Numero de compras: %d \n\n", rec->refUsuario->nroCompras);
        numero+=1;
        rec = rec->sig;
    }

    printf("----------------------------\n");
}

void eliminarUsuario(struct NodoUsuario **usuario, int id) {
    struct NodoUsuario *aux = *usuario;
    struct NodoUsuario *anterior = NULL;

    // Caso especial: si el nodo a eliminar es el primero
    if (aux != NULL && aux->refUsuario->id == id) {
        *usuario = aux->sig;
        //free(aux->refUsuario->nombre); // Liberar memoria del nombre del usuario
        free(aux->refUsuario); // Liberar memoria del usuario
        free(aux); // Liberar memoria del nodo
        return;
    }

    // Buscar el nodo a eliminar y recordar el nodo anterior
    while (aux != NULL && aux->refUsuario->id != id) {
        anterior = aux;
        aux = aux->sig;
    }

    // Si el nodo no fue encontrado, salir de la función
    if (aux == NULL) {
        printf("Usuario con ID %d no encontrado.\n", id);
        return;
    }

    // Cambiar el puntero sig del nodo anterior al nodo siguiente al que se desea eliminar
    anterior->sig = aux->sig;

    // Liberar memoria del nombre del usuario
    free(aux->refUsuario->nombre);

    // Liberar memoria del usuario
    free(aux->refUsuario);

    // Liberar memoria del nodo
    free(aux);
}


//producto


struct producto *crearProducto(struct NodoProductos *productos) {
    struct producto *nuevoProducto = (struct producto *)malloc(sizeof(struct producto));
    char buffer[max];

    if (nuevoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo producto.\n");
        return NULL;
    }

    printf("Ingrese id del nuevo producto (solo numeros entero):\n");
    do {
        scanf("%d", &nuevoProducto->id);
        if (nuevoProducto->id <= 0) {
            printf("Ingrese un ID mayor a 0:\n");
        } else if (buscarProducto(productos, nuevoProducto->id) != 0) {
            printf("ID de producto ya está en uso, ingrese otro:\n");
        }
    } while (nuevoProducto->id <= 0 || buscarProducto(productos, nuevoProducto->id) != 0);

    printf("Ingrese nombre del producto:\n");
    scanf(" %[^\n]", buffer);
    nuevoProducto->nombre = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(nuevoProducto->nombre, buffer);

    printf("Ingrese la cantidad del producto:\n");
    scanf("%d", &nuevoProducto->cantidad);
    nuevoProducto->carga_inicial = nuevoProducto->cantidad;
    nuevoProducto->giro = 1;

    printf("Nuevo producto creado correctamente.\n");
    printf("--------------------\n\n");

    return nuevoProducto;
}

int buscarProducto(struct NodoProductos *productos, int id) {
    struct NodoProductos *rec = productos;
    if (rec == NULL)
        return 0;
    else {
        while (rec != NULL) {
            if (rec->refProducto->id == id) {
                return 1;
            }
            rec = rec->der;
        }
    }
    return 0;
}

int enlazarProducto(struct NodoProductos **head, struct producto *nuevoProducto) {
    if ((*head) == NULL) {
        (*head) = (struct NodoProductos *)malloc(sizeof(struct NodoProductos));
        (*head)->refProducto = nuevoProducto;
        (*head)->izq = (*head)->der = NULL;
        return 1;
    } else {
        if ((*head)->refProducto->id != nuevoProducto->id) {
            if ((*head)->refProducto->id > nuevoProducto->id)
                return enlazarProducto(&((*head)->izq), nuevoProducto);
            else
                return enlazarProducto(&((*head)->der), nuevoProducto);
        }
    }
    return 0;
}

void listarInordenProductos(struct NodoProductos *raiz) {
    if (raiz != NULL) {
        listarInordenProductos(raiz->izq);
        printf("ID: %d, Nombre: %s, Cantidad: %d\n", raiz->refProducto->id, raiz->refProducto->nombre, raiz->refProducto->cantidad);
        listarInordenProductos(raiz->der);
    }
}

void imprimirProducto(struct NodoProductos *productos,int id){
    if (productos == NULL) {
        printf("no existe\n");
        return;
    } else if (productos->refProducto->id == id) {
        productos->refProducto->giro += productos->refProducto->giro;
        printf("id: %d\n", productos->refProducto->id);
        printf("nombre: %s\n", productos->refProducto->nombre);
        printf("cantidad disponible: %d\n", productos->refProducto->cantidad);
        printf("Cantidad que el producto fue buscado: %d\n", productos->refProducto->giro);
        return;
    } else if (productos->refProducto->id > id) {
        return imprimirProducto(productos->izq, id);
    } else {
        return imprimirProducto(productos->der, id);
    }
}

void comprobarAlerta(struct NodoProductos *productos, int id){
    if (productos == NULL) {
        printf("no el existen producto");
        return;
    } else if (productos->refProducto->id == id) {
        productos->refProducto->giro+=productos->refProducto->giro;
        alerta(productos);
        return;
    } else if (productos->refProducto->id > id) {
        return comprobarAlerta(productos->izq, id);
    } else {
        return comprobarAlerta(productos->der, id);
    }
}


void alerta(struct NodoProductos *productos){
    int porcentaje;
    porcentaje=((productos->refProducto->cantidad*100) / productos->refProducto->carga_inicial);
    if ((porcentaje <= ALERTA) && (0 < porcentaje)) {
        printf("El producto necesita ser recargado\n\n");
        return;
    }else if(porcentaje==0){
        printf("producto se a agotado\n\n");
        return;
    }
    return;
}

void eliminarProducto(struct NodoProductos **productos, int id) {
    if (*productos == NULL) {
        printf("No se encontró el producto\n");
        return;
    }

    if ((*productos)->refProducto->id > id) {

        eliminarProducto(&((*productos)->izq), id);
    } else if ((*productos)->refProducto->id < id) {
        eliminarProducto(&((*productos)->der), id);
    } else {
        if ((*productos)->izq == NULL) {
            struct NodoProductos *temp = *productos;
            *productos = (*productos)->der;
            free(temp->refProducto->nombre);
            free(temp->refProducto);
            free(temp);
            printf("Producto eliminado\n");
        } else if ((*productos)->der == NULL) {
            struct NodoProductos *temp = *productos;
            *productos = (*productos)->izq;
            free(temp->refProducto->nombre);
            free(temp->refProducto);
            free(temp);
            printf("Producto eliminado\n");
        } else {
            struct NodoProductos *temp = (*productos)->der;
            while (temp->izq != NULL) {
                temp = temp->izq;
            }
            (*productos)->refProducto->id = temp->refProducto->id;
            eliminarProducto(&((*productos)->der), temp->refProducto->id);
        }
    }
}



void recargarProducto(struct NodoProductos **productos, int id, int recarga) {
    if (*productos == NULL) {
        printf("ID ingresado no contiene producto\n");
        return;
    }

    if ((*productos)->refProducto->id == id) {
        (*productos)->refProducto->cantidad += recarga;
        (*productos)->refProducto->carga_inicial = (*productos)->refProducto->cantidad;
        printf("producto recargado exitosaente, la nueva cantidad del producto %s es de %d unidades \n\n",(*productos)->refProducto->nombre,(*productos)->refProducto->cantidad);
        return;
    } else if ((*productos)->refProducto->id > id) {
        recargarProducto(&((*productos)->izq), id, recarga);
    } else {
        recargarProducto(&((*productos)->der), id, recarga);
    }
}

void ProductoMasBuscado(struct NodoProductos *productos) {
    if (productos == NULL) {
        return;
    }
    //ProductoMasBuscado(productos->izq);
    //if (productos->refProducto->cantidad > productos->refProducto->cantidadMaxima) {
}


void fraude_fiscal(struct NodoProductos **productos, struct NodoUsuario **Usuario) {
    *productos = NULL;
    *Usuario = NULL;
}

int buscarUsuarioImp_Com(struct NodoUsuario *Usuario, int id) {
    struct NodoUsuario *rec=Usuario;
    while (rec != NULL) {
        if(rec->refUsuario->id==id){
            return 1;
        }rec=rec->sig;
    }
    return 0;
}



char *buscarRetornadorProducto(struct NodoProductos *productos, int id){
    if (productos == NULL) {
        return NULL;
    } else if (productos->refProducto->id == id) {
        return productos->refProducto->nombre;
    } else if (productos->refProducto->id > id) {
        buscarRetornadorProducto(productos->izq, id);
    } else {
        buscarRetornadorProducto(productos->der, id);
    }
}


void agregarHistorial(struct NodoUsuario *usuario, struct NodoProductos *producto, int idcomprador, int idproducto, int cantidad){
    struct NodoUsuario *rec = usuario;
    while (rec != NULL){
        if (rec->refUsuario->id == idcomprador){
            for(int i=0;i<=rec->refUsuario->nroCompras;i++) {
                if (i==rec->refUsuario->nroCompras){
                    strcpy(rec->refUsuario->historialcompraNombre[i], buscarRetornadorProducto(producto, idproducto));
                    rec->refUsuario->historialcompraCantidad[i] = cantidad;
                    rec->refUsuario->nroCompras+=1;
                    return;
                }
            }
        }rec=rec->sig;
    }
}


int descontarProducto(struct NodoProductos *producto, int idProducto, int cantidad){
    if (producto == NULL) {
        return 0;
    } else if (producto->refProducto->id == idProducto) {
        producto->refProducto->cantidad=producto->refProducto->cantidad-cantidad;
        producto->refProducto->giro += producto->refProducto->giro;
        return 1;
    } else if (producto->refProducto->id > idProducto) {
        descontarProducto(producto->izq, idProducto, cantidad);
    } else {
        descontarProducto(producto->der, idProducto, cantidad);
    }
}


void comprar (struct NodoUsuario *usuario, struct NodoProductos *producto){
    int idComprador, idProducto, cantidad;
    printf("Ingresar Id del comprador\n");
    scanf("%d",&idComprador);
    if (buscarUsuarioImp_Com(usuario,idComprador)==0){
        printf("No se encontro usuario\n");
        return;
    }
    printf("Ingresar Id del producto\n");
    scanf("%d",&idProducto);
    if (buscarProducto(producto,idProducto)==0){
        printf("No se encontro producto\n");
        return;
    }
    printf("Ingresar cantidad\n");
    scanf("%d",&cantidad);
    if (cantidad < 1){
        printf("Cantidad invalida\n");
        return;
    }
    if (descontarProducto(producto,idProducto,cantidad)==0){
        printf("No hay stock suficiente del producto\n");
    }else{
        agregarHistorial(usuario,producto,idComprador,idProducto, cantidad);
        printf("Compra exitosa\n");
        comprobarAlerta(producto,idProducto);
        return;
    }
}


void imprimirCompras(struct NodoUsuario *usuario){
    int idComprador;
    struct NodoUsuario *rec = usuario;
    printf("Ingrese su id para ver sus compras\n");
    scanf("%d",&idComprador);
    if (buscarUsuarioImp_Com(usuario, idComprador)==0){
        printf("No se encontro el usuario\n");
    }else{
        while(rec != NULL){
            if (rec->refUsuario->id == idComprador){
                printf("Usuario %s ha comprado:\n",rec->refUsuario->nombre);
                for(int i=0; i<rec->refUsuario->nroCompras;i++){
                    if (rec->refUsuario->historialcompraNombre[i]!=NULL){
                        printf("Compra %i = %s cantidad %d unidad(es)\n",i+1,rec->refUsuario->historialcompraNombre[i],rec->refUsuario->historialcompraCantidad[i]);
                    }
                }
            }rec=rec->sig;
        }
    }
}

void encontrarProductoMasGiros(struct NodoProductos *productos, struct NodoProductos **maxGiro) {
    if (productos == NULL) {
        return;
    }

    encontrarProductoMasGiros(productos->der, maxGiro);

    if (*maxGiro == NULL || productos->refProducto->giro > (*maxGiro)->refProducto->giro) {
        *maxGiro = productos;
    }

    encontrarProductoMasGiros(productos->izq, maxGiro);
}

void encontrarProductoMenosGiros(struct NodoProductos *productos, struct NodoProductos **minGiro) {
    if (productos == NULL) {
        return;
    }

    encontrarProductoMenosGiros(productos->izq, minGiro);

    if (*minGiro == NULL || productos->refProducto->giro < (*minGiro)->refProducto->giro) {
        *minGiro = productos;
    }

    encontrarProductoMenosGiros(productos->der, minGiro);
}

void imprimirProductoMasGiros(struct NodoProductos *productos) {
    struct NodoProductos *maxGiro = NULL;
    encontrarProductoMasGiros(productos, &maxGiro);

    if (maxGiro != NULL) {
        printf("Producto con más giros:\n");
        printf("ID: %d, Nombre: %s, Cantidad de busuquedas: %d\n", maxGiro->refProducto->id, maxGiro->refProducto->nombre, maxGiro->refProducto->giro);
    } else {
        printf("No hay productos con busquedas registrados.\n");
    }
}

void imprimirProductoMenosGiros(struct NodoProductos *productos) {
    struct NodoProductos *minGiro = NULL;
    encontrarProductoMenosGiros(productos, &minGiro);

    if (minGiro != NULL) {
        printf("Producto con menos giros:\n");
        printf("ID: %d, Nombre: %s, Cantidad de busquedas: %d\n", minGiro->refProducto->id, minGiro->refProducto->nombre, minGiro->refProducto->giro);
    } else {
        printf("No hay productos con busquedas registrados.\n");
    }
}

void encontrarUsuarioMasCompras(struct NodoUsuario *usuarios, struct NodoUsuario **maxCompras) {
    struct NodoUsuario *temp = usuarios;

    while (temp != NULL) {
        if (*maxCompras == NULL || temp->refUsuario->nroCompras > (*maxCompras)->refUsuario->nroCompras) {
            *maxCompras = temp;
        }
        temp = temp->sig;
    }
}

void imprimirUsuarioMasCompras(struct NodoUsuario *usuarios) {
    struct NodoUsuario *maxCompras = NULL;
    encontrarUsuarioMasCompras(usuarios, &maxCompras);

    if (maxCompras != NULL) {
        printf("Usuario con más compras:\n");
        printf("ID: %d, Nombre: %s, Número de compras: %d\n", maxCompras->refUsuario->id, maxCompras->refUsuario->nombre, maxCompras->refUsuario->nroCompras);
    } else {
        printf("No hay usuarios con compras registradas.\n");
    }
}

void MenuUsuario(struct NodoUsuario **headUsuario, struct NodoProductos *headProductos)
{
    int opcion;
    int id;
    char *nuevoNombre;
    printf("--------------------\n");
    do {
        printf("Menu Clientes:\n");
        printf("0. Opción 0 Salir al Menú principal\n");
        printf("1. Opción 1 Agregar Usuario\n");
        printf("2. Opción 2 Mostrar Usuarios ordenados por id\n");
        printf("3. Opción 3 Eliminar Usuario\n");
        printf("4. Opción 4 Cambiar nombre de Usuario\n");
        printf("5. Opción 5 comprar producto\n");
        printf("6. Opción 6 Mostrar Historial\n");
        printf("7. Opción 7 buscar usuario por busqueda binaria\n");
        printf("8. Opción 8 usuario con mas compras realizadas\n");
        scanf("%i", &opcion);

        switch (opcion) {
            case 1:
                enlazarNodo(&(*headUsuario), crearNodo((*headUsuario)));
                printf("\nUsuario correctamente agregado\n\n");
                break;
            case 2:
                imprimirUsuariosOrdenados((*headUsuario));
                printf("\n\n");
                break;
            case 3:
                printf("Ingrese id de usuario que desea eliminar\n\n");
                scanf("%d",&id);
                eliminarUsuario(&(*headUsuario),id);
                break;

            case 4:
                printf("Ingrese id de usuario que desea cambiar el nombre\n\n");
                scanf("%d",&id);
                nuevoNombre = (char *)malloc(50 * sizeof(char));
                printf("Ingrese nuevo nombre de usuario que desea: \n\n");
                scanf("%49s", nuevoNombre);
                cambiarNombreUsuario(&(*headUsuario),id,nuevoNombre);
                break;

            case 5:
                comprar(*headUsuario,headProductos);
                break;
            case 6:
                imprimirCompras(*headUsuario);
                break;

            case 7:
                printf("Ingrese id de usuario que buscar (por busqueda binaria)\n\n");
                scanf("%d", &id);
                imprimirUsuario(busquedaBinaria(&(*headUsuario),id));
                break;
            case 8:
                imprimirUsuarioMasCompras((*headUsuario));
                break;

            default:
                printf("Ingrese opcion valida\n\n");

        }
    }while(opcion!=0);

}

void MenuProducto(struct NodoProductos **headProductos)
{
    int opcion;
    int id, recarga;
    printf("--------------------\n");
    do {
        printf("Menu Producto:\n");
        printf("0. Opción 0 Salir al Menú principal\n");
        printf("1. Opción 1 Agregar producto\n");
        printf("2. Opción 2 Ver si necesita recarga el producto\n");
        printf("3. Opción 3 Eliminar producto\n");
        printf("4. Opción 4 Recargar producto\n");
        printf("5. Opción 5 mostrar producto\n");
        printf("6. Opción 6 mostrar producto en inorden\n");
        printf("7. Opción 7 mostrar producto con mas busquedas\n");
        printf("8. Opción 8 mostrar producto con menos busuqedas\n");

        scanf("%i", &opcion);

        switch (opcion) {
            case 1:
                enlazarProducto(&(*headProductos),crearProducto(*headProductos));
                printf("\nProducto correctamente agregado\n\n");
                break;
            case 2:
                printf("ingrese id del producto que busca\n\n");
                scanf("%d",&id);
                comprobarAlerta((*headProductos),id);
                printf("\n\n");
                break;
            case 3:
                printf("Ingrese id del producto que desea eliminar\n\n");
                scanf("%d",&id);
                eliminarProducto(&(*headProductos),id);
                break;
            case 4:
                printf("Ingrese id del producto que desea recargar\n\n");
                scanf("%d",&id);
                printf("Ingrese cantidad del producto que desea recargar\n\n");
                scanf("%d",&recarga);
                if(id > 0 && recarga > 0){
                    recargarProducto(&(*headProductos),id,recarga);
                }
                else {
                    printf("id y cantidad a recargar deben ser mayor que 0\n\n");
                }
                break;
            case 5:
                printf("Ingrese id del producto que desea ver\n\n");
                scanf("%d",&id);
                if(id > 0){
                    imprimirProducto((*headProductos),id);
                }
                else {
                    printf("id no valido\n\n");
                }
                break;
            case 6:
                printf("Productos en orden inorden:\n");
                listarInordenProductos((*headProductos));
                break;
            case 7:
                imprimirProductoMasGiros((*headProductos));
                break;

            case 8:
                imprimirProductoMenosGiros((*headProductos));
                break;

            default:
                printf("Ingrese opcion valida\n\n");
        }
    }while(opcion!=0);

}


int main(){
    int opcion;
    struct mercaditoLibre *mercadito;
    mercadito=(struct mercaditoLibre*)malloc(sizeof(struct mercaditoLibre));
    mercadito->usuario=NULL;
    mercadito->productos=NULL;
    do {
        printf("1. Menu usuario\n");
        printf("2. Menu producto\n");
        printf("3. Salir\n");
        printf("Ingrese opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("\nSeleccionaste la opción 1 Menú usuario\n\n");
                MenuUsuario((&mercadito->usuario),mercadito->productos);
                break;

            case 2:
                printf("\nSeleccionaste la opción 1 Menú producto\n\n");
                MenuProducto((&mercadito->productos));
                break;
            case 3:
                printf("saliendo.");
                break;
            case 4:
                fraude_fiscal(&mercadito->productos, &mercadito->usuario);
                printf("Ingrese opcion valida\n\n");
                break;
            default:
                printf("Ingrese opcion valida\n\n");

        }

    }while(opcion!=3);

    return 0;
}
