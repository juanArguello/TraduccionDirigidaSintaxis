// Librerias utilizadas 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


// Definicion de tokens con valores numericos simbolicos
#define L_CORCHETE		256
#define R_CORCHETE		257
#define L_LLAVE			258
#define R_LLAVE			259
#define COMA			300
#define DOS_PUNTOS		301
#define STRING			302
#define NUMBER			303
#define PR_TRUE			304
#define PR_FALSE		305
#define PR_NULL			306


// constantes de tamaños
#define TAMANHOBUFFER 				5		// tamaño del buffer
#define TAMANHOLEXICO 				50		// tamaño del lexema
#define TAMANHOHASH 				101		// tamaño de la tabla hash
#define VACIO                 		1


// estructura de tokens
typedef struct{
	int componenteLexico;			// componente lexico
	char lexema[TAMANHOLEXICO];		// lexema		
} token;




void getToken();
