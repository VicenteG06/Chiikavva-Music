# 𐙚₊Ｃｈｉｉｋａｗａ⋆Ｍｕｓｉｃ˚⊹♡

## Descripción 

Nuestro proyecto CHIIKAWA MUSIC permite organizar música en Playlists y colas de reproducción, ver tu Top de Artistas Más Escuchados, y expandir tu gusto musical con un Algoritmo de Recomendación, con un menú personalizado que incluye tu canción favorita y la canción que estes escuchando.

## Cómo compilar y ejecutar 

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente a través de repl.it. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:
- Tener cuenta en replit.com (https://replit.com/).

### Pasos para compilar y ejecutar: 
- Descarga el archivo `.zip` en una carpeta de tu elección. 
- Importar archivo a Replit.
  - Inicia sesión con tu cuenta en replit.com.
  - En la parte izquierda haz click en la opción `Import code or design` y haz click en la opción `Zip file`.
  - Haz click en `Choose zip file or drag here` y elige el archivo desde su carpeta o arrastra el archivo desde su carpeta hasta encima de esa área.
  - Haz click en `Import from zip`.
- Compila el código y ejecuta el programa
  - En un apartado `Shell` compila el siguiente comando :
  ````
  gcc tdas/*.c main.c -Wno-unused-result -o main
  ````
  - Para ejecutar el programa, usa el siguiente comando:
  ````
  ./main
  ````

## Funcionalidades

### Funcionando correctamente:
-  Cargar toda la información (catálogo de canciones) en memoria para su uso inmediato.
  
-  Crear una Playlist.
-  Crear Playlist a partir de Cola de Reproducción
-  Agregar o eliminar canciones a la Playlist.
-  Mostrar Playlist.
-  Algoritmo de Recomendación.
-  Eliminar Playlist.

-  Iniciar una Cola de Reproducción.
-  Agregar o eliminar canciones a la Cola de Reproducción.
-  Avanzar a la Siguiente Canción.
-  Mostrar Cola de Reproducción.
-  Mostrar Canción sonando por pantalla en el Menú Principal.
-  Eliminar Cola de Reproducción.

-  Agregar canción favorita.

-  Top 3 Artistas.

-  Guardado de Playlists, Top de Artistas y Canción favorita en archivos.

### Problemas conocidos: 
- No existe una validación de entrada (input) al momento de ingresar texto, por lo que se deben ingresar como máximo la cantidad de caracteres que se pide. Y si es que se piden números o letras se *DEBE* ingresar el tipo correspondiente.
- Los nombres de las canciones y playlists son únicos, por lo que al querer agregar o eliminar alguna de estas, el nombre debe ser escrito tal como se registró en el archivo, de lo contrario el sistema no lo reconocerá (“HOLA” no es lo mismo que “hola”).
- Cuando aparece el mensaje "*Presione una tecla para continuar...*", la única opción válida que permite continuar es *Enter*.

### A mejorar:
- Implementar una validación de entrada que limite el largo del texto y que valide si es que el tipo de dato ingresado (número, texto) corresponde al solicitado.
- Implementar una función que permita normalizar el texto al hacer la búsqueda para que las entradas no sean sensibles a mayúsculas.

## Ejemplo de uso 

### Paso 1: Crear Cola de Reproducción
Se desea crear una cola de reproducción.  
````
Opción seleccionada: 5) Mi Cola de Reproducción 
````
Se muestra el menú de Mi Cola de Reproducción.
````
Opción seleccionada: 1) Iniciar Cola de Reproducción
Ingrese una canción a la cola : Closer
````
Esta canción ahora se mostrará como "Sonando" en el menú principal.

### Paso 2: Mi Cola de Reproducción -> Añadir canciones a la Cola de Reproducción
Desde el menú de 5) Mi Cola de Reproducción.
````
Opción seleccionada: 2) Añadir Canción a la Cola
Ingrese una canción a la cola : Something Just Like This
````
````
Opción seleccionada: 2) Añadir Canción a la Cola
Ingrese una canción a la cola : Roses
````
````
Opción seleccionada: 2) Añadir Canción a la Cola
Ingrese una canción a la cola : Problem
````
````
Opción seleccionada: 2) Añadir Canción a la Cola
Ingrese una canción a la cola : Grenade
````
Estas canciones se añaden a la Cola de Reproducción.

### Paso 3: Mi Cola de Reproducción -> Mostrar Cola de Reproducción
Desde el menú de 5) Mi Cola de Reproducción.
````
Opción seleccionada: 4) Mostrar Cola de Reproducción.
````
Se mostrará el Título, Artista, Genero, Año y Duración de cada una de las canciones añadidas.

### Paso 4: Mi Cola de Reproducción -> Eliminar Cancion de la Cola de Reproducción
Desde el menú de 5) Mi Cola de Reproducción.
````
Opción seleccionada: 3) Eliminar Canción de Cola
Ingrese el nombre de la canción a eliminar: Something Just Like This
````
Se eliminará la canción de la Cola de Reproducción.

### Paso 5: Mi Cola de Reproducción -> Avanzar a la Siguiente Canción
Desde el menú de 5) Mi Cola de Reproducción.
````
Opción seleccionada: 5) Siguiente Canción
````
Ahora en el Menú Principal se mostrará como "Sonando" la siguiente canción de la Cola de Reproducción.

### Paso 6: Crear una Playlist a partir de una Cola de Reproducción
Para crear una playlist a partir de una Cola de reproducción, primero deberemos ir al menú de Mis Playlists
````
Opción seleccionada: 4) Mis Playlists
````
Se abrirá el Menú Mis Playlists
Desde el Menú Mis Playlists
````
Opción seleccionada: 1) Crear Playlist
¿Desea crear una Playlist a partir de su Cola de Reproducción? (s/n): s
Ingrese un Nombre para su Playlist (máx 30 caracteres): Chiikawa Music
````
Se habrá creado una Playlist conteniendo todas las canciones de la Cola de Reproducción, agregadas anteriormente.

### Paso 7: Mis Playlist -> Mostrar Playlist
Desde el menú 4) Mis Playlist
````
Opción seleccionada: 4) Mostrar Playlist
Playlists Guardadas: 
Nombre Playlist: Chiikawa Music
¿Qué Playlist desea Ver?: Chiikawa Music

````
Se mostrarán por pantalla todas las canciones que fueron agregadas a la Playlist

### Paso 8: Mis Playlist -> Mostrar Playlists -> Agregar Canciones Recomendadas
Al mostrar una playlists, se mostrarán 3 canciones recomendadas en base al género mas frecuente de la Playlist, que no hayan sido ya agregadas a esta.
Se le da a la usuaria la opción de añadir una de las 3 canciones recomendadas a la Playlist, la cual se puede elegir ingresando el número correspondiente a la canción que se desee agregar (1 / 2 / 3).
````
Deseas agregar alguna canción recomendada a tu Playlist? 
2) Agregar la Opción
Opción 2 se ha agregado con éxito a Chiikawa Music!
````
Se agregará la Canción Recomendada mostrada en la posición número 2.

### Paso 9: Mis Playlist -> Eliminar Canción de Playlist
Desde el menú 4) Mis Playlist.
````
Opción seleccionada: 3) Eliminar canción de Playlist
````
Se mostrarán todas las Playlists creadas y la usuaria deberá ingresar el nombre de la Playlist que desee modificar
````
Playlists Guardadas: 
Nombre Playlist: Chiikawa Music
````
````
¿Qué Playlist desea Modificar?
Chiikawa Music
````
Se mostrarán por pantalla todas las canciones guardadas en la Playlist escogida por la usuaria
````
¿Qué canción desea eliminar?: 
Grenade
¡Canción eliminada correctamente!
````
La canción se habrá eliminado de la Playlist
### Paso 10: Ingresar canción favorita 
Desde el 6) Volver al menú principal
````
Opción seleccionada: 3) Ingresar Canción Favorita
Ingrese su canción favorita: A L I E N S
````
Ahora en el menú principal, se mostrará la canción favorita de la usuaria.
### Paso 1: Top 3 Artistas
Desde el menú principal
````
Opción seleccionada: 2) Artistas más escuchados
````
Se mostrarán el top 3 de artistas más escuchados, junto con la frecuencia de cada uno.

## Contribuciones

### Vicente Gamboa
-  Menú de Mi cola de reproducción.
-  Implementación de funciones de guardado de datos.

### Alexia Gallardo
- Menú de Playlists y todas sus subfunciones.

## Viviana Navarrete 
- Top 3 Artistas más escuchados y todas sus subfunciones.

## Antonia Avello
- Implementación de algoritmo de recomendación.
- Desarrollo de archivo README.
