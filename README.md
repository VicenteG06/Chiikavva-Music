# Base de Datos de Películas (StreamFinder)

## Descripción 

Esta herramienta permite gestionar y explorar un catálogo masivo de películas y series de manera eficiente. La aplicación permitirá cargar datos externos, realizar búsquedas complejas, calificaciones por usuario y gestionar una lista de favoritos personalizada.

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
-  Cargar toda la información (catálogo de contenido) en memoria para su uso inmediato (desde un archivo CSV ingresado por la usuaria).
-  Búsqueda por id, director, género, década y de doble criterio, siendo estas las dos últimas mencionadas.
-  Gestión de "Watchlist" (ver más tarde/por ver), en la que se puede: agregar, retirar una película y mostrar las ingresadas al Watchlist.
-  Calificación por usuario.

### Problemas conocidos: 
- La usuaria *DEBE* cargar primero las películas (opción 1) para que la aplicación funcione.
- No existe una validación de entrada (input) al momento de ingresar texto, por lo que se deben ingresar como máximo la cantidad de caracteres que se pide. Y si es que se piden números o letras se *DEBE* ingresar el tipo correspondiente.
- Los nombres de los géneros, id y directores son únicos por lo que al querer ingresar, retirar o buscar cualquiera de esos datos, este debe ser escrito tal como se registró en el archivo, de lo contrario el sistema no los reconoce (“HOLA” no es lo mismo que “hola”).
- Cuando aparece el mensaje "*Presione una tecla para continuar...*", la única opción válida que permite continuar es *Enter*.

### A mejorar:
- Implementar una validación de entrada que limite el largo del texto y que valide si es que el tipo de dato ingresado (número, texto) corresponde al solicitado. 

## Ejemplo de uso 

### Paso 1: Cargar Películas
Este paso es *necesario* para que las otras opciones funcionen.
````
Opción seleccionada: 1) Cargar Películas
````
El sistema carga el catálogo de películas para su uso inmediato y se muestran TODAS las películas del archivo junto con su información completa.

### Paso 2: Buscar por década
Se quiere buscar todas las películas que hayan sido estrenadas en cierta década.
````
Opción seleccionada: 5) Buscar por Década
Ingrese la decada de la película: 1942
````
Se muestran TODAS las películas del archivo que hayan sido estrenadas desde 1940 a 1949 y su información completa.

### Paso 3: Buscar por id
Se quiere buscar una película específica por su id.
````
Opción seleccionada: 2) Buscar por id
Ingrese el id de la película: tt0040982
````
Se muestra el ID, título, año, rating en IMDb, sus géneros, directores y reseñas ingresadas por usuario (reviews).

### Paso 4: Buscar por género
Se quiere buscar todas las películas que pertenecen a cierto género.
````
Opción seleccionada: 3) Buscar por Género
Ingrese el género de la película: Drama
````
Se muestran TODAS las películas del archivo que pertenecen al género DRAMA y su información completa.

### Paso 5: Buscar por director
Se quiere buscar todas las películas que hayan sido dirigidas por cierto/a director/a.
````
Opción seleccionada: 4) Buscar por Director
Ingrese el/la director/a de la película: Tim Burton
````
Se muestran TODAS las películas del archivo que hayan sido dirigidas por Tim Burton y su información completa.

### Paso 6: Agregar reseña
Se quiere agregar una reseña a cierta película
````
Opción seleccionada: 8) Agregar Reseña
Ingrese el ID de la Película: tt0040982
Ingrese su usuario: Javiera
Ingrese su calificación de la película (0 - 10): 8
````
Ahora la película de ID tt0040982 tiene una reseña de 8 a nombre de Javiera.

### Paso 7: Mi watchlist -> agregar película
Se quiere agregar dos películas por su ID a la watchlist.
````
Opción seleccionada: 7) Mi Watchlist
Opción seleccionada: 1) Agregar Película
Ingrese la ID de la Película a Añadir: tt0040982
````
Ahora la película de ID tt0040982 se encuentra dentro del watchlist de la usuaria.
````
Opción seleccionada: 1) Agregar Película
Ingrese la ID de la Película a Añadir: tt0018756
````
Ahora la película de ID tt0018756 se encuentra dentro del watchlist de la usuaria.

### Paso 8: Mi watchlist -> mostrar películas en watchlist
Se quieren ver todas las películas guardadas en watchlist.
````
Opción seleccionada: 3) Mostrar Watchlist
````
Se muestra TODA la información de las películas de ID tt0040982 y tt0018756.

### Paso 9: Mi watchlist -> eliminar película
Se quiere eliminar una película de watchlist.
````
Opción seleccionada: 2) Eliminar Película
Ingrese la ID de la Película a Eliminar: tt0040982
````
Ahora dentro del watchlist sólo se encuentra la película de ID tt0018756.

### Paso 10: Búsqueda avanzada (por género y década)
Se quiere buscar las películas que pertenezcan a cierto género y sean de cierta década.
````
Opción seleccionada: 6) Buscar por Género y Década
Ingrese el género de la pelicula: Drama
Ingrese la decada de la película: 1942
````
Se muestran TODAS las películas que pertenezcan al género de Drama y hayan sido estrenados entre 1940 y 1949.

## Contribuciones

### Vicente Gamboa
-  Documentación y comentarios en el código.
-  Implementación de funciones de carga de películas (y sus subfunciones relacionadas a la organización de los datos del archivo para su uso inmediato), buscar por década, búsqueda avanzada (género y década), agregar reseña por usuario.
-  Revisión y corrección final.
-  Auto-evaluación: 3 (Aporte excelente).

### Alexia Gallardo
- Documentación y comentarios en el código.
- Diseño y redacción del README.
- Implementación de funciones de watchlist (menú, agregar, eliminar, mostrar), mostrar información de película, búsqueda por género y por director.
- Auto-evaluación: 3 (Aporte excelente).
