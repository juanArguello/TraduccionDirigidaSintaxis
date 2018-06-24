#include "traduccion.h"



void json_trad() {
    element_trad();
}


void element_trad(){
    if(t.componenteLexico == L_LLAVE){
        object_trad();
    }
    else if(t.componenteLexico == L_CORCHETE){
        array_trad();
    }
}


void array_trad(){  
    if(t.componenteLexico == L_CORCHETE){
        match(L_CORCHETE);
        a_trad();
    }
}


void a_trad(){
    if(t.componenteLexico == L_CORCHETE || t.componenteLexico == L_LLAVE){
		fputs("\n",ptrOutput);
        identacion();
		fputs("<item>\n",ptrOutput);
		espacios+=1;
        element_list_trad();
        match(R_CORCHETE);
        identacion();
		fputs("</item>\n",ptrOutput);
    }
    else if(t.componenteLexico == R_CORCHETE){
		espacios-=1;
        match(R_CORCHETE);
    }
}


void element_list_trad(){
    if(t.componenteLexico == L_CORCHETE || t.componenteLexico == L_LLAVE){
        element_trad();
        el_trad();
        
    }
}


void el_trad(){
    if(t.componenteLexico == COMA){  
        match(COMA);
		identacion();
		fputs("</item>\n",ptrOutput);
		identacion();
		fputs("<item>\n",ptrOutput);
		espacios+=1;
        element_trad();
        el_trad();     
    }
}


void object_trad(){
    if(t.componenteLexico == L_LLAVE){
        match(L_LLAVE);
        o_trad();
    }
}


void o_trad(){
    if(t.componenteLexico == STRING){
        atributte_list_trad();
        match(R_LLAVE);

    }
    else if(t.componenteLexico == R_LLAVE){
        match(R_LLAVE);
    }
}


void atributte_list_trad(){
    if(t.componenteLexico == STRING){
        atributte_trad();
        al_trad();
    }
}


void al_trad(){
    if (t.componenteLexico == R_LLAVE){
        espacios-=1;
		return;
    }
    if(t.componenteLexico == COMA){
        match(COMA);
        atributte_trad();
        al_trad();
    }
}


void atributte_trad(){
    if(t.componenteLexico == STRING){
		identacion();
        char lexema[TAMANHOLEXICO];
        strcpy(lexema,t.lexema);
		fputs("<",ptrOutput);
        atributte_name_trad();
		fputs(">",ptrOutput);
        match(DOS_PUNTOS);
        atributte_value_trad();
		fputs("</",ptrOutput);
        string_trad_sin_comillas(lexema);
		fputs(">\n",ptrOutput);		
    }
}


void atributte_name_trad(){
    if(t.componenteLexico == STRING){    
        string_trad_sin_comillas(t.lexema);
        match(STRING);
    }
}



void atributte_value_trad(){
    if(t.componenteLexico == L_CORCHETE || t.componenteLexico == L_LLAVE){
		espacios += 1;
        element_trad();
    }
    else if(t.componenteLexico == STRING){
		fputs(t.lexema, ptrOutput);
        match(STRING);
    }
    else if(t.componenteLexico == NUMBER){
		fputs(t.lexema, ptrOutput);
        match(NUMBER);
    }
    else if(t.componenteLexico == PR_TRUE){
		fputs(t.lexema, ptrOutput);
        match(PR_TRUE);
    }
    else if(t.componenteLexico == PR_FALSE){
		fputs(t.lexema, ptrOutput);
        match(PR_FALSE);
    }
    else if(t.componenteLexico == PR_NULL){
		fputs(t.lexema, ptrOutput);
        match(PR_NULL);
    }
}

void string_trad_sin_comillas(char* cadena){
    char string[TAMANHOLEXICO];
	char cad[TAMANHOLEXICO] = "";
    strcpy(string,cadena);
    int i=1;
    while(string[i] != '"'){
		cad[i-1] = string[i];
        i++;
    }
	fputs(cad, ptrOutput);
}

void identacion(){
    int i;
    for (i=1;i<=espacios;i++){
        fputs("\t", ptrOutput);
    }
}



// funcion principal
int main (int argc,char* args[]){
    if(argc > 1)
    {
        if(!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrada.\n");
            exit(1);
        }
        getToken();
        json();
        if(estadoError){
            fclose(archivo);
            archivo=fopen(args[1],"rt");
			ptrOutput=(fopen("output.xml","w"));
            getToken();
            json_trad();
        }
		fclose(ptrOutput);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}

