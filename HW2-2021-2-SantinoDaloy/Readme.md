# Base-T1-2021-2
Código base para tu tarea 2.

## Compilar y ejecutar
Compilar:
```make```

Compilar desde cero:
```make clean && make```

Ejecutar:
```./potterphoto <A/B> <input.png> <output.png>```

El parámetro ```<A/B>``` Indica si el test corresponde a de Hashing o Backtracking respectivamente. El codigo base incluye la deteccion de esto, sin embargo el resto de lectura de archivo lo tienes que realizar tu

Para optimizar el tiempo de ejecucion de tarea, puedes modificar el Makefile cambiando el -g por -O2

Para la Parte 1 el código de la tarea fue basado en la ayudantía de hashing del curso y en el código de este artículo: https://www.geeksforgeeks.org/c-program-to-check-if-two-given-strings-are-isomorphic-to-each-other/

Para la Parte 2 el flujo del programa fue basado en el código del sigueinte artículo: https://www.geeksforgeeks.org/the-knights-tour-problem-backtracking-1/