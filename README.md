
## Ejecución del programa

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
Ejecute cualquiera con
~~~
$>> ./archivo->(cualquiera => master,repositorio,client)
~~~
Copie la base de datos
~~~
$>> copiar mydata.db que esta en source a la carpeta build
~~~
# Ejecucion Local
## Servidor
~~~
$Terminal1>> ./master
~~~
## Repositorio  en MSG: R
~~~
$Terminal2>> ./repositorio  
~~~
## Cliente
~~~
$>> ./client
~~~


