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
  size_t relevancia;
  float frecuencia;
} Palabra;

typedef struct {
  char *title;
  char *id;
  char *autor;
  bool flag; //para comprobar si un libro ya a sido procesado
  int contPalabras;
  int contCaracteres;
  HashMap *Palabras;
  int relevanciaActual;
} Libro;

//prototipos
Libro *crearLibro();
void mostrarDocumentosOrdenados(List *);
void mostrarDocumentosOrdenados2(List *Libros, int cantidad);

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
  new->frecuencia = 0;
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
    //Ver primera l??nea
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
    //Ver segunda l??nea (si es que tiene)
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
  char x[1024]="";
  int c,i=0;
  while (1)
  {
    c=fgetc(f);
    if(c==-1)return NULL;
    if(c==' ')break;
    if(c=='-')break;
    if(c=='\n')return strdup(x);
    x[i]=c;
    x[i+1]='\0';
    i++;
  }
  return strdup(x);
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

int compare_2(const void *a , const void *b){

    const Palabra* orderA = (Palabra*)a;
    const Palabra* orderB = (Palabra*)b;
    return (orderB->frecuencia - orderA->frecuencia);
}

Palabra** shellSort(Palabra **vector, size_t talla) {
  for (size_t intervalo = talla / 2; intervalo > 0; intervalo /= 2) {
    for (long i = intervalo; i < talla; i += 1) {
      float temp = vector[i]->frecuencia;
      long k;
      for (k = i; k >= intervalo && vector[k - intervalo]->frecuencia > temp; k -= intervalo) {
        vector[k]->frecuencia = vector[k - intervalo]->frecuencia;
      }
      vector[k]->frecuencia = temp;
    }
  }
  return vector;
}

void palabrasFrecuencia(List *Libros){

    //TOP 10
    //FRECUENCIA: CANTIDAD DE VECES QUE APARECE P / CANTIDAD TOTAL P

    printf("Ingrese id del libro: ");
    char id[1024] = {};
    getchar();
    scanf("%[0-9a-zA-Z ,-]",id);
    printf("id: %s\n",id);
    system("pause");
    //ABRIR LIBRO
    //verificamos si el libro existe
    Libro *aux = firstList(Libros);
    bool flag = true;
    while (1){
        if (strcmp(aux->id,id) == 0) break;
        aux = nextList(Libros);
        if (aux == NULL){
            flag = false;
            break;
        }
    }
    if (flag){
        printf("[ID encontrada]\n");
        //analizar libro
        int totalPalabras = (aux->contPalabras)-1;
        Palabra* word;
        word = firstMap(aux->Palabras)->value;
        int cont = 0;
        while (cont < totalPalabras - 1){
            word->frecuencia = (float)(word->cantidad)/(totalPalabras);
            word = nextMap(aux->Palabras)->value;

            cont++;
        }
        printf("(se sale del while)\n");

        //TOMAR LOS 10 CON MAYOR FRECUENCIA
        //crear vector

        Palabra **vector = (Palabra**)malloc(totalPalabras * sizeof(Palabra));

        printf("llega aqui\n");
        word = firstMap(aux->Palabras)->value;
        for (int i = 0 ; i < totalPalabras; i++){
            vector[i] = word;
            word = nextMap(aux->Palabras)->value;
        }

        vector=shellSort(vector,totalPalabras);

        printf("\n[TOP 10 PALABRAS]\n\n");
        for (int i = totalPalabras-1; i > totalPalabras-10 ; i--){
            printf("%d.-%s-Apariciones: %d-Frecuencia:%f\n",i,vector[i]->palabra,vector[i]->cantidad,vector[i]->frecuencia);
            word = nextMap(aux->Palabras)->value;
        }


    }
    else{
        printf("[El libro ingresado no se encuentra procesado...]\n");
    }
    system("pause");

}




void crearMapaPalabras(FILE * file, Libro * libro){//busca las palabras
  //INicio
  printf("[Entra en crearmapaPalabras]\n");
  HashMap* MapaPalabras=createMap(10000);
  Palabra * word = (Palabra *) malloc (sizeof(Palabra));

  //Saltar hasta la l??nea que cuente.
  char* palabra=buscarPalabra(file);
  //AQUI SE CAEEEEEE
  while (strcmp(palabra,"***") != 0)
  {
    palabra=buscarPalabra(file);
  }

  printf("paso al otro while\n");

  palabra=buscarPalabra(file);
  while (strcmp(palabra,"***")!=0)
  {
    palabra=buscarPalabra(file);
  }

  printf("Comienza Recorrido General\n");
  //Recorrido General del Texto.
  palabra=buscarPalabra(file);
  int idxPalabras = 0;
  int cantPalabras = 0;
  int cantCaracteres = 0;

  while (strcmp(palabra,"***")!=0)
  {
    //Modificar Palabra.

    palabra=criterioPalabra(palabra);
    palabra=quitarEspacios(palabra);
    //printf("%s\n",palabra);
    //Ingresar al mapa.
    if (searchMap(MapaPalabras, palabra) == NULL) {
      word = CrearPalabra(palabra);
      insertMap(MapaPalabras, palabra, word);
      cantPalabras++;
      cantCaracteres += strlen(palabra);
    }
    //Rellenar idx y cantidad.
    word = searchMap(MapaPalabras, palabra)->value;
    // word->idx[word->cantidad] = idxPalabras;
    word->cantidad++;

    //Reiniciar el ciclo
    idxPalabras++;
    palabra=buscarPalabra(file);
  }
  printf("Termina Recorrido General\n");

  //Cantidad de Palabras y Caracteres por libro.
  libro->contPalabras = cantPalabras;
  libro->contCaracteres = cantCaracteres;

  //printf("%s\n",buscarPalabra(file));
  eraseMap(MapaPalabras,firstMap(MapaPalabras)->key);
  libro->Palabras = MapaPalabras;
  printf("[PALABRAS PROCESADAS]\n");
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

void buscarPorTitulo(List* Libros){
  printf("Ingrese linea con archivos (sin .txt)  y con espacios: ");
  char linea[1024] = {};
  getchar();
  scanf("%[0-9a-zA-Z ,-]",linea);

  char delimitador[] = "' '";

  char *token = strtok(linea, delimitador);
  Libro* l=firstList(Libros);
  HashMap* p=l->Palabras;
  Pair* a=firstMap(p);
  
  while (token)
  {
    printf("libros que contienen la palabra [%s]:\n\n",token);
    while (l)
    {
      token=criterioPalabra(token);
      token=quitarEspacios(token);
      p=l->Palabras;
      if(searchMap(p,token)) printf(" -%s\n",l->title);
      l=nextList(Libros);
    }
    printf("\n");
    l=firstList(Libros);
    token=strtok(NULL,delimitador);
  }
  
  
}


int comparar(const void *pivote, const void *elemento)
{
    Libro *ptrPiv = (Libro *)pivote;
    Libro *ptrElm = (Libro *)elemento;
    
    return strcmp(ptrPiv->title,ptrElm->title);
}

//Crea arreglo y los ordeno con un qsort
Libro *ordenarLibros(List *Libros,size_t cantidad){
  Libro *aux = firstList(Libros);
  size_t i = 0;
  Libro *array = NULL;
  //Lo copio en un qsort
  array = (Libro*)malloc(cantidad*sizeof(Libro));
  while(aux!=NULL && i < cantidad){
    *(array+(i)) = *aux;
    aux = nextList(Libros);
    i++;
  }
  qsort(array,cantidad,sizeof(Libro),comparar);
  
  return array;
}

void mostrarDocumentosOrdenados2(List *Libros, int cantidad) {
  printf("[ENTRA A FUNCION MOSTRAR]\n");
  Libro *aux = ordenarLibros(Libros,cantidad);
  size_t i;
  for(i = 0;i < cantidad;i++){
    printf("\n*****************Libro*********************\n");
    if(aux[i].title == NULL){
      printf("Titulo: Sin titulo\n");
    }
    else{
      printf("Titulo: %s\n", aux[i].title);
    }
    printf("id: %s\n", aux[i].id);
    printf("Cantidad de Palabras: %i\n", aux[i].contPalabras);
    printf("Cantidad de Caracteres: %i\n", aux[i].contCaracteres);
    printf("********************************************\n\n");
  }
  free(aux);
}

double calcularRelevancia (List * Libros, int cantLibros, Palabra * p, Libro * mainBook){
  //Calcular variables:
  double a, b, c, d;
  double res;

  //Ocurrencias de p en MainBook
  a = p->cantidad;

  //Cantidad de Palabras en MainBook.
  b = mainBook->contPalabras;

  //N??mero de documentos.
  c = cantLibros;

  //Documentos que tienen la palabra p.
  d = 0;
  Libro * aux;
  aux = firstList(Libros);
  for (int i = 0; i < cantLibros; i++) {
    //Recorrer Mapa de Palabras del libro.
    if (searchMap(aux->Palabras, p->palabra) != NULL) {
      d++;
    }
    aux = nextList(Libros);
  }
  if (d == 0) printf("Esto es mentira");
  
  //Realizar el C??lculo.
  res = (double) (a/b) * log(c/d);

  return res;
}

void ordenarVector (size_t * vector, int talla)
{
  size_t i, k;
  size_t temp;
  for (i = 1; i < talla; i++)
  {
      temp = vector[i];
      k = i-1;
      while ((k >= 0) && (vector[k] >= temp))
      {
          vector[k+1] = vector[k];
          k--;
      }
      vector[k+1] = temp;
  }
}

void PalabrasMasRelevantes (List * Libros, int cantLibros) {
  //Ingresar por teclado el t??tulo de un Libro.
  char * mainTitle = (char*) malloc (sizeof(char));
  scanf("%1023s", mainTitle);

  //Buscar el Mapa de Palabras de aquel t??tulo.
  Libro * mainBook;
  mainBook = firstList(Libros);
  for (int i = 0; i < cantLibros; i++) {
    if (strstr(mainBook->title, mainTitle)) break;
    mainBook = nextList(Libros);
  }
  if (mainBook == NULL) printf("NO EXISTEN LIBROS CON ESE TiTULO");

  //Calcular la Relevancia de todos las Palabras en el Texto.
  Palabra * p;
  size_t * vector = (size_t *) malloc (mainBook->contPalabras * sizeof(size_t));
  p = firstMap(mainBook->Palabras)->value;
  for (int i = 0; i < mainBook->contPalabras; i++) {
    p->relevancia = (double) calcularRelevancia(Libros, cantLibros, p, mainBook);
    printf("%lf\n", p->relevancia);
    vector[i] = p->relevancia;
    p = nextMap(mainBook->Palabras)->value;
  }

  //Ordenar por Relevancia
  ordenarVector(vector, mainBook->contPalabras);

  // //Imprimir los 10 mayores del Vector.
  // for (int i = 0; i < 10; i++) {
  //   printf("%s, %f", p->palabra, p->relevancia);
  // }


}

int comparar1(const void *pivote, const void *elemento){

  Libro *ptrPiv = (Libro *)pivote;
  Libro *ptrElm = (Libro *)elemento;

  return ptrPiv->relevanciaActual - ptrElm->relevanciaActual;
}

void BuscarPorPalabra(List *Libros){
  char *variable = (char*) malloc(sizeof(char)*100);
  printf("Ingrese la palabra a buscar en los textos: ");
  getchar();
  scanf("%[0-9a-zA-Z ,-]",variable);

  variable = criterioPalabra(variable);
  variable = quitarEspacios(variable);

  size_t capacidad = 0;
  Libro *array = NULL;
  Libro *aux = firstList(Libros);
  while(aux){
    Palabra *p = searchMap(aux->Palabras,variable)->value;
    if(p!=NULL){
      capacidad++;
      array = (Libro*) realloc(array,sizeof(Libro)*capacidad);
      array[capacidad-1] = *aux;
      array[capacidad-1].relevanciaActual = p->relevancia;
    }
    aux = nextList(Libros);
  }
  qsort(array,capacidad,sizeof(Libro),comparar1);
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
          // Funci??n que agrega los nombres necesarios a la lista.
          system("pause");
          break;
        case 2:
          mostrarDocumentosOrdenados2(Libros,cantidadDeLibros);
          system("pause");
          break;
        case 3:
          buscarPorTitulo(Libros);
          system("pause");
          break;
        case 4:
          palabrasFrecuencia(Libros);
          break;
        case 5:
          PalabrasMasRelevantes(Libros,cantidadDeLibros);
          system("pause");
          break;
        case 6:
          BuscarPorPalabra(Libros);
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
