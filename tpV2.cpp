#include <iostream>
#include <fstream>
#include <string.h>
#include <string>


using namespace std;

#define CANT_MAX_MESES 7

enum tipo {
    HISOPADOS = 0,
    INFECTADOS,
    RECUPERADOS,
    FALLECIDOS
};

struct
{
    unsigned long int tipo[4];
}typedef acumuladosMensuales;

struct
{
    unsigned long int tipo[4];
}typedef acumuladosTotales;

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

struct tsCalcLista
{
    char nombrePais[21];
    unsigned int cantidadHabitantes;
    acumuladosMensuales estadisticasMensuales[CANT_MAX_MESES];
    acumuladosTotales totales;
    tsCalcLista* next;
};

void Abrir(ifstream&,ifstream&);
void Cerrar(ifstream&,ifstream&);
void leerPais(ifstream&,tsPais&);
void leerParteDiario(ifstream&,tsParDia&);
void procPaises(ifstream&,tsCalcLista*&);
void procParteDiario(ifstream&,tsCalcLista*&);
void push(tsCalcLista*&, tsPais);
void push(tsCalcLista*&, tsParDia);
void inicValoresParDia(tsCalcLista*&);
void insertarParteDiario(tsCalcLista*&, tsParDia);
bool compare(char[],char[]);


int main()
{
    ifstream arParteDiarioBIN, arPaisesBIN;
    tsCalcLista* headPais = NULL;
    
    Abrir(arPaisesBIN,arParteDiarioBIN);
    

    procPaises(arPaisesBIN,headPais);
    procParteDiario(arParteDiarioBIN,headPais);

    

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

void leerPais(ifstream &arPaises, tsPais &regPais){
    arPaises.read((char*)&regPais, sizeof(regPais));
    

    if(!arPaises.good()){
        strcpy(regPais.nombrePais,"");
    }
}

void leerParteDiario(ifstream &arParDia, tsParDia &registro){
    
    arParDia.read((char*)&registro, sizeof(registro));
    registro.mes -= 1;
    if(!arParDia.good()){
        strcpy(registro.nombrePais,"");
    }
    
}

void procPaises(ifstream &arPaises, tsCalcLista*& head_ref){
    
    tsPais regPais;

    
    while(!arPaises.eof()){
        leerPais(arPaises,regPais);

        if(strcmp(regPais.nombrePais, "") == 0){
            break;
        }
        
        /* Logica de guardado del registro */
        push(head_ref,regPais);
    }
}

void procParteDiario(ifstream &arParDia, tsCalcLista*& head_ref){

    tsParDia registro;

    
    while(!arParDia.eof()){
        leerParteDiario(arParDia,registro);

        if(strcmp(registro.nombrePais,"") == 0){
            break;
        }

        // Logica de guardado del registro
        push(head_ref,registro);

    }
}

void push(tsCalcLista*& head_ref, tsPais regPais){
    
    // Crear los nodo que voy a necesitar
    tsCalcLista* nuevo_pais = new tsCalcLista();
    tsCalcLista* aux1 = head_ref;
    tsCalcLista* aux2;

    // Agregar la data al nodo a insertar
    strcpy(nuevo_pais->nombrePais,regPais.nombrePais);
    nuevo_pais->cantidadHabitantes = regPais.cantHabitantes;
    nuevo_pais->next = NULL;

    // Se posiciona el nodo de manera ordenada en la lista
    while(aux1 != NULL && strcmp(aux1->nombrePais,regPais.nombrePais)<0){
        aux2 = aux1;
        aux1 = aux1->next;
    }

    if(head_ref == aux1){
        head_ref = nuevo_pais;
    }else{
        aux2->next = nuevo_pais;
     }
    nuevo_pais->next = aux1;

    // Inicializo los valores que faltan llenar, para luego llenarlos en la lectura de ParteDiario
    inicValoresParDia(nuevo_pais);
    
}

void push(tsCalcLista* &head_ref, tsParDia regParDia){
    
   
    tsCalcLista* aux1 = new tsCalcLista();
    aux1 = head_ref;
    
    
    // Recorro la lista buscando el pais que haga match
    while(aux1 != NULL){

        if(compare(regParDia.nombrePais,aux1->nombrePais)){
            
            
            // Actualizo todos los contadores
            insertarParteDiario(aux1,regParDia);
            
            break;
        }
        aux1 = aux1->next;
    }
}

void inicValoresParDia(tsCalcLista*& node){
    for(int i=0;i<7;i++){
        for(int j=0;j<4;j++){
            node->estadisticasMensuales[i].tipo[j] = 0;
        }
    }
    for(int i=0;i<4;i++){
        node->totales.tipo[i] = 0;
    }
}

void insertarParteDiario(tsCalcLista* &node, tsParDia regParDia){

    for(int j=0;j<4;j++){
        node->estadisticasMensuales[regParDia.mes].tipo[j] += regParDia.cantidades[j];
        
    }

    for(int i=0;i<7;i++){
        node->totales.tipo[HISOPADOS] += node->estadisticasMensuales[i].tipo[HISOPADOS];
        node->totales.tipo[INFECTADOS] += node->estadisticasMensuales[i].tipo[INFECTADOS];
        node->totales.tipo[RECUPERADOS] += node->estadisticasMensuales[i].tipo[RECUPERADOS];
        node->totales.tipo[FALLECIDOS] += node->estadisticasMensuales[i].tipo[FALLECIDOS];
    }
}

bool compare(char cad1[],char cad2[]){
    string cad1Str = "";
    string cad2Str = "";

    for(int i=0;i<21;i++){
        if(cad1[i] == ' '){
            break;
        }
        cad1Str += cad1[i];
    }
    for(int i=0;i<21;i++){
        if(cad2[i] == ' '){
            break;
        }
        cad2Str += cad2[i];
    }
    if(cad1Str == cad2Str){
        return true;
    }else{
        return false;
    }

}