# Informe de TP2 Grupo 4

En este informe se explican las caracteristicas, algoritmos, estructuras y funciones del Kernel implementado.

## Programas
A continuacion se muestra el listado de programas disponibles
en el sistema operativo.

| Numero | Nombre |
| :----: | :----: |
| 0      | idle   |
| 1      | init   |
| 2      | sh   |
| 3      | help   |
| 4      | ps     |
| 5      | producer |
| 6      | consumer |
| 7      | sample |
| 8      | philosophers |
| 9      | chat |

## Procesos y Threads
A continuacion se explica el significado de **thread** y **proceso** en nuestro kernel, es decir, el scope que abarca cada uno y como se realiza uso de ellos.

### Thread
Un **thread** es una entidad que representa codigo ejecutable y la unica planificable. Tiene informacion tal como
- Un tid (thread id)
- Un estado (indica si esta en listo o bloqueado)
- Un stack
- Un IP (guardado en su stack)
- Un conjunto de registros (guardado en su stack)
- El proceso al que pertenece

### Proceso
Un **proceso** es la entidad que administra los recursos de sus threads. Tiene informacion tal como
- Un pid (process id)
- Un ppid (parent process id)
- Las paginas que forman parte del heap
- Una lista no vacia de threads
- La terminal a la que esta conectado
- Flags

### Vida de un proceso
Desde Userland un proceso puede ser creado utilizando la syscall **fork**, la cual clona el proceso que la llama creando un nuevo hijo solo con el thread que hizo la llamada a fork. Luego es posible utilizar la syscall **execve** que reemplaza la imagen del proceso cargando el modulo que se solicite.
A diferencia del resto, los unicos procesos que no son creados de esta forma son los procesos creados por el kernel, *idle* e *init*, y las *sh*, los cuales son creados con la funcion create_process durante la inicialización del kernel.
Finalmente, para terminar un proceso, se utiliza la syscall **exit**, la cual finaliza el proceso (y sus threads) que lo llama.

### Vida de un thread
Un thread se crea siempre que se cree un proceso. Ademas un thread puede ser creado en Userland con la syscall **new_thread** o en el kernel con la funcion *create_thread*. Si se utiliza esta ultima se debe especificar a que proceso va a pertenecer el thread ya que no puede existir un thread sin un proceso que lo contenga. En el caso de utilizar la syscall, el thread sera contenido por el proceso que hizo la llamada. Luego, para finalizar un thread se utiliza la syscall **exit_thread**.

## Scheduler
En esta seccion se explica tanto el algoritmo de scheduling asi como las estructuras utilizadas en este.

### Algoritmo
El algoritmo de scheduling utilizado es Round Robin con un quantum equivalente al intervalo
de interrupcion del timer tick. Para implementar este algoritmo se utiliza una cola circular de threads (cola de listos), la cual es "rotada" (con *rotate_thread_cqueue*) cada vez que se corre el algoritmo de scheduling.

### Colas de bloqueados
Nuestro scheduler tiene colas de bloqueados de manera que cuando un thread se bloquea, se lo saca de la cola circular de listos y se lo encola en la cola de bloqueados indicada.
Para esto, cada modulo del kernel (la tty por ejemplo), en tiempo de incializacion le pide al scheduler una nueva cola de bloqueo.

#### Bloqueo
Cuando tiene que bloquear a un thread le indica a quien quiere bloquear y en que cola (la suya), ademas se puede incluir con cada thread bloqueado informacion extra utilizada cuando se desbloquea el proceso.

#### Desbloqueo
Cuando un modulo del kernel tiene que desbloquear a un thread porque ya se cumplio una condicion, llama al scheduler y le indica que quiere desbloquear un proceso de su cola de bloquados y le pasa un callback el cual es ejecutado con la infomacion extra (pasada durante el bloqueo) en el espacio de direcciones del thread a desbloquear.

## Cola de mensajes
Implementamos cola de mensajes como IPC. Estas se identifican con un string de 256 caracteres de longitud maxima y permiten escribir mensajes de longitud variable.
Para la implementacion se utilizo un lista enlazada donde los nodos son colas de mensajes cada una con su string identificador.
Para crear una cola de mensajes se utiliza la syscall **mq_open** la cual recibe el string identificador y crea la cola si no existia. Una vez creada la cola se pueden enviar mensajes utilizando la syscall **mq_send** la cual recibe el string identificador de la cola, el mensaje (cadena de caracteres) y la longitud de este. Luego se puede desencolar mensajes utilizando **mq_receive** la cual recibe la cola (su string) un buffer donde se va a copiar el mensaje y la longitud maxima del mismo. La llamada a mq_receive bloquea al thread en la cola de mensajes si es que esta no tiene mensajes y lo desbloquea cuando llega uno.

## Mutex
Los mutex, al igual que las colas de mensajes, se identifican con un string. Estos se crean con la syscall **mx_create** y se bloquean y desbloquean con las syscall **mx_block** y **mx_unblock**. Cuando un thread llama a block y el mutex ya estaba bloqueado por otro thread, este se bloquea hasta que el otro thread lo desbloquee.
Finalmente decidimos hacer la implementacion sin utilizar las instrucciones *xchg* y *lock* de intel ya que de igual forma todas nuestras llamadas a systemcalls son atomicas con respecto a los procesos de usuario. Esto quiere decir que si un thread realiza una llamada a block, el bloqueo se ejecuta completamente antes de que otro thread ejecute alguna instruccion.

## Semaforos
Decidimos implementar semaforos ya que los necesitabamos para implementar **prodcons**. La implementacion (a pesar de que su uso sea distinto) es muy similar a la de mutex con la obviedad de el rango de valores es diferente.

## Terminales
Para poder mostrar mejor las capacidades de multitasking del TP decidimos incluir un **multiplexor de terminales**. Este nos permite tener varias terminales con una unica pantalla y teclado.
El funcionamienton es simple, se tiene un ADT que representa una terminal virtual. Este tiene informacion como el buffer de entrada y salida (pantalla), cola de bloqueados, etc.
Luego, se tiene el mux de terminales que contiene 8 de estos ADT y que permite "switchear" con **F1-F8** entre terminales. De las cuales las primeras 7 son utilizadas por los procesos para entrada y salida, y la ultima (8) corresponde al log de kernel. En este ultimo se puede ver informacion de scheduling, inicializacion, etc.
Con respecto a la lectura y escritura en pantalla, estas llamadas son recibidas por el mux, el cual decide (en base a la terminal a la que esta conectada el proceso del thread que quiere escribir/leer) a que terminal se debe escribir/leer y si se debe actualizar la pantalla (en caso de que este activa).

## Page allocator
Para reservar paginas fisicas utilizamos un simple page allocator cuyo algoritmo encontramos en el sitio OSDev. Consiste en que cada vez que se reserva una pagina, esta es entregada de un arreglo de paginas preallocadas. De esta forma se evita recorrer el arreglo que indica cuales estan libres, cada vez que se quiere reservar una pagina.

## Background / Foreground
Cuando se ejecuta un comando y se incluye al final el caracter '&', no se le cede el foreground y la shell (sh) no espera a que este termine. Por lo tanto, si en esta situacion requiere leer de teclado, se lo suspende enviandolo a una cola de suspendidios. Hubiera sido deseable implementar  un comando para la shell que permita entregar el foreground a un proceso nuevamente.
Es importante tener en cuenta que el foreground lo tiene el thread principal de cada proceso.

## Memory protection
No pudimos implementar memory protection porque tuvimos problemas para volver de ring 3 a ring 0. Se necesitaba configurar correctamente una TSS que si bien no era utilizada, servia para pasar al contexto de kernel. Otra solucion posible era utilizar las instrucciones syscall y sysret para realizar las llamas a systemcalls. Pero por problemas de tiempo no llegamos a investigar esto ya que decidimos priorizar el resto de las cosas.

## Aclaraciones extras
- En philosophers se puede incrementar la cantidad de filosofos con I y se decrementa con D, para salir se utiliza Q.
- Luego de ejecutar sample, la llamada a PS puede llegar a causar una exception. Creimos importante dejar a modo de ejemplo este programa sabiendo que produce un error el cual no llegamos a solucionar por cuestiones de tiempo.
- No llegamos a hacer un cuadro con las syscalls, pero estas se encuentran en  la carpeta **syscalls** dentro del Kernel.
