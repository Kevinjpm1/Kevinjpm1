#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

#include "lista_doble_enlazada.h"

// Prototipo de función que imprime un menu en la terminal
int menu();

int main(void)
{
    int opcion = 0;
    string nombre = "";
    string numero = "";
    contacto *temp = NULL;

    do
    {
        opcion = menu();

        switch(opcion)
        {
            case 1:
                nombre = get_string("Nombre del contacto a ingresar: ");
                numero = get_string("Número del contacto a ingresar: ");

                temp = crear_contacto(nombre, numero);
                ingresar_contacto_inicio(temp);
                break;
            case 2:
                nombre = get_string("Nombre del contacto a ingresar: ");
                numero = get_string("Número del contacto a ingresar: ");

                temp = crear_contacto(nombre, numero);
                ingresar_contacto_final(temp);
                break;
            case 3:
                imprimir_agenda_orden();
                break;
            case 4:
                imprimir_agenda_orden_inverso();
                break;
            case 5:
                //Mensaje de salida del programa
                printf("\t\t----- Hola 📞 ----- \n");
                printf("\t\t----- Adios ☎ ----- \n");
                break;
        }
    }while (opcion != 5);

    return 0;
}

int menu()
{
    int opcion = 0;

    printf("\t\t----- AGENDA -----\n");
    printf("\t 1. Crear un contacto al inicio de la agenda\n");
    printf("\t 2. Crear un contacto al final de la agenda\n");
    printf("\t 3. Imprimir agenda\n");
    printf("\t 4. Imprimir agenda en orden descendente\n");
    printf("\t 5. Salir\n");
    printf("Escriba el número correspondiente a la opción que desee realizar\n");

    // Solicitar la opción al usuario
    do
    {
        opcion = get_int("Opción: ");
    }
    while(opcion < 1 || opcion > 5); // Validar opciones permitidas

    // Devolver el valor de la opción ingresado por el usuario
    return opcion;
}