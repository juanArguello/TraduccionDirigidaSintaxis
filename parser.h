// Librerias utilizadas 
#include "analizadorLexico.c"


// variable global
int estadoError = 1;


// Prototipos de los procedimientos propias
void errorSintactico();
void match(int  expectedToken);
void json();
void element(int []);
void object(int []);
void array(int []);
void o(int []);
void a(int []);
void atributte_list(int []);
void element_list(int []);
void atributte(int []);
void al(int []);
void el(int []);
void atributte_name(int []);
void atributte_value(int []);
void check_input(int [], int []);
void scanner(int []);

