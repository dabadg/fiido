# fiido
/*
                   Version SinCon acelerador 1.5.9.13

                    MODOS DE FUNCIONAMIENTO
                    
-----------------------------------------------------------------------

       --- MODO 1 - Por Niveles ---

   En el nuevo modo alternativo (1) la velocidad de crucero se puede ir
aumentando o disminuyendo, con un toque de freno se aumenta y con dos
seguidos se disminuye el nivel o limite de velocidad siempre que se
esten moviendo los pedales.

    Se pueden configurar la cantidad de niveles que queramos y los
 voltajes/velocidad de cada uno segun los gustos en la lista Niveles.

-----------------------------------------------------------------------

       --- MODO 2 - Velocidad por pedaleo ---

   En este modo no hay crucero o limite de velocidad, se puede regular
la velocidad a la que queremos ir parando los pedales (dismminuye)
o pedaleando muy despacio (mantiene) la velocidad.

   Este modo se puede acceder a el dando mas de 7 toques de freno
seguidos al encender la bici o tras oir el beep inicial.

--------------------- NUEVO desde la version 1.4 ---------------------

  Incorporado sistema de ayuda al arranque para la salida cuesta arriba
o semaforos, estando parado damos cuatro toques de freno seguidos (maximo 1/3
de segundo entre toques) y hara un progresivo de un corto tiempo que si le
acompanyamos de pedal saldremos sin esfuerzo, en caso de que se activara por
error con otro toque mas de freno se pararia y se anula la ayuda.
   Este sistema esta desactivado por defecto, para activarlo hay que encender
la bicicleta con cualquiera de los dos manetas de frenos apretadas.

--------------------- NUEVO desde la version 1.5 ---------------------

   Nueva funcion auto-progresivo, si se deja de pedalear el motor se para como
de costumbre, pero si continuamos pedaleando antes de transcurridos 10 segundos
no inciara el progresivo desde cero si no que el motor continuara a una velocidad
ligeramente inferior a la que ibamos, si se frena antes de los 10 segundos se
anula la funcion y comenzara progresivo desde cero.

    Ajustes en el algoritmo de aceleracion progresiva (mas suave) y en el modo
de aceleracion por pedaleo.
-----------------------------------------------------------------------
   
   Opcionalmente puede incorporarse un zumbador piezoelectrico en el pin D11
y GND y sonara al subir o bajar nivel, seleccion de modos, ayuda salida en cuesta 
y alcanzar el final del progresivo...

 -----------------------------------------------------------------------
 
   El acelerador no haria falta conectarle, se puede desmontar o bloquear. Si no 
se quiere utilizar hay que dejar la variable modo_acelerador_activo a false

para evitar el uso del acelerador y la funcionalidad de nivel zero, que permite 
el uso del acelerador en modo scooter.

--------------------- NUEVO en la version 1.5.9.11 dabad ---------------------

Añadida lectura de acelerador en caso de estar conectado al pin A0.

  Añadido nivel Zero, que anula la asistencia y permite utilizar el acelerador 
manualmente. Este modo solo puede ser activado si la variable 
modo_acelerador_activo está a true, y se ha activado en el arranque 
la opción de acelerador en nivel zero.
Esta funcionalidad se activa al arranque dejando pulsado el freno y se activa 
junto con la operación  de activación con la asistencia en cuestas. 
El acelerador entregará la potencia deseada  mientras se esté pedaleando.

  Se ha añadido un control de cambio de nivel, para que no se pueda salir 
del Zero hasta no haber ejecutado el comando de salida. 
Este caso es para evitar salir del nivel Zero por error al pulsar el freno mientras se pedalea.
Para salir del nivel Zero, sin pedalear hay que mantener el acelerador en una posición 
alta y hacer un cambio brusco para bajar la velocidad al mínimo. Esto nos llevará 
directamente al nivel 2 de asistencia.
 
  Modificada la asistencia de cuestas para permitir realizar los tres pulsos y dejar
el freno pulsado a la espera de soltarlo y entregar la potencia.
 
Modificados los tonos de cambio de nivel.

  Añadida posibilidad de incrementar los pulsos necesarios para entrar en modo 
asistencia en cuestas desde variable de configuración.
-----------------------------------------------------------------------------

   Desarrollado por dabad partiendo de la versión de ciberus partiendo del programa incial de legalizacion del
acelerador de Fulano, MIL GRACIAS por su ayuda :-) tambien gracias a David
por su ayuda con las innumerables pruebas del programa, Dca por su croquis de
conexiones con los cables de los conectores, Pablo y Fulano por los manuales y
en general a todos los integrantes del grupo de desarrollo que sin su granito
de arena no hubiera sido posible llegar hasta aqui.

-----------------------------------------------------------------------------

   Ayuda, sugerencias, preguntas, etc. en el grupo Fiido telegram:
                     http://t.me/FiidoD1Spain
          
   Grupo telegram de desarrollo privado, si vas a montar el circuito y 
   necesitas ayuda o colaborar pide acceso en el general de arriba.
                 
   Canal con montaje, enlaces, programas, etc. http://t.me/fiidolegal

------------------------------------------------------------------------------
*/
