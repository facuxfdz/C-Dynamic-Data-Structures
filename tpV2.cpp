#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

struct
{
    char nombrePais[21];
    unsigned short int mes;
    unsigned short int dia;
    unsigned int cantidades[4];
}typedef tsParDia;

struct
{
    char nombrePais[21];
    char continente[21];
    unsigned int cantHabitantes;
}typedef tsPais;

struct Node
{
    int val;
    Node* next;
};

void Abrir(ifstream&,ifstream&);
void Cerrar(ifstream&,ifstream&);
void procParteDiario(ifstream&,Node*);
void leerParteDiario(ifstream&,tsParDia&);
void procPaises(ifstream&,Node*);
void leerPais(ifstream&,tsPais&);

int main(int argc, char const *argv[])
{
    ifstream arParteDiarioBIN, arPaisesBIN;

    Abrir(arPaisesBIN,arParteDiarioBIN);
    Node* head = NULL;
    Node* headPais = NULL;

    procPaises(arPaisesBIN,headPais);
    procParteDiario(arParteDiarioBIN,head);

    Cerrar(arPaisesBIN,arParteDiarioBIN);
    
    return 0;
}

void Abrir(ifstream &arPaises, ifstream &arParDia){
    arPaises.open("Paises.bin", ios::in|ios::binary);
    arParDia.open("ParteDiario.bin", ios::in|ios::binary);
}

void Cerrar(ifstream &arPaises, ifstream &arParDia){
    arPaises.close();
    arParDia.close();
}
void leerParteDiario(ifstream &arParDia, tsParDia &registro){
    
    arParDia.read((char*)&registro, sizeof(registro));
    if(!arParDia.good()){
        strcpy(registro.nombrePais,"");
    }
    
}

void procParteDiario(ifstream &arParDia, Node* head_ref){

    tsParDia registro;

    while(!arParDia.eof()){
        leerParteDiario(arParDia,registro);

        if(strcmp(registro.nombrePais,"") == 0){
            break;
        }

        /* Logica de guardado del registro */
    }
}

void procPaises(ifstream &arPaises, Node* head_ref){
    
    tsPais regPais;

    while(!arPaises.eof()){
        leerPais(arPaises,regPais);

        if(strcmp(regPais.nombrePais, "") == 0){
            break;
        }
        
        /* Logica de guardado del registro */
    }
}

void leerPais(ifstream &arPaises, tsPais &regPais){
    arPaises.read((char*)&regPais, sizeof(regPais));
    if(!arPaises.good()){
        strcpy(regPais.nombrePais,"");
    }
}