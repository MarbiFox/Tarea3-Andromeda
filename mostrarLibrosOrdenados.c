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

void mostrarDocumentosOrdenados(List *Libros, int cantidad) {
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