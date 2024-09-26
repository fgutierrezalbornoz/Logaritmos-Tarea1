# Logaritmos-Tarea1

Calendario:
 - 19 de Septiembre: decir cómo vamos (ver si pudimos comenzar bien)
 - 25 de Septiembre: el código debe estar listo.
 - Experimentos hasta el 1 de octubre.
 - Intentar coordinar reunión para sábado 21.
 - 2 días para el informe


# Cómo funciona

en el main se detalla que comienza con una página la tabla de hashing.
Se inicializa al hacer 
`HashTable hT(num_pages);`
hT contiene:
- `p`: número de páginas.
- `t`: t del enunciado, por ahora no lo he utilizado.
- `pages` : vector de páginas.
Al inicializar un HashTable, inicializa las páginas.

`Page` tiene como parámetros:
- `page[]`: arreglo que representa la página con capacidad para 128 elementos, aquí se guardan los elementos que se insertan.
- `last_post`: parámetro que indica el último elemento insertado sirve para checkear algunas condiciones de forma sencilla.
- `page_index`: numeración que utiliza dentro de la tabla de hashing, sirve para checkear algunas condiciones de forma sencilla.
- `linkedPage`: puntero a una página (se utiliza en el caso que rebalsa) inicialmente es nulo.

se inicializa con todos los elementos de page[] en cero.

En el main sigue un for para insertar números continuos, si se prueba con i<128 debería solo haber una página con 128<=i<256 hay una página de rebalse.

`insert()` se preocupa de ver en qué página se insertará. Si al insertar se supera el costo promedio máximo de búsqueda, se expande la tabla de hashing (`expand()`) y se redistribuyen los valores insertados (falta por implementar `redistribute()`).
La inserción como tal la realiza `insertinPage` esta función checkea si hay espacio o no, si es que hay, inserta, si no, se habilita la `LinkedPage` que antes era un puntero nulo.

con `setLinkedPage()`se habilita la página (deja de ser un null pointer).
