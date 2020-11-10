#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <iomanip>


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
void ListadoPaises(tsCalcLista*, ofstream&);
void headerListado(string,ofstream&);
void ListadoCasos(tsCalcLista*&);
void OrdXBur(tsCalcLista*&,int);
void IntCmb(tsCalcLista*&, tsCalcLista*&);
void fabricarListado(tsCalcLista*,int,string, ofstream&);


int main()
{
    ifstream arParteDiarioBIN, arPaisesBIN;
    tsCalcLista* headPais = NULL;
    ofstream arPaises("ListadoPaises.txt");

    Abrir(arPaisesBIN,arParteDiarioBIN);
    

    procPaises(arPaisesBIN,headPais);
    procParteDiario(arParteDiarioBIN,headPais);

    ListadoPaises(headPais, arPaises);
    ListadoCasos(headPais);

    Cerrar(arPaisesBIN,arParteDiarioBIN);

    return 0;
}

void Abrir(ifstream &arPaises, ifstream &arParDia){
    arPaises.open("Paises.dat", ios::in|ios::binary);
    arParDia.open("ParteDiario.dat", ios::in|ios::binary);
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

void ListadoPaises(tsCalcLista* head_ref, ofstream &arPaises){
    
    arPaises<<setw(60)<<left<<"Listado ordenado por países y meses de casos mensuales"<<endl<<endl;
    while(head_ref != NULL){
        arPaises<<"================================================================"<<endl<<endl;
        arPaises<<"Pais: "<<head_ref->nombrePais<<setw(30)<<"Cant. Habit.: "
            <<head_ref->cantidadHabitantes<<endl<<endl;
        arPaises<<"Totales por casos de Enero a Julio"<<endl<<endl;
        arPaises<< setw(15) << right << "Hisop."
            << setw(15) << right << "Infec."
            << setw(15) << right << "Recup."
            << setw(15) << right << "Fallec." << endl;

        for(int i=0;i<4;i++){
            for(int j=0;j<7;j++){
                head_ref->totales.tipo[i] += head_ref->estadisticasMensuales[j].tipo[i]; 
            }
        }

        arPaises<< setw(15) << right << head_ref->totales.tipo[HISOPADOS]
            << setw(15) << right << head_ref->totales.tipo[INFECTADOS]
            << setw(15) << right << head_ref->totales.tipo[RECUPERADOS]
            << setw(15) << right << head_ref->totales.tipo[FALLECIDOS]<<endl<<endl;;
        arPaises<<"Totales por meses"<<endl<<endl;
        arPaises<< setw(3) << "Mes"
            << setw(15) << "Hisop."
            << setw(15) << "Infec."
            << setw(15) << "Recup."
            << setw(15) << "Fallec." << endl;
        for(int i=0;i<7;i++){
            arPaises << setw(3) << i+1
                << setw(15) << head_ref->estadisticasMensuales[i].tipo[HISOPADOS]
                << setw(15) << head_ref->estadisticasMensuales[i].tipo[INFECTADOS]
                << setw(15) << head_ref->estadisticasMensuales[i].tipo[RECUPERADOS]
                << setw(15) << head_ref->estadisticasMensuales[i].tipo[FALLECIDOS] << endl << endl;  
            
        }
        arPaises<<"================================================================"<<endl<<endl;


        head_ref = head_ref->next;
    }

    cout<<"ListadoPaises.txt ha sido generado correctamente!"<<endl<<endl;

}

void headerListado(string nombreListado, ofstream& arListado){
    arListado << setw(75) <<fixed<<"Listado de " << nombreListado << endl;
	arListado << setw( 5) << left << "Nro. "
			   << setw(14) << left << "Nom. "
			   << setw(17) << right<< "Cant.Hab. "
			    << right<< "--------------------   Cantidades de " << nombreListado << " por mes   --------------------"
			   << setw(12) << right<< "Cant." 
			   << setw(20) << right<< "Porcentajes"	<< endl;
	arListado << setw( 5) << left << "Ord. "
			   << setw(14) << left << "Pais"
			   << setw(14) << left << " "
			   << setw(11) << right << "Ene "
			   << setw(11) << right << "Feb "
			   << setw(11) << right << "Mar "
			   << setw(11) << right << "Abr "
			   << setw(11) << right << "May "
			   << setw(11) << right << "Jun "
			   << setw( 11) << right << "Jul "
			   << setw(15) << right << "Tot. " << endl;
}

void ListadoCasos(tsCalcLista*& lista){
    string nombreListado;

    for(int i=0;i<4;i++){
        string nombreArchivo = "Listado";
        if(i == 0){
            nombreListado = "Hisopados";
        }else if (i == 1){
            nombreListado = "Infectados";
        }else if (i == 2){
            nombreListado = "Recuperados";
        }else{
            nombreListado = "Fallecidos";
        }
        
        OrdXBur(lista,i);
        nombreArchivo += nombreListado + ".txt";
        ofstream arListado;
        arListado.open(nombreArchivo.c_str());
        
        fabricarListado(lista,i,nombreListado,arListado);
        cout<<nombreArchivo<<" ha sido generado exitosamente"<<endl<<endl;

        arListado.close();
    }
}

void OrdXBur(tsCalcLista*& head_ref, int tipoListado){

    tsCalcLista* i = new tsCalcLista();
    tsCalcLista* j;
    i  = head_ref;
    
        while(i->next != NULL){
            j = i->next;
            while(j != NULL){
                // Condicion: i->totales.tipo[0] < j->next->totales.tipo[0]
                unsigned long int totalesActual = i->totales.tipo[tipoListado];
                unsigned long int totalesSgte = j->totales.tipo[tipoListado]; 
                if(totalesActual < totalesSgte){
                    IntCmb(i,j);
                }
                
                j = j->next;
                
            }
            i = i->next;
            
        }
    
}

void IntCmb(tsCalcLista*& node1, tsCalcLista*& node2){
    
    char auxNombre[21];
    unsigned int auxCantHabit;
    unsigned long int auxEstadisticas[7][4];
    unsigned long int auxTotales[4];


    //aux = node1
    strcpy(auxNombre,node1->nombrePais);
    auxCantHabit = node1->cantidadHabitantes;
    for(int i=0;i<7;i++){
        auxEstadisticas[i][HISOPADOS] = node1->estadisticasMensuales[i].tipo[HISOPADOS];
        auxEstadisticas[i][INFECTADOS] = node1->estadisticasMensuales[i].tipo[INFECTADOS];
        auxEstadisticas[i][RECUPERADOS] = node1->estadisticasMensuales[i].tipo[RECUPERADOS];
        auxEstadisticas[i][FALLECIDOS] = node1->estadisticasMensuales[i].tipo[FALLECIDOS];
    }
    auxTotales[HISOPADOS] = node1->totales.tipo[HISOPADOS];
    auxTotales[INFECTADOS] = node1->totales.tipo[INFECTADOS];
    auxTotales[RECUPERADOS] = node1->totales.tipo[RECUPERADOS];
    auxTotales[FALLECIDOS] = node1->totales.tipo[FALLECIDOS];


    //node1 = node2;
    strcpy(node1->nombrePais,node2->nombrePais);
    node1->cantidadHabitantes = node2->cantidadHabitantes;
    for(int i=0;i<7;i++){
        node1->estadisticasMensuales[i].tipo[HISOPADOS] = node2->estadisticasMensuales[i].tipo[HISOPADOS];
        node1->estadisticasMensuales[i].tipo[INFECTADOS] = node2->estadisticasMensuales[i].tipo[INFECTADOS];
        node1->estadisticasMensuales[i].tipo[RECUPERADOS] = node2->estadisticasMensuales[i].tipo[RECUPERADOS];
        node1->estadisticasMensuales[i].tipo[FALLECIDOS] = node2->estadisticasMensuales[i].tipo[FALLECIDOS];
    }
    node1->totales.tipo[HISOPADOS] = node2->totales.tipo[HISOPADOS];
    node1->totales.tipo[INFECTADOS] = node2->totales.tipo[INFECTADOS];
    node1->totales.tipo[RECUPERADOS] = node2->totales.tipo[RECUPERADOS];
    node1->totales.tipo[FALLECIDOS] = node2->totales.tipo[FALLECIDOS];
    
    //node2 = aux;
    strcpy(node2->nombrePais,auxNombre);
    node2->cantidadHabitantes = auxCantHabit;
    for(int i=0;i<7;i++){
        node2->estadisticasMensuales[i].tipo[HISOPADOS] = auxEstadisticas[i][HISOPADOS];
        node2->estadisticasMensuales[i].tipo[INFECTADOS] = auxEstadisticas[i][INFECTADOS];
        node2->estadisticasMensuales[i].tipo[RECUPERADOS] = auxEstadisticas[i][RECUPERADOS];
        node2->estadisticasMensuales[i].tipo[FALLECIDOS] = auxEstadisticas[i][FALLECIDOS];
    }
    node2->totales.tipo[HISOPADOS] = auxTotales[HISOPADOS];
    node2->totales.tipo[INFECTADOS] = auxTotales[INFECTADOS];
    node2->totales.tipo[RECUPERADOS] = auxTotales[RECUPERADOS];
    node2->totales.tipo[FALLECIDOS] = auxTotales[FALLECIDOS];
    

}   

void fabricarListado(tsCalcLista* lista, int tipo, string nombreListado, ofstream& arListado){
    double totalPais = 0;
	double totalTipo = 0;
	double habitantesTotal = 0;
	double por100Pais = 0;
	double por100Total = 0;
	
    headerListado(nombreListado,arListado);
	
	for(int i=0 ; i<10 ; i++){
		
		totalPais = 0;
		arListado	<< setw( 5) << left << i+1
					<< setw(16) << left  << lista->nombrePais
					<< setw(12) << right << lista->cantidadHabitantes;
					habitantesTotal += lista->cantidadHabitantes;
						
					for(int j=0 ; j<CANT_MAX_MESES ; j++){
						arListado << setw(11) << setprecision(0) << right << lista->estadisticasMensuales[j].tipo[tipo];
						totalPais += lista->estadisticasMensuales[j].tipo[tipo];
					}
					
					totalTipo += totalPais;	
					arListado << setw(15) << setprecision(0) << right << totalPais;
					por100Pais = (totalPais * 100) / lista->cantidadHabitantes;
					arListado << setw( 14) << setprecision(6)<< right << por100Pais << " %" << endl;		
        lista = lista->next;		 
	}
	
	arListado << endl << endl;
	arListado << "Cantidad total de " << nombreListado << " hasta la fecha actual: " << setprecision(0) << totalTipo << setprecision(5) << endl;
	por100Total = (totalTipo * 100) / habitantesTotal;
	arListado << "Porcentaje de "	   << nombreListado << ": " << por100Total << " %" << endl;
}
