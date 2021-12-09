/*
    Fecha: 15/09/2021
    Descripción: Biblioteca que define la estructura de un contacto, y las funciones a utilizar
                 para crear un contacto, ingresar un contacto al inicio, o al final de una lista
                 doblemente enlazada.

    Código realizado por Anneke, Dewin, Fabiola, Jeffrey, Moisés y los fantasmas de Nindirí.
*/

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

typedef struct contacto
{
    string nombre;
    string numero;
    struct contacto *contacto_siguiente;
    struct contacto *contacto_anterior;
}contacto;

// Variable que se encarga de guardar la posición del encabezado en la agenda
contacto *encabezado = NULL;

//Funcion que crea una estructura contacto doblemente enlazada
contacto *crear_contacto(string nombre_contacto, string numero_contacto)
{
    contacto *nuevo_contacto = malloc(sizeof(contacto));

    // Guardamos los valores recibidos por la función
    nuevo_contacto->nombre = nombre_contacto;
    nuevo_contacto->numero = numero_contacto;

    // Valores por defecto de contacto anterior y siguiente, sin asignar
    nuevo_contacto->contacto_siguiente = NULL;
    nuevo_contacto->contacto_anterior = NULL;

    // Con nuestro contacto ya creado y los valores guardados
    // Retornamos el tipo contacto
    return nuevo_contacto;
}

//Funcion que crea un contacto al inicio de una lista doblemente enlazada de contactos
void ingresar_contacto_inicio(contacto *contacto_a_ingresar)
{
    // Validación de si intentamos ingresar el primer elemento, en otras palabras, aún no existe una cabecera
    if (encabezado == NULL)
    {
        encabezado = contacto_a_ingresar;
    }
    else // Si ya hay contactos ingresados antes
    {
        // Asignamos la referencia en el encabezado como contacto anterior
        encabezado->contacto_anterior = contacto_a_ingresar;
        // Asignamos la referencia al encabezado para no perder el enlace en el contacto a ingresar
        contacto_a_ingresar->contacto_siguiente = encabezado;

        // Indicamos el nuevo contacto de encabezado
        encabezado = contacto_a_ingresar;
    }
}

//Funcion que ingresa un contacto al final de una lista doblemente enlazada de contactos
void ingresar_contacto_final(contacto *contacto_a_ingresar)
{
    // Validación de si intentamos ingresar el primer elemento, en otras palabras, aún no existe una cabecera
    if (encabezado == NULL)
    {
        encabezado = contacto_a_ingresar;
    }
    else // Si ya hay contactos ingresados
    {
        // Utilizamos una variable contacto temporal, para movernos de posición en la lista
        contacto *actual = encabezado;

        // Hasta llegar al final
        while(actual->contacto_siguiente != NULL)
        {
            // Movernos al siguiente contacto
            actual = actual->contacto_siguiente;
        }

        // A este punto, actual, es el último concacto en la lista
        // Asi que, colocamos al final, el contacto a ingresar
        actual->contacto_siguiente = contacto_a_ingresar;
        // Colocamos una referencia al contacto anterior, para no perder el enlace
        contacto_a_ingresar->contacto_anterior = actual;
    }
}

//Funcion que imprime una lista doblemente enlazada de contactos, de inicio a fin
void imprimir_agenda_orden()
{
    // Utilizamos una variable temporal para obtener los datos del primer contacto
    // Y recorrer uno por uno
    contacto *actual = encabezado;
    int contador = 0;

    // Mientras no estemos en el último contacto
    while(actual != NULL)
    {
        // Imprimir bonito los datos del contacto
        printf("\t\t----- Contacto %d -----\n", contador + 1);
        printf("\tNombre: %s\n", actual->nombre);
        printf("\tTeléfono: %s\n", actual->numero);
        printf("\t\t-----------------------\n");

        // Nos movemos al siguiente contacto, si existe, o si es nulo
        actual = actual->contacto_siguiente;

        // Aumentar contador en 1
        contador++;
    }

    printf("\t\t----- FIN DE LA AGENDA -----\n");
}

//Funcion que obtiene el ultimo contacto de una lista doblemente enlazada de contactos
contacto *obtener_ultimo_contacto()
{
    // Utilizamos una variable contacto temporal, para movernos de posición en la lista
    contacto *actual = encabezado;

    // Hasta llegar al final
    while(actual->contacto_siguiente != NULL)
    {
        // Movernos al siguiente contacto
        actual = actual->contacto_siguiente;
    }

    // Si llegamos a este punto
    // la variable actual, tiene el valor del último contacto
    // en la lista enlazada
    return actual;
}

//Funcion que imprime una lista doblemente enlazada de contactos, del final al inicio
void imprimir_agenda_orden_inverso()
{
    // Crear una variable que corresponda al último contacto de la lista
    contacto *actual = obtener_ultimo_contacto();

    int contador = 0;

    // Mientras no estemos en el primer contacto nulo
    while(actual != NULL)
    {
        // Imprimir bonito los datos del contacto
        printf("\t\t----- Contacto (Descendente) %d -----\n", contador + 1);
        printf("\tNombre: %s\n", actual->nombre);
        printf("\tTeléfono: %s\n", actual->numero);
        printf("\t\t-----------------------\n");

        // Nos movemos al contacto anterior, si existe, o si es nulo
        actual = actual->contacto_anterior;

        // Aumentar contador en 1
        contador++;
    }

    printf("\t\t----- FIN DE LA AGENDA -----\n");
}