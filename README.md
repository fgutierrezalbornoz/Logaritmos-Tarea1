# Tarea 1 - Diseño y Análisis de Algoritmos

Integrantes: Cristóbal Stamm P, Dante Cárcamo A, Francisco Gutiérrez A

## Descripción

Este proyecto implementa un algoritmo de inserción en hashing lineal que distribuye elementos en páginas de memoria dinámica, permitiendo la expansión de la tabla hash cuando se supera un costo promedio de búsqueda máximo predefinido. El objetivo es evaluar el comportamiento de  hashing lineal al insertar elementos variando el costo máximo promedio permitido y medir el rendimiento en términos de I/O, costo real y porcentaje de llenado de las páginas.

## Dependencias

- La implementación del código fue realizada en C++ y compilada en GNU C++.
- Biblioteca estándar C++

El código hace uso de las siguientes bibliotecas:

    <iostream>: Para la entrada y salida de datos.
    <vector>: Para la manipulación de vectores que almacenan las páginas.
    <math.h>: Para operaciones matemáticas.
    <cstdlib>: Para la generación de números aleatorios.
    <fstream>: Para la escritura de archivos CSV.

## Compilación

Puedes compilar el código utilizando un compilador como g++ de la siguiente forma:

```
g++ -o t1 t1.cpp
```

## Ejecución

Una vez compilado, ejecuta el programa:

```
./t1
```

Esto generará archivos CSV con los resultados de los experimentos. 

El código inserta 2^24 elementos en la tabla hash para diferentes valores de max_cost. Se crea un archivo CSV para cada valor de max_cost, que almacena el Costo real promedio de I/Os, el Número total de I/Os y el Porcentaje de llenado de las páginas para 2^10, 2^11, 2^12, . . . , 2^24 inserciones.

## Descripción del código:

### 1. Constantes, variables y Funciones útiles

*Constantes:*

- MAX_SIZE: Tamaño máximo de una página en bytes (1024 bytes).
- ELE_SIZE: Tamaño de un elemento long long (8 bytes).
- PAGE_SIZE: Número máximo de elementos por página (128 elementos).

*Variables:*

- max_cost: Costo promedio de búsqueda máximo permitido. Si el costo promedio lo excede, la tabla se expande.

*Funciones:*

- h(long long y): función hash devuelve un valor aleatorio entre 0 y 2^64 - 1.


### 2. Estructuras de datos

#### 2.1. HashTable

La clase HashTable representa el almacenamiento de datos en la tabla hash. Inicializa la tabla con un número de páginas (num_pages) e invoca el método fill_page_index() para asignar índices a las páginas.

*Atributos:*

- p: Número de páginas actuales.
- t: Valor usado para controlar el crecimiento de la tabla (expansión).
- pages: Un vector de objetos Page que representa la tabla hash.
- total_io_cost: Acumula el costo total real de las operaciones I/Os.

*Métodos:*
- fill_page_index(): Asigna a cada página su respectivo índice en la tabla.

- getPage(int index): Retorna la página en la posición index y aumenta el costo de I/O en 1 por la operación de lectura.

- getNumPages(): Devuelve el número de páginas actuales.

- gett(): Devuelve el valor de t (relacionado con la expansión de la tabla).

- getTotalIOCost(): Devuelve el costo total de las operaciones de I/O.

- printHT(): Imprime el contenido de todas las páginas de la tabla.

- expand(): Expande la tabla hash agregando una nueva página. Actualiza el valor de t si el número de páginas es una potencia de 2.

- insert(long long x): Inserta el elemento x en la tabla hash. Si el costo promedio de búsqueda excede el límite, expande la tabla y redistribuye los elementos.

- searchMeanCost(): Calcula el costo promedio de búsqueda sumando los cost() de todas las páginas.

- redistribute(): Redistribuye los elementos cuando se añade una nueva página, verificando si algunos elementos deben moverse a la nueva página.

- insertInPage(long long x, Page& P): Inserta el elemento x en la página P. Si la página está llena, crea o utiliza una página enlazada.

- averageFillPercentage(): Calcula el porcentaje promedio de llenado de las páginas en la tabla hash.

- clean(): Limpia la tabla hash, reseteando las páginas y eliminando las páginas enlazadas.


#### 2.2. Page

La clase Page (Página) representa una página en la tabla hash. Almacena elementos y puede enlazarse con otras páginas cuando se producen colisiones (rebalses). Se nicializa la página con ceros y el puntero linkedPage en nullptr.

*Atributos:*
- page: Un arreglo de tamaño 128, donde se almacenan los valores.
- last_pos: Indica la posición del próximo lugar disponible en la página.
- page_index: Índice de la página en la tabla hash.
- linkedPage: Puntero a otra página en caso de que la actual esté llena.

*Métodos:*
- print(): Imprime los elementos de la página actual y cualquier página enlazada (si existe).

- getLinkedPage(): Retorna el puntero a la página enlazada.

- setLinkedPage(int p_index): Crea y enlaza una nueva página.

- initialize(int n): Inicializa una página con su índice (page_index) y resetea sus valores.

- cost(): Calcula el costo de la búsqueda, teniendo en cuenta las páginas enlazadas. Si hay una página enlazada, suma 1 al costo por cada página adicional.