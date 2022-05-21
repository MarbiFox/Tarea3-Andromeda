#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"
#include "list.c"
//#include "treemap.h"

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
    if(Libros->head == NULL) return 0;
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
          strcpy(lib->id,aux);
          pushBack(Libros, lib);
          (*contArchivos)++;
          //Si esta recien insertado marcarlo como no procesado        
      }
      // Recorrer el resto de la cadena.
      token = strtok(NULL, delimitador);
    }
  }
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

//leer hasta que aparezca una linea entera con caracteres nulos
//podria intentar copiar toda la linea donde encuentre el objeto buscado
char *obtenerDatos(FILE *file,char *obj){
  char cadena[1024] = {};
  char *modificable = (char*) calloc(61,sizeof(char));
  //printf("antes del while()...\n");
  while((fgets(cadena,1023,file)) != NULL){
    char *aux = strstr(cadena,obj);
    //printf("antes de los condicionales\n");
    if(strstr(cadena,"*** START OF THE PROJECT GUTENBERG") != NULL) break;
    if(aux != NULL){
      strcpy(modificable,aux);
      fgets(cadena,1023,file);
      eliminarSaltosDeLinea(modificable);
      aux = strstr(cadena,"       ");
      if(aux != NULL){
        eliminarEspaciosAdicionales(aux);
        strcat(modificable," ");
        strcat(modificable,aux);
      }
      return modificable;
    }
  }
  return NULL;
}

void procesarArchivos(List *Libros){
    Libro *libro = firstList(Libros);
    while(libro != NULL){
      //comprobar si el libro fue procesado o no
      if(libro->flag == false){
        //copio el la id y le agrego el .txt
        char aux[35] = {};
        strcpy(aux,libro->id);
        char pre[8]="Libros\\";
        strcat(pre,aux);
        strcpy(aux,pre);
        strcat(aux,".txt");

        printf("|%s|\n",aux);
        //abro archivo
        FILE *file = fopen(aux,"r");
        if (file == NULL) {
          printf("Error al abrir archivo.csv\n");
          printf("%s\n",aux);
          system("pause");
          exit(1);
        }
        libro->title = obtenerDatos(file,"Title:");  
        libro->autor = obtenerDatos(file,"Author:");



        //poner aqui lo de leer las palabras y guardarlo en el hashMap









        libro->flag = true;
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

char* buscarPalabra (FILE *f) {//busca una palabra en la lista
  char x[1024];
  if (fscanf(f, " %1023s", x) == 1)
    return strdup(x);
  else
    return NULL;
}

char* critrioPalabra(char* palabra){
  int i,len=strlen(palabra);
  for(i=0;i<len;i++){
    if(isalpha(palabra[i])==0){
      palabra[i]=' ';
    }else{
      palabra[i]=tolower(palabra[i]);
    }
  }
  return palabra;
}

/*char * eliminarEspacios(char  *cadena){
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
}*/

void IngresarAlMapa(List* ListaLibros){//busca las palabras 
  printf("Ingrese la id del libro: ");
  char linea[1024] = {};
  getchar();
  scanf("%[0-9a-zA-Z ,-]",linea);

  Libro* libro=buscarLibro(ListaLibros,linea);

  strcat(linea, ".txt");

  FILE* contenido = fopen(linea,"r");
  if(contenido == NULL){
      printf("No se ha podido leer el archivo\n");
      return exit(1);
  }
  
  char* plbr=buscarPalabra(contenido);

  HashMap* MapaPalabras=createMap(1000);

  int cont=1;
  while(plbr){
    insertMap(MapaPalabras,plbr,plbr);
    printf("cargando:%i%\n",cont);
    system("cls");
    //ingresar al hashmap
    plbr=buscarPalabra(contenido);
    cont++;
  }

  libro->Palabras=MapaPalabras;

  fclose(contenido);
  
}//falta agregar cada palabra al mapa

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
          //BuscarLibroPorTitulo(Libros);
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
  Libro *new  = (Libro*)malloc(sizeof(Libro));
  new->flag = false;
  new->autor = (char*) malloc(61*sizeof(char));
  new->title = (char*) malloc(61*sizeof(char));
  new->id = malloc(16*sizeof(char));
  new->contCaracteres = 0;
  new->contPalabras = 0;
  new->Palabras = createMap(100);
  return new;
}

void mostrarDocumentosOrdenados(List *Libros) {
  // ordenarLibros(Libros);
  
  // Ver si se guardaron bien los archivos
  Libro *aux = firstList(Libros);
  while (aux != NULL) {
    printf("\n*****************Libros*********************\n");
    if(aux->title == NULL){
      printf("Titulo: Sin titulo\n");
    }
    else{
      printf("Titulo: %s\n", aux->title);
    }
    if(aux->autor == NULL){
      printf("Titulo: Sin Autor\n");
    }
    else{
      printf("Autor: %s\n",aux->autor);
    }
    printf("id: %s\n", aux->id);
    printf("Cantidad de Palabras: %i\n", aux->contPalabras);
    printf("Cantidad de Caracteres: %i\n", aux->contCaracteres);
    printf("********************************************\n\n");
    aux = nextList(Libros);
  }
}
