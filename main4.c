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
        int totalPalabras = aux->contPalabras;
        Palabra* word;
        word = firstMap(aux->Palabras)->value;
        int cont = 0;
        while (cont < totalPalabras - 1){
            word->frecuencia = (float)(word->cantidad)/(totalPalabras);
            printf("%d.-%s\n",cont + 1,word->palabra);
            word = nextMap(aux->Palabras)->value;

            cont++;
        }
        printf("(se sale del while)\n");

        //TOMAR LOS 10 CON MAYOR FRECUENCIA
        //crear vector

        Palabra **vector = (Palabra**)malloc(totalPalabras * sizeof(Palabra*));
        printf("llega aqui\n");
        word = firstMap(aux->Palabras)->value;
        for (int i = 0 ; i < totalPalabras - 1; i++){
            vector[i] = word;
            word = nextMap(aux->Palabras)->value;
        }

        /*
        qsort(vector,totalPalabras,sizeof(Palabra*),compare_2);

        printf("llega aqui\n");
        printf("\n[TOP 10 PALABRAS]\n\n");
        for (int i = 1 ; i < 11; i++){
            printf("%d.-%s-Frecuencia:%f\n",i,vector[i]->palabra,vector[i]->frecuencia);
            word = nextMap(aux->Palabras)->value;
        }
        */

    }
    else{
        printf("[El libro ingresado no se encuentra procesado...]\n");
    }
    system("pause");

}
