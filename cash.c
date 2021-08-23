//Abrimos las librerias
#include <stdio.h>
#include <math.h>
#include <cs50.h>

//Declaramos una funcion que pregunte el "cambio" y guarde esa información
float cambio(void)
{
    //Esta variable informará al programa si el cambio es positivo o negativo
    float positivo;

    bool negativo = true;
    while (negativo)
    {
        //Pedimos el cambio al usuario
        positivo = get_float("Escribe tu cambio: \n");

        //Si el valopr de "positivo" es + este le dira al programa que el numero es + pero si "positivo" es - pedirá denuevo al usuario que ingrese un cambio valido
        if (positivo > 0)
        {
            //Si esto es verdadero, negativo no "existe"
            negativo = false;
        }
    }
    //Hacemos que el programa retorne el valor "positivo" que el usuario coloco
    return positivo;
}

//Declaramos una funcion que cuente el numero de monedas que se usan para dar el cambio
int monedas(void)
{
    //Redondeamos los centavos para evitar errores
    int centavos = round (cambio()* 100);

    //Inicializamos la variable que lo guarda
    int total = 0;

    //Declaramos la variable que guardará el cambio sobrante
    int residuo;

    int divisor;

    //Declaramos los tipos de de varibles que serán las monedas
    int cuarto = 25;
    int dime = 10;
    int niquel = 5;
    int centavo = 1;

    //Si nuestro cambio es mayor que el cuarto, les restará el cuarto y añadira una moneda "usada"
    while (centavos >=  cuarto)
    {
        residuo = centavos - cuarto;

        divisor = centavos -residuo;

        total = total + (divisor / cuarto);

        centavos = residuo;
    }

    //Si nuestro cambio es mayor que el dime, les restará el cuarto y añadira una moneda "usada"
    while (centavos >= dime)
    {
        residuo = centavos - dime;

        divisor = centavos - residuo;

        total = total + (divisor / dime);

        centavos = residuo;
    }

    //Si nuestro cambio es mayor que el niquel, les restará el cuarto y añadira una moneda "usada"
    while (centavos >= niquel)
    {
        residuo = centavos - niquel;

        divisor = centavos - residuo;

        total = total + (divisor / niquel);

        centavos = residuo;
    }

    //Si nuestro cambio es mayor que el centavo, les restará el cuarto y añadira una moneda "usada"
    while(centavos >= centavo)
    {
        residuo = centavos - centavo;

        divisor = centavos - residuo;

        total = total + (divisor / centavo);

        centavos = residuo;
    }

    return total;

}

int main(void)
{
    printf("%d\n", monedas());
}
