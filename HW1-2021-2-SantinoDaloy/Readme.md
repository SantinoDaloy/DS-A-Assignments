# Base-T1-2021-1
Código base para tu tarea 1.

## Requerimientos
Debes tener instalada la librería `libpng` para poder leer las imágenes cómo matrices.  
Esta la puedes instalar con el comando en linux:
```sudo apt install libpng-dev```

Opcionalmente en Docker (Dentro de la carpeta del container)
```docker-compose run <mycontainer> apt-get install libpng-dev```

## Compilar y ejecutar
Compilar:
```make```

Ejecutar:
```./potterphoto <input.png> <output.png> <marker.png> <alpha> <R>```

El parámetro ```<R>``` es opcional, Invierte la imagen para encontrar objetos negros, vas a tener que utilizarlo para algunas imagenes.

No te preocupes por el valor del alpha a utilizar, se indicara en los test que valor de alpha debes utilizar para llegar al resultado.
