#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/queue.h"
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/heap.h"

typedef struct {
    char cancion[151];
    char artista[101];
    char genero[101];
    int anyo;
    int duracion;
} CANCION;

typedef struct {
    char nombre[101];
    int frecuencia;
} ARTISTA; 

typedef struct {
    char nombre_playlist[31];
    List * canciones;  
    List * canciones_recomendadas;
    Map * generos_p;
} PLAYLIST;

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

void act_generos(PLAYLIST* playlist, CANCION* cancion, int OP);

void leer_playlists(Map * mapa_PL){
    // Se inicializa un contador en 1 para construir el nombre de los archivos
    int contador = 1;
    while(1){
        char nombre_archivo[50];
        char nombre_pl[50];
        char nombre_pl_archivo[50];
        // Se construye el nombre del archivo de las canciones de la playlist
        sprintf(nombre_archivo, "playlist/playlist_%d.bin", contador);

        // Se abre el archivo binario de las canciones de la playlist en modo lectura
        FILE * archivo = fopen(nombre_archivo, "rb");
        if(archivo == NULL) break;

        // Se construye el nombre del archivo del nombre de la playlist
        sprintf(nombre_pl_archivo, "playlist/nombre_playlist_%d.txt", contador);
        // Se abre el archivo con el nombre de la playlist en modo lectura
        FILE * archivo_nombre = fopen(nombre_pl_archivo, "r");
        // Se lee el contenido del archivo (Nombre de la playlist) y se tratan los posibles errores
        if(archivo_nombre != NULL){
            if(fscanf(archivo_nombre, " %[^\n]", nombre_pl) != 1) strcpy(nombre_pl, "ERROR AL LEER EL NOMBRE");
            fclose(archivo_nombre);
        }
        else{
            strcpy(nombre_pl, "ERROR AL LEER EL NOMBRE");
        }
        // Se asigna memoria a la playlist que esta dentro del archivo
        PLAYLIST * pl = (PLAYLIST *) malloc(sizeof(PLAYLIST));

        // Se guarda el nombre de la playlist leído anteriormente del archivo
        strcpy(pl -> nombre_playlist, nombre_pl);
        // Se inicializan las listas y mapas de la playlist
        pl -> canciones = list_create();
        pl -> canciones_recomendadas = list_create();
        pl -> generos_p = map_create(is_equal_str);

        // Se crea una variable auxiliar para leer las canciones dentro del archivo de las canciones
        CANCION temp;

        // Se leen todas las canciones del archivo de canciones de la playlist
        while(fread(&temp, sizeof(CANCION), 1, archivo) == 1){
            CANCION * song = (CANCION *) malloc(sizeof(CANCION));
            *song = temp;
            // Se añade la canción a la playlist
            list_pushBack(pl -> canciones, song);
            // Se añade el género de la canción o se actualiza su frecuencia al mapa de géneros de la playlist
            act_generos(pl, song, 1);
        }
        // Se inserta la playlist en el mapa de playlist
        map_insert(mapa_PL, pl -> nombre_playlist, pl);
        // Se cierra el archivo que contiene las canciones de la playlist
        fclose(archivo);
        // Se le suma uno al contador para seguir buscando archivos
        contador++;
    }
}

CANCION * leer_cancion_fav(){
    // Se reserva memoria para la canción favorita
    CANCION * cancion_fav = (CANCION *) malloc(sizeof(CANCION));
    // Se abre el archivo de la canción favorita en modo lectura
    FILE* archivo = fopen("cancion_fav.bin", "rb");
    // Si no se pudo leer el archivo, este se crea y se retorna
    if(archivo == NULL){
        //se crea el archivo nuevo
        FILE* archivo_nuevo = fopen("cancion_fav.bin", "wb");
        //se cierra el archivo nuevo y se libera la memoria de la variable creada
        if (archivo_nuevo != NULL) fclose(archivo_nuevo);
        free(cancion_fav);
        return NULL;
    }
    // Si se pudo abrir, se lee la canción favorita de la usuaria y se retorna
    else{
        int leidos = fread(cancion_fav, sizeof(CANCION), 1, archivo);
        if(leidos == 1){
            // Se cierra el archivo
            fclose(archivo);
            return cancion_fav;
        }
    }
    // En caso de que no se pudo abrir el archivo, se libera la memoria de la variable creada y se cierra el archivo
    fclose(archivo);
    free(cancion_fav);
    return NULL;
}

// MENÚS
void mostrar_menu_principal(CANCION * cancion_fav, CANCION * song){
    limpiarPantalla();
    puts("========================================");
    puts("     Chiikavva Playist!");
    puts("========================================");
    puts("1) Ver Canciones Cargadas");
    puts("2) Artistas más escuchados");
    puts("3) Ingresar Canción Favorita");
    puts("4) Mis Playlists");
    puts("5) Mi Cola de Reproducción");
    puts("6) Salir");
    // Si la usuaria ingreso su canción favorita esta se muestra por pantalla
    if(cancion_fav != NULL) printf("Tu canción favorita :D : %s | %s\n", cancion_fav -> cancion, cancion_fav -> artista);
    else printf("No has ingresado tu canción favorita! D:\n");
    // Si hay una cola activa, se muestra la primera canción que este dentro de la cola
    if(song != NULL) printf("Sonando: %s \n", song -> cancion);
}

void mostrar_menu_PL(){
    limpiarPantalla();
    puts("========================================");
    puts("            MIS PLAYLISTS");
    puts("========================================");
    puts("1) Crear Playlist");
    puts("2) Añadir Canción a Playlist");
    puts("3) Eliminar Canción de Playlist");
    puts("4) Mostrar Playlist");
    puts("5) Eliminar Playlist");
    puts("6) Volver al Menú Principal");
}

void mostrar_menu_cola(){
    limpiarPantalla();
    puts("========================================");
    puts("     Mi Cola De Reproducción");
    puts("========================================");
    puts("1) Iniciar Cola de Reproducción");
    puts("2) Añadir Canción a la Cola");
    puts("3) Eliminar Canción de Cola");
    puts("4) Mostrar Cola de Reproducción");
    puts("5) Siguiente Canción");
    puts("6) Eliminar Cola de Reproducción");
    puts("7) Volver al menú Principal");
}

// FUNCIONES EXTRA

void mostrar_info_canciones(CANCION * song){
    printf("\n");
    printf("|| TÍTULO: %s | ARTISTA: %s | GÉNERO: %s | AÑO PUBLICACIÓN: %d | DURACIÓN: %d:%d",
        song -> cancion, song -> artista, song -> genero, song -> anyo, (song -> duracion) / 60, (song -> duracion) % 60);
    printf("\n");
}

void mostrar_PL_guardadas(Map* mapa_PL){
    puts("Playlists Guardadas: ");
    MapPair* aux_par= map_first(mapa_PL);
    while (aux_par!=NULL){
        printf("Nombre Playlist: %s\n", aux_par->key);
        aux_par= map_next(mapa_PL);
    }
}

void cargar_canciones(Map * canciones, Map * canciones_g){
    // Se abre el archivo csv que contiene las canciones
    FILE * archivo = fopen("data/Spotify Dataset.csv", "r");
    // Tratamiento de error por si el archivo no se puede abrir
    if(archivo == NULL){
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;
    // Se lee la linea de encabezados del archivo csv
    campos = leer_linea_csv(archivo, ',');

    // Se leen las lineas del archivo csv
    while((campos = leer_linea_csv(archivo, ',')) != NULL){
        // Se reserva memoria para la canción
        CANCION * song = (CANCION *) malloc(sizeof(CANCION));
        // Se guardan los datos de la canción
        strcpy(song -> cancion, campos[0]);
        strcpy(song -> artista, campos[1]);
        strcpy(song -> genero, campos[2]);
        song -> anyo = atoi(campos[3]);
        song -> duracion = atoi(campos[21]);

        // Se inserta la canción dentro del mapa de canciones
        map_insert(canciones, song -> cancion, song);

        // Se busca el género de la canción dentro del mapa de géneros
        MapPair * genre_pair = map_search(canciones_g, song -> genero);
        // Si el género no se encuentra dentro de este se añade y se crea una lista de canciones
        if(genre_pair == NULL){
            List * nueva_lista = list_create();
            // Se inserta la canción en la lista de canciones del género
            list_pushBack(nueva_lista, song);
            map_insert(canciones_g, song -> genero, nueva_lista);
        }
        // Si el género ya existe, solo se obtiene la lista de canciones y se añade la nueva canción a esta
        else{
            List * lista_g = (List *) genre_pair -> value;
            list_pushBack(lista_g, song);
        }
    }
}

// 1 VER ARCHIVO DE CANCIONES 
void ver_archivo(Map * canciones){
    // Se obtiene la primera canción del mapa de canciones
    MapPair * song_pair = map_first(canciones);
    // Si esta canción es nula, se avisa del error y se retorna
    if(song_pair == NULL){
        printf("Error al cargar el archivo. Por favor, intente nuevamente");
        return;
    }
    // Se itera sobre el mapa de canciones mostrando la información de cada una de las canciones
    while(song_pair != NULL){
        mostrar_info_canciones(song_pair -> value);
        song_pair = map_next(canciones);
    }
}

// 3 CANCION FAV
void guardar_cancion_fav(CANCION  fav_song){
    // Se abre el archivo donde se guarda la canción favorita en modo escritura
    FILE * archivo = fopen("cancion_fav.bin", "wb");
    // Si el archivo se pudo abrir se guarda la canción favorita ingresada por la usuaria
    if(archivo != NULL){
        fwrite(&fav_song, sizeof(CANCION), 1, archivo);
    }
    // Se cierra el archivo
    fclose(archivo);
}

void cancion_favorita(Map * canciones, CANCION ** cancion_fav){
    char fav_song[150];
    int encontrado = 0;
    int contador = 0;
    do{
        /* 
        Se pregunta si el usuario quiere continuar añadiendo su canción favorita 
        si es que se equivoca al escribir 3 veces o más
        */
        if(contador >= 3){
            char opcion;
            puts("Desea continuar? (s/n)");
            scanf("%s", &opcion);
            // Si el usuario desea continuar, se resetea el contador
            if(opcion == 's') contador = 0;
            // Si no, se retorna
            else return;
        }
        // Se le pregunta a la usuaria su canción favorita
        printf("Ingrese su canción favorita: \n");
        scanf(" %[^\n]", fav_song);

        // Se busca la canción en el mapa de canciones
        MapPair * song_pair = map_search(canciones, fav_song);
        // Si la canción existe, se guarda en archivo y en una variable y se retorna al menú principal
        if(song_pair != NULL){
            *cancion_fav = song_pair -> value;
            guardar_cancion_fav(**cancion_fav);
            encontrado = 1;
            return;
        }
        // Si la canción no existe, se avisa y se le vuelve a preguntar a la usuaria por su canción favorita
        else{
            printf("Esa canción no se encuentra en el catálogo\n");
        }
        contador++;
    } while(encontrado != 1);
}
// 4 MIS PLAYLISTS

// -case 1.2
void crear_PL_cola(Map* mapa_PL, Queue* cola_rep){  
    //SE LE SOLICITA A LA USUARIA QUE ELIJA UN NOMBRE PARA SU PLAYLIST 
    puts("Ingrese un Nombre para su Playlist (máx 30 caracteres): ");
    char nombre[31];
    scanf(" %30[^\n]", nombre);
    //SE VERIFICA QUE NO HAYA UNA PLAYLIST CON ESE NOMBRE. SI HAY UNA, SE DA UN AVISO Y SE RETORNA AL MENU DE PLAYLISTS
    MapPair * par_PL = map_search(mapa_PL, nombre);
    if(par_PL != NULL){
        puts("Ya Existe una Playlist con ese Nombre.");
        return; 
    }
    //SI EL NOMBRE NO ESTÁ REGISTRADO, SE CREA UNA NUEVA PLAYLIST
    PLAYLIST* nueva_pl= (PLAYLIST*)malloc(sizeof(PLAYLIST));
    strcpy(nueva_pl->nombre_playlist, nombre);
    //SE CREA LA LISTA VACIA DE SUS CANCIONES Y SE GUARDA LA NUEVA LISTA EN LA PLAYLIST
    List* nueva_canciones= list_create(); 
    nueva_pl->canciones= nueva_canciones;
    //SE CREA LISTA VACIA DE CANCIONES RECOMENDADAS Y SE GUARDA EN LA PLAYLIST
    List* nueva_recomedadas= list_create();
    nueva_pl->canciones_recomendadas= nueva_recomedadas;
    //SE CREA UN MAPA VACIO DE LOS GENEROS Y SE GUARDA EN LA PLAYLIST
    Map* nueva_generos= map_create(is_equal_str); 
    nueva_pl->generos_p= nueva_generos;
    //SE RECORRE LA COLA Y SE COPIA SUS CANCIONES EN LA NUEVA LISTA
    CANCION* aux_c = queue_front(cola_rep);
    while(aux_c!=NULL){
        list_pushBack(nueva_pl->canciones, aux_c);
        act_generos(nueva_pl,aux_c,1); 
        aux_c = queue_next(cola_rep);
    }
    //SE INSERTA LA PLAYLIST EN EL MAPA DE PLAYLIST 
    map_insert(mapa_PL, nueva_pl->nombre_playlist, nueva_pl);
    printf("¡Playlist %s creada con Éxito!\n", nueva_pl->nombre_playlist);
}

// -case 1.1
void crear_PL(Map* mapa_PL, Queue* cola_rep){  
    //SI ES QUE LA COLA EXISTE, SE PREGUNTA SI ES QUE QUIERE CREAR LA PLAYLIST A PARTIR DE ELLA 
    if (queue_front(cola_rep) != NULL){
        char resp;
        do {
        puts("¿Desea crear una Playlist a partir de su Cola de Reproducción? (s/n)");
        scanf(" %c", &resp);
        //SI DICE QUE SI, SE LLAMA A LA FUNCION CORRESPONDIENTE 
        if (resp=='s') {
            crear_PL_cola(mapa_PL, cola_rep);
            return;
        }
        //SI DICE QUE NO, SE ROMPE EL DO WHILE PARA CREAR LA PLAYLIST DESDE CERO
        else if (resp=='n') break;
        //SI INGRESA UNA OPCION QUE NO SEA 's' O 'n', SE DA UN AVISO Y SE VUELVE A PREGUNTAR HASTA QUE DECIDA ENTRE S O N
        puts("Ingrese una opción válida.");
        } while (resp!='n' && resp!='s');
    }
    //SE LE SOLICITA A LA USUARIA QUE ELIJA UN NOMBRE PARA SU PLAYLIST 
    puts("Ingrese un Nombre para su Playlist (máx 30 caracteres): ");
    char nombre[31];
    scanf(" %30[^\n]", nombre);
    //SE VERIFICA QUE NO HAYA UNA PLAYLIST CON ESE NOMBRE. SI HAY UNA, SE DA UN AVISO Y SE RETORNA AL MENU DE PLAYLISTS
    MapPair * par_PL = map_search(mapa_PL, nombre);
    if(par_PL != NULL){
        puts("Ya Existe una Playlist con ese Nombre.");
        return; 
    }
    //SI EL NOMBRE NO ESTÁ REGISTRADO, SE CREA UNA NUEVA PLAYLIST Y SE INICIALIZAN SUS ATRIBUTOS
    PLAYLIST* nueva_pl= (PLAYLIST*)malloc(sizeof(PLAYLIST));
    strcpy(nueva_pl->nombre_playlist, nombre);
    //SE CREA LISTA DE CANCIONES VACIA Y SE GUARDA EN LA PLAYLIST
    List* nueva_canciones= list_create();
    nueva_pl->canciones= nueva_canciones;
    //SE CREA LISTA DE CANCIONES RECOMENDADAS Y SE GUARDA EN LA PLAYLIST
    List* nueva_recomedadas= list_create();
    nueva_pl->canciones_recomendadas= nueva_recomedadas;
    //SE CREA MAPA DE GENEROS VACIA Y SE GUARDA EN LA PLAYLIST
    Map* nueva_generos= map_create(is_equal_str); 
    nueva_pl->generos_p= nueva_generos;
    //SE INSERTA LA NUEVA PLAYLIST AL MAPA DE PLAYLISTS
    map_insert(mapa_PL, nueva_pl->nombre_playlist, nueva_pl);
    printf("¡Playlist %s creada con Éxito!\n", nueva_pl->nombre_playlist); 
}

// -case 2
void anadir_cancion_PL(Map* archivo_c, Map* mapa_PL){ 
    //SI EL MAPA DE PLAYLISTS ESTA VACIO, SE DA UN AVISO Y SE RETORNA
    if (map_first(mapa_PL)==NULL) {
        puts("No se ha creado Ninguna Playlist. Cree al menos una.");
        return;
    }
    //SE PREGUNTA QUE CANCION DESEA AÑADIR A LA PLAYLIST
    puts("¿Qué canción desea guardar?: ");
    char nombre_c_agregar[151];
    scanf(" %150[^\n]", nombre_c_agregar);
    //SE BUSCA EL PAR DE LA CANCION QUE SE DESEA AGREGAR, SI ESTE ES NULO ES PORQUE NO EXISTE EN EL ARCHIVO
    MapPair * par_cancion = map_search(archivo_c, nombre_c_agregar);
    if(par_cancion == NULL){
        puts("Esa canción no se encuentra en el catálogo.");
        return; 
    }
    //SE OBTIENE LA CANCION DEL PAR 
    CANCION* cancion_agregar= par_cancion->value;
    printf("Canción escogida: %s\n", cancion_agregar->cancion);
    //SE MUESTRAN LAS PLAYLISTS GUARDADAS
    mostrar_PL_guardadas(mapa_PL);
    //SE PREGUNTA A QUE PLAYLIST DESEA AGREGAR LA CANCION
    puts("¿A qué Playlist desea agregarla?: ");
    char nombre_p_agregar[31];
    scanf(" %30[^\n]", nombre_p_agregar);
    //SE BUSCA LA PLAYLIST EN EL MAPA DE PLAYLISTS. SI ESTE NO EXISTE, SE DA UN AVISO Y SE RETORNA
    MapPair * par_PL = map_search(mapa_PL, nombre_p_agregar);
    if(par_PL == NULL){
        puts("No Existe una Playlist con ese Nombre.");
        return; 
    }
    //SE INSERTA LA NUEVA CANCION EN LA PLAYLIST 
    PLAYLIST* playlist_agregar= par_PL->value;
    list_pushBack(playlist_agregar->canciones, cancion_agregar);
    act_generos(playlist_agregar,cancion_agregar,1); //Se llama a la función que actualiza los géneros del mapa de géneros y su frecuencia
    printf("¡%s Agregada con Éxito a %s!", cancion_agregar->cancion, playlist_agregar->nombre_playlist);
}

// -case 3
void eliminar_cancion_PL(Map* archivo_c, Map* mapa_PL){  
    //SI EL MAPA DE PLAYLISTS ESTA VACIO, SE DA UN AVISO Y SE RETORNA
    if (map_first(mapa_PL)==NULL) {
        puts("No se ha creado Ninguna Playlist. Cree al menos una.");
        return;
    }
    //SE MUESTRAN LAS PLAYLISTS GUARDADAS
    mostrar_PL_guardadas(mapa_PL);
    //SE PREGUNTA A QUÉ PLAYLIST LE QUIERE ELIMINAR UNA CANCION
    puts("¿Qué Playlist desea Modificar?");
    char nombre_pc_eliminar[31];
    scanf(" %30[^\n]", nombre_pc_eliminar);
    //SE BUSCA LA PLAYLIST EN EL MAPA DE PLAYLISTS. SI ESTE NO EXISTE, SE DA UN AVISO Y SE RETORNA
    MapPair * par_PL = map_search(mapa_PL, nombre_pc_eliminar);
    if(par_PL == NULL){
        puts("No Existe una Playlist con ese Nombre.");
        return; 
    }
    //SI EXISTE, SE OBTIENE LA PLAYLIST
    PLAYLIST* playlist_c_eliminar= par_PL->value;
    //SE MUESTRAN SUS CANCIONES GUARDADAS. SI NO TIENE, SE DA UN AVISO
    puts("Canciones Guardadas: ");
    if (list_first(playlist_c_eliminar->canciones)==NULL) {
        printf("No hay canciones Guardadas en %s\n", playlist_c_eliminar->nombre_playlist);
        return;
    }
    else {
        CANCION* aux= list_first(playlist_c_eliminar->canciones);
        while (aux!=NULL){
            mostrar_info_canciones(aux);
            aux= list_next(playlist_c_eliminar->canciones);
        }
    }
    //SE PREGUNTA LA CANCIÓN QUE DESEA ELIMINAR
    puts("¿Qué canción desea eliminar?: ");
    char nombre_c_eliminar[151];
    scanf(" %150[^\n]", nombre_c_eliminar);
    //SE BUSCA EL PAR DE LA CANCION QUE SE DESEA ELIMINAR, SI ESTE ES NULO ES PORQUE NO EXISTE EN EL ARCHIVO
    MapPair * par_cancion = map_search(archivo_c, nombre_c_eliminar);
    if(par_cancion == NULL){
        puts("Esa canción no se encuentra en el catálogo.");
        return; 
    }
    printf("Canción a eliminar: %s\n", par_cancion->key);
    //SE RECORRE LA PLAYLIST PARA ENCONTRAR LA CANCION 
    CANCION* aux= list_first(playlist_c_eliminar->canciones);
    while(aux!=NULL){
        //SI SE ENCUENTRA, SE ELIMINA DE LA PLAYLIST Y SE DA UN AVISO 
        if (strcmp(aux->cancion, par_cancion->key) == 0){
            CANCION* basura= list_popCurrent(playlist_c_eliminar->canciones);
            act_generos(playlist_c_eliminar,basura,-1); //se llama a la función para que baje la frecuencia del género de la canción a eliminar en el mapa de géneros de la playlist
            printf("¡Canción eliminada correctamente!\n");
            return;
        }
        aux= list_next(playlist_c_eliminar->canciones);
    }
    //SI NO SE ENCUENTRA ES PORQUE NO ESTA GUARDADA EN LA PLAYLIST
    printf("La Canción %s No se encuentra Guardada en %s", par_cancion->key, playlist_c_eliminar->nombre_playlist);
}

//Actualizar generos de la playlist, subir o bajar la frecuencia de //un genero en el mapa de géneros
// OP = 1 subir frencuencia / OP = -1 bajar frecuencia
void act_generos(PLAYLIST* playlist, CANCION* cancion, int OP){
    MapPair* par = map_search(playlist->generos_p, cancion->genero);
    
    if (par!= NULL){ //actualiza la frecuencia del genero | SI EL GENERO YA ESTA EN EL MAPA
        int*frec = (int *)par->value; 
        *frec += OP;
        if(*frec <=0){
            free(frec);
            map_remove(playlist->generos_p, cancion->genero); // si la frecuencia queda como 0, eliminamos el genero del mapa
        }
    }
    else if(OP==1){ //SI GENERO NO ESTA EN EL MAPA 
        
        int* nueva_frec =(int*) malloc(sizeof(int)); 
        *nueva_frec = 1; 
        map_insert(playlist->generos_p,cancion->genero,nueva_frec); //se inserta el genero nuevo con frecuencia 1
    }
}

//Encontrar el genero dentro del mapa de generos de una playlist, que tenga la frecuencia mayor
char* encontrar_genero_top(Map* generos_p){
    char* genero_top = NULL; 
    int max_frec = 0; 
    
    MapPair* par= map_first(generos_p);
    
    while(par!=NULL){//recorremos el mapa
        int frec = *(int*) par->value; 
        
        if (frec > max_frec){ //Si la frecuencia actual es mayor a la frecuencia registrada como maxima, la nueva frecuencia será registrada como la máxima
            max_frec = frec;
            genero_top = (char*) par->key; 
        }
        
        par= map_next(generos_p);
    }

    return genero_top; //Se retorna el género con la mayor frecuencia en el mapa
}

void canciones_rec(PLAYLIST* playlist, Map* canciones_g){
    //se limpia la lista anterior de canciones recomendadas, para agregar recomendaciones nuevas
    list_clean(playlist->canciones_recomendadas);
    //obtener el género con mayor frecuencia en el mapa de géneros de la playlist
    char* genero_top = encontrar_genero_top(playlist->generos_p);
    if(genero_top == NULL)return;

    //buscar el género con mayor frecuencia en la playlist, en el catálogo global de géneros
    MapPair* par_g = map_search(canciones_g, genero_top);
    if(par_g==NULL) return;

    //obtenemos la lista de canciones de ese género desde el catálogo global de géneros
    List* canciones_genero = (List*) par_g->value;
    int total;
    total = list_size(canciones_genero);

    //reservamos memoria para guardar temporalmente las canciones que no hayan sido agregadas a la playlist
    CANCION** dispo =(CANCION**)malloc(sizeof(CANCION*) * (total)); //las guardamos en un arreglo para su acceso inmediato más adelante cuando escogamos una canción aleatoria
    int num_dispo= 0; //contador que indica cuantas canciones hay disponibles para recomendar (canciones que no hayan sido ya agregadas a la playlist)

    CANCION* aux_c_g = list_first(canciones_genero); //Auxiliar para recorrer las canciones del género (c_g)

    while (aux_c_g!=NULL){ //recorremos las canciones del género mñas escuchado de la playlist
        int en_PL = 0; //Indicador si la canción actual está en la playlist (1) o no (0)
        CANCION* aux_c_PL = list_first(playlist->canciones); //Auxiliar para recorrer las canciones de la playlist (c_PL)

        while(aux_c_PL!=NULL){ 
            if(strcmp((aux_c_PL->cancion),(aux_c_g->cancion))==0 ){ //se busca si la cancion del género está en la playlist
                en_PL =1; //si la canción se encuentra en la playlist, cambiamos su indicador a 1
                break; //y rompemos el bucle
            }
            aux_c_PL = list_next(playlist->canciones);
            
        }

        if(!en_PL) { //si la canción no estaba en la playlist
            dispo[num_dispo++] = aux_c_g; //la añadimos a nuestro arreglo temporal de canciones disponibles
        }
        aux_c_g = list_next(canciones_genero);
    }

    int num_rec; //cantidad de canciones que recomendaremos
    if(num_dispo<3){
        num_rec = num_dispo; //si hay menos de 3 canciones disponibles para recomendar, se recomendarán las que hayan disponibles
    } 
    else{
        num_rec = 3; //pero si hay mas de 3 canciones disponibles para recomendar, se recomendarán 3
    }

    for(int i =0; i<num_rec; i++){ //iteramos la cantidad de veces que recomendaremos canciones (según num_rec)
        int rango = num_dispo-i; //se calcula la cantidad de canciones disponibles, según cuantas hemos recomendado ya, para que no se repitanm
        int j = i+ rand() % rango; // Elige una posición al azar entre las canciones que quedan disponibles para recomendar

        //este es el "algoritmo de barajado" Fisher-Yates
        CANCION* aux = dispo[i]; 
        dispo[i] = dispo[j];// se mueve la canción en la posición i a la posición j, para que en la proxima iteración cuando el rango se achique, no se pueda repetir la misma canción, y así se mantenga la aleatoridad
        dispo[j] = aux; //se mueve la canción en la posición j elegida al azar, a la posición i, para que esta sea agregada a canciones_recomendadas por el lista_pushBack

        list_pushBack((playlist->canciones_recomendadas),dispo[i]); //guardamos la canción seleccionada aleatoriamente
    }

    free(dispo); //liberamos el arreglo temporal de canciones disponibles para recomendar
}

// -case 4
void mostrar_PL(Map* mapa_PL, Map* canciones_g){
    //SI EL MAPA DE PLAYLISTS ESTA VACIO, SE DA UN AVISO Y SE RETORNA
    if (map_first(mapa_PL)==NULL) {
        puts("No se ha creado Ninguna Playlist. Cree al menos una.");
        return;
    }
    //SE MUESTRAN LAS PLAYLISTS GUARDADAS
    mostrar_PL_guardadas(mapa_PL);
    //SE LE PREGUNTA LA PLAYLIST QUE QUIERE VER
    puts("¿Qué Playlist desea Ver?: ");
    char nombre_p_mostrar[31];
    scanf(" %30[^\n]", nombre_p_mostrar);
    //SE BUSCA EL PAR DE LA PLAYLIST. SI ESTE ES NULO, ES PORQUE NO EXISTE Y SE DA UN AVISO
    MapPair * par_PL = map_search(mapa_PL, nombre_p_mostrar);
    if(par_PL == NULL){
        puts("No Existe una Playlist con ese Nombre.");
        return; 
    }  
    //SE OBTIENE LA PLAYLIST 
    PLAYLIST* playlist_mostrar= par_PL->value;
    printf("Nombre Playlist: %s \n", playlist_mostrar->nombre_playlist);
    //SE MUESTRAN SUS CANCIONES GUARDADAS. SI NO TIENE, SE DA UN AVISO
    puts("Canciones Guardadas: ");
    if (list_first(playlist_mostrar->canciones)==NULL) printf("No hay canciones Guardadas en %s\n", playlist_mostrar->nombre_playlist);
    else {
        CANCION* aux= list_first(playlist_mostrar->canciones);
        while (aux!=NULL){
            mostrar_info_canciones(aux);
            aux= list_next(playlist_mostrar->canciones);
        }
    }

    puts("Canciones Recomendadas: ");

    //SE MUESTRAN SUS CANCIONES RECOMENDADAS. SI NO TIENE, PORQUE NO HAY CANCIONES GUARDADAS, SE DA UN AVISO 
    canciones_rec(playlist_mostrar, canciones_g);
    /*
    if (list_first(playlist_mostrar->canciones_recomendadas)==NULL) printf("No hay canciones Recomendadas en %s\n", playlist_mostrar->nombre_playlist);
    else {
        CANCION* aux2= list_first(playlist_mostrar->canciones_recomendadas);
        while (aux2!=NULL){
            mostrar_info_canciones(aux2);
            aux2= list_next(playlist_mostrar->canciones_recomendadas);
        }
    }
    */
    int num_rec = list_size(playlist_mostrar->canciones_recomendadas);

    CANCION* reco_arr[3] = {NULL,NULL,NULL};
    CANCION* rec= list_first(playlist_mostrar->canciones_recomendadas);

    for(int i=0; (i<num_rec) && (i<3)&&(rec!= NULL) ; i++){ //!!!_----------------------
        //if(rec!=NULL){
        reco_arr[i] = rec;
        rec= list_next(playlist_mostrar->canciones_recomendadas);
        
    }

    for(int i = 0; i<3; i++){
        if(reco_arr[i] != NULL){
            printf("%d) ", i+1);
            mostrar_info_canciones(reco_arr[i]);
        }
        else{
            printf("%d) No quedan más canciones para recomendar! \n",i+1);
        }
    }
    
    if(num_rec>0){
        printf("\nDeseas agregar alguna canción recomendada a tu Playlist?\n");
        printf("1 / 2 / 3) : Agregar la opción\n");
        printf("0) Volver al menú\n");
        printf("Ingrese su opción: ");
        char op_rec;
        scanf(" %c", &op_rec);

        switch(op_rec){
            case '1':
                if (reco_arr[0] != NULL){
                    list_pushBack(playlist_mostrar->canciones, reco_arr[0]);
                    act_generos(playlist_mostrar,reco_arr[0],1);
                    printf("%s se ha agregado con éxito a %s! \n",reco_arr[0]->cancion,playlist_mostrar->nombre_playlist);
                }
                else{
                    puts("No hay una canción recomendada en esta opción");
                }
                break;
            case '2':
                if (reco_arr[1] != NULL){
                    list_pushBack(playlist_mostrar->canciones, reco_arr[1]);
                    act_generos(playlist_mostrar,reco_arr[1],1);
                    printf("%s se ha agregado con éxito a %s! \n",reco_arr[1]->cancion,playlist_mostrar->nombre_playlist);
                }
                else{
                    puts("No hay una canción recomendada en esta opción");
                }
                break;
            case '3':
                if (reco_arr[2] != NULL){
                    list_pushBack(playlist_mostrar->canciones, reco_arr[2]);
                    act_generos(playlist_mostrar,reco_arr[2],1);
                    printf("%s se ha agregado con éxito a %s! \n",reco_arr[2]->cancion,playlist_mostrar->nombre_playlist);
                }
                else{
                    puts("No hay una canción recomendada en esta opción");
                }
                break;
            case '0':
                puts("Volviendo a menú...");
                break;
            
            default:
               puts("Opción no valida");



            
        }
    }
}

// -case 5
void eliminar_PL(Map* mapa_PL){
    //SI EL MAPA DE PLAYLISTS ESTA VACIO, SE DA UN AVISO Y SE RETORNA.
    if (map_first(mapa_PL)==NULL) {
        puts("No se ha creado Ninguna Playlist. Cree al menos una.");
        return;
    }
    //SE MUESTRAN LAS PLAYLISTS GUARDADAS
    mostrar_PL_guardadas(mapa_PL);
    //SE LE PREGUNTA LA PLAYLIST QUE QUIERE ELIMINAR
    puts("¿Qué Playlist desea Eliminar?: ");
    char nombre_p_eliminar[31];
    scanf(" %30[^\n]", nombre_p_eliminar);
    //SE BUSCA EL PAR DE LA PLAYLIST. SI ESTE ES NULO, ES PORQUE NO EXISTE Y SE DA UN AVISO
    MapPair * par_PL = map_search(mapa_PL, nombre_p_eliminar);
    if(par_PL == NULL){
        puts("No Existe una Playlist con ese Nombre.");
        return; 
    }
    //SE PREGUNTA SI REALMENTE QUIERE ELIMINAR LA PLAYLIST
    printf("¿Realmente quiere Eliminar %s? Esta acción no se puede deshacer. (s/n)\n", par_PL->key);
    char opcion;
    do {
        scanf(" %c", &opcion);
        if(opcion == 'n') return;
        else if (opcion == 's') break;
        puts("Ingrese una opción válida.");
    } while (opcion != 'n' || opcion != 's');
    //SE ELIMINA PLAYLIST DEL MAPA DE PLAYLISTS
    MapPair* playlist_eliminar= map_remove(mapa_PL, par_PL->key);
    //SE LIBERA LA MEMORIA RESERVADA PARA LA PLAYLIST Y SE DA UN AVISO
    free(playlist_eliminar->value);
    printf("Se ha Eliminado %s con Éxito.\n", nombre_p_eliminar);
}

void menu_playlists(Map* archivo_c, Map* mapa_PL, Queue* cola_repro, Map* canciones_g){ 
    char opcionP;
    do {
        mostrar_menu_PL();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcionP);
        switch(opcionP){
            case '1':
                crear_PL(mapa_PL, cola_repro);
                break;
            case '2':
                anadir_cancion_PL(archivo_c, mapa_PL);
                break;  
            case '3':
                eliminar_cancion_PL(archivo_c, mapa_PL); 
                break;
            case '4':
                mostrar_PL(mapa_PL, canciones_g);
                break;
            case '5':
                eliminar_PL(mapa_PL); 
                break;
            case '6':
                puts("Volviendo...");
                break;
            default:
                puts("Opción no válida. Intente de nuevo.");
        }
        printf("\n");
        if (opcionP != '6') presioneTeclaParaContinuar();
    } while(opcionP != '6');
}

// FUNCIONALIDADES TOP 3 ARTISTAS

void actualizar_frecuencia_art(Map *artistas, char *nombre_artista){
    //se busca si el artista ya está registrado en el mapa
    MapPair *par_artista = map_search(artistas, nombre_artista);

    //en caso de ya estar dentro del mapa, se aumenta su frecuencia en uno
    if (par_artista != NULL)
    {
        ARTISTA *artista = par_artista->value;
        artista->frecuencia = artista->frecuencia+1;
    }

    //y sino, es la primera vez que se escucha, por lo que se agrega al mapa con su frecuencia en 1
    else
    {
        ARTISTA *artista_nuevo = (ARTISTA*)malloc(sizeof(ARTISTA));
        strcpy(artista_nuevo->nombre, nombre_artista);
        artista_nuevo->frecuencia = 1;
        map_insert(artistas, artista_nuevo->nombre, artista_nuevo);
    }
}

void mostrar_top3(Map *artistas){
    //en caso de que el mapa de artistas esté vacío se manda el mensaje de error
    if (map_first(artistas) == NULL)
    {
        printf("No ha sido posible mostrar el top 3 de artistas.\n");
        return;
    }

    //se crea el Heap vacío para poder ordenar el mapa de artistas por su frecuencia
    Heap *top_artistas = heap_create();

    //se recorre todo el mapa y se ingresan al Heap usando la frecuencia como prioridad
    MapPair *par_artista = map_first(artistas);
    while (par_artista != NULL)
        {
            ARTISTA *artista = (ARTISTA*)par_artista->value;
            heap_push(top_artistas, artista, artista->frecuencia);
            par_artista = map_next(artistas);
        }

    printf("\n==================================\n");
    printf("TOP 3 ARTISTAS MÁS ESCUCHADOS\n");
    printf("==================================\n");


    int top = 1;
    while (top <= 3 && heap_top(top_artistas) != NULL)
        {
            //se obtiene el mejor artista del Heap que sería el de mayor frecuencia
            ARTISTA *mejor_art = (ARTISTA *)heap_top(top_artistas); //casting para evitar errores de memoria
            printf("%d) %s (%d veces)\n", top, mejor_art->nombre, mejor_art->frecuencia);
            //se elimina al primer artista para asi poder obtener el 2° y 3° artista con más escucha
            heap_pop(top_artistas);
            top++;
        }
}

// 5 MI COLA DE REPRODUCCION
void iniciar_cola(Map * canciones, Queue * cola_repro, Map *artistas){
    // Si ya había una cola activa, esta se vacía para iniciar una nueva
    if(queue_front(cola_repro) != NULL) queue_clean(cola_repro); 
    char song_user[150];
    int encontrado = 0;
    int intentos = 0;
    do{
        /* 
        Se pregunta si el usuario quiere continuar añadiendo su canción favorita 
        si es que se equivoca al escribir 3 veces o más
        */
        if(intentos >= 3){
            char opcion;
            puts("Desea continuar? (s/n)");
            scanf("%s", &opcion);
            if(opcion == 's') intentos = 0;
            else return;
        }
        // Se le pide a la usuaria una canción para iniciar la cola
        printf("Ingrese una canción a la cola: ");
        scanf(" %[^\n]", song_user);

        // Se busca la canción dentro del mapa de canciones
        MapPair * song_pair = map_search(canciones, song_user);
        // Si la canción se encuentra dentro del mapa esta se inserta dentro de la cola y se devuelve al menú
        if(song_pair != NULL){
            CANCION *cancion_insertada = (CANCION *)song_pair->value;
            queue_insert(cola_repro, cancion_insertada);
            // Se actualiza la frecuencia de los artistas del mapa de artistas
            actualizar_frecuencia_art(artistas, cancion_insertada->artista);
            encontrado = 1;
            return;
        }
        // Si la canción no se encuentra dentro del archivo, se avisa y se le pregunta nuevamente a la usuaria por la canción a agregar
        else{
            printf("Esa canción no se encuentra en el catálogo.\n");
        }
        // Se le suma uno al contador de intentos
        intentos++;
    } while(encontrado != 1);
}

void anadir_cancion(Map * canciones, Queue * cola_repro, Map *artistas){
    // Si no hay ninguna cola activa se le pregunta al usuario si desea iniciar una
    if(queue_front(cola_repro) == NULL){
        char opcion;
        printf("No existe ninguna cola activa!\n");
        printf("Desea iniciar una? (s/n)\n");
        scanf(" %c", &opcion);
        // Si desea iniciar una, se le redirecciona a la función de iniciar cola
        if(opcion == 's') iniciar_cola(canciones, cola_repro, artistas);
        // Si no desea iniciar una, se le devuelve al menú
        return;
    }

    char song_user[150];
    int encontrado = 0;
    int intentos = 0;
    do{
        /* 
        Se pregunta si el usuario quiere continuar añadiendo su canción favorita 
        si es que se equivoca al escribir 3 veces o más
        */
        if(intentos >= 3){
            char opcion;
            puts("Desea continuar? (s/n)");
            scanf("%s", &opcion);
            if(opcion == 's') intentos = 0;
            else return;
        }
        
        // Se le pregunta a la usuaria que canción desea añadir a la cola
        printf("Ingrese una nueva canción a la cola :");
        scanf(" %[^\n]", song_user);
        // Se busca la canción en el mapa de canciones
        MapPair * song_pair = map_search(canciones, song_user);
        // Si la canción se encuentra en el mapa de canciones se inserta en la cola y se devuelve al menú
        if(song_pair != NULL){
            CANCION *cancion_insertada = (CANCION *)song_pair->value;
            queue_insert(cola_repro, cancion_insertada);
            // Se actualiza la frecuencia del artista en el mapa de artistas
            actualizar_frecuencia_art(artistas, cancion_insertada->artista);
            encontrado = 1;
            return;
        }
        // Si no se encuentra, se avisa y se le vuelve a preguntar a la usuaria por la canción a agregar
        else{
            printf("Esa canción no se encuentra en el catálogo.\n");
        }
        // Se le suma uno al contador de intentos
        intentos++;
    } while(encontrado != 1);
}

void eliminar_cancion(Queue * cola_repro){
    // Si no hay ninguna cola activa, se avisa y se devuelve al menú
    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    char song_user[150];
    // Se le pregunta a la usuaria que canción de su cola desea eliminar
    printf("Ingrese el nombre de la canción a eliminar: ");
    scanf(" %[^\n]", song_user);

    // Se recorre la cola en busca de la canción
    CANCION * song_aux = queue_front(cola_repro);
    while(song_aux != NULL){
        // Si la canción se encuentra dentro de la cola, se elimina y se imprime por pantalla un mensaje indicandolo
        if(strcmp(song_aux -> cancion, song_user) == 0){
            CANCION* deleted_song = queue_removeCurrent(cola_repro);
            printf("Canción eliminada correctamente!\n");
            return;
        }
        song_aux = queue_next(cola_repro);
    }
    // Si la canción no se encuentra dentro de la cola, se le avisa por pantalla a la usuaria
    printf("Esta canción no se encuentra en tu cola!\n");
}

void mostrar_cola(Queue * cola_repro){
    CANCION * song_aux = queue_front(cola_repro);
    // Si no hay ninguna cola activa, se avisa y se devuelve al menú
    if(song_aux == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    // Se recorre toda la cola mostrando la información de cada una de sus canciones
    while(song_aux != NULL){
        mostrar_info_canciones(song_aux);
        song_aux = queue_next(cola_repro);
    }
}

void sgte_cancion(Queue * cola_repro){
    // Si no hay ninguna cola activa, se avisa y se devuelve al menú
    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }
    // Se quita la primera canción de la cola
    CANCION * deleted_song = queue_remove(cola_repro);
    
    // Si hay una canción dentro de la cola aún se imprime por pantalla la información de esta
    CANCION * next_song = queue_front(cola_repro);
    if(next_song != NULL){
        printf("Ahora está sonando: \n");
        mostrar_info_canciones(next_song);
    }
    // Si la cola queda vacía se avisa y se devuelve al menú
    else printf("Se ha terminado la cola!\n");
}


void eliminar_cola(Queue* cola_repro){
    // Si no hay ninguna cola activa, se avisa y se devuelve al menú
    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }
    // Se limpia la cola
    queue_clean(cola_repro);
    if(queue_front != NULL){
        // Si la cola falla al ser vaciada, se avisa y se devuelve al menú
        printf("Error al vaciar la cola, intentelo nuevamente\n");
        return;
    }
    // Si no falla se indica que se ha eliminado correctamente
    printf("Se ha eliminado correctamente la cola! \n");
}

void menu_cola(Map * canciones, Queue * cola_repro, Map *artistas){
    char opcion;

    do{
        mostrar_menu_cola();

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            iniciar_cola(canciones, cola_repro, artistas);
            break;
        case '2':
            anadir_cancion(canciones, cola_repro, artistas);
            break;
        case '3':
            eliminar_cancion(cola_repro);
            break;
        case '4':
            mostrar_cola(cola_repro);
            break;
        case '5':
            sgte_cancion(cola_repro);
            break;
        case '6':
            eliminar_cola(cola_repro);
            break;
        case '7':
            puts("Saliendo del menú...");
            break;
        default:
            puts("Opción no válida. Por favor intente nuevamente.");

        }
        printf("\n");
        if(opcion != '7') presioneTeclaParaContinuar();
    } while(opcion != '7');
}

// 6 TOP 3 ARTISTAS

//esta función es para cargar los artistas guardados en sesiones anteriores
void cargar_artistas(Map *artistas){
    //se intenta abrir el archivo en modo lectura
    FILE *archivo = fopen("artistas.bin", "rb");
    //si no hay archivo, se crea uno nuevo y se retorna
    if (archivo == NULL)
    {
        FILE *archivo = fopen("artistas.bin", "wb");
        fclose(archivo);
        return;
    }

    //si el archivo existe, se lee un registro cada artista
    ARTISTA art_leido;
    while(fread(&art_leido, sizeof(ARTISTA), 1, archivo) == 1)
        {
            //se reserva memoria para cada artista y así podemos agregarlos al mapa con los datos leídos desde el archivo
            ARTISTA *art = (ARTISTA*)malloc(sizeof(ARTISTA));
            //se copian los datos del artista leído desde el archivo al artista que se va a ingresar al mapa
            strcpy(art->nombre, art_leido.nombre);
            art->frecuencia = art_leido.frecuencia;
            map_insert(artistas, art->nombre, art);
        }
    fclose(archivo);
}

//esta función es para guardar todo el contenido actual del mapa artistas, sobreescribiendo el archivo
//o creando uno nuevo en el caso de que no exista el archivo
void guardar_artistas(Map *artistas){
    //se abre el archivo en modo escritura
    FILE *archivo = fopen("artistas.bin", "wb");
    //si no existe el archivo, no guardamos nada y se retorna
    if (archivo == NULL) return;

    //se recorre el mapa de artistas
    MapPair *par_art = map_first(artistas);
    while (par_art != NULL)
        {
            //por cada artista actual del mapa se reescriben sus datos en el archivo
            ARTISTA *art = (ARTISTA*)par_art->value;
            fwrite(art, sizeof(ARTISTA), 1, archivo);
            par_art = map_next(artistas);
        }
    fclose(archivo);
}

void guardar_playlist(Map * mapa_PL){
    MapPair * pl_pair = map_first(mapa_PL);
    int contador = 1;
    while(pl_pair != NULL){
        char nombre_archivo[50];
        char nombre_pl[50];
        PLAYLIST * pl_aux = pl_pair -> value;
        sprintf(nombre_archivo, "playlist/playlist_%d.bin", contador);
        sprintf(nombre_pl, "playlist/nombre_playlist_%d.txt", contador);
        FILE * archivo_nombre = fopen(nombre_pl, "w");
        if(archivo_nombre != NULL){
            fprintf(archivo_nombre, "%s\n", pl_aux -> nombre_playlist);
            fclose(archivo_nombre);
        }
        FILE * archivo = fopen(nombre_archivo, "wb");
        if(archivo != NULL){
            List * songs_pl = pl_aux -> canciones;
            CANCION * song = (CANCION *)list_first(songs_pl);
            while(song != NULL){
                fwrite(song, sizeof(CANCION), 1, archivo);
                song = list_next(songs_pl);
            }
            contador ++;
            fclose(archivo);
        }
        pl_pair = map_next(mapa_PL);
    }
}

// MAIN
int main()
{
    char opcion;

    Map * mapa_PL = map_create(is_equal_str); // Se inicia el mapa de playlist
    leer_playlists(mapa_PL); // Se leen los archivos de las playlists
    Map * canciones = map_create(is_equal_str); // Se inicia el mapa de canciones
    Map * canciones_g = map_create(is_equal_str); // Se incia el mapa de generos de canciones
    CANCION * cancion_fav = leer_cancion_fav(); // Se lee el archivo de canción favorita
    Queue * cola_repro = queue_create(cola_repro); // Se inicia la cola de reproducción
    Map *mapa_artistas = map_create(is_equal_str); // Se inicia el mapa de artistas
    cargar_canciones(canciones, canciones_g); // Se cargan las canciones del archivo csv
    cargar_artistas(mapa_artistas); // Se cargan los artistas más escuchados desde el archivo
    printf("Archivo Cargado!\n"); // Se imprime por pantalla un aviso de archivo cargado
    presioneTeclaParaContinuar();

    do{
        mostrar_menu_principal(cancion_fav, queue_front(cola_repro));
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            ver_archivo(canciones);
            break;
        case '2':
            mostrar_top3(mapa_artistas);
            break;
        case '3':
            cancion_favorita(canciones, &cancion_fav);
            break;
        case '4':
            menu_playlists(canciones, mapa_PL, cola_repro, canciones_g);
            break;
        case '5':
            menu_cola(canciones, cola_repro, mapa_artistas);
            break;
        case '6':
            guardar_artistas(mapa_artistas); // Se guardan en un archivo el top 3 artistas de la usuaria
            guardar_playlist(mapa_PL); // Se guardan en un archivo las playlists creadas por la usuaria
            puts("Saliendo del programa.....");
            break;
        default:
            puts("Opción no válida. Por favor, intente nuevamente.");
        }
        presioneTeclaParaContinuar();
    } while(opcion != '6');
    return 0;
}