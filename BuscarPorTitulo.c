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