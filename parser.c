
#include "parser.h"


// Implementacion de los procedimientos propios

void errorSintactico(){
	estadoError=0;
   	printf("\nError sintactico: Linea %d. Se encontro %s.\n", numeroLinea, t.lexema);
}
	
void match(int expectedToken ){
	if(t.componenteLexico == expectedToken)
		getToken();	
}
	

void json(){
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	element(conjunto_siguiente);
		
}

void element(int conjSincronizacion[]){
	int conjunto_primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int conjunto_siguiente[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};

    check_input(conjunto_primero, conjSincronizacion);

    if(t.componenteLexico == L_LLAVE)
        object(conjunto_siguiente);
    else if(t.componenteLexico == L_CORCHETE)
        array(conjunto_siguiente);

    check_input(conjunto_siguiente, conjunto_primero);
	
}

void object(int conjSincronizacion[]){
	int conjunto_primero[] = {L_LLAVE, '\0'};
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	
	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == L_LLAVE) {
		match(L_LLAVE);
		o(conjunto_siguiente);
	}
	
	check_input(conjunto_siguiente, conjunto_primero);	
}

void array(int conjSincronizacion[]){
	int conjunto_primero[] = {L_CORCHETE, '\0'};
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == L_CORCHETE){
		match(L_CORCHETE);
		a(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void o(int conjSincronizacion[]){
	int conjunto_primero[] = {STRING, R_LLAVE, '\0'};

	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, STRING, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == STRING){
		atributte_list(conjSincronizacion);
		match(L_LLAVE);
	} else if (t.componenteLexico == R_LLAVE){
		match(L_LLAVE);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void a(int conjSincronizacion[]){
	int conjunto_primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};

	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, L_CORCHETE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if ((t.componenteLexico == L_LLAVE) || (t.componenteLexico == L_CORCHETE)){
		
		element_list(conjunto_siguiente);
		match(R_CORCHETE);
	}
	else if(t.componenteLexico == R_CORCHETE){
		match(R_CORCHETE);
		
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void atributte_list(int conjSincronizacion[]){
	int conjunto_primero[] = {STRING, '\0'};
	int conjunto_siguiente[] = {R_LLAVE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == STRING){
		atributte(conjunto_siguiente);
		al(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void element_list(int conjSincronizacion[]){
	if(t.componenteLexico == R_CORCHETE)
       return;

	int conjunto_primero[] = {COMA,'\0'};
	int conjunto_siguiente[] = {R_CORCHETE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	

	if (t.componenteLexico == COMA){
		match(COMA);
		element(conjunto_siguiente);
		el(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void atributte(int conjSincronizacion[]){
	int conjunto_primero[] = {STRING, '\0'};
	int conjunto_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == STRING){
		atributte_name(conjunto_siguiente);
		match(DOS_PUNTOS);
		atributte_value(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void al(int conjSincronizacion[]){
	if (t.componenteLexico == R_LLAVE)
        return;

	int conjunto_primero[] = {COMA, '\0'};
	int conjunto_siguiente[] = {R_LLAVE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);


	if (t.componenteLexico == COMA){
		match(COMA);
		atributte(conjunto_siguiente);
		al(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void el(int conjSincronizacion[]){
	if(t.componenteLexico == R_CORCHETE)
       return;

	int conjunto_primero[] = {COMA,'\0'};
	int conjunto_siguiente[] = {R_CORCHETE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	

	if (t.componenteLexico == COMA){
		match(COMA);
		element(conjunto_siguiente);
		el(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void atributte_name(int conjSincronizacion[]){
	int conjunto_primero[] = {STRING, '\0'};
	int conjunto_siguiente[] = {DOS_PUNTOS, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == STRING)
		match(STRING);

	check_input(conjunto_siguiente, conjunto_primero);
}


void atributte_value(int conjSincronizacion[]){
	int conjunto_primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int conjunto_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(conjunto_primero, conjSincronizacion);

	if (t.componenteLexico == L_LLAVE){
		element(conjunto_siguiente);
	}
	else if (t.componenteLexico == L_CORCHETE) {
		element(conjunto_siguiente);
	}
	else if (t.componenteLexico == STRING){
		match(STRING);
	}
	else if (t.componenteLexico == NUMBER){
		match(NUMBER);
	}
	else if (t.componenteLexico == PR_TRUE){
		match(PR_TRUE);
	}
	else if (t.componenteLexico == PR_FALSE){
		match(PR_FALSE);
	}
	else if (t.componenteLexico == PR_NULL){
		match(PR_NULL);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}


void check_input(int conjunto_primero[], int conjunto_siguiente[]){

    int indice1=0;
    if(t.componenteLexico == EOF)
    	return;

    //entra cuando no hay errores
    while(conjunto_primero[indice1] != '\0') 
    {
        if(t.componenteLexico == conjunto_primero[indice1])
            return;
        indice1++;
    }
 
 	indice1=0;    
    
    int conjSincronizacion[]={1}; //acumulador
    
    while(conjunto_primero[indice1] != '\0'){ 
        conjSincronizacion[indice1] = conjunto_primero[indice1];
        indice1++;
    }
    int indice2=0;

    while(conjunto_siguiente[indice2] != '\0'){
        conjSincronizacion[indice1] = conjunto_siguiente[indice2];
        indice1++; //aumenta indice1
        indice2++; //aumenta indice2
    }

    scanner(conjSincronizacion);

}



void scanner(int synch[]){
    errorSintactico();
	int indice1 = 0;
    while(t.componenteLexico != synch[indice1] && t.componenteLexico != EOF){   
        if (synch[indice1] == '\0'){
            getToken();
            indice1=0;        
        }
        indice1++;
    }
    //el getToken que falta luego de salir
    getToken();
    return;    	   
}




