typedef struct {
  char *title;
  char *id;
  char *autor;
  bool flag; //para comprobar si un libro ya a sido procesado
  int contPalabras;
  int contCaracteres;
  int relevanciaActual;
  HashMap *Palabras;
} Libro;

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