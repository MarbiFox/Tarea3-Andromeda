#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"

typedef struct {
  char *palabra;
  size_t cantidad;
  size_t *idx;
  List *apariciones;
  size_t relevancia;
} Palabra;

typedef struct {
  char *title;
  char *id;
  char *autor;
  bool flag; //para comprobar si un libro ya a sido procesado
  int contPalabras;
  int contCaracteres;
  HashMap *Palabras;
} Libro;

//prototipos
Libro *crearLibro();
void mostrarDocumentosOrdenados(List *);
//ඞඞඞඞඞඞඞඞඞඞඞ

int search(char *token,List *Libros){
    Libro *aux = firstList(Libros);
    while(aux != NULL){
        if(strcmp(token,aux->id) == 0) return -1;
        aux = nextList(Libros);
    }
    return 0;
}

//Primero hay que cargar los archivos, so..
void cargarArchivos(int *contArchivos,List *Libros){
  //Ingresar linea con archivos
  printf("Ingrese linea con archivos (sin .txt)  y con espacios: ");
  char linea[1024] = {};
  getchar();
  scanf("%[0-9a-zA-Z ,-]",linea);


  // Especificar Delimitador para StrTok.
  char delimitador[] = "' '";

  // Obtener primera Palabra.
  char *token = strtok(linea, delimitador);
  if (token != NULL) {
    while (token != NULL) {
      //comprobar si el token existe en nuestra lista
      char aux[11] = {};
      if(search(token,Libros) == 0){
          strcpy(aux,token);
          printf("Entra a guardar libro...\n");
          Libro *lib = crearLibro(aux);
          //printf("Crea el libro...\n");
          strcpy(lib->id,aux);
          pushBack(Libros, lib);
          (*contArchivos)++;
          printf("[Guardando libro de id: %s]\n\n",lib->id);
          //Si esta recien insertado marcarlo como no procesado
      }
      // Recorrer el resto de la cadena.
      token = strtok(NULL, delimitador);
    }
  }
}

Palabra * CrearPalabra (char * plbr) {
  //Crear Palabra
  Palabra * new = (Palabra *) malloc (sizeof(Palabra));
  new->palabra = plbr;
  new->relevancia = 0;
  new->idx = (size_t*) malloc (sizeof(size_t));
  new->cantidad = 0;
  return new;
}




void eliminarSaltosDeLinea(char *a){
  int i = 0;
  while(a[i] != '\0'){
    if(a[i] == '\n') a[i] = '\0';
    i++;
  }
}

void eliminarEspaciosAdicionales(char *a){
  char aux[61] = {};
  int i = 0;
  int k = 0;
  int z = strlen(a);
  int *array = (int*)calloc(z,sizeof(int));
  for(i = 0 ; i < z ; i++){
    if(a[i] != ' '){
      array[i] = 1;
    }
    else array[i] = 0;
  }
  for(i = 1; i < z ;i++){
    if(array[i] == 0 && array[i-1] == 1){
      array[i] = -1;
    }
  }
  for(i = 0 ; i < z - 1; i++){
    if(array[i] == 1){
      aux[k] = a[i];
      k++;
    }
    if(array[i] == -1){
      aux[k] = a[i];
      k++;
    }
  }
  strcpy(a,aux);
}



char * construirLinea (FILE *file) {
  //Buscar inicio
  char cadena[1024] = {};
  int size = 0;
  char * aux = (char *) malloc (1000 * sizeof(char));

  while ((fgets(cadena,1023,file)) != NULL) {
    //Ver primera línea
    if (strstr(cadena,"Title: ")) {
      for (int i = 7; i < strlen(cadena); i++) {
        aux[size] = cadena[i];
        size++;
      }
      break;
    }
  }

  while ((fgets(cadena,1023,file)) != NULL) {
    if (strstr(cadena,"Author:") || strstr(cadena,"Authors:") || strstr(cadena,"Release Date:")) break;
    //Ver segunda línea (si es que tiene)
    aux[size-1] = ' ';
    if (strstr(cadena,"       ")) {
      for (int i = 7; i < strlen(cadena); i++) {
        aux[size] = cadena[i];
        size++;
      }
    }
  }

  aux[size] = '\0';
  return aux;
}

char* buscarPalabra (FILE *f) {//busca una palabra en la lista
  char x[1024];
  if (fscanf(f, " %1023s", x) == 1){
    return strdup(x);
  }
  else{
    return NULL;
  }
}

char* criterioPalabra(char* palabra){
  int i,len=strlen(palabra);
  for(i=0;i<len;i++){
    if(isalpha(palabra[i])==0 && i==len-1){
      palabra[i]='\0';
    }else if(isalpha(palabra[i])==0){
      palabra[i]=' ';
    }else{
      palabra[i]=tolower(palabra[i]);
    }
  }
  return palabra;
}

char * quitarEspacios(char  *cadena){
    char *aux = (char *) calloc(31,sizeof(char));
    int k = 0;
    int h = 0;
    while (*(cadena + k) != '\0'){
        if(*(cadena + k) != ' '){
            *(aux + h) = *(cadena + k);
            h++;
        }
        k++;
    }
    return aux;
}

void crearMapaPalabras(FILE * file, Libro * libro){//busca las palabras
    printf("[Entra en crearmapaPalabras]\n");
  //HashMap* MapaPalabras=createMap(1000);
  //Palabra * word = (Palabra *) malloc (sizeof(Palabra));
  //Saltar hasta la línea que cuente.
  char* palabra=buscarPalabra(file);
  int cont = 0;

  printf("palabra: %s\n",palabra);
  system("pause");
  //AQUI SE CAEEEEEE
  while (strcmp(palabra,"***") != 0)
  {
    palabra=buscarPalabra(file);
  }

  palabra=buscarPalabra(file);
  while (strcmp(palabra,"***")!=0)
  {
    palabra=buscarPalabra(file);
  }


  palabra=buscarPalabra(file);
  printf("palabra: %s\n",palabra);
  system("pause");

  while (strcmp(palabra,"***")!=0)
  {
    palabra=criterioPalabra(palabra);
    palabra=quitarEspacios(palabra);
    printf("%s\n",palabra);
    palabra=buscarPalabra(file);
    cont++;
  }


  printf("%s\n",buscarPalabra(file));

  printf("[PALABRAS PROCESADAS]\n");
  libro->contPalabras = cont;
}

void procesarArchivos(List *Libros){
    Libro *libro = firstList(Libros);
    while(libro != NULL){
      //comprobar si el libro fue procesado o no
      if(libro->flag == false){
        //copio el la id y le agrego el .txt
        char aux[35] = {"Libros\\"};
        strcat(aux,libro->id);
        strcat(aux,".txt");

        //abro archivo
        FILE *file = fopen(aux,"r");
        if (file == NULL) {
          printf("Error al abrir archivo.txt\n");
          system("pause");
          exit(1);
        }
        libro->title = construirLinea(file);
        //poner aqui lo de leer las palabras y guardarlo en el hashMap
        libro->flag = true;
        crearMapaPalabras(file,libro);

        fclose(file);
      }
      libro = nextList(Libros);
    }
    mostrarDocumentosOrdenados(Libros);
}

Libro* buscarLibro(List* Libros,char* id){//busca el libro en la lista

  Libro *aux = firstList(Libros);

  while(aux != NULL){
      if(strcmp(aux->id,id)==1)return aux;
      aux = nextList(Libros);
  }

  return NULL;
}


int main() {
  // Lista de Nombres de Libros.
  List *Libros = createList();
  int cantidadDeLibros = 0;

  int menu = -1;
  while (menu != 0) {
    printf("\n**************Menu*****************\n");
    printf("1.Cargar Documentos\n");
    printf("2.Mostrar Documentos Ordenados\n");
    printf("3.Buscar Un Libro Por Titulo\n");
    printf("4.Palabras Con Mayor Frecuencia\n");
    printf("5.Palabras Mas Relevantes\n");
    printf("6.Buscar Por Palabra\n");
    printf("7.Mostrar Palabra En Su Contexto En libro\n");
    printf("0.Salir\n");
    printf("\n");
    printf("\n***********************************\n");
    printf("Opcion: \n");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
          cargarArchivos(&cantidadDeLibros,Libros);
          printf("Entrando a procesar archivos...\n");
          procesarArchivos(Libros);
          // Función que agrega los nombres necesarios a la lista.
          system("pause");
        case 2:
          //mostrarDocumentosOrdenados(Libros);
          break;
        case 3:
          //IngresarAlMapa(Libros);
          break;
        case 4:
          // PalabraConMayorFrecuencia;
          break;
        case 5:
          // PalabrasMasRelevantes();
          break;
        case 6:
          // BuscarPorPalabra();
          break;
        case 7:
          // MostrarPalabraEnSuContexto();
          break;
        case 0:
          printf(
              "Saliendo...\nGracias por ocupar nuestro programa :)\nVuelva pronto\n");
          break;
        default:
          printf("Opcion ingresada no valida...\n");
          break;
    }
    system("cls");
  }

  return 0;
}

Libro *crearLibro(){
  printf("[FUNCION CREAR LIBRO]\n");
  Libro *new  = (Libro*)malloc(sizeof(Libro));
  new->flag = false;
  new->autor = (char*) malloc (1000 * sizeof(char));
  new->title = (char*) malloc (1000 * sizeof(char));
  new->id = (char*) malloc (1000 * sizeof(char));
  new->contCaracteres = 0;
  new->contPalabras = 0;
  new->Palabras = createMap(1000);
  return new;
}

void mostrarDocumentosOrdenados(List *Libros) {
    printf("[ENTRA A FUNCION MOSTRAR]\n");
  // ordenarLibros(Libros);

  // Ver si se guardaron bien los archivos
  Libro *aux = firstList(Libros);
  while (aux != NULL) {
    printf("\n*****************Libro*********************\n");
    if(aux->title == NULL){
      printf("Titulo: Sin titulo\n");
    }
    else{
      printf("Titulo: %s\n", aux->title);
    }
    printf("id: %s\n", aux->id);
    printf("Cantidad de Palabras: %i\n", aux->contPalabras);
    printf("Cantidad de Caracteres: %i\n", aux->contCaracteres);
    printf("********************************************\n\n");
    aux = nextList(Libros);
  }
}
