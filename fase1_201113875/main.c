/*    Universidad de San Carlos de Guatemala
            Facultad de Ingenieria
        Escuela de Ciencias y Sistemas
  Laboratorio Manejo e Implementacion de Archivos
            Seccion "A-"   Fase 1
         Entrega: 12/06/2016 23:59 hrs
    Peter Alexander Samuels Franco 2011-13875
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//############ VARIABLES GLOBALES ##################
int fin=0;
char Dot_MBR[5000];
char Dot_EBR[5000]=" ";
int switch_mbr=0;
int switch_ebr=0;

char cadG[5000];
char cadE[5000];
char cadd[5000];

char sincomillas[100];

char *auxconc;
char *auxconc2;
//############ FIN VARIABLES GLOBALES ##################


//############ ESTRUCTURAS ##################
typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
    int formateada;
}Particion;

typedef struct{
    int mbr_tamano;
    char mbr_fecha_creacion[25];
    int mbr_disk_signature;
    Particion particiones[4];
}MbrDisco;

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

typedef struct{
//ESTRUCTURA PARA LOS MONTADOS
    int estado;
    char id[5];
}Posicion;

typedef struct{
    char path[200];
    char *path2;
    Posicion posicion[26];
}Montaje;

typedef struct{
    int status;
    char path3[100];
    char *path2;
    char letrafinal;
    int numerofinal;
    char id[6];
    char*name;
    char name2[100];
}Montaje2;


typedef struct{
    int status;
    char letra;
    int numero;
    char pathPerteneciente[200];
    char *pathPertenecienteP;
    int posiciones[26];
}letra;

typedef struct
{
    char id[100];
}ids;

//############ FIN ESTRUCTURAS ##################
letra letras[26];
Montaje montadas[20];
Montaje2 montadas2[50];
ids listaIds[20];

int NumeroDeMontadas=0;
int auxletras=0;
Montaje2 resienMontada;
int posDeMontada_rep=0;

//############ METODOS ##################

void llenarvacios(){
    int i;
    for(i=0; i<20; i++){
        strcpy(montadas[i].path,"vacio");
        int y=0;
        for(y = 0; y< 26;y++){
            montadas[i].posicion[y].estado=0;
        }
    }
}

void limpiarvar(char aux[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        aux[i] = '\0';
    }
}

int EsLetra(char caracter){
    if((caracter >= 'a' && caracter <= 'z' ) || (caracter >= 'A' && caracter <= 'Z' ))
    {
        return 1;
    }else
    {
        return 0;
    }
}

int CadenaEsNumero(char entrada[]){
    int resultado=0;
    int contador=0;
    while(entrada[contador]){
        if(EsNumero(entrada[contador])==0)
        {
            resultado++;
        }
        contador++;
    }
    return resultado;
}

int EsNumero (char caracter2[]){
    if(caracter2 >= '0' && caracter2 <= '9')
    {
        return 1;
    }else
    {
        return 0;
    }
}

char*ConcatenarCadenaCaracter(char* cadena,char caracter){
    auxconc=(char*)malloc(1+(strlen(cadena)+1));
    int contador=0;
    while (contador<(strlen(cadena)))
    {
        auxconc[contador]=cadena[contador];
        contador++;
    }
    auxconc[contador]=caracter;
    contador++;

    while(auxconc[contador]!=NULL)
    {
        auxconc[contador]=NULL;
        contador++;
    }
    return auxconc;
}

char* LeerScript(char path[]){
    char* pathoriginal=path;
    char finalizado[250];
    strcpy(finalizado,"cd /\n");
    if(pathoriginal[0]=='\"')
    {
        pathoriginal++;
        path++;
        int q=0;
        while(path[q]!='\"')
        {
            q++;
        }
        path[q]='\0';
        pathoriginal[q]='\0';
    }
    FILE *archivo;
    char caracter;
    char* cadenafinal="";
    archivo = fopen(path,"r");

    if (archivo == NULL)
    {
        cadenafinal="Archivo No Encontrado en el Directorio";
	printf("\n FATAL ERROR: En Apertura del Archivo. \n\n");
        ErrorInterprete++;
    }
    else
    {
        printf("\n--------- Contenido del Archivo de Entrada ---------- \n\n");
        int n=0;
        char nuevo;
        int contador=0;
        while(feof(archivo)==0){
        fseek(archivo,(sizeof(char)*contador),SEEK_SET);
        fread(&nuevo,sizeof(char),1,archivo);
        cadenafinal=ConcatenarCadenaCaracter(cadenafinal,nuevo);
	contador++;
        }
        fclose(archivo);
    }
    return cadenafinal;
}

void llenarletras(){
    letras[0].letra='a';
    letras[1].letra='b';
    letras[2].letra='c';
    letras[3].letra='d';
    letras[4].letra='e';
    letras[5].letra='f';
    letras[6].letra='g';
    letras[7].letra='h';
    letras[8].letra='i';
    letras[9].letra='j';
    letras[10].letra='k';
    letras[11].letra='l';
    letras[12].letra='m';
    letras[13].letra='n';
    letras[14].letra='o';
    letras[15].letra='p';
    letras[16].letra='q';
    letras[17].letra='r';
    letras[18].letra='s';
    letras[19].letra='t';
    letras[20].letra='u';
    letras[21].letra='v';
    letras[22].letra='w';
    letras[23].letra='x';
    letras[24].letra='y';
    letras[25].letra='z';

    int i=0;
    for(i=0; i<50; i++){
    montadas2[i].status=0;
    }

    i=0;
    for(i=0; i<26; i++){
    letras[i].status=0;
    strcpy(letras[i].pathPerteneciente,"vacio");
    letras[i].pathPertenecienteP="vacio";
    }
}
//############ FIN METODOS ##################

//############ EMPIEZA LA MAGIA ##################
int main(){
    llenarletras();
    menu1();
	printf("\n");
   	printf("Gracias por utilizar este programa. \n");
   	printf("\n");
    return 0;
}

void Analizador(char entrada[]){
    Funcion nuevafuncion;
    int contadorIds=0;
    int idlistado=0;
    int size=0;
    int unit=0;
    int path=0;
    int type=0;
    int name=0;
    int delete_=0;
    int add=0;
    int fit=0;
    int id=0;
    int numeroparametros=0;
    int barraactiva=0;
    int contador=0;
    int p=0;
    int count=0;
    int activarmount=0;
    int contadorParametrosObligatorios=0;
    char instruccion[100];
    limpiarvar(instruccion,100);
    char nombreparametro[100];
    limpiarvar(nombreparametro,100);
    char parametro[100];
    limpiarvar(parametro,100);
    nuevafuncion.count2=0;


    printf("---------------------- INICIA ANALIZADOR -----------------------\n");
    printf( "Se A Ejecutado ----- Analizador:: \"%s\" \n", entrada );

    while(entrada[contador]!=NULL){
        //COMENTARIO
        if(entrada[contador]=='#'){
            printf(" *************** RECONOCIDO: COMENTARIO(#) ****************\n");
            while(entrada[contador]!='\n' && entrada[contador]!='\0')
            {
                contador++;
            }
        }
         else if(entrada[contador]==' ')
        {
            printf("*************** RECONOCIDO: ESPACIO( ) ****************\n");
            contador++;
        }else if(EsLetra(entrada[contador])==1){
            while(EsLetra(entrada[contador])==1)
            {
                char y1[1];
                y1[0]=entrada[contador];
                strncat(instruccion,y1,1);
                contador++;
            }
            size=0;
            unit=0;
            path=0;
            type=0;
            fit=0;
            delete_=0;
            name=0;
            add=0;
            numeroparametros=0;
            id=0;
            idlistado=0;

            int contadormay=0;
            while(instruccion[contadormay]){
                instruccion[contadormay]=tolower(instruccion[contadormay]);
                contadormay++;
            }
            strcpy(nuevafuncion.nombre,instruccion);
            printf("Analizador - Instruccion ::= %s\n", nuevafuncion.nombre);
        }else if(entrada[contador]=='\\')
        {
            barraactiva=1;
            printf("*************** RECONOCIDO: CONTINUACION DE COMANDO(\) ***************\n");
            contador++;
        }else if(entrada[contador]=='-'){
            numeroparametros++;
            contador++;
            while(entrada[contador]!=':'){
                char y2[1];
                y2[0]=entrada[contador];
                strncat(nombreparametro,y2,1);
                contador++;
            }
            if(entrada[contador]==':'&&entrada[contador+1]==':'){
                 contador=contador+2;
                 int contadorpar=0;
                 while(nombreparametro[contadorpar]){
                     nombreparametro[contadorpar]=tolower(nombreparametro[contadorpar]);
                     contadorpar++;
                 }
                 printf("Analizador - RECONOCIDO: PARAMETRO(sin mayusculas)= %s\n",nombreparametro);
                 char prueba;
                 if(entrada[contador]!='\"'){
                     while(entrada[contador]!=' ' && entrada[contador]!='\n' && entrada[contador]!='#' && entrada[contador]!=NULL){
                     char y3[1];
                     y3[0]=entrada[contador];
                     strncat(parametro,y3,1);
                     contador++;
                     }
                     printf("-------- CONTENIDO PARAMETRO = %s --------\n",parametro);
                     char fn[1];
                     fn[0]='\0';
                     strncat(parametro,fn,1);
                 }else{
                     char y4[1];
                     y4[0]=entrada[contador];
                     strncat(parametro,y4,1);
                     contador++;
                     while(entrada[contador]!='\"'){
                     prueba=entrada[contador];
                     char y5[1];
                     y5[0]=entrada[contador];
                     strncat(parametro,y5,1);
                     contador++;
                     }
                     char y6[1];
                     y6[0]=entrada[contador];
                     strncat(parametro,y6,1);
                     contador++;
                     char fn[1];
                     fn[0]='\0';
                     strncat(parametro,fn,1);
                     printf("-------- CONTENIDO PARAMETRO = %s ---------\n",parametro);
                 }
            }else{
            printf("Analizador - FATAL ERROR: Sintaxis Incorrecta Falta ':' \n");
            ErrorInterprete++;
            }
            if(!strcmp(nombreparametro,"size")){
                size=1;
                if(CadenaEsNumero(parametro)==0){
                    nuevafuncion.size=atoi(parametro);
                    printf("size: %i\n",nuevafuncion.size);

                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);

                }else if(parametro[0]=='-'){
                    if(CadenaEsNumero(parametro)==0)
                    {
                        nuevafuncion.size=atoi(parametro)*-1;
                        printf("size: %i\n",nuevafuncion.size);
                        limpiarvar(parametro,100);
                        limpiarvar(nombreparametro,100);
                    }
                }else{
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'size' invalido. \n\n");
                    ErrorInterprete++;
                    size=0;
                }
            }else
            if(!strcmp(nombreparametro,"unit")){
                unit=1;
                if(parametro[0]=='m' || parametro[0]=='k' || parametro[0]=='b'||parametro[0]=='M' || parametro[0]=='K' || parametro[0]=='B'){
                    nuevafuncion.unit=parametro[0];
                    printf("unit: %c\n",nuevafuncion.unit);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    unit=0;
                    printf("\n\n Analizador -  FATAL ERROR: Parametro 'unit' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
             if(!strcmp(nombreparametro,"path")){
                path=1;
                strcpy(nuevafuncion.path,parametro);
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);

            }else
            if(!strcmp(nombreparametro,"name")){
                name=1;
                char env[100];
                strcpy(env,parametro);
                quitarComillas(env);
                strcpy(nuevafuncion.name,sincomillas);
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);
            }else
            if(!strcmp(nombreparametro,"type")){
                type=1;
                if(parametro[0]=='p' || parametro[0]=='e'  || parametro[0]=='l'||parametro[0]=='P' || parametro[0]=='E'  || parametro[0]=='L'){
                    nuevafuncion.type=parametro[0];
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                type=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'type' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
            if(!strcmp(nombreparametro,"fit")){
                fit=1;
                if(!strcmp(parametro,"bf") || !strcmp(parametro,"ff") || !strcmp(parametro,"wf")||!strcmp(parametro,"BF") || !strcmp(parametro,"FF") || !strcmp(parametro,"WF")  )
                {
                    strcpy(nuevafuncion.fit,parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    fit=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'fit' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
            if(!strcmp(nombreparametro,"delete")){
                delete_=1;
                if((!strcmp(parametro,"fast")) || (!strcmp(parametro,"full"))||(!strcmp(parametro,"FULL")) || (!strcmp(parametro,"FAST")) )
                {
                    strcpy(nuevafuncion.delete_,parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    delete_=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'delete' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else if(!strcmp(nombreparametro,"add")){
                add=1;
                if(CadenaEsNumero(parametro)==0){
                    nuevafuncion.add=atoi(parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else if(parametro[0]=='-'){
                    if(CadenaEsNumero(parametro)==0){
                        nuevafuncion.add=atoi(parametro)*-1;
                        limpiarvar(parametro,100);
                        limpiarvar(nombreparametro,100);
                    }
                }
                else{
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'add' invalido. (esperando numero)\n\n");
                    ErrorInterprete++;
                    add=0;
                }
            }else if(!strcmp(nombreparametro,"id")){
                id=1;
                strcpy(nuevafuncion.id,parametro);
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);
            }
            else if(nombreparametro[0]=='i'&&nombreparametro[1]=='d'&&EsNumero(nombreparametro[2])==1){
                idlistado=1;
                strcpy(listaIds[contadorIds].id,parametro);
                contadorIds++;
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);
            }
            else{
                printf("\n\n Analizador - FATAL ERROR: Parametro Opcional Invalido. \n\n");
                ErrorInterprete++;
            }
        }else if(entrada[contador]=='+'){
            numeroparametros++;
            contador++;
            while(entrada[contador]!=':'){
                char y2[1];
                y2[0]=entrada[contador];
                strncat(nombreparametro,y2,1);
                contador++;
            }
            if(entrada[contador]==':'&&entrada[contador+1]==':'){
                 contador=contador+2;
                 int contadorpar=0;
                 while(nombreparametro[contadorpar]){
                     nombreparametro[contadorpar]=tolower(nombreparametro[contadorpar]);
                     contadorpar++;
                 }
                 printf("----------- Analizador - RECONOCIDO: PARAMETRO(sin mayusculas)= %s ------------\n",nombreparametro);
                 char prueba;
                 if(entrada[contador]!='\"'){
                     while(entrada[contador]!=' ' && entrada[contador]!='\n' && entrada[contador]!='#' && entrada[contador]!=NULL){
                     char y3[1];
                     y3[0]=entrada[contador];
                     strncat(parametro,y3,1);
                     contador++;
                     }
                     printf("---------- CONTENIDO PARAMETRO SC= %s -----------\n",parametro);
                     char fn[1];
                     fn[0]='\0';
                     strncat(parametro,fn,1);
                 }else{
                     char y4[1];
                     y4[0]=entrada[contador];
                     strncat(parametro,y4,1);
                     contador++;
                     while(entrada[contador]!='\"') {
                     prueba=entrada[contador];
                     char y5[1];
                     y5[0]=entrada[contador];
                     strncat(parametro,y5,1);
                     contador++;
                     }
                     char y6[1];
                     y6[0]=entrada[contador];
                     strncat(parametro,y6,1);
                     contador++;
                     char fn[1];
                     fn[0]='\0';
                     strncat(parametro,fn,1);
                     printf("---------- CONTENIDO PARAMETRO CC = %s -----------\n",parametro);
                 }
            }else{

            printf("Analizador - FATAL ERROR: Sintaxis Incorrecta Falta ':' \n");
            ErrorInterprete++;
            }
            if(!strcmp(nombreparametro,"size")){
                size=2;
                if(CadenaEsNumero(parametro)==0)
                {
                    nuevafuncion.size=atoi(parametro);
                    printf("size: %i\n",nuevafuncion.size);

                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);

                }else if(parametro[0]=='-')
                {
                    if(CadenaEsNumero(parametro)==0)
                    {
                        nuevafuncion.size=atoi(parametro)*-1;
                        printf("size: %i\n",nuevafuncion.size);
                        limpiarvar(parametro,100);
                        limpiarvar(nombreparametro,100);
                    }
                }else{
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'size' invalido. \n\n");
                    ErrorInterprete++;
                    size=0;
                }
            }else
            if(!strcmp(nombreparametro,"unit")){
                unit=2;
                if(parametro[0]=='m' || parametro[0]=='k' || parametro[0]=='b'||parametro[0]=='M' || parametro[0]=='K' || parametro[0]=='B'){
                    nuevafuncion.unit=parametro[0];
                    printf("unit: %c\n",nuevafuncion.unit);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    unit=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'unit' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
             if(!strcmp(nombreparametro,"path")){
                path=2;
                strcpy(nuevafuncion.path,parametro);
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);
            }else
            if(!strcmp(nombreparametro,"name")){
                name=2;
                strcpy(nuevafuncion.name,parametro);
                limpiarvar(parametro,100);
                limpiarvar(nombreparametro,100);
            }else
            if(!strcmp(nombreparametro,"type")){
                type=2;
                if(parametro[0]=='p' || parametro[0]=='e'  || parametro[0]=='l'||parametro[0]=='P' || parametro[0]=='E'  || parametro[0]=='L'){
                    nuevafuncion.type=parametro[0];
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                type=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'type' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
            if(!strcmp(nombreparametro,"fit")){
                fit=2;
                if(!strcmp(parametro,"bf") || !strcmp(parametro,"ff") || !strcmp(parametro,"wf")||!strcmp(parametro,"BF") || !strcmp(parametro,"FF") || !strcmp(parametro,"WF")  ){
                    strcpy(nuevafuncion.fit,parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    fit=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'fit' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else
            if(!strcmp(nombreparametro,"delete")){
                delete_=2;
                if((!strcmp(parametro,"fast")) || (!strcmp(parametro,"full"))||(!strcmp(parametro,"FULL")) || (!strcmp(parametro,"FAST")) ){
                    strcpy(nuevafuncion.delete_,parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else{
                    delete_=0;
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'delete' invalido. \n\n");
                    ErrorInterprete++;
                }
            }else if(!strcmp(nombreparametro,"add")){
                add=2;
                if(CadenaEsNumero(parametro)==0){
                    nuevafuncion.add=atoi(parametro);
                    limpiarvar(parametro,100);
                    limpiarvar(nombreparametro,100);
                }
                else if(parametro[0]=='-'){
                    if(CadenaEsNumero(parametro)==0){
                        nuevafuncion.add=atoi(parametro)*-1;
                        limpiarvar(parametro,100);
                        limpiarvar(nombreparametro,100);
                    }
                }
                else{
                    printf("\n\n Analizador - FATAL ERROR: Parametro 'add' invalido. (esperando numero)\n\n");
                    ErrorInterprete++;
                    add=0;
                }
            }else{
            printf("\n\n Analizador - FATAL ERROR: Parametro Obligatorio Invalido. \n\n");
            ErrorInterprete++;
            }
        }else if(entrada[contador]=='\n'){
                contador++;
                printf("-------- RECONOCIDO: SALTO DE LINEA ----------\n");
                if(!strcmp(instruccion,"mount") && name==0 && path==0){
                    activarmount=1;
                }
                if((entrada[contador]=='-'||entrada[contador]=='+')){
                }else{
                    if(ErrorInterprete==0 && fin==0){
                        if(instruccion!=NULL||instruccion[0]!='\0'){
                            if(!strcmp(instruccion,"exec")){
                                printf("\n-----------------------------------------------------------------------\n");
                                printf("/######################### EJECUTANDO COMANDO ###########################\n");
                                printf("-----------------------------------------------------------------------\n\n");

                                ContadorInstrucciones++;
                                if(path==1){
                                    char *f=LeerScript(nuevafuncion.path);
                                    printf("\n\n Cadena final es:  %s \n\n",f);
                                   Analizador(f);
                                   limpiarvar(instruccion,100);
                                }else{
                                    printf("\n\n Analizador - FATAL ERROR: Faltan parametros del comando 'exec'.\n\n");
                                    ErrorComando++;
                                }
                            }else if(!strcmp(instruccion,"mkdisk")){
                                printf("\n-----------------------------------------------------------------------\n");
                                printf("/######################### EJECUTANDO COMANDO ###########################\n");
                                printf("-----------------------------------------------------------------------\n\n");
                                if(unit==0){
                                    nuevafuncion.unit='m';
                                }else{
                                    if(nuevafuncion.unit=='b'){
                                        printf("\n\n Analizador - ERROR: Parametros 'mkdisk' invalidos.\n\n");
                                        printf("Colocando Dimencional en MB \n");
                                        nuevafuncion.unit='m';
                                    }else{
                                        //NAAAAAAAAAA....!!!
                                    }
                                }
                                if(unit==0 && numeroparametros==3   && path==1 && name==1 && size==1){
                                    printf("Creando disco...\n");
                                    CrearDisco(nuevafuncion);
                                    limpiarvar(instruccion,100);
                                }
                                else if(unit==2 && numeroparametros==4   && path==1 && name==1 && size==1){
                                    printf("Creando disco...\n");
                                    CrearDisco(nuevafuncion);
                                    limpiarvar(instruccion,100);
                                }
                                else{
                                    printf("\n\n Analizador - ERROR: Parametros 'mkdisk' invalidos.\n\n");
                                    ErrorComando++;
                                }
                                ContadorInstrucciones++;
                                limpiarvar(instruccion,100);
                            }
                            else if(!strcmp(instruccion,"rmdisk")){
                                ContadorInstrucciones++;
                                if(path==1&&numeroparametros==1){
                                    int opcion;
                                    printf("%s\n", "(!)-¿Desea eliminar este Disco?");
                                    printf("%s\n", "1. Si");
                                    printf("%s\n", "2. No");

                                    printf("%s\n", "");
                                    printf("====:~$ ");
                                    scanf("%d",&opcion);

                                    if(opcion>0 && opcion<3){
                                    switch(opcion){
                                    case 1:
                                        EliminarDisco(nuevafuncion);
                                        limpiarvar(instruccion,100);
                                    break;
                                    case 2:
                                        printf("------- OPERACION CANCELADA -------\n");
                                    break;
                                    default:
                                        printf("------- FATAL ERROR ------- \n");
                                    break;
                                    }
                                    }else{
                                     opcion=1;
                                     printf(" \n");
                                     printf("%s\n", "-------- Seleccion Invalida --------");
                                    }
                                }else{
                                    printf("\n\n Analizador - ERROR: Parametros 'rmdisk' invalidos.\n\n");
                                    ErrorComando++;
                                }
                            }
                            else if(!strcmp(instruccion,"fdisk")){
                                printf("\n-----------------------------------------------------------------------\n");
                                printf("/######################### EJECUTANDO COMANDO ##########################/\n");
                                printf("-----------------------------------------------------------------------\n\n");
                                ContadorInstrucciones++;
                                if(path==1 && name==1){
                                    if(unit==0){
                                        nuevafuncion.unit='k';
                                    }
                                    if(type==0){
                                        nuevafuncion.type='p';
                                    }
                                    if(fit==0){
                                        strcpy(nuevafuncion.fit,"wf");
                                    }
                                    if((add==1 && delete_==1)||(add==2 && delete_==2)||(add==1 && delete_==2)||(add==2 && delete_==1))
                                    {
                                        printf("\n\n Analizador - ERROR: Parametros 'fdisk' invalidos. (add & delete estan juntos)\n\n");
                                        ErrorComando++;
                                    }else if(add==2 && delete_==0){
                                        printf("Reducir o Aumentar...\n");
                                        limpiarvar(instruccion,100);
                                    }else if(add==0 && delete_==2){
                                         if(nuevafuncion.type=='p' || nuevafuncion.type=='e'||nuevafuncion.type=='P' || nuevafuncion.type=='E'){
                                            int opcion;
                                            printf("%s\n", "(!)-¿Desea eliminar esta Particion?");
                                            printf("%s\n", "1. Si");
                                            printf("%s\n", "2. No");

                                            printf("%s\n", "");
                                            printf("====:~$ ");
                                            scanf("%d",&opcion);

                                            if(opcion>0 && opcion<3){
                                                switch(opcion){
                                                case 1:
                                                EliminarParticion(nuevafuncion);
                                                limpiarvar(instruccion,100);
                                                break;
                                                case 2:
                                                    printf("------- OPERACION CANCELADA -------\n");
                                                break;
                                                default:
                                                    printf("------- FATAL ERROR -------\n");
                                                break;
                                                }
                                            }else{
                                                opcion=1;
                                                printf(" \n");
                                                printf("%s\n", "-------- Seleccion Invalida --------");
                                            }
                                        }
                                        else{
                                            limpiarvar(instruccion,100);
                                        }
                                    }else if (add==0 && delete_==0 && size==1){
                                        if(nuevafuncion.type=='p' ||  nuevafuncion.type=='e'||nuevafuncion.type=='P' ||  nuevafuncion.type=='E'){
                                            CrearParticion(nuevafuncion);
                                            limpiarvar(instruccion,100);
                                        }
                                        else{
                                             CrearParticionLogica(nuevafuncion);
                                             limpiarvar(instruccion,100);
                                        }
                                    }else{
                                        printf("\n\n Analizador - ERROR: Parametros 'fdisk' invalidos.\n\n");
                                        ErrorComando++;
                                    }
                                }else{
                                    printf("\n\n Analizador - ERROR: Faltan Parametros del 'fdisk'.\n\n");
                                    ErrorComando++;
                                }
                            }
                            else if(!strcmp(instruccion,"lspart")){
                                if(path==1){
                                    printf("\n\n Informacion ls \n\n");
                                    ls(nuevafuncion);
                                    limpiarvar(instruccion,100);
                                }else{
                                    printf("\n\n Analizador - ERROR: Faltan Parametros Obligatorios de 'ls'.\n\n");
                                    ErrorComando++;
                                }
                            }
                            else if(!strcmp(instruccion,"mount")){
                                if(path==1 && name==1){
                                    MontarParticionF1(nuevafuncion);
                                    limpiarvar(instruccion,100);
                                }else if(activarmount==1){
                                    MostrarMontadas();
                                    activarmount=0;
                                    name=5;
                                    path=5;
                                    limpiarvar(instruccion,100);
                                    ContadorComandosExitosos++;
                                }else{
                                    printf("\n\n Analizador - ERROR: Faltan Parametros Obligatorios de 'mount'.\n\n");
                                    ErrorComando++;
                                }
                            }
                            else if(!strcmp(instruccion,"umount")){
                                if(id==1){
                                    printf("Parametro: %s\n\n",nuevafuncion.id);
                                    DesmontarParticion(nuevafuncion);
                                    limpiarvar(instruccion,100);
                                }else{
                                printf("\n\n Analizador - ERROR: Faltan Parametros Obligatorios de 'unmount'.\n\n");
                                }
                            }
                            else if (!strcmp(instruccion,"rep")){
                                    if(path==1 && name==1 && id==1){
                                        if(!strcmp(nuevafuncion.name,"mbr")){
                                            printf("\nREPORTE DE MBR... %s\n\n",nuevafuncion.path);
                                            ReporteMBR_dot(nuevafuncion);
                                            limpiarvar(instruccion,100);
                                        }else if(!strcmp(nuevafuncion.name,"disk")){
                                            printf("\nREPORTE DE DISK...\n\n");
                                            ReporteDiskMBR(nuevafuncion);
                                            limpiarvar(instruccion,100);
                                        }else{
                                            printf("\n\n Analizador - ERROR: El Parametro 'name' es invalido.\n\n");
                                            ErrorComando++;
                                        }
                                    }else{
                                    printf("\n\n Analizador - ERROR: Faltan Parametros Obligatorios de 'rep'.\n\n");
                                    ErrorComando++;
                                    }
                        }else{
                        }
                    }else{
                        printf("\n Errores en Analizador : '%i' \n", ErrorInterprete);
                    }
            }
        }else{
            contador++;
        }
    }
    printf("\n Errores en Analizador : '%i' \n", ErrorInterprete);
    printf("--------------------- FIN INSTRUCCION -------------------------\n");

if(ErrorInterprete==0 && fin==0){
    if(instruccion!=NULL||instruccion[0]!='\0'){
        if(!strcmp(instruccion,"exec")){
            printf("\n-----------------------------------------------------------------------\n");
            printf("/######################### EJECUTANDO COMANDO ##########################\n");
            printf("-----------------------------------------------------------------------\n\n");
            ContadorInstrucciones++;
            if(path==1){
                char *f=LeerScript(nuevafuncion.path);
                printf("\n\n Cadena Final:  %s \n\n",f);
                Analizador(f);
                limpiarvar(instruccion,100);
            }else{
                printf("\n\nInterprete - ERROR: Faltan Parametros Obligatorios de 'exec'.\n\n");
                ErrorComando++;
            }
        }else if(!strcmp(instruccion,"mkdisk")){
            printf("\n-----------------------------------------------------------------------\n");
            printf("/######################### EJECUTANDO COMANDO ##########################\n");
            printf("-----------------------------------------------------------------------\n\n");
            if(unit==0){
                nuevafuncion.unit='m';
            }else{
                if(nuevafuncion.unit=='b'){
                    printf("\n\nInterprete - ERROR: Parametros 'mkdisk' invalidos.\n\n");
                    printf("Colocando Dimencional en MB \n");
                    nuevafuncion.unit='m';
                }else{
                }
            }
            if(unit==0 && numeroparametros==3   && path==1 && name==1 && size==1){
                printf("Creando disco...\n");
                CrearDisco(nuevafuncion);
                limpiarvar(instruccion,100);
            }
            else if(unit==2 && numeroparametros==4   && path==1 && name==1 && size==1){
                printf("Creando disco...\n");
                CrearDisco(nuevafuncion);
                limpiarvar(instruccion,100);
            }
            else{
                printf("\n\nInterprete - ERROR: Parametros 'mkdisk' invalidos.\n\n");
                ErrorComando++;
            }
            ContadorInstrucciones++;
            limpiarvar(instruccion,100);
        }
        else if(!strcmp(instruccion,"rmdisk")){
            ContadorInstrucciones++;
            if(path==1&&numeroparametros==1){
            int opcion;
            printf("%s\n", "(!)-¿Desea Eliminar este Disco?");
            printf("%s\n", "1. Si");
            printf("%s\n", "2. No");

            printf("%s\n", "");
            printf("====:~$ ");
            scanf("%d",&opcion);

            if(opcion>0 && opcion<3){
            switch(opcion){
            case 1:
                EliminarDisco(nuevafuncion);
                limpiarvar(instruccion,100);
            break;
            case 2:
                printf("------- OPERACION CANCELADA -------\n");
            break;
            default:
                printf("------- FATAL ERROR -------\n");
            break;
            }
            }else
            {
             opcion=1;
             printf(" \n");
             printf("%s\n", "-------- Seleccion Invalida --------");
            }
        }else{
            printf("\n\nInterprete - ERROR: Parametros 'rmdisk' invalidos.\n\n");
            ErrorComando++;
        }
        }
        else if(!strcmp(instruccion,"fdisk")){
            printf("\n-----------------------------------------------------------------------\n");
            printf("/######################### EJECUTANDO COMANDO ##########################\n");
            printf("-----------------------------------------------------------------------\n\n");

            ContadorInstrucciones++;
            if(path==1 && name==1){
                if(unit==0){
                    printf("Colocando unit Aleatoria...\n");
                    nuevafuncion.unit='k';
                }
                if(type==0){
                    printf("Colocando type Aleatorio...\n");
                    nuevafuncion.type='p';
                }
                if(fit==0){
                    printf("Colocando fit Aleatorio...\n");
                    strcpy(nuevafuncion.fit,"wf");
                }

                if((add==1 && delete_==1)||(add==2 && delete_==2)||(add==1 && delete_==2)||(add==2 && delete_==1)){
                    printf("\n\n Analizador - ERROR: Parametros 'fdisk' invalidos. (add & delete estan juntos)\n\n");
                    ErrorComando++;
                }else if(add==2 && delete_==0){
                    printf("Reducir o Aumentar...\n");
                    limpiarvar(instruccion,100);
                }else if(add==0 && delete_==2){
                     if(nuevafuncion.type=='p' || nuevafuncion.type=='e'||nuevafuncion.type=='P' || nuevafuncion.type=='E'){
                        int opcion;
                        printf("%s\n", "(!)-¿Desea Eliminar esta Particion?");
                        printf("%s\n", "1. Si");
                        printf("%s\n", "2. No");

                        printf("%s\n", "");
                        printf("====:~$ ");
                        scanf("%d",&opcion);

                        if(opcion>0 && opcion<3){
                            switch(opcion){
                            case 1:
                            printf("Eliminar Particion...\n");
                            EliminarParticion(nuevafuncion);
                            limpiarvar(instruccion,100);
                            break;
                            case 2:
                                printf("------- OPERACION CANCELADA -------\n");
                            break;
                            default:
                                printf("------- FATAL ERROR -------\n");
                            break;
                            }
                        }else{
                            opcion=1;
                            printf(" \n");
                            printf("%s\n", "-------- Seleccion Invalida --------");
                        }
                    }
                    else{
                        printf("Eliminar Particion Logica...\n");
                        limpiarvar(instruccion,100);
                    }
                }else if (add==0 && delete_==0 && size==1){
                    if(nuevafuncion.type=='p' ||  nuevafuncion.type=='e'||nuevafuncion.type=='P' ||  nuevafuncion.type=='E'){
                        printf("Creando Particion...\n\n");
                        CrearParticion(nuevafuncion);
                        limpiarvar(instruccion,100);
                    }
                    else{
                         printf("Creando Particion Logican...\n");
                         CrearParticionLogica(nuevafuncion);
                         limpiarvar(instruccion,100);
                    }
                }else{
                    printf("\n\n Analizador - ERROR: Parametros 'fdisk' invalidos.\n\n");
                    ErrorComando++;
                }
            }else{
                printf("\n\n Analizador - ERROR: Faltan Parametros Obligatorios de 'fdisk'.\n\n");
                ErrorComando++;
            }
        }
        else if(!strcmp(instruccion,"lspart")){
                if(path==1){
                    printf("\n\n Informacion ls \n\n");
                    ls(nuevafuncion);
                    limpiarvar(instruccion,100);
                }else{
                    printf("\n\n FATAL ERROR: Faltan Parametros Obligatorios de comando 'ls'.\n\n");
                    ErrorComando++;
                }
        }
        else if(!strcmp(instruccion,"mount")){
                if(path==1 && name==1){
                    printf("Montar Particion...\n\n");
                    MontarParticionF1(nuevafuncion);
                    limpiarvar(instruccion,100);
                }else if(activarmount==1 || (path==0 && name==0)){
                    printf("\n------------- LISTA DE PARTICIONES MONTADAS ---------------\n");
                    MostrarMontadas();
                    activarmount=0;
                    name=5;
                    path=5;
                    limpiarvar(instruccion,100);
                    ContadorComandosExitosos++;
                    printf("\n ------------- FIN LISTA DE PARTICIONES MONTADAS ---------------\n\n");
                }else{
                    printf("\n\n FATAL ERROR: Faltan Parametros Obligatorios de comando 'mount'.\n\n");
                    ErrorComando++;
                }
        }
         else if(!strcmp(instruccion,"umount")){
                if(id==1){
                    printf("Desmontar Particion...\n\n");
                    printf("Imprime El PArametro:... %s\n\n",nuevafuncion.id);
                    DesmontarParticion(nuevafuncion);
                    limpiarvar(instruccion,100);
                }else if(idlistado==1){
                    printf("Desmontar Particion LISTADO...\n\n");
                    int j=0;
                    for(j=0; j<contadorIds; j++){
                        limpiarvar(nuevafuncion.id,100);
                        strcpy(nuevafuncion.id,listaIds[j].id);
                        printf("Desmontando Particion...%s\n\n",nuevafuncion.id);
                        DesmontarParticion(nuevafuncion);
                    }
                    contadorIds=0;
                    int i=0;
                    for(i=0; i<20; i++){
                     limpiarvar(listaIds[i].id,100);
                    }
                }
                else{
                printf("\n\n FATAL ERROR: Faltan Parametros Obligatorios de comando 'unmount'.\n\n");
                ErrorComando++;
                }
        }
        else if (!strcmp(instruccion,"rep")){
                if(path==1 && name==1 && id==1){
                    if(!strcmp(nuevafuncion.name,"mbr")){
                        printf("\nREPORTE DE MBR... %s\n\n",nuevafuncion.path);
                        ReporteMBR_dot(nuevafuncion);
                        limpiarvar(instruccion,100);
                    }else if(!strcmp(nuevafuncion.name,"disk")){
                        printf("\nREPORTE DE DISK...\n\n");
                        ReporteDiskMBR(nuevafuncion);
                        limpiarvar(instruccion,100);
                    }else{
                        printf("\n\n FATAL ERROR: Parametro 'name' invalido. \n\n");
                        ErrorComando++;
                    }
                }else{
                printf("\n\n FATAL ERROR: Faltan Parametros Obligatorios de comando 'rep'.\n\n");
                ErrorComando++;
                }
        }
    }else{
    }
}else{
    printf("\n Errores en Analizador : '%i' \n", ErrorInterprete);
    printf("\n");
    printf(" Error de Sintaxis \n");
    }
}

void menu1(){
    int opcion=1;
    char comando[150];
    while(strcmp(comando,"exit")!=0){
       //#################### MENU 1.0 #############################
        fin=0;
        printf("%s\n", "-----------------------------------------------------");
        printf("        %s\n", "Universidad de San Carlos de Guatemala");
        printf("    %s\n", "Laboratorio Manejo e Implementacion de Archivos");
        printf("      %s\n", "FILE SYSTEM EXT2/EXT3 (Seccion 'A-' Fase 1)");
        printf("%s\n", "-----------------------------------------------------");
        printf("%s\n", "");
        printf("%s\n", "NOTA: Ingrese exit en cualquier momento para salir.");
        printf("%s\n", "Ingrese un Comando:");
        printf("%s\n", "");
        printf("====~$ ");

        limpiarvar(comando,100);
        scanf(" %[^\n]", comando);
        Analizador(comando);
        fin=0;
    }
}

void CrearDisco(Funcion funcion){
    MbrDisco mbr;
    mbr.mbr_tamano=funcion.size;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    printf("SYSTEM $_ HORA Creacion: %s\n",output);
    int id=funcion.path;

    int k=0;
    int l=0;
    for(k=0;k<25;k++){
    mbr.mbr_fecha_creacion[l++]=output[k];
    }
    mbr.mbr_disk_signature=id;

    int tam=0;
    if(funcion.unit=='k'||funcion.unit=='K'){
        tam=funcion.size*1024;
    }
    else{
        tam=funcion.size*(1024*1024);
    }
    if(tam<10485760){
        printf("\n FATAL ERROR: El Tamaño Minimo es de 10MB \n\n");
        ErrorCrearDisco++;
    }else{
    char auxpath1[150];
    strcpy(auxpath1,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(auxpath1[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(auxpath1[q]!='\"'){
            char c2[1];
            c2[0]=auxpath1[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    char pathauxiliar2[100];
    strcpy(pathauxiliar2,funcion.name);
    char finalizado2[100];
    strcpy(finalizado2,"cd /\n");
    if(pathauxiliar2[0]=='\"'){
        limpiarvar(funcion.name,100);
        int q=1;
        while(pathauxiliar2[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar2[q];
            strncat(funcion.name,c2,1);
            q++;
        }
    }
    //###### CHECK NAME ######
    int con1=0;
    int valExtencion=0;
    while(funcion.name[con1]!=NULL){
        if(funcion.name[con1]=='.'){
            if(funcion.name[con1+1]=='d'&&funcion.name[con1+2]=='s'&&funcion.name[con1+3]=='k'){
                valExtencion=1;
            }
        }
        con1++;
    }
    strcat(funcion.path,funcion.name);
    if(valExtencion==1){
        int indice=0;
        char carpeta[100];
        while(funcion.path[indice]!='.'){
            if(funcion.path[indice]!='/'){
                char c1[1];
                c1[0]=funcion.path[indice];
                strncat(carpeta,c1,1);
            }
            else{
                strcat(finalizado,"mkdir ");
                strcat(finalizado,"\"");
                strcat(finalizado,carpeta);
                strcat(finalizado,"\"");
                strcat(finalizado,"\n");
                strcat(finalizado,"cd ");
                strcat(finalizado,"\"");
                strcat(finalizado,carpeta);
                strcat(finalizado,"\"");
                strcat(finalizado,"\n");
                strcat(carpeta,"");
                limpiarvar(carpeta,100);
            }
            indice++;
        }
        printf("\nComo de directorio inexistente: %s\n",finalizado);
        system(finalizado);
        FILE* archivo= fopen(funcion.path, "ab");
        if (archivo==NULL){
            printf("\n FATAL ERROR: Acceso Denegado. \n\n");
            ErrorCrearDisco++;
        }
        else{
            int fin=(tam/1024);
            char buffer[1024];
            int i=0;
            for(i=0;i<1024;i++){
            buffer[i]='\0';
            }
            int j=0;
            while(j!=fin){
            fwrite(&buffer,1024 , 1, archivo);
            j++;
            }
            fclose(archivo);
        }
        mbr.mbr_tamano=tam;
        int i;
        for(i=0;i<4;i++){
        mbr.particiones[i].part_start=-1;
        mbr.particiones[i].part_status='d';
        mbr.particiones[i].part_size=0;
        }

        FILE* file= fopen(funcion.path, "rb+");
        fseek(file,0,SEEK_SET);
        if (file==NULL){
            printf("\n Interprete #_ ERROR_2.4 Al tratar de Acceder al Archivo \n\n");
            ErrorCrearDisco++;
        }
        else{
            fwrite(&mbr, sizeof(MbrDisco), 1, file);
            fclose(file);
            printf("\n");
            printf(" DISCO CREADO SATISFACTORIAMENTE: %s\n",funcion.name);
            ContadorComandosExitosos++;
        }
    }else{
        printf("\n FATAL ERROR: Extencion del Disco no es Correcta. \n\n");
        ErrorCrearDisco++;
        }
    }
}

void EliminarDisco(Funcion funcion){
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }

    FILE *fichero;
    fichero = fopen(funcion.path, "r" );
    if(fichero != NULL){
        fclose(fichero);
        if(remove(funcion.path) == 0) {
            printf("\n");
            printf( "DISCO ELIMINADO: %s \n",funcion.path );
            ContadorComandosExitosos++;
        }
        else{
            printf( "\nFATAL ERROR: Disco no pudo Eliminarse.\n" );
            ErrorEliminarDisco++;
        }
    }
    else {
    printf( "FATAL ERROR: Disco no Encontrado.\n" );
    ErrorEliminarDisco++;
    }
}

void CrearParticion(Funcion funcion){
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int nombresiguales=0;
    int numeroprimarias=0;
    int numeroextendida=0;
    int TempPrimarias=0;
    int TempExt=0;

    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
            printf("\n FATAL ERROR: Acceso no Permitido. \n\n");
            ErrorCrearParticionPrimaria++;
    }else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);

        printf("------------ INFORMACION DEL DISCO --------------------\n");
        printf("%i",mbr2.mbr_disk_signature);
        printf("\n");
        printf(mbr2.mbr_fecha_creacion);
        printf("\n");
        printf("Tamaño %i",mbr2.mbr_tamano);
        printf("---------------- PRIMARIAS INICIALES ----------------------------\n");

        int z=0;
        for(z=0;z<4;z++){
            int k=0;
            int l=0;
            while(funcion.name[k]!=NULL){
            if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                l++;
            }
                k++;
            }
            if(l==k && mbr2.particiones[z].part_status!='0'){
                nombresiguales=1;
                printf("\n FATAL ERROR: Particiones Iguales  %i \n\n",nombresiguales);
                ErrorCrearParticionPrimaria++;
            }
            printf("Bit Inicial: %i \n",mbr2.particiones[z].part_start);
            printf("Nombre: %s \n",mbr2.particiones[z].part_name);
            printf("Tipo Estado: %c \n",mbr2.particiones[z].part_status);
            printf("Tipo Particion: %c \n",mbr2.particiones[z].part_type);
            printf("---------------------------------------------------------\n");

            if(mbr2.particiones[z].part_type=='p'||mbr2.particiones[z].part_type=='P'){
                numeroprimarias++;
            }
            if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                printf("------------ EXTENDIDAS INICIALES ---------------------\n");
                numeroextendida++;
                EBR mostrar;
                fseek(file2,mbr2.particiones[z].part_start,SEEK_SET);
                fread(&mostrar, sizeof(EBR), 1, file2);
                printf("Inicio EBR: %i \n",mostrar.part_start);
                printf("Siguiente ebr: %i \n",mostrar.part_next);
                printf("Estado ebr: %c \n",mostrar.part_status);
            }
                printf("Tipo De Ajuste: %c \n",mbr2.particiones[z].part_fit);
                printf("Tamaño Particion %i \n", mbr2.particiones[z].part_size);
        }
        printf("---------------------------------------------------------\n");
        printf("# Particiones Primarias: %i \n",numeroprimarias);
        printf("# PArticiones Extendidas: %i \n",numeroextendida);
        printf("---------------------------------------------------------\n");

        //############## CREACION DE PARTICION ###############
        TempPrimarias=numeroprimarias;
        TempExt=numeroextendida;
        int tamanoparticion=0;

         if(funcion.unit=='b'||funcion.unit=='B'){
            tamanoparticion=funcion.size;
        }
        else if(funcion.unit=='k'||funcion.unit=='K'){
            tamanoparticion=(funcion.size*1024);
        }
        else{
            tamanoparticion=funcion.size*(1024*1024);
        }
        printf("Tamaño Particion a Crear: %i \n",tamanoparticion);

        if(tamanoparticion<2097152){
            printf("\n FATAL ERROR: Tamaño Minimo de Particion es 2M. \n\n");
            ErrorCrearParticionPrimaria++;
        }else{
        int vacio=1;
        int i=0;
        int numeroparticion=0;
        int inicio=sizeof(MbrDisco);
        int fin=inicio+tamanoparticion;
        int ebractivo=0;
        EBR primerebr;

        if(funcion.type=='p'||funcion.type=='P'){
            numeroprimarias++;
        }
        else if(funcion.type=='e'||funcion.type=='E'){
            numeroextendida++;
            ebractivo=1;
        }
        if(nombresiguales>0){
            printf("\n FATAL ERROR: Nombre de Disco ya Existe. \n\n");
            ErrorCrearParticionPrimaria++;
            ebractivo=0;
        }
        else{
            if(numeroextendida<=1 && numeroprimarias<=3 && (numeroextendida+numeroprimarias)<=4){
                if(tamanoparticion>mbr2.mbr_tamano){
                    ebractivo=0;
                    printf("\n FATAL ERROR: Tamaño de Particion Demasiado Grande.\n\n");
                    ErrorCrearParticionPrimaria++;
                }else{
                    for(i=0;i<4;i++){
                        if(mbr2.particiones[i].part_type=='e'||mbr2.particiones[i].part_type=='E'){
                            ebractivo=0;
                        }
                        if(mbr2.particiones[i].part_start!=-1 && mbr2.particiones[i].part_status!='0'){
                            vacio=0;
                            if(fin<=mbr2.particiones[i].part_start){
                                break;
                            }
                            else{
                                inicio=mbr2.particiones[i].part_start+mbr2.particiones[i].part_size;
                                fin=inicio+tamanoparticion;
                                numeroparticion=i+1;
                            }
                        }
                    }
                    if(vacio==1 && fin<=mbr2.mbr_tamano && numeroparticion<4){
                        printf("1. Disco Vacio.\n");
                        mbr2.particiones[numeroparticion].part_start=sizeof(MbrDisco);
                        int k=0;
                        int l=0;
                        if(ebractivo==1){
                            primerebr.part_status='0';
                            primerebr.part_next=-1;
                            primerebr.part_start=-1;
                        }
                        while(funcion.name[k]!=NULL){
                            mbr2.particiones[numeroparticion].part_name[l++]=funcion.name[k];
                            k++;
                        }
                        mbr2.particiones[numeroparticion].part_size=tamanoparticion;
                        mbr2.particiones[numeroparticion].part_fit=funcion.fit[0];
                        mbr2.particiones[numeroparticion].part_status='1';
                        mbr2.particiones[numeroparticion].part_type=funcion.type;
                        mbr2.particiones[numeroparticion].formateada='0';

                        pprintf("\n");
                        printf("PARTICION CREADA SATISFACTORIAMENTE: %s\n",funcion.name);
                        ContadorComandosExitosos++;
                    }
                    else if(vacio==0 && fin<=mbr2.mbr_tamano && numeroparticion<4){
                        printf("2) Disco tiene por lo menos una particion...\n");
                        if(mbr2.particiones[numeroparticion].part_start!=1 && mbr2.particiones[numeroparticion].part_status!='0'){
                            int s=0;
                            for(s=3;s>numeroparticion;s--){
                                printf("actual: %s \n",mbr2.particiones[s].part_name);
                                printf("siguiente: %s \n",mbr2.particiones[s].part_name);
                                mbr2.particiones[s]=mbr2.particiones[(s-1)];
                            }
                        }
                        mbr2.particiones[numeroparticion].part_start=inicio;
                        int k=0;
                        int l=0;
                        if(ebractivo==1){
                            primerebr.part_status='0';
                            primerebr.part_next=-1;
                            primerebr.part_start=-1;
                            primerebr.part_size=0;
                        }
                        for(k=0;k<16;k++){
                            mbr2.particiones[numeroparticion].part_name[l++]=funcion.name[k];
                        }
                        mbr2.particiones[numeroparticion].part_size=tamanoparticion;
                        mbr2.particiones[numeroparticion].part_fit=funcion.fit[0];
                        mbr2.particiones[numeroparticion].part_status='1';
                        mbr2.particiones[numeroparticion].part_type=funcion.type;
                        mbr2.particiones[numeroparticion].formateada='0';

                        printf("\n");
                        printf("PARTICION CREADA SATISFACTORIAMENTE: %s\n",funcion.name);
                        ContadorComandosExitosos++;
                    }
                    else{
                        printf("\n FATAL ERROR: Espacion Insuficiente para Particion. \n\n");
                        ErrorCrearParticionPrimaria++;
                        ebractivo=0;
                    }
                }
            }
            else{
                ebractivo=0;
                printf("\n FATAL ERROR: Imposible Crear Particion por el Numero de Particiones Extendidas y Primarias \n\n");
                ErrorCrearParticionPrimaria++;
            }
        }
        if(ebractivo==1){
            fseek(file2,mbr2.particiones[numeroparticion].part_start,SEEK_SET);
            fwrite(&primerebr, sizeof(EBR), 1, file2);

            printf("\n EBR Guardado \n\n");
        }
        fseek(file2,0,SEEK_SET);
        fwrite(&mbr2, sizeof(MbrDisco), 1, file2);
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        z=0;
        for(z=0;z<4;z++){
            printf("bit inicial: %i ",mbr2.particiones[z].part_start);
            printf("nombre: %s ",mbr2.particiones[z].part_name);
            printf("tipo estado: %c ",mbr2.particiones[z].part_status);
            printf("tipo particion: %c\n",mbr2.particiones[z].part_type);
            if(mbr2.particiones[z].part_type=='e'){
                EBR mostrar;
                fseek(file2,mbr2.particiones[z].part_start,SEEK_SET);
                fread(&mostrar, sizeof(EBR), 1, file2);
                printf("inicio ebr: %i \n",mostrar.part_start);
                printf("siguiente ebr: %i \n",mostrar.part_next);
                printf("estado ebr: %c \n",mostrar.part_status);
                printf("tamaño ebr: %i \n",mostrar.part_size);
            }
        }
        printf("\n\n---------- PARTICIONES ANTES DE LA OPERACION ------------\n");
        printf("# Particiones Primarias= %i                          \n ",TempPrimarias);
        printf("# Particiones Extendidas= %i                         \n",TempExt);

        printf("\n ------------ PARTICIONES FINALES ---------------          \n");
        printf("# Particiones Primarias= %i                          \n",numeroprimarias);
        printf("# Particiones Extendidas= %i                         \n",numeroextendida);
        printf("-----------------------------------------------------\n");
        fclose(file2);
        }
    }
}

void EliminarParticion(Funcion funcion)
{
    ErrorEliminarParticionPrimaria=0;
    int numeroprimarias=0;
    int numeroextendida=0;
    int nombresiguales=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int TempPrimarias=0;
    int TempExt=0;
    FILE* file2= fopen(funcion.path, "rb+");

    if (file2==NULL){
            printf("\n Interprete #_ ERROR_4.2 Al tratar de Acceder al Archivo \n\n");
            ErrorEliminarParticionPrimaria++;
    }else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int partcionborrar=-1;

        int z=0;
        int k=0;
        int l=0;
        for(z=0;z<4;z++){
            if(!strcmp(mbr2.particiones[z].part_name,funcion.name)){
                partcionborrar=z;
            }
        }
        if(partcionborrar>=0){
            if(!strcmp(funcion.delete_,"fast")||!strcmp(funcion.delete_,"FAST")){
                    mbr2.particiones[partcionborrar].part_status='0';
                    mbr2.particiones[partcionborrar].part_type='i'; //inactiva
                    printf("****************particion borrada en modo fast: %s\n",funcion.name);
                    ContadorComandosExitosos++;
            }
            else if(!strcmp(funcion.delete_,"full")||!strcmp(funcion.delete_,"FULL")){
                    int inicio=mbr2.particiones[partcionborrar].part_start;
                    mbr2.particiones[partcionborrar].part_size=0;
                    mbr2.particiones[partcionborrar].part_start=-1;
                    int contador=0;
                    for(contador=0;contador<16;contador++){
                        mbr2.particiones[partcionborrar].part_name[contador]='\0';
                    }
                    mbr2.particiones[partcionborrar].part_status='0';
                    int ebrborrar=0;
                    if(mbr2.particiones[partcionborrar].part_type=='e'||mbr2.particiones[partcionborrar].part_type=='E'){
                        ebrborrar=1;
                    }
                    mbr2.particiones[partcionborrar].part_type='\0';
                    mbr2.particiones[partcionborrar].part_fit='\0';
                    printf("PARTICION BORRADA: %s\n",funcion.name);
                    ContadorComandosExitosos++;
            }
        }
        else{
            printf("\n PARTICION: '%s'  No existe en Primarias o Extendidas. \n",funcion.name);
            int comparador=ExisteLogica(funcion.name,funcion);
            if(comparador==0){
            printf("\n\n Particion no Existe.\n ");
            printf("\n Analizador - ERROR: Al Acceder al Archivo (no existe en las particiones Primarias , Extendidas y Logicas) \n Nombre= %s \n",funcion.name);
            ErrorT++;
            }else{
            printf("Particion Existe es Logica.");
            EliminarLogica(funcion);
            }
        }
        fseek(file2,0,SEEK_SET);
        fwrite(&mbr2, sizeof(MbrDisco), 1, file2);
        fclose(file2);
    }
    file2= fopen(funcion.path, "rb+");
    MbrDisco mbr2;
    fseek(file2,0,SEEK_SET);
    fread(&mbr2, sizeof(MbrDisco), 1, file2);
    fclose(file2);
    int z=0;
    for(z=0;z<4;z++){
        int k=0;
        int l=0;
        while(funcion.name[k]!=NULL){
            if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                l++;
            }
            k++;
        }
        if(l==k && mbr2.particiones[z].part_status!='0'){
            printf("ESTATUS: 0");
        }
        if(mbr2.particiones[z].part_type=='p'||mbr2.particiones[z].part_type=='P'){
            numeroprimarias++;
        }
        if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E')
            numeroextendida++;
        }
    }
    printf("\n\n ------ Eliminado Finalizado ------\n\n");
    printf("# Particiones Primarias= %i\n",numeroprimarias);
    printf("# Particiones Extendidas= %i\n\n",numeroextendida);
    printf("ERRORES: %i\n",ErrorEliminarParticionPrimaria);
}

void CrearParticionLogica(Funcion funcion){
    Funcion funcionaux=funcion;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int existe=ExisteLogica(funcionaux.name,funcion);
    printf("EXISTE:  %i \n",existe);
    if(existe==0){
        FILE* file2= fopen(funcion.path, "rb+");
        if (file2==NULL){
            printf("\n Analizador - ERROR: ACCESO DENEGADO. \n\n\n");
            ErrorCrearParticionLogica++;
        }
        else{
            MbrDisco mbr2;
            fseek(file2,0,SEEK_SET);
            fread(&mbr2, sizeof(MbrDisco), 1, file2);
            int z;
            for(z=0;z<4;z++){
                if(!strcmp(mbr2.particiones[z].part_name,funcion.name)&& mbr2.particiones[z].part_status!='0'){
                    nombresiguales=1;
                    printf("\n Analizador - ERROR: Particiones Iguales. \n\n %i \n",nombresiguales);
                    ErrorCrearParticionLogica++;
                }
                if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                    numeroextendida++;
                    idextendida=z;
                    tamanooextendida=mbr2.particiones[z].part_size;
                    nombreextendida=mbr2.particiones[z].part_name;
                }
            }
             if(nombresiguales>0){
                printf("\n Analizador - ERROR: Particion ya Existente. \n\n %s \n",funcion.name);
                ErrorCrearParticionLogica++;
            }
            else{
                printf("Particion Logica a Crear= %s \n",funcion.name);
                printf("Particion Extendida donde se Crea= %s \n",nombreextendida);
                printf("Tamaño de Particion Extendida= %i \n",tamanooextendida);
                printf("ID Particion = %i \n",idextendida);
                printf("Tamaño EBR: %i \n",sizeof(EBR));
                int tamanoparticion=0;
                if(funcion.unit=='b'||funcion.unit=='B'){
                    tamanoparticion=funcion.size;
                }
                else if(funcion.unit=='k'||funcion.unit=='K'){
                    tamanoparticion=(funcion.size*1024);
                }
                else{
                    tamanoparticion=funcion.size*(1024*1024);
                }
                printf("tamaño particion: %i \n",tamanoparticion);
                int vacio=1;
                EBR ebr;
                int actual=mbr2.particiones[idextendida].part_start;
                printf("posicion actual %i\n",actual);
                fseek(file2,actual,SEEK_SET);
                fread(&ebr, sizeof(EBR), 1, file2);
                int next=ebr.part_next;
                inicio=sizeof(EBR);
                int fin=inicio+tamanoparticion;
                int numeroebr=0;
                int espaciolibre=mbr2.particiones[idextendida].part_size;
                espaciolibre-=32;
                do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    printf("posicion actual %i\n",actual);
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
                }while(next!=-1);
                EBR indices[contador+1];
                contador=0;
                actual=mbr2.particiones[idextendida].part_start;
                fseek(file2,actual,SEEK_SET);
                fread(&indices[contador], sizeof(EBR), 1, file2);
                do{
                printf("------------------------ OLD ------------------------------------\n");
                printf("Fit %c\n",indices[contador].part_fit);
                printf("Name %s\n",indices[contador].part_name);
                printf("Next %i\n",indices[contador].part_next);
                printf("Size %i\n",indices[contador].part_size);
                printf("Inicio %i\n",indices[contador].part_start);
                printf("Estado %c\n",indices[contador].part_status);
                printf("-----------------------------------------------------------------\n");
                if(indices[contador].part_next!=-1){
                    printf("contador %i\n",contador);
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    printf("posicion actual %i\n",actual);
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                    printf("siguiente %i\n",next);
                }else{
                    printf("contador %i\n",contador);
                    next=-1;
                }
                    contador++;
                }while(next!=-1);
                int i=0;
                for(i=0;i<contador;i++){
                    if(indices[i].part_start!=-1 ){
                        vacio=0;
                        printf("mostrar part_name: %s\n",indices[i].part_name);
                        if(fin<=(indices[i].part_start-sizeof(EBR))){
                            break;
                        }
                        else{
                            inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                            fin=inicio+tamanoparticion;
                            numeroebr=i+1;
                        }
                        if(i==0){
                            espaciolibre=espaciolibre-indices[i].part_size;
                        }else{
                            espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                        }
                    }
                }
                printf("Total: %i\n",contador);
                printf("Posicion %i\n",numeroebr);
                printf("Espacio libre %i\n",espaciolibre);
                printf("Inicio %i\n",inicio);
                printf("Final %i\n",fin);
                if(fin<tamanooextendida){
                    if(indices[numeroebr].part_start==-1){
                        indices[numeroebr].part_start=inicio;
                        int k=0;
                        int l=0;
                        for(k=0;k<16;k++){
                            indices[numeroebr].part_name[l++]=funcion.name[k];
                        }
                        indices[numeroebr].part_size=tamanoparticion;
                        indices[numeroebr].part_fit=funcion.fit[0];
                        indices[numeroebr].part_status='1';
                        indices[numeroebr].part_next=-1;
                        actual=mbr2.particiones[idextendida].part_start;
                        fseek(file2,actual,SEEK_SET);
                        fwrite(&indices[numeroebr], sizeof(EBR), 1, file2);
                        printf("GUARDADO\n");
                        ContadorComandosExitosos++;
                        contador++;
                    }else{
                    int j=0;
                    for(j=numeroebr;j<(contador+1);j++){
                        if((j+1)<(contador+1)){
                            indices[j+1]=indices[j];
                        }
                    }
                    indices[numeroebr].part_start=inicio;
                    int k=0;
                    int l=0;
                    for(k=0;k<16;k++){
                        indices[numeroebr].part_name[l++]=funcion.name[k];
                    }
                    indices[numeroebr].part_size=tamanoparticion;
                    indices[numeroebr].part_fit=funcion.fit[0];
                    indices[numeroebr].part_status='1';
                    indices[numeroebr].part_next=-1;
                    if((numeroebr-1)>=0){
                        indices[numeroebr-1].part_next=indices[numeroebr].part_start-sizeof(EBR);
                    }
                    if((numeroebr+1)<(contador+1)){
                        indices[numeroebr].part_next=indices[numeroebr+1].part_start-sizeof(EBR);
                    }
                    actual=mbr2.particiones[idextendida].part_start;
                    int escribir=0;
                    for(escribir=0;escribir<(contador+1);escribir++){
                        printf("----------------------- NEW -------------------------------------\n");
                        printf("Fit %c\n",indices[escribir].part_fit);
                        printf("Name %s\n",indices[escribir].part_name);
                        printf("Next %i\n",indices[escribir].part_next);
                        printf("Size %i\n",indices[escribir].part_size);
                        printf("Inicio %i\n",indices[escribir].part_start);
                        printf("Estado %c\n",indices[escribir].part_status);
                        printf("-----------------------------------------------------------------\n");
                    printf("ACTUAL: %i\n",actual);
                    fseek(file2,actual,SEEK_SET);
                    fwrite(&indices[escribir], sizeof(EBR), 1, file2);
                    actual+=sizeof(EBR);
                    actual+=indices[escribir].part_size;
                    }
                    printf("GUARDADO\n");
                    ContadorComandosExitosos++;
                    contador++;
                }
            }else{
                printf("\n Analizador - ERROR: No hay Espcio Suficiente. \n\n");
                ErrorCrearParticionLogica++;
            }
            }
            fclose(file2);
        }
        printf("\n\n ------- Particionado Logico Finalizado --------\n\n");
        printf("# Particiones extendidas= %i\n\n",numeroextendida);
        printf("Nombre de la Particion Extendida= %s\n\n",nombreextendida);
        printf("# Particiones Logicas en La Extendida= %i\n\n",contador);
        printf("ERRORES ENCONTRADOS: %i\n",ErrorCrearParticionLogica);
    }else{
       printf("\n Analizador - ERROR: Nombre de Particion ya Existe. \n\n\n");
       ErrorCrearParticionLogica++;

    }
}

int ExisteLogica(char nombre[], Funcion funcion){
    int aux=0;
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }

    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO. \n\n\n");
        ErrorT++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;
        int tamanoparticion=0;
        for(z=0;z<4;z++){
            int k=0,l=0;
            while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                l++;
                }
                k++;
            }
            if(mbr2.particiones[z].part_type=='e'|| mbr2.particiones[z].part_type=='E'){
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        if(nombresiguales>0){
        printf("\n Analizador - ERROR: Particion ya Existente. \n\n %s \n",funcion.name);
        ErrorT++;
        }
        else{
            int vacio=1;
            EBR ebr;
            int actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&ebr, sizeof(EBR), 1, file2);
            int next=ebr.part_next;
            inicio=sizeof(EBR);
            int fin=inicio+tamanoparticion;
            int contador=0;
            int numeroebr=0;
            int espaciolibre=mbr2.particiones[idextendida].part_size;
            espaciolibre-=32;
            do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
            }while(next!=-1);

            printf("-----------------Lista de Particiones------------------------\n");
            EBR indices[contador+1];
            contador=0;
            actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&indices[contador], sizeof(EBR), 1, file2);
            do{
                if(indices[contador].part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                }else{
                    next=-1;
                }
                contador++;
            }while(next!=-1);
                char a[100];
                int i=0;
                for(i=0;i<contador;i++){
                    if(indices[i].part_start!=-1 ){
                        vacio=0;
                        if(strcasecmp(nombre,indices[i].part_name)==0 && indices[i].part_status!='0'){
                            aux=1;
                        }
                        if(fin<=(indices[i].part_start-sizeof(EBR))){
                            break;
                        }
                        else
                        {
                            inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                            fin=inicio+tamanoparticion;
                            numeroebr=i+1;
                        }
                        if(i==0){
                            espaciolibre=espaciolibre-indices[i].part_size;
                        }else{
                            espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                        }
                    }
                }
        }
        fclose(file2);
    }
    return aux;
}

void EliminarLogica(Funcion funcion){
    int aux=0;
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1;
    int fin=-1;
    char*nombreextendida;
    int contador=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"')
    {
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"')
        {
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }

    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO \n\n\n");
        ErrorEliminarLogica++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;

        for(z=0;z<4;z++){
            if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        int tamanoparticionBorrada=0;
        EBR ebr;
        int actual=mbr2.particiones[idextendida].part_start;
        printf("0_posicion actual %i\n",actual);
        fseek(file2,actual,SEEK_SET);
        fread(&ebr, sizeof(EBR), 1, file2);
        int next=ebr.part_next;
        do{
            if(ebr.part_next!=-1){
                actual+=sizeof(EBR);
                actual+=ebr.part_size;
                printf("1_posicion actual %i\n",actual);
                fseek(file2,actual,SEEK_SET);
                fread(&ebr, sizeof(EBR), 1, file2);
                next=ebr.part_next;
                contador++;
            }
            contador++;
        }while(next!=-1);
        int contador2=contador;
        EBR indices[contador];
        contador=0;
        actual=mbr2.particiones[idextendida].part_start;
        fseek(file2,actual,SEEK_SET);
        fread(&indices[contador], sizeof(EBR), 1, file2);

        do{
            printf("------------------------------------------------------------\n");
            printf("Fit %c\n",indices[contador].part_fit);
            printf("Name %s\n",indices[contador].part_name);
            printf("Next %i\n",indices[contador].part_next);
            printf("Size %i\n",indices[contador].part_size);
            printf("Inicio %i\n",indices[contador].part_start);
            printf("Estado %c\n",indices[contador].part_status);
            printf("------------------------------------------------------------\n");
            if(indices[contador].part_next!=-1){
                printf("contador %i\n",contador);
                actual+=sizeof(EBR);
                actual+=indices[contador].part_size;
                printf("posicion actual %i\n",actual);
                fseek(file2,actual,SEEK_SET);
                fread(&indices[contador+1], sizeof(EBR), 1, file2);
                next=indices[contador].part_next;
                printf("siguiente %i\n",next);
            }else{
                printf("contador %i\n",contador);
                next=-1;
            }
            contador++;
        }while(next!=-1);
        char*a;
        int i=0;
        for(i=0;i<contador;i++){
            if(indices[i].part_start!=-1 ){
                printf("Mostrar part_name: %s\n",indices[i].part_name);
                a=indices[i].part_name;
                printf("Mostrar a borrar: %s\n",funcion.name);
                if(strcasecmp(funcion.name,a)==0 && indices[i].part_status!='0'){
                    if(!strcmp(funcion.delete_,"fast"))/{
                        indices[i].part_status='0';
                    }else if(!strcmp(funcion.delete_,"full")){
                        int inicio2=indices[i].part_start;
                        int tam=indices[i].part_size;
                        int fin2=tam+inicio2;
                        indices[i].part_status='0';
                        indices[i].part_size='0';
                        indices[i].part_start=-1;
                        indices[i].part_next=-1;
                        int contador=0;
                        for(contador=0;contador<16;contador++){
                            indices[i].part_name[contador]='\0';
                        }
                        indices[i].part_fit='\0';
                        char relleno='\0';
                        int cont=0;
                        for (cont = 0; cont < fin2; cont++) {
                            fwrite(&relleno, 1, 1, file2);
                        }
                        rewind(file2);
                        fclose(file2);
                    }
                }

            }
        }
            actual=mbr2.particiones[idextendida].part_start;
            int escribir=0;
            for(escribir=0;escribir<(contador2);escribir++){
                        printf("--------------------- ESCRIBE ------------------------------\n");
                        printf("Fit %c\n",indices[escribir].part_fit);
                        printf("Name %s\n",indices[escribir].part_name);
                        printf("Next %i\n",indices[escribir].part_next);
                        printf("Size %i\n",indices[escribir].part_size);
                        printf("Inicio %i\n",indices[escribir].part_start);
                        printf("Estado %c\n",indices[escribir].part_status);
                        printf("------------------------------------------------------------\n");
                        printf("ACTUAL: %i\n",actual);
            fseek(file2,actual,SEEK_SET);
            fwrite(&indices[escribir], sizeof(EBR), 1, file2);
            actual+=sizeof(EBR);
            actual+=indices[escribir].part_size;
            }
    }
    fclose(file2);
}

void lsl(Funcion funcion){
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO \n\n\n");
        ErrorT++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;

        for(z=0;z<4;z++){
            int k=0,l=0;
            while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }k++;
            }
            if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        if(nombresiguales>0){
        printf("\n Analizador - ERROR: Particion ya Existente. \n\n %s \n",funcion.name);
        ErrorT++;
        }
        else{
            int tamanoparticion=0;
            if(funcion.unit=='b'){
                tamanoparticion=funcion.size;
            }
            else if(funcion.unit=='k'){
                tamanoparticion=(funcion.size*1024);
            }
            else{
                tamanoparticion=funcion.size*(1024*1024);
            }
            int vacio=1;
            EBR ebr;
            int actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&ebr, sizeof(EBR), 1, file2);
            int next=ebr.part_next;
            inicio=sizeof(EBR);
            int fin=inicio+tamanoparticion;
            int numeroebr=0;
            int espaciolibre=mbr2.particiones[idextendida].part_size;
            espaciolibre-=32;
            do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
            }while(next!=-1);
            printf("-----------------Lista de Particiones------------------------\n");
            EBR indices[contador+1];
            contador=0;
            actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&indices[contador], sizeof(EBR), 1, file2);
            do{
                if(indices[contador].part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                }else{
                    next=-1;
                }
                contador++;
            }while(next!=-1);
            int i=0;
            for(i=0;i<contador;i++){
                if(indices[i].part_start!=-1 ){
                vacio=0;
                printf("Logica, Nombre: %s",indices[i].part_name);
                printf(",Tamaño: %i",indices[i].part_size);
                printf("\n");
                printf(",Status: %c",indices[i].part_status);
                printf("\n");
                printf(",Start: %i",indices[i].part_start);
                printf("\n");
                printf(",Next: %i",indices[i].part_next);
                printf("\n");

                if(fin<=(indices[i].part_start-sizeof(EBR))){
                    break;
                }
                else{
                    inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                    fin=inicio+tamanoparticion;
                    numeroebr=i+1;
                }
                if(i==0){
                espaciolibre=espaciolibre-indices[i].part_size;
                }else{
                espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                }
            }
        }
            printf("--------------------- INFORMACION ------------------------ \n");
            printf("Total: %i\n",NumeroDeLogicas(funcion));
            printf("Posicion: %i\n",numeroebr);
            printf("Espacio Libre: %i\n",espaciolibre);
            printf("Inicio: %i\n",inicio);
            printf("Final: %i\n",fin);
            printf("------------------------------------------------------------\n");
            printf("# Particiones Logicas:   %i \n",NumeroDeLogicas(funcion));
            printf("------------------------------------------------------------\n");
            printf("# Particiones Logicas por Funcion:   %i \n",NumeroDeLogicas(funcion));
        }
        fclose(file2);
    }
        printf("**ERRORES LSL: %i\n",ErrorT);
}

void ls(Funcion funcion){
    char* nombreextendida;
    printf("\n\n----------------- INFORMACION DE DISCO -------------------\n\n");
    int ErrorT=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int nombresiguales=0;
    int numeroprimarias=0;
    int numeroextendida=0;
    int TempPrimarias=0;
    int TempExt=0;
    printf("Disco: %s",pathauxiliar);
    printf("\n");

        FILE* file2= fopen(funcion.path, "rb+");
        if (file2==NULL){
            printf("\n Analizador - ERROR: ACCESO DENEGADO. \n\n");
            ErrorT++;
        }else{
            MbrDisco mbr2;
            fseek(file2,0,SEEK_SET);
            fread(&mbr2, sizeof(MbrDisco), 1, file2);
            printf("Asignacion: %i",mbr2.mbr_disk_signature);
            printf("\n");
            printf("Fecha de Consulta: %s",mbr2.mbr_fecha_creacion);
            printf("\n");
            printf("Tamaño de Unidad: %i",mbr2.mbr_tamano);
            printf("\n\n");
            printf("---------------- PARTICIONES EN DISCO ------------------------\n");
            int z=0;
            for(z=0;z<4;z++){
                int k=0;
                int l=0;
                while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }
                    k++;
                }
                if(mbr2.particiones[z].part_type=='p'||mbr2.particiones[z].part_type=='P')
                printf("*Primaria, Nombre: %s",mbr2.particiones[z].part_name);
                printf(",Tamaño: %i",mbr2.particiones[z].part_size);
                printf("\n");
                numeroprimarias++;
                }
                if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E')
                printf("*Extendida, Nombre: %s",mbr2.particiones[z].part_name);
                printf(",Tamaño: %i",mbr2.particiones[z].part_size);
                printf("\n");

                nombreextendida=mbr2.particiones[z].part_name;
                numeroextendida++;
            }
            printf("-------------------------------------------------------------- \n");
            printf("# Particiones Primarias:   %i \n",numeroprimarias);
            printf("# de Particiones Extendidas:  %i \n",numeroextendida);
            printf("-------------------------------------------------------------- \n");

            if(numeroextendida!=0){
                printf("Reporte De Particion Extendida: '%s'  \n",nombreextendida);
                lsl(funcion);
            }else{
                printf("No Existe Particiones Extendidas en la Unidad \n");
            }
        }
}

int NumeroDeLogicas(Funcion funcion){
    int aux=0;
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO \n\n\n");
        ErrorT++;
    }
    else
    {
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;
        int tamanoparticion=0;

        for(z=0;z<4;z++){
            int k=0,l=0;
            while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                l++;
                }
                k++;
            }
            if(mbr2.particiones[z].part_type=='e'|| mbr2.particiones[z].part_type=='E')
            {
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        if(nombresiguales>0){
        printf("\n Analizador - ERROR: Particion ya Existente. \n\n %s \n",funcion.name);
        ErrorT++;
        }
        else{
            int vacio=1;
            EBR ebr;
            int actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&ebr, sizeof(EBR), 1, file2);
            int next=ebr.part_next;
            inicio=sizeof(EBR);
            int fin=inicio+tamanoparticion;
            int numeroebr=0;
            int espaciolibre=mbr2.particiones[idextendida].part_size;
            espaciolibre-=32;
            do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
            }while(next!=-1);

            printf("----------------- LISTA DE PARTICIONES ------------------------\n");
            EBR indices[contador+1];
            contador=0;
            actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&indices[contador], sizeof(EBR), 1, file2);
            do{
                if(indices[contador].part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                }else{
                    next=-1;
                }
                contador++;
            }while(next!=-1);
            char a[100];
            int i=0;
            for(i=0;i<contador;i++){
                if(indices[i].part_start!=-1 ){
                vacio=0;
                    aux++;
                if(fin<=(indices[i].part_start-sizeof(EBR))){
                    break;
                }
                else
                {
                    inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                    fin=inicio+tamanoparticion;
                    numeroebr=i+1;
                }
                if(i==0){
                espaciolibre=espaciolibre-indices[i].part_size;
                }else{
                espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                }
            }
        }
    }
        fclose(file2);
    }
    return aux;
}

int PosicionDeMontada;
int EstaMontada(Funcion funcion){
    int estamontada=0;
    int posDeMontada=0;
    int ErrorT=0;
    int i;
     for(i=0; i<50; i++){
         if(!strcmp(montadas2[i].id,funcion.id)){
             printf("Particion Montada.\n");
             posDeMontada_rep=i;
             PosicionDeMontada=i;
             estamontada=1;
             break;
         }else{
         }
     }
     return estamontada;
}

void MostrarMontadas(){
    printf("-----------------------------------------------------------------------------\n");
    printf("------------------------- PARTICIONES MONTADAS ------------------------------\n");
    printf("-----------------------------------------------------------------------------\n");
    int i;
    for(i=0; i<50; i++){
        if(montadas2[i].status==1){
        printf("\n---------------------- POSICION EN ARREGLO= %i -------------------------\n" , i);
        printf("Particion Montada: '%s' \n",montadas2[i].name2);
        printf("ID : %s \n",montadas2[i].id);
        printf("\n");
        printf("\n");
        printf("Path: %s \n",montadas2[i].path3);
        printf("Letra: %c \n",montadas2[i].letrafinal);
        printf("Numero: %d \n",montadas2[i].numerofinal);
        printf("Particion Montada: '%s' \n",montadas2[i].name);
        printf("Path: %s \n",montadas2[i].path2);
        printf("\n------------------------------------------------------------------------\n");
        }else{
        }
    }
}

void MostrarMontadasJ(){
    int i=0;
    for(i=0; i<20; i++){
    printf("Posicion PATH: %s\n",montadas[i].path);
        int y=0;
        for(y = 0; y< 26;y++){
            if(montadas[i].posicion[y].estado == 1){
            printf("Posicion Estado: %i\n",montadas[i].posicion[y].estado);
            printf("Posicion ID: %s\n",montadas[i].posicion[y].id);
            }
        }
    }
}

void MontarParticionJ(Funcion funcion){
    llenarvacios();
    char valornumero[6];
    int pathEncontrada=0;
    int x;
    for(x = 0; x < 50; x++){
        if(strcasecmp(montadas[x].path,funcion.path)==0){
                pathEncontrada = 1;
                int y;
                for(y = 0; y< 26;y++){
                    if(montadas[x].posicion[y].estado == 0){
                    montadas[x].posicion[y].estado = 1;
                    sprintf(valornumero,"%d",(y+1));
                    }
                }
                    }else if(strcasecmp(montadas[x].path,"vacio")== 0 && pathEncontrada == 0){
                        strcpy(montadas[x].path,funcion.path);
                        pathEncontrada = 1;
                        int y;
                        for(y = 0; y< 26;y++){
                            if(montadas[x].posicion[y].estado == 0){
                                montadas[x].posicion[y].estado = 1;
                                sprintf(valornumero,"%d",(y+1));
                            }
                        }
                    }
                }
    MostrarMontadasJ();
}

void MontarParticionF1(Funcion funcion){
    int ErrorT=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO. \n\n\n");
        ErrorT++;
        ErrorMontar++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int partcionborrar=-1;
        int z=0;
        int k=0;
        int l=0;
        for(z=0;z<4;z++){
            if(!strcmp(mbr2.particiones[z].part_name,funcion.name)&&mbr2.particiones[z].part_status!='0'){
                partcionborrar=z;
            }
        }
        if(partcionborrar>=0)
        {
            printf("\n\nParticion ya Existe.\n ");
            MontarParticionF2(funcion);
        }else{
            printf("\n\nParticion NO Existe en Primarias o Extendida, Verificando en Logicas.\n ");
            int comparador=ExisteLogica(funcion.name,funcion);
            if(comparador==0){
            printf("\n\nParticion NO Existe en Logicas.\n ");
            printf("\n Analizador - ERROR: No existe en la Particion \n Nombre: %s \n",funcion.name);
            ErrorT++;
            ErrorMontar++;
            }else{
            printf("\n\nParticion es Logica. \n ");
            MontarParticionF2(funcion);
            }
        }
        fclose(file2);
    }
    printf("\n\n ----- FIN DE MONTADO -----\n\n");
    printf("ERRORES: %i\n",ErrorT);
}

void MontarParticionF2(Funcion funcion){
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
printf("\n------------------ PATH (*)= %s ---------------------\n" , funcion.path);
char letraAasignar;
int numeroAsignar;
int auxLetraSeteada;
int i;
for(i=0; i<50; i++){
    if(montadas2[i].status==0){
      int auxLetraDisponible;
      int j;
      for(j=0; j<26; j++){
          if(letras[j].status==1){
          }else{
          auxLetraDisponible=j;
          auxLetraSeteada=j;
          break;
          }
      }
      int tieneletra=0;
      j=0;
      for(j=0; j<26; j++){
          if(!strcmp(letras[j].pathPerteneciente,funcion.path)){
              letraAasignar=letras[j].letra;
              auxLetraSeteada=j;
              tieneletra=1;
          }
      }
      if(tieneletra==1){
          printf("Letra a Asignarse es la: '%c' \n", letraAasignar);
          printf("Posicion de La Letra Asignada: '%i' \n", auxLetraSeteada);
      }else{
          letraAasignar=letras[auxLetraDisponible].letra;
          printf("Letra Disponible y se Asigno es: '%c' \n", letraAasignar);
          printf("Posicion de La Letra Asignada: '%i' \n", auxLetraSeteada);
          letras[auxLetraDisponible].status=1;
          strcpy(letras[auxLetraDisponible].pathPerteneciente,funcion.path);
          letras[auxLetraDisponible].pathPertenecienteP=funcion.path;
          for(j=0; j<26; j++){
              letras[auxLetraDisponible].posiciones[j]=0;
          }
      }
      for(j=0; j<26; j++){
          if(letras[auxLetraSeteada].posiciones[j]==0){
              numeroAsignar=j+1;
              letras[auxLetraSeteada].posiciones[j]=numeroAsignar;
              break;
          }else{
          }
      }
      limpiarvar(montadas2[i].name2,100);
      montadas2[i].letrafinal=letraAasignar;
      montadas2[i].numerofinal=numeroAsignar;
      montadas2[i].path2=funcion.path;
      montadas2[i].status=1;
      montadas2[i].name=funcion.name;
      strcpy(montadas2[i].name2,funcion.name);
      strcpy(montadas2[i].path3,funcion.path);
      montadas2[i].id[0]='v';
      montadas2[i].id[1]='d';
      montadas2[i].id[2]=letraAasignar;
      char texto[2];
      sprintf(texto, "%d", numeroAsignar);
      printf("Numero a Concatenar: '%s' \n", texto);
      int k=0;
      for(k; k<2;k++){
          if(texto[k+1]==NULL){
          montadas2[i].id[3]=texto[k];
          break;
          }else{
          montadas2[i].id[3]=texto[k];
          montadas2[i].id[4]=texto[k+1];
          break;
          }
      }
      printf("-------------------------------------------------\n" );
      printf("Concatenada Final: '%s' \n", montadas2[i].id );
      printf("-------------------------------------------------\n" );
      ContadorComandosExitosos++;
      break;
    }else{
    }
}
MostrarMontadas();
}

void DesmontarParticion(Funcion funcion){
    int estamontada=0;
    int posDeMontada=0;
    int ErrorT=0;
    int i;
     for(i=0; i<50; i++){
         if(!strcmp(montadas2[i].id,funcion.id)){
             printf("Si esta Montada la Particion.\n");
             posDeMontada=i;
             estamontada=1;
             break;
         }else{
         }
     }
     if(estamontada==1){
      char letraDeLaDesmontada;
      int auxLetraSeteada;
      int tieneletra=0;
      int j=0;
      for(j=0; j<26; j++){
          if(strcasecmp(letras[j].pathPertenecienteP,montadas2[posDeMontada].path2)==0){
              letraDeLaDesmontada=letras[j].letra;
              auxLetraSeteada=j;
              tieneletra=1;
          }
      }
      for(j=0; j<26; j++){
          if(letras[auxLetraSeteada].posiciones[j]==montadas2[posDeMontada].numerofinal){a
              letras[auxLetraSeteada].posiciones[j]=0;
              break;
          }else{
          }
      }
        printf("Particion Desmontada: %s\n",montadas2[posDeMontada].name);
        printf("Particion ID: %s\n",montadas2[posDeMontada].id);
        printf("\n\nDesmontado Finalizado: %s\n",montadas2[posDeMontada].name);
        montadas2[posDeMontada].status=0;
     }else{
        printf("\n Analizador - ERROR: La particion '%s' NO esta Montada. \n\n\n",funcion.id);
        ErrorT++;
        ErrorDesmontar++;
     }
    MostrarMontadas();
}

void ReporteMBR_dot(Funcion funcion){
    limpiarvar(cadd,5000);
    limpiarvar(cadE,5000);
    Funcion temporal=funcion;
    char cad[5000]="digraph g {\ngraph [rankdir = \"LR\"];\nlabel= \"Reporte MBR\";\nfontsize = 20;\n";
    char nombreextendida[500];

    printf("\n\n----------------- INFO DE DISCO --------------------\n\n");
    int ErrorT=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"')
    {
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"')
        {
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }

    if(EstaMontada(funcion)==1){

    int posM=posDeMontada_rep;
    posDeMontada_rep=0;
    int nombresiguales=0;
    int numeroprimarias=0;
    int numeroextendida=0;
    int TempPrimarias=0;
    int TempExt=0;
    printf("\n");

    char conc[500]="Subgraph cluster_mbr{\nrank=same;\nfontsize = 9;\nlabel=\"";
    strcat(conc,funcion.path);
    strcat(conc,"\";\nnode[shape=record, fontsize = 8,rankdir = LR];\n");
    strcat(cad,conc);

        FILE* file2= fopen(montadas2[posM].path3, "rb+");
        if (file2==NULL){
            printf("\n FATAL ERROR: Acceso Denegado o No Existe. \n\n");
            ErrorReporte1++;

        }else{
            MbrDisco mbr2;
            fseek(file2,0,SEEK_SET);
            fread(&mbr2, sizeof(MbrDisco), 1, file2);
            char concatenadoFinal[5000]="MBR[label=\"{ ";
            char cerrartabla1[100]="}|";
            char cerrartabla2[100]="} }\"];}         ";
            char conc1[5000]="{<d>Nombre|<d>mbr_tamano|<d>mbr_fecha_creacion|<d>mbr_disk_signature";
            char conc2[5000]="{<d>Valor|<d>";
            char texto[10];
            sprintf(texto, "%d",mbr2.mbr_tamano);
            strcat(conc2,texto);
            strcat(conc2,"|<d>");
            strcat(conc2,mbr2.mbr_fecha_creacion);
            strcat(conc2,"|<d>");
            char texto2[10];
            sprintf(texto2, "%d",mbr2.mbr_disk_signature);
            strcat(conc2,texto2);

            printf("Asignacion:: %i",mbr2.mbr_disk_signature);
            printf("\n");
            printf("Fecha de Consulta:: %s",mbr2.mbr_fecha_creacion);
            printf("\n");
            printf("Tamaño de Unidad:: %i",mbr2.mbr_tamano);
            printf("\n\n");
            printf("---------------- PARTICIONES EN DISCO ------------------------\n");
            char conc3[5000]=".";
            char conc4[5000]=".";
            char texto3[50];

            int z=0;
            for(z=0;z<4;z++){
                int k=0;
                int l=0;
                while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }
                    k++;
                }
                int interruptorPrimarias=0;
                int interruptorExt=0;

                if(mbr2.particiones[z].part_type=='p'||mbr2.particiones[z].part_type=='P'){
                interruptorPrimarias=1;
                char env[100];
                strcpy(env,mbr2.particiones[z].part_name);
                quitarComillas(env);
                printf("Primaria, Nombre: %s",sincomillas);
                printf(",Tamaño: %i",mbr2.particiones[z].part_size);
                printf("\n");
                numeroprimarias++;
                }
                if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                interruptorExt=1;
                char env4[100];
                strcpy(env4,mbr2.particiones[z].part_name);
                quitarComillas(env4);
                printf("Extendida, Nombre: %s",sincomillas);
                printf(",Tamaño: %i",mbr2.particiones[z].part_size);
                printf("\n");
                strcpy(nombreextendida,mbr2.particiones[z].part_name);
                numeroextendida++;
                }
                int ind=z+1;
                if(interruptorPrimarias==1){
                    interruptorPrimarias=0;

                    strcat(conc3,"|<d>part_status_");
                    sprintf(texto3, "%d",ind);
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_type_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_fit_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_start_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_size_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_name_");
                    strcat(conc3,texto3);

                    strcat(conc4,"|<d>");
                    char*x=mbr2.particiones[z].part_status;
                    strcat(conc4,&x);
                    strcat(conc4,"|<d>");
                    char*y=mbr2.particiones[z].part_type;
                    strcat(conc4,&y);
                    strcat(conc4,"|<d>");
                    char*w=mbr2.particiones[z].part_fit;
                    strcat(conc4,&w);
                    strcat(conc4,"|<d>");
                    char texto4[10];
                    sprintf(texto4, "%d",mbr2.particiones[z].part_start);
                    strcat(conc4,texto4);
                    strcat(conc4,"|<d>");
                    char texto5[10];
                    sprintf(texto5, "%d",mbr2.particiones[z].part_size);
                    strcat(conc4,texto5);
                    strcat(conc4,"|<d>");
                    char env3[100];
                    strcpy(env3,mbr2.particiones[z].part_name);
                    quitarComillas(env3);
                    strcat(conc4,sincomillas);

                }else if (interruptorExt==1){
                    interruptorExt=0;

                    strcat(conc3,"|<d>part_status_");

                    sprintf(texto3, "%d",ind);
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_type_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_fit_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_start_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_size_");
                    strcat(conc3,texto3);
                    strcat(conc3,"|<d>part_name_");
                    strcat(conc3,texto3);

                    strcat(conc4,"|<d>");
                    char*a=mbr2.particiones[z].part_status;
                    strcat(conc4,&a);
                    strcat(conc4,"|<d>");
                    char*b=mbr2.particiones[z].part_type;
                    strcat(conc4,&b);
                    strcat(conc4,"|<d>");
                    char*c=mbr2.particiones[z].part_fit;
                    strcat(conc4,&c);
                    strcat(conc4,"|<d>");
                    char texto4[10];
                    sprintf(texto4, "%d",mbr2.particiones[z].part_start);
                    strcat(conc4,texto4);
                    strcat(conc4,"|<d>");
                    char texto5[10];
                    sprintf(texto5, "%d",mbr2.particiones[z].part_size);
                    strcat(conc4,texto5);
                    strcat(conc4,"|<d>");
                    char env2[100];
                    strcpy(env2,mbr2.particiones[z].part_name);
                    quitarComillas(env2);
                    strcat(conc4,sincomillas);
                }else{
                }
            }
            strcat(concatenadoFinal,conc1);
            strcat(concatenadoFinal,conc3);
            strcat(concatenadoFinal,cerrartabla1);
            strcat(concatenadoFinal,conc2);
            strcat(concatenadoFinal,conc4);
            strcat(concatenadoFinal,cerrartabla2);
            strcat(cad,concatenadoFinal);
            printf("\nDOT %s",concatenadoFinal);

            printf("-------------------------------------------------------------- \n");
            printf("# de Particiones Primarias:   %i \n",numeroprimarias);
            printf("# de Particiones Extendidas:  %i \n",numeroextendida);
            printf("-------------------------------------------------------------- \n");

            if(numeroextendida!=0){
                printf("Reporte De Particion Extendida: '%s'  \n",nombreextendida);
                strcpy(funcion.path,montadas2[posM].path3);
                ReporteEBR_dot(funcion);
            }else{
                printf("No Existe Particiones Extendidas en la Unidad. \n");
            }
        }
    fclose(file2);
    }else{
     printf("\n Analizador - ERROR: La Particion No Esta Montada. \n\n");
     ErrorReporte1++;
    }
    strcat(cad,cadE);
    strcat(cad,"\n}\n");

    if(ErrorT==0){
        printf("\nDOT %s",cad);
        switch_mbr=1;
        limpiarvar(Dot_MBR,5000);
        strcat(Dot_MBR,cad);
        ReporteMBR_Generar(temporal);
    }else{
        printf("\nVACIO Y CON ERRORES= %d\n",ErrorT);
        switch_mbr=0;
    }
}

void ReporteEBR_dot(Funcion funcion){
    char cad[5000]="digraph g {\ngraph [rankdir = \"LR\"];\nlabel= \"Reporte EBR\";\nfontsize = 20;\n";
    strcat(cadE,"Subgraph ebr{");
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;

    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO \n\n\n");
        ErrorReporte1++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;
        for(z=0;z<4;z++){
            int k=0,l=0;
            while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }k++;
            }
            if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        if(nombresiguales>0){
        printf("\n Analizador - ERROR: Particion ya Existe. \n\n %s \n",funcion.name);
        ErrorT++;
        ErrorReporte1++;
        }
        else{
            int tamanoparticion=0;
            if(funcion.unit=='b'||funcion.unit=='B'){
                tamanoparticion=funcion.size;
            }
            else if(funcion.unit=='k'||funcion.unit=='K'){
                tamanoparticion=(funcion.size*1024);
            }
            else{
                tamanoparticion=funcion.size*(1024*1024);
            }
            int vacio=1;
            EBR ebr;
            int actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&ebr, sizeof(EBR), 1, file2);
            int next=ebr.part_next;
            inicio=sizeof(EBR);
            int fin=inicio+tamanoparticion;
            int numeroebr=0;
            int espaciolibre=mbr2.particiones[idextendida].part_size;
            espaciolibre-=32;
            do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
            }while(next!=-1);

            printf("----------------- LISTA DE PARTICIONES ------------------------\n");
            EBR indices[contador+1];
            contador=0;
            actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&indices[contador], sizeof(EBR), 1, file2);
            do{
                if(indices[contador].part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                }else{
                    next=-1;
                }
                contador++;
            }while(next!=-1);
            char texto3[50];
            char cerrartabla1[100]="}|";
            char cerrartabla2[100]="} }\"];}         \n}";
            int i=0;
            for(i=0;i<contador;i++){
                if(indices[i].part_start!=-1 ){
                vacio=0;
                int ind=i+1;
                strcat(cadE,"Subgraph cluster");
                sprintf(texto3, "%d",ind);
                strcat(cadE,texto3);
                strcat(cadE,"{\nrank=same;\nfontsize = 9;\nlabel=\"");
                strcat(cadE,"EBR_");
                strcat(cadE,texto3);
                strcat(cadE,"\";\nnode[shape=record, fontsize = 8,rankdir = LR];\n");
                strcat(cadE,"EBR");
                strcat(cadE,texto3);
                strcat(cadE,"[label=\"{ ");
                strcat(cadE,"{<d>Nombre|<d>part_status|<d>part_fit|<d>part_start|<d>part_size|<d>part_next|<d>part_name");
                strcat(cadE,"}|");

                strcat(cadE,"{<d>Valor|<d>");
                char*popo=indices[i].part_status;
                strcat(cadE,&popo);
                strcat(cadE,"|<d>");
                char *popo2=indices[i].part_fit;
                strcat(cadE,&popo2);
                strcat(cadE,"|<d>");
                char texto4[10];
                sprintf(texto4, "%d",indices[i].part_start);
                strcat(cadE,texto4);
                strcat(cadE,"|<d>");
                char texto5[10];
                sprintf(texto5, "%d",indices[i].part_size);
                strcat(cadE,texto5);
                strcat(cadE,"|<d>");
                char texto6[10];
                sprintf(texto6, "%d",indices[i].part_next);
                strcat(cadE,texto6);
                strcat(cadE,"|<d>");
                char env[100];
                strcpy(env,indices[i].part_name);
                quitarComillas(env);
                strcat(cadE,sincomillas);
                strcat(cadE,"} }\"];}");
                char env2[100];
                strcpy(env,indices[i].part_name);
                quitarComillas(env);

                printf("**Logica, Nombre:: %s",sincomillas);
                printf(",Tamaño:: %i",indices[i].part_size);
                printf("\n");
                printf(",Status:: %c",indices[i].part_status);
                printf("\n");
                printf(",Start:: %i",indices[i].part_start);
                printf("\n");
                printf(",Next:: %i",indices[i].part_next);
                printf("\n");

                if(fin<=(indices[i].part_start-sizeof(EBR))){
                    break;
                }
                else{
                    inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                    fin=inicio+tamanoparticion;
                    numeroebr=i+1;
                }
                if(i==0){
                espaciolibre=espaciolibre-indices[i].part_size;
                }else{
                espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                }
                }
            }
            strcat(cadE,"\n}");
            printf("----------------- INFORMACION ------------------- \n");
            printf("Total: %i\n",NumeroDeLogicas(funcion));
            printf("Posicion: %i\n",numeroebr);
            printf("Espacio Libre: %i\n",espaciolibre);
            printf("Inicio: %i\n",inicio);
            printf("Final: %i\n",fin);

            printf("------------------------------------------------------------ \n");
            printf("# Particiones Logicas:   %i \n",NumeroDeLogicas(funcion));
            printf("------------------------------------------------------------ \n");
            printf("# Particiones Logicas por Funcion:   %i \n",NumeroDeLogicas(funcion));
        }
        fclose(file2);
    }
            printf("ERRORES LSL: %i\n",ErrorT);
            if(ErrorT==0){
             printf("\nDOT %s \n\n",cadE);
            }else{
                 printf("Estructura con Errores.\n\n");
                 switch_ebr=0;
            }
}

void ReporteMBR_Generar (Funcion funcion){
    int ErrorT=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"')
    {
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int indice=0;
    char carpeta[100];
    while(funcion.path[indice]!='.'){
        if(funcion.path[indice]!='/'){
            char c1[1];
            c1[0]=funcion.path[indice];
            strncat(carpeta,c1,1);
        }
        else
        {
            strcat(finalizado,"mkdir ");
            strcat(finalizado,"\"");
            strcat(finalizado,carpeta);
            strcat(finalizado,"\"");
            strcat(finalizado,"\n");
            strcat(finalizado,"cd ");
            strcat(finalizado,"\"");
            strcat(finalizado,carpeta);
            strcat(finalizado,"\"");
            strcat(finalizado,"\n");
            strcat(carpeta,"");
            limpiarvar(carpeta,100);
        }
        indice++;
    }
    printf("\n Comando si el directorio no existe: %s\n",finalizado);
    system(finalizado);

    char consola[200]=" ";
    FILE *flujo=fopen("/home/samuels/Documents/[MIA]_201113875/proy/fase1_201113875/Reporte_mbr.dot","w");
    if (flujo==NULL){
        printf("\n Analizador - ERROR: Para Crear Archivo. \n\n \n");
        ErrorT++;
        ErrorReporte1++;
    }else{
        if(switch_mbr==1){
        strcat(consola,"dot -Tpdf /home/samuels/Documents/[MIA]_201113875/proy/fase1_201113875/Reporte_mbr.dot -o");
        strcat(consola,funcion.path);
        fputs(Dot_MBR,flujo);
        switch_mbr=0;
        fclose(flujo);
        system(consola);
        ContadorComandosExitosos++;
        }else{
            fclose (flujo);
        }
    }
}

void quitarComillas (char a[100]){
    limpiarvar(sincomillas,100);
    char pathauxiliar[100];
    strcpy(pathauxiliar,a);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(a,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(a,c2,1);
            q++;
        }
    }
    strcpy(sincomillas,a);
}

void ReporteDiskEBR(Funcion funcion){
    strcat(cadd,"subgraph cluster_0 {\nrankdir = \"RL\";\nlabel = \"LOGICAS\";");
    int ErrorT=0;
    int nombresiguales=0;
    int numeroextendida=0;
    int idextendida=-1;
    int tamanooextendida=0;
    int inicio=-1; int fin=-1;
    char*nombreextendida;
    int contador=0;
    FILE* file2= fopen(funcion.path, "rb+");
    if (file2==NULL){
        printf("\n Analizador - ERROR: ACCESO DENEGADO \n\n\n");
        ErrorT++;
        ErrorReporte2++;
    }
    else{
        MbrDisco mbr2;
        fseek(file2,0,SEEK_SET);
        fread(&mbr2, sizeof(MbrDisco), 1, file2);
        int z;
        for(z=0;z<4;z++){
            int k=0,l=0;
            while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }k++;
            }
            if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                numeroextendida++;
                idextendida=z;
                tamanooextendida=mbr2.particiones[z].part_size;
                nombreextendida=mbr2.particiones[z].part_name;
            }
        }
        if(nombresiguales>0){
        printf("\n Analizador - ERROR: Nombre de Particion ya Existente. \n\n %s \n",funcion.name);
        ErrorT++;
        ErrorReporte2++;
        }
        else{
            int tamanoparticion=0;
            if(funcion.unit=='b'||funcion.unit=='B'){
                tamanoparticion=funcion.size;
            }
            else if(funcion.unit=='k'||funcion.unit=='K'){
                tamanoparticion=(funcion.size*1024);
            }
            else{
                tamanoparticion=funcion.size*(1024*1024);
            }
            int vacio=1;
            EBR ebr;
            int actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&ebr, sizeof(EBR), 1, file2);
            int next=ebr.part_next;
            inicio=sizeof(EBR);
            int fin=inicio+tamanoparticion;
            int numeroebr=0;
            int espaciolibre=mbr2.particiones[idextendida].part_size;
            espaciolibre-=32;
            do{
                if(ebr.part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=ebr.part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file2);
                    next=ebr.part_next;
                    contador++;
                }
                contador++;
            }while(next!=-1);
            printf("-----------------Lista de Particiones------------------------\n");
            EBR indices[contador+1];
            contador=0;
            actual=mbr2.particiones[idextendida].part_start;
            fseek(file2,actual,SEEK_SET);
            fread(&indices[contador], sizeof(EBR), 1, file2);
            do{
                if(indices[contador].part_next!=-1){
                    actual+=sizeof(EBR);
                    actual+=indices[contador].part_size;
                    fseek(file2,actual,SEEK_SET);
                    fread(&indices[contador+1], sizeof(EBR), 1, file2);
                    next=indices[contador].part_next;
                }else{
                    next=-1;
                }
                contador++;
            }while(next!=-1);
            char texto3[50];
            int i=0;
            for(i=0;i<contador;i++){
                if(indices[i].part_start!=-1 && indices[i].part_status!='0'){
                vacio=0;
                int ind=i+20;
                strcat(cadd,"n_");
                sprintf(texto3, "%d",i);
                strcat(cadd,texto3);
                strcat(cadd,"[shape=\"rectangle\",label=\"EBR\"];");
                strcat(cadd,"n_");
                sprintf(texto3, "%d",ind);
                strcat(cadd,texto3);
                strcat(cadd,"[shape=\"rectangle\",label=\"NOMBRE:\n");
                strcat(cadd,indices[i].part_name);
                strcat(cadd,"\nseze:\n");
                sprintf(texto3, "%d",indices[i].part_size);
                strcat(cadd,texto3);
                strcat(cadd,"\nstart:\n");
                sprintf(texto3, "%d",indices[i].part_start);
                strcat(cadd,texto3);
                strcat(cadd,"\nnext:\n");
                sprintf(texto3, "%d",indices[i].part_next);
                strcat(cadd,texto3);
                strcat(cadd,"\"];");
                printf("Logica, Nombre: %s",indices[i].part_name);
                printf(",Tamaño: %i",indices[i].part_size);
                printf("\n");
                printf(",Status: %c",indices[i].part_status);
                printf("\n");
                printf(",Start: %i",indices[i].part_start);
                printf("\n");
                printf(",Next: %i",indices[i].part_next);
                printf("\n");
                if(fin<=(indices[i].part_start-sizeof(EBR))){
                    break;
                }
                else{
                    inicio=indices[i].part_start+indices[i].part_size+sizeof(EBR);
                    fin=inicio+tamanoparticion;
                    numeroebr=i+1;
                }
                if(i==0){
                espaciolibre=espaciolibre-indices[i].part_size;
                }else{
                espaciolibre=espaciolibre-indices[i].part_size-sizeof(EBR);
                }
            }
        }
            strcat(cadd,"\n}");
            printf("------------------- INFORMACION --------------------------- \n");
            printf("Total: %i\n",NumeroDeLogicas(funcion));
            printf("Posicion: %i\n",numeroebr);
            printf("Espacio Libre: %i\n",espaciolibre);
            printf("Inicio: %i\n",inicio);
            printf("Final: %i\n",fin);
            printf("----------------------------------------------------------- \n");
            printf("Numero de Particiones Logicas:   %i \n",NumeroDeLogicas(funcion));
            printf("----------------------------------------------------------- \n");
            printf("Numero de Particiones Logicas por funcion:   %i \n",NumeroDeLogicas(funcion));
        }
        fclose(file2);
    }
        printf("ERRORES LSL: %i\n",ErrorT);
}

void ReporteDiskMBR (Funcion funcion){
    limpiarvar(cadd,5000);
    Funcion temporal=funcion;
    int cc=0;
    int numeroextendida=0;
    strcat(cadd,"digraph g {\nnodesep=.05;rankdir=BT;node [shape=record];\nlabel= \"Disco\";\nfontsize = 20; subgraph cluster0{\n");
    char* nombreextendida;
    printf("\n\n---------------- INFORMACION DE DISCO ---------------------\n\n");
    int ErrorT=0;
    if(funcion.name[0]=='\"'){
        int r=0;
        while(funcion.name[r]!='\"'){
            r++;
        }
        funcion.name[r]='\0';
    }
    if(EstaMontada(funcion)==1){
    int posM=posDeMontada_rep;
    posDeMontada_rep=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
        FILE* file2= fopen(montadas2[posM].path3, "rb+");
        if (file2==NULL){
            printf("\n Analizador - ERROR: Acceso Denegado. \n\n");
            ErrorT++;
            ErrorReporte2++;
        }else{
            MbrDisco mbr2;
            fseek(file2,0,SEEK_SET);
            fread(&mbr2, sizeof(MbrDisco), 1, file2);
            printf("Asignacion: %i",mbr2.mbr_disk_signature);
            printf("\n");
            printf("Fecha de Consulta: %s",mbr2.mbr_fecha_creacion);
            printf("\n");
            printf("Tamaño de Unidad: %i",mbr2.mbr_tamano);
            printf("\n\n");
            printf("----------------Particiones En Disco------------------------\n");
            strcat(cadd,"\nlabel=\"PARTICIONES\";\n");
            strcat(cadd,"node2[label=\"<f0> MBR");
            strcat(cadd,"ID:");
            char texto[10];
            sprintf(texto, "%d",mbr2.mbr_disk_signature);
            strcat(cadd,texto);
            strcat(cadd,"TAmaño:");
            char texto2[10];
            sprintf(texto2, "%d",mbr2.mbr_tamano);
            strcat(cadd,texto2);
            strcat(cadd,"Fecha:");
            strcat(cadd,mbr2.mbr_fecha_creacion);
            strcat(cadd,"\", height=1];");
            int nombresiguales=0;
            int numeroprimarias=0;
            int numeroextendida=0;
            int TempPrimarias=0;
            int TempExt=0;
            int z=0;
            for(z=0;z<4;z++){
                int k=0;
                int l=0;
                while(funcion.name[k]!=NULL){
                if(mbr2.particiones[z].part_name[k]==funcion.name[k]){
                    l++;
                }
                    k++;
                }
                int interruptorPrimarias=0;
                int interruptorExt=0;
                if(mbr2.particiones[z].part_type=='p'||mbr2.particiones[z].part_type=='P'){
                interruptorPrimarias=1;
                printf("*Primaria, Nombre:: %s",mbr2.particiones[z].part_name);
                printf(",Tamaño:: %i",mbr2.particiones[z].part_size);
                printf("\n");
                numeroprimarias++;
                strcat(cadd,"node");
                char texto4[10];
                sprintf(texto4, "%d",cc);
                strcat(cadd,texto4);
                strcat(cadd,"[label=\"<f0>Part:");
                strcat(cadd,mbr2.particiones[z].part_name);
                strcat(cadd," size:");
                char texto3[10];
                sprintf(texto3, "%d",mbr2.particiones[z].part_size);
                strcat(cadd,texto3);
                strcat(cadd," Tipo: Primaria ");
                strcat(cadd,"\",height=1];");
                cc++;
                }else if(mbr2.particiones[z].part_type=='e'||mbr2.particiones[z].part_type=='E'){
                interruptorExt=1;
                printf("*Extendida, Nombre:: %s",mbr2.particiones[z].part_name);
                printf(",Tamaño:: %i",mbr2.particiones[z].part_size);
                printf("\n");
                nombreextendida=mbr2.particiones[z].part_name;
                numeroextendida++;
                strcat(cadd,"node");
                char texto4[10];
                sprintf(texto4, "%d",cc);
                strcat(cadd,texto4);
                strcat(cadd,"[label=\"<f0>Part:");
                strcat(cadd,mbr2.particiones[z].part_name);
                strcat(cadd," size:");
                char texto3[10];
                sprintf(texto3, "%d",mbr2.particiones[z].part_size);
                strcat(cadd,texto3);
                strcat(cadd," Tipo: Extendida ");
                strcat(cadd,"\",height=1];");
                cc++;
                }else{
                    strcat(cadd,"node");
                    char texto4[10];
                    sprintf(texto4, "%d",cc);
                    strcat(cadd,texto4);
                    strcat(cadd,"[label=\"<f0>Part:Libre");
                    strcat(cadd," size:");
                    char texto3[10];
                    sprintf(texto3, "%d",mbr2.particiones[z].part_size);
                    strcat(cadd,texto3);
                    strcat(cadd," Tipo: Libre ");
                    strcat(cadd,"\",height=1];");
                    cc++;
                }
            }
            if(numeroextendida!=0){
                printf("Particion Extendida: '%s'  \n",nombreextendida);
                strcpy(funcion.path,montadas2[posM].path3);
                lsl(funcion);
                ReporteDiskEBR(funcion);
            }else{
                printf("No Existe Particiones Extendidas en el Disco. \n");
            }
        }
    fclose(file2);
    }else{
     printf("\n Analizador - ERROR: Particion no esta Montada. \n\n");
            ErrorT++;
            ErrorReporte2++;
    }
    strcat(cadd,"\n}\n}");
    if(ErrorT==0){
        printf("\n::DOT:: %s",cadd);
        switch_mbr=1;
        ReporteDisk_Generar(temporal);
    }else{
        printf("\n VACIO Y CON ERRORES= %d\n",ErrorT);
        switch_mbr=0;
    }
}

void ReporteDisk_Generar(Funcion funcion){
    int ErrorT=0;
    char pathauxiliar[100];
    strcpy(pathauxiliar,funcion.path);
    char finalizado[100];
    strcpy(finalizado,"cd /\n");
    if(pathauxiliar[0]=='\"'){
        limpiarvar(funcion.path,100);
        int q=1;
        while(pathauxiliar[q]!='\"'){
            char c2[1];
            c2[0]=pathauxiliar[q];
            strncat(funcion.path,c2,1);
            q++;
        }
    }
    int indice=0;
    char carpeta[100];
    while(funcion.path[indice]!='.'){
    if(funcion.path[indice]!='/'){
            char c1[1];
            c1[0]=funcion.path[indice];
            strncat(carpeta,c1,1);
        }
        else{
            strcat(finalizado,"mkdir ");
            strcat(finalizado,"\"");
            strcat(finalizado,carpeta);
            strcat(finalizado,"\"");
            strcat(finalizado,"\n");
            strcat(finalizado,"cd ");
            strcat(finalizado,"\"");
            strcat(finalizado,carpeta);
            strcat(finalizado,"\"");
            strcat(finalizado,"\n");
            strcat(carpeta,"");
            limpiarvar(carpeta,100);
        }
        indice++;
    }
    printf("\nImprimir el comando q ejecuta en la terminal si el directorio no existe: %s\n",finalizado);
    system(finalizado);
    char consola[200]=" ";
    FILE *flujo=fopen("/home/samuels/Documents/[MIA]_201113875/proy/fase1_201113875/Reporte_disk.dot","w");
    if (flujo==NULL){
        printf("\n Analizador - ERROR: Error para Crear el Archivo. \n\n \n");
        ErrorT++;
        ErrorReporte2++;
    }else{
        if(switch_mbr==1){
        strcat(consola,"dot -Tpdf /home/samuels/Documents/[MIA]_201113875/proy/fase1_201113875/Reporte_disk.dot -o");
        strcat(consola,funcion.path);

        fputs(cadd,flujo);
        switch_mbr=0;
        fclose(flujo);
        system(consola);
        ContadorComandosExitosos++;
        }else{
            fclose (flujo);
        }
    }
}
//############ FIN DE LA MAGIA ##################
