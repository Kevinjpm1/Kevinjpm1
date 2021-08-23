//Abrimos las librerias
#include <stdio.h>
#include <cs50.h>

//Declaramos la funcion principal
int main (void)
{
    //Declaramos variable
    int niveles;
    int l = 1;

    //Si el nivel de la piramide de mayor a 23 ó un numero negativo el programa no lo hará
    do
    {
        niveles = get_int("Height: ");
    }
    while (niveles > 23 || niveles < 0);

    //Creara los niveles de la piramide
    for (int i = 1; i <= niveles; i++)
    {

        //Coloca los espacios en blanco
        for (int j = 0; j < niveles - i; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < l; k++)
        {
            printf("#");
        }
        //Imprimidos el simbolo que será los bloques de nuestra priramide
        printf("#");
        l++;
        //Dejamos un espacio
        printf("\n");
    }
    
    //Le decimos al programa que concluyo
    return 0;
}