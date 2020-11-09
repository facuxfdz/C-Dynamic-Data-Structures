#include <iostream>
#include <fstream>

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


void procParteDiario(ifstream&,Node*);
void leerParteDiario(ifstream&,tsParDia&);
void procPaises(ifstream&,Node*);
void leerPais(ifstream&,tsPais&);

int main(int argc, char const *argv[])
{
    ifstream arParteDiarioBIN("ParteDiario.bin", ios::in|ios::binary);
    ifstream arPaisesBIN("Paises.bin", ios::in|ios::binary);

    
    Node* head = NULL;
    Node* headPais = NULL;

    procPaises(arPaisesBIN,headPais);
    procParteDiario(arParteDiarioBIN,head);


    return 0;
}

void leerParteDiario(ifstream &arParDia, tsParDia &registro){
    
    arParDia.read((char*)&registro, sizeof(registro));
    if(!arParDia.good()){
    	return;
	}
    cout<<registro.nombrePais<<endl;
    cout<<registro.mes<<endl;
    cout<<registro.dia<<endl;
    cout<<registro.cantidades[0]<<endl;
    cout<<registro.cantidades[1]<<endl;
    cout<<registro.cantidades[2]<<endl;
    cout<<registro.cantidades[3]<<endl<<endl;
    
}

void procParteDiario(ifstream &arParDia, Node* head_ref){
    tsParDia registro;
    cout<<"================= REGISTROS PARTE DIARIO ================="<<endl;
    while(!arParDia.eof()){
        leerParteDiario(arParDia,registro);
    }
}

void procPaises(ifstream &arPaises, Node* head_ref){
    
    tsPais regPais;
    cout<<"================= REGISTROS PAISES ================="<<endl;
    while(!arPaises.eof()){
        leerPais(arPaises,regPais);
    }
}

void leerPais(ifstream &arPaises, tsPais &regPais){
    arPaises.read((char*)&regPais, sizeof(regPais));
    if(!arPaises.good()){
    	return;
	}
    cout<<regPais.nombrePais<<endl;
    cout<<regPais.continente<<endl;
    cout<<regPais.cantHabitantes<<endl<<endl;
    
}
