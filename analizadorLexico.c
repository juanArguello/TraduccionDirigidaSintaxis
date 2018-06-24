// Librerias utilizadas 
#include "analizadorLexico.h"


// Variables globales 

token t;							// token global para recibir componentes del Analizador Lexico
FILE *archivo;						// Fuente json
FILE *ptrOutput;					// output archivo de salida de tokens json
char id[TAMANHOLEXICO];				// Utilizado por el analizador lexico
int numeroLinea=1;					// Numero de Linea




// Funciones y procedimientos

void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numeroLinea,mensaje);	
}

void getToken()
{
	int i=0;
    char c=0;
    int bandera=0;
	int acepto=0;
	int estado=0;
	char msg[50];
	char aux[TAMANHOLEXICO] = " "; // Vector auxiliar para leer false null true
     
   	while((c=fgetc(archivo))!=EOF)
	{
        if(c=='\n')
	    {
		    //incrementar el numero de linea
		    numeroLinea++;

		    continue;
	    }
	    else if (c==' ')
	    {
            do
            {
                // se encarga de cargar los espacios leidos

                c=fgetc(archivo); 
            }while(c ==' ');
            c=ungetc(c,archivo);
	    }
        else if (c=='\t')
        { 
            //si es un tabulador que guarde los 4 espacios correspondientes
            while(c=='\t')
            {

	        	c=fgetc(archivo);
            }
        }
    	else if (isdigit(c))
	    {
            //es un numero
            i=0;
            estado=0;
            acepto=0;
            id[i]=c;
			while(!acepto)
			{
				switch(estado)
				{
				    case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.')
						{
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;		
					case 1://un punto, debe seguir un digito 
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else{
							sprintf(msg,"No se esperaba '%c' despues del . ",c);
							fputs("Error lexico",ptrOutput);
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
						{
							sprintf(msg,"Se esperaba signo o digitos despues del exponente");
							fputs("Error lexico",ptrOutput);
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
						{
							sprintf(msg,"No se esperaba '%c' despues del signo",c);
							fputs("Error lexico",ptrOutput);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
							estado=6;
						break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF)
							ungetc(c,archivo);
						else
                            c=0;
						id[++i]='\0';
						acepto=1;
                        t.componenteLexico=NUMBER;
						strcpy(t.lexema,id);
//						fputs("NUMBER",ptrOutput);
//						fputs(" ",ptrOutput);
						break;
					case -1:
						if (c==EOF){
                            error("No se esperaba el fin de archivo\n");
							fputs("Error lexico",ptrOutput);
						}
						else
                            error(msg);
                        acepto=1;
                    t.componenteLexico=VACIO;
                    while(c!='\n')
                        c=fgetc(archivo);
                    ungetc(c,archivo);
					break;
				}
			}
			break;
		}
        else if (c=='\"')
		{
            //un caracter o una cadena de caracteres
			i=0;
			id[i]=c;
			i++;
			do
			{
				c=fgetc(archivo);
				if (c=='\"')
				{
                    id[i]=c;
                    i++;
                    bandera=1;
                    break;
				}
                else if(c==EOF || c==',' || c=='\n' || c==':')
				{
                    sprintf(msg,"Se esperaba que finalice el literal");
					error(msg);
					fputs("Error lexico",ptrOutput);
                    
                    while(c!='\n')
                        c=fgetc(archivo);

                    ungetc(c,archivo);
                    break;                       
				}
				else
				{
					id[i]=c;
					i++;
				}
			}while(isascii(c) || bandera==0);
			    id[i]='\0';
            strcpy(t.lexema,id);
			t.componenteLexico = STRING;
//			fputs("STRING",ptrOutput);
//			fputs(" ",ptrOutput);
			break;
		}
		else if (c==':')
		{
            //puede ser un :
            t.componenteLexico=DOS_PUNTOS;
            strcpy(t.lexema,":");

            break;
		}
		else if (c==',')
		{
			t.componenteLexico=COMA;
			strcpy(t.lexema,",");

			break;
		}
		else if (c=='[')
		{
			t.componenteLexico=L_CORCHETE;
			strcpy(t.lexema,"[");

			break;
		}
		else if (c==']')
		{
			t.componenteLexico=R_CORCHETE;
			strcpy(t.lexema,"]");

			break;
		}
		else if (c=='{')
		{
			t.componenteLexico=L_LLAVE;
			strcpy(t.lexema,"{");

			break;		
        }
        else if (c=='}')
		{
			t.componenteLexico=R_LLAVE;
			strcpy(t.lexema,"}");
			
			break;		
        }
		else if (c=='n' || c=='N')
        {
            ungetc(c,archivo);
            fgets(aux,5,archivo);//ver si es null
            if (strcmp(aux, "null")==0 || strcmp(aux, "NULL")==0)
            {
                t.componenteLexico = PR_NULL;
                strcpy(t.lexema,aux);

            }
            else
            {
                sprintf(msg,"%c no esperado",c);
			    error(msg);
				fputs("Error lexico",ptrOutput);

                while(c!='\n')
                    c=fgetc(archivo);

                t.componenteLexico = VACIO;
                ungetc(c,archivo);
            }
            break;
        }   
        else if (c=='f' || c=='F')
        {
            ungetc(c,archivo);
            fgets(aux,6,archivo);//ver si es null
            if (strcmp(aux, "false")==0 || strcmp(aux, "FALSE")==0)
            {
                t.componenteLexico = PR_FALSE;
                strcpy(t.lexema,aux);

            }
            else{
                sprintf(msg,"%c no esperado",c);
			    error(msg);
				fputs("Error lexico",ptrOutput);

                while(c!='\n')
                    c=fgetc(archivo);    

                t.componenteLexico = VACIO;
                ungetc(c,archivo);
            }
            break;
        }   
        else if (c=='t' || c=='T')
        {
            ungetc(c,archivo);
            fgets(aux,5,archivo);//ver si es null
            if (strcmp(aux, "true")==0 || strcmp(aux, "TRUE")==0)
            {
                t.componenteLexico = PR_TRUE;
                strcpy(t.lexema,aux);

            }
            else
            {
                sprintf(msg,"%c no esperado",c);
			    error(msg);
				fputs("Error lexico",ptrOutput);

                while(c!='\n')
                    c=fgetc(archivo);

                t.componenteLexico = VACIO;
                ungetc(c,archivo);
            }
            break;
        }
        else if (c!=EOF)
		{
			sprintf(msg,"%c no esperado",c);
			error(msg);
			fputs("Error lexico",ptrOutput);
            while(c!='\n')
                c=fgetc(archivo);
            ungetc(c,archivo);
		}
	}
	if (c==EOF)
	{
		t.componenteLexico=EOF;
		strcpy(t.lexema,"EOF");
		sprintf(t.lexema,"EOF");

	}
	
}

