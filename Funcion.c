double calcularRelevancia (List * Libros, int cantLibros, Palabra * p, Libro * mainBook){
  //Calcular variables:
  double a, b, c, d;
  double res;

  //Ocurrencias de p en MainBook
  a = p->cantidad;

  //Cantidad de Palabras en MainBook.
  b = mainBook->contPalabras;

  //Número de documentos.
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
  
  //Realizar el Cálculo.
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
  //Ingresar por teclado el título de un Libro.
  char * mainTitle = (char*) malloc (sizeof(char));
  scanf("%1023s", mainTitle);

  //Buscar el Mapa de Palabras de aquel título.
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