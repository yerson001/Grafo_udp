
## Ejecución del programa
## Grafo
![test para la cola](img/grafo.jpeg)
## Hash Repositorio
![test para la cola](img/hash.png)

Diríjase al directorio grafo
~~~
$>> cd grafo
~~~
Digitar
~~~
$>> cmake -B "build" -S .
~~~ 
Se creará la carpeta bulid entre en build
~~~
$>> cd build
~~~
Compilar
~~~
$>> make
~~~
Los ejecutables 
~~~
$ master  repositorio  client  main
~~~

## Para esta prueba usaremos 1 cliente; 1 Master ; 3 Repositorios
~~~
            C
            |
            M
          / | \
        R0->R1->R2    
~~~

1 Ejecute master->(gestiona al los clientes  y los repositorios)
~~~
$>> ./master
~~~

2 Ejecute un repositorio1 -> envíele la base de datos repo0.db  y el puesto del mater (LOCAL use:127.0.0.1)
~~~
si se ejecuta localmente
$>> ./repositorio repo0 127.0.0.1
~~~

2.1 Ejecute un repositorio2 -> envíele la base de datos repo1.db
~~~
si se ejecuta localmente
$>> ./repositorio repo1 127.0.0.1
~~~

2.2 Ejecute un repositorio3 -> envíele la base de datos repo2.db
~~~
si se ejecuta localmente
$>> ./repositorio repo2 127.0.0.1
~~~
Puede Crear cuantos repositorios requiera

Copie las bases de datos (repo0.db) (repo1.db) (repo2.db)
~~~
$>> copiar mydata.db que esta en source a la carpeta build
~~~
3 Ejecute un cliente
~~~
$>>./client
~~~
Puede crear varios clientes 
## Prueba en Google-Cloud
![test para la cola](img/cloud.png)
## Diagrama
![test para la cola](img/Selección_051.png.png)
