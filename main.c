#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  int contPalabras;
  int contCaracteres;
  HashMap *Palabras;
} Libro;

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
  
  //voy a modificarlo para que guarde la id de inmediato en el libro
  //Crear Lista de Id's
  //List * idBooks = createList();
  
  // Especificar Delimitador para StrTok.
  char delimitador[] = "' '";
  
  // Obtener primera Palabra.
  char *token = strtok(linea, delimitador);
  if (token != NULL) {
    while (token != NULL) {
        //comprobar si el token existe en nuestra lista
        if(search(token,Libros) == 0){
            printf("Entra a guardar libro...\n");
            Libro *lib = (Libro*)malloc(sizeof(Libro));
            char *aux= (char *) calloc(31,sizeof(char));
            strcpy(aux,token);
            lib->id = aux;
            pushBack(Libros, lib);
            (*contArchivos)++;
        }
      // Recorrer el resto de la cadena.
      token = strtok(NULL, delimitador);
    }
  }
    printf("imprimiendo libros...\n");
  //Imprimir las id de los libros para verificar que se guardan bien
  Libro *aux = firstList(Libros);
  while(aux != NULL){
      printf("Primera ID:%s\n\n",aux->id);
      aux = nextList(Libros);
  }
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
          // Función que agrega los nombres necesarios a la lista.
          system("pause");
          break;
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
