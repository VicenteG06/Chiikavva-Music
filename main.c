#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/queue.h"
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"


typedef struct {
    char cancion[150];
    char artista[100];
    char genero[100];
    int anyo;
    int duracion;
} CANCION;

typedef struct {
    char nombre[30];
    int frecuencia;
} ARTISTA; 

typedef struct {
    char nombre_playlist[30];
    List * canciones;
    List * canciones_recomendadas;
    Map * generos_p;
    char genero_top[30];
    int frec_genero_top;
} PLAYLIST;

// EN ESTA FUNCIÓN ME AYUDO GEMINI 
CANCION * leer_cancion_fav(){
    CANCION fav_song;
    CANCION * cancion_fav = (CANCION *) malloc(sizeof(CANCION));
    FILE* archivo = fopen("cancion_fav.bin", "rb");
    if(archivo == NULL){
        FILE* archivo = fopen("cancion_fav.bin", "wb");
        fclose(archivo);
    }
    else{
        int leidos = fread(&fav_song, sizeof(CANCION), 1, archivo);
        if(leidos == 1){
            strcpy(cancion_fav -> cancion, fav_song.cancion);
            strcpy(cancion_fav -> artista, fav_song.cancion);
            strcpy(cancion_fav -> genero, fav_song.genero);
            cancion_fav -> anyo = fav_song.anyo;
            cancion_fav -> duracion = fav_song.duracion;
            fclose(archivo);
            return cancion_fav;
        }
    }
    fclose(archivo);
    free(cancion_fav);
    return NULL;
}

void mostrar_menu_principal(CANCION * cancion_fav, CANCION * song){
    limpiarPantalla();
    puts("========================================");
    puts("     Chiikavva Playist!");
    puts("========================================");
    puts("1) Ver Canciones Cargadas");
    puts("2) Artistas más escuchados");
    puts("3) Ingresar Canción Favorita");
    puts("4) Mis Playlist");
    puts("5) Mi Cola de Reproducción");
    puts("6) Salir");
    if(cancion_fav != NULL) printf("Tu canción favorita :D : %s\n", cancion_fav -> cancion);
    else printf("No has ingresado tu canción favorita! D:\n");
    if(song != NULL) printf("Sonando: %s \n", song -> cancion);
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

void mostrar_info_canciones(CANCION * song){
    printf("\n");
    printf("|| TÍTULO: %s | ARTISTA: %s | GÉNERO: %s | AÑO PUBLICACIÓN: %d | DURACIÓN: %d",
        song -> cancion, song -> artista, song -> genero, song -> anyo, song -> duracion);
    printf("\n");
}

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

void cargar_canciones(Map * canciones, Map * canciones_g){
    FILE * archivo = fopen("data/Spotify Dataset.csv", "r");
    if(archivo == NULL){
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;

    campos = leer_linea_csv(archivo, ',');

    while((campos = leer_linea_csv(archivo, ',')) != NULL){
        CANCION * song = (CANCION *) malloc(sizeof(CANCION));
        strcpy(song -> cancion, campos[0]);
        strcpy(song -> artista, campos[1]);
        strcpy(song -> genero, campos[2]);
        song -> anyo = atoi(campos[3]);
        song -> duracion = atoi(campos[21]);

        map_insert(canciones, song -> cancion, song);

        MapPair * genre_pair = map_search(canciones_g, song -> genero);
        if(genre_pair == NULL){
            List * nueva_lista = list_create();
            list_pushBack(nueva_lista, song);
            map_insert(canciones_g, song -> genero, nueva_lista);
        }
        else{
            List * lista_g = (List *) genre_pair -> value;
            list_pushBack(lista_g, song);
        }
    }
}

// EN ESTA IGUAL ME AYUDO GEMINI
void ver_archivo(Map * canciones){
    MapPair * song_pair = map_first(canciones);
    if(song_pair == NULL){
        printf("Error al cargar el archivo. Por favor, intente nuevamente");
        return;
    }
    while(song_pair != NULL){
        mostrar_info_canciones(song_pair -> value);
        song_pair = map_next(canciones);
    }
}

void guardar_cancion_fav(CANCION  fav_song){
    FILE * archivo = fopen("cancion_fav.bin", "wb");
    if(archivo != NULL){
        fwrite(&fav_song, sizeof(CANCION), 1, archivo);
    }
    fclose(archivo);
}

void cancion_favorita(Map * canciones, CANCION ** cancion_fav){
    char fav_song[150];
    int encontrado = 0;

    do{
        printf("Ingrese su canción favorita: \n");
        scanf(" %[^\n]", fav_song);

        MapPair * song_pair = map_search(canciones, fav_song);
        if(song_pair != NULL){
            *cancion_fav = song_pair -> value;
            guardar_cancion_fav(**cancion_fav);
            encontrado = 1;
            return;
        }
        else{
            printf("Esa canción no se encuentra en el catálogo");
        }
    } while(encontrado != 1);
}

void iniciar_cola(Map * canciones, Queue * cola_repro){
    if(queue_front(cola_repro) != NULL) queue_clean(cola_repro); 
    char song_user[150];
    int encontrado = 0;
    do{
        printf("Ingrese una canción a la cola :");
        scanf(" %[^\n]", song_user);

        MapPair * song_pair = map_search(canciones, song_user);
        if(song_pair != NULL){
            queue_insert(cola_repro, song_pair -> value);
            encontrado = 1;
            return;
        }
        else{
            printf("Esa canción no se encuentra en el catálogo.\n");
        }
    } while(encontrado != 1);
}

void anadir_cancion(Map * canciones, Queue * cola_repro){

    if(queue_front(cola_repro) == NULL){
        char opcion;
        printf("No existe ninguna cola activa!\n");
        printf("Desea iniciar una? (s/n)\n");
        scanf(" %c", &opcion);
        if(opcion == 's') iniciar_cola(canciones, cola_repro);
        return;
    }

    char song_user[150];
    int encontrado = 0;
    do{
        printf("Ingrese una nueva canción a la cola :");
        scanf(" %[^\n]", song_user);

        MapPair * song_pair = map_search(canciones, song_user);
        if(song_pair != NULL){
            queue_insert(cola_repro, song_pair -> value);
            encontrado = 1;
            return;
        }
        else{
            printf("Esa canción no se encuentra en el catálogo.\n");
        }
    } while(encontrado != 1);
}

void eliminar_cancion(Queue * cola_repro){

    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    char song_user[150];

    printf("Ingrese el nombre de la canción a eliminar: ");
    scanf(" %[^\n]", song_user);
    
    CANCION * song_aux = queue_front(cola_repro);
    while(song_aux != NULL){
        if(strcmp(song_aux -> cancion, song_user) == 0){
            CANCION* deleted_song = queue_removeCurrent(cola_repro);
            printf("Canción eliminada correctamente!\n");
            return;
        }
        song_aux = queue_next(cola_repro);
    }
    printf("Esta canción no se encuentra en tu cola!\n");
}

void mostrar_cola(Queue * cola_repro){
    CANCION * song_aux = queue_front(cola_repro);

    if(song_aux == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    while(song_aux != NULL){
        mostrar_info_canciones(song_aux);
        song_aux = queue_next(cola_repro);
    }
}

void sgte_cancion(Queue * cola_repro){
    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    CANCION * deleted_song = queue_remove(cola_repro);

    CANCION * next_song = queue_front(cola_repro);
    if(next_song != NULL){
        printf("Ahora está sonando: \n");
        mostrar_info_canciones(next_song);
    }
    else printf("Se ha terminado la cola!\n");
}

void eliminar_cola(Queue* cola_repro){
    if(queue_front(cola_repro) == NULL){
        printf("No hay ninguna cola activa!\n");
        return;
    }

    queue_clean(cola_repro);
    printf("Se ha eliminado correctamente la cola! \n");
}

void menu_cola(Map * canciones, Queue * cola_repro){
    char opcion;

    do{
        mostrar_menu_cola();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            iniciar_cola(canciones, cola_repro);
            break;
        case '2':
            anadir_cancion(canciones, cola_repro);
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
            puts("Saliendo del menú.....");
            break;
        default:
            puts("Opción no válida. Por favor intente nuevamente.");

        }
        presioneTeclaParaContinuar();
    } while(opcion != '7');
}

int main()
{
    char opcion;

    Map * canciones = map_create(is_equal_str);
    Map * canciones_g = map_create(is_equal_str);
    CANCION * cancion_fav = leer_cancion_fav();
    Queue * cola_repro = queue_create(cola_repro);
    cargar_canciones(canciones, canciones_g);
    printf("Archivo Cargado!\n");
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
            break;
        case '3':
            cancion_favorita(canciones, &cancion_fav);
            break;
        case '4':
            break;
        case '5':
            menu_cola(canciones, cola_repro);
            break;
        case '6':
            puts("Saliendo del programa.....");
            break;
        default:
            puts("Opción no válida. Por favor, intente nuevamente.");
        }
        presioneTeclaParaContinuar();
    } while(opcion != '6');
    return 0;
}
