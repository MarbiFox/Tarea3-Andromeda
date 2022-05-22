//retorna 0 si no esta y 1 si esta en el mapa la cadena
int filtrado(char*cadena,HashMap* mapa){
    cadena = criterioPalabra(cadena);
    cadena = quitarEspacios(cadena);
    if(searchMap(mapa,cadena)!=NULL){
        return 1;
    }
    return 0;
}

//crea un mapa de palabras de filtrado, invocar en el main
HashMap* agregarFiltro(HashMap *mapa){
  //abro archivo de filtro
  FILE *filtro = fopen("listaDeFiltrado.txt","r");
  if(filtro == NULL) exit(1);
  
  //generar cadena dinamica donde guardar los filtros
  char *cadena = (char*) malloc(1024*sizeof(char));
  while(fgets(cadena,1023,filtro)!=NULL){
    //como la lista de filtro no tiene repetidos simplemento inserto
    cadena = quitarEspacios(cadena);
    cadena = criterioPalabra(cadena);
    insertMap(mapa,cadena,cadena);
    cadena = (char*) malloc(1024*sizeof(char));
  }
  //cierro archivo
  fclose(filtro);
  return mapa;
}