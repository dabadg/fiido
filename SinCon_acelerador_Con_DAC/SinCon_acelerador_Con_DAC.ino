/*
                   Version SinCon acelerador 1.5.9.11

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

   Desarrollado por dabad partiendo de la versión de ciberus partiendo del 
programa incial de legalizacion del acelerador de Fulano, MIL GRACIAS por su ayuda :-) 
Tambien gracias a David por su ayuda con las innumerables pruebas del programa, Dca por 
su croquis de conexiones con los cables de los conectores, Pablo y Fulano por los manuales y
En general a todos los integrantes del grupo de desarrollo que sin su granito
de arena no hubiera sido posible llegar hasta aqui.

-----------------------------------------------------------------------------

   Ayuda, sugerencias, preguntas, etc. en el grupo Fiido telegram:
                     http://t.me/FiidoD1Spain
          
   Grupo telegram de desarrollo privado, si vas a montar el circuito y 
   necesitas ayuda o colaborar pide acceso en el general de arriba.
                 
   Canal con montaje, enlaces, programas, etc. http://t.me/fiidolegal

------------------------------------------------------------------------------
*/

// ================ Variables configurables ===========
// ===  Jugando con estos valores se modifica el comportamiento
// >>> Si no sabes lo que estas tocando mejor que no lo toques. ;-) <<<

// Numero de pulsos por 1/4 de segundo para que se considere 
// que se esta pedaleando, configurar segun sensor y gustos
int cadencia = 2;

// Retardo en segundos para parar el motor una vez se deja de pedalear
// Usar multiplos de 0.25
float retardo_paro_motor = 0.25; // 0.25 = 1/4 de segundo.


// Retardo en segundos para ponerse a velocidad maxima o crucero.  
int retardo_aceleracion = 5;

// Retardo para inciar progresivo desde cero tras parar pedales (freno anula el tiempo)
// Poner a cero para desconectar esta opcion y que inicie siempre desde cero
int retardo_inicio_progresivo = 10;

// Direccion del bus I2C [DAC] (0x60) si esta soldado, si no (0x62)
// Usar escaner i2C si no se esta seguro de en que direccion esta y para
// verificar el correcto funcionamiento del DAC.
const int dir_dac = 0x60; 

// Nivel al que se desea iniciar el progresivo
// Aumentar si se desea salir con mas tiron, >> NO PASAR DE 2.00 <<                                                                          
const float nivel_inicial_progresivo = 1.5;

// Desacelera al parar los pedales, Poner en true para activar
// o false para que no desacelere. Recomendable TRUE siempre.
boolean desacelera_al_parar_pedal = true;

// Activado modo acelerador manual en nivel 0
boolean modo_acelerador_activo = true;

// Tipo por defecto de funcionamiento: 
// 1 = Crucero (nuevo por niveles) sube con un toque y baja con 2 toques,
// 2 = Velocidad por movimiento de pedales [No tiene crucero]), tambien se pueden
// elegir o cambiar con 5-6 toques o mas de freno al encender la bici.
int modo_crucero = 1; // Valores = 1 y 2 ...

// Configuracion de los pitidos del zumbador piezoelectrico
const boolean tono_inicial = true;          // Al encender la bici y confirmacion de modos de funcionamiento
const boolean tono_crucero = true;          // Al cambiar el nivel de asistencia
const boolean tono_fin_progresivo = false;  // Al alcanzar el final del progresivo
const boolean tono_arranque = true;         // En el segundo tercer y cuarto toque de freno para activar la salida en cuesta
const boolean tonos_cambia_nivel = true;     // Tonos cuando se opera con el cambio de nivel de asistencia 

// Voltajes para los distintos niveles de asistencia en tipo 1 (se pueden añadir, editar o quitar niveles dejando siempre el zero)
const float niveles[] = {0.85, 2.0 , 2.40 , 2.80 , 3.20 , 3.90}; // saltos de 0.40 4n
//                       zero   5     10     15     20     max      kmh

// Las velocidades medidas son aproximadas, depende de la carga, cuesta, peso y mas factores.
// Aprox 0.20 son saltos de 2,5kmh , 0.25 de 3kmh, 0.35 de 4kmh ,0.40 de 5kmh

// Constantes de control de valor de acelerador; 
// Valores mínimos y mávimos leidos por el pin A0
float a0_min_value = 190.0;
const float a0_med_value = 550.0;
const float a0_max_value = 847.0;

// Nivel de asistencia por defecto al arranque.
const int nivel_por_defecto = 2;
int nivel_minimo = 1;

const int toques_salida_cuesta=3;


//================ FIN VARIABLES CONFIGURABLES POR EL USUARIO ====================

#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

//======= CONFIGURACION DE LOS PINES ==========
const int pin_acelerador = A0; // pin acelerador
const int pin_pedal = 2; // Pin sensor pas, en nano o uno usar solo 2 o 3
const int pin_freno = 3; // Pin de activacion del freno
const int pin_piezo = 11; // Pin donde se ha conectado el zumbador (9,10,11)

//======= Variables para calculos

// Nivel inicial al encender la bici (el primero de la lista de niveles es cero)
int nivel = nivel_por_defecto;

// Flag de control de ayuda de salida en cuestas y de acelerador activo en nivel zero.
// Ambas asistencias están desactivadas por defecto al arranue, por seguridad por si nos paran,
// ya que es ilegal que el motor funcione sin pedalear, para activarlos hay que
// encender la bici con el freno pulsado.
boolean ayuda_salida_cuesta = false;
boolean acelerador_nivel_zero_activo = false;


// Último valor recogido del acelerador
float v_acelerador; 
// Valor recogido del acelerador estado anterior
float v_aceleradorprev; 

// Voltaje maximo a tope, >>> (No pasar de 4.20) <<<
const float voltaje_maximo = 3.90;

// Valor minimo de voltaje (acelerador en reposo)
const float voltaje_minimo = 0.85;

// Tiempo en milisegundos para contar pulsos
const int tiempo_cadencia = 250;

// Contadores de paro, aceleracion y auto_progresivo del motor
unsigned contador_retardo_paro_motor = 0;
int contador_retardo_aceleracion = 0;
unsigned contador_retardo_inicio_progresivo = 0;
int bkp_contador_retardo_aceleracion = 0;
boolean auto_progresivo = false;

// Muestra en el plotter si el motor esta encendido(-5) o no (-8)
int motor = -8;

// Variable donde se almacena la velocidad de crucero
float v_crucero = voltaje_maximo; 

// Los voltios que se mandan a la controladora
float nivel_aceleracion = voltaje_minimo;

// Contador de pulsos del pedal
int pulsos = 0; 

// Contador de pulsaciones de freno
int frenadas = 0; 

// Switch de aviso de final de progresivo
boolean aviso = false; 

// Modo depuracion (grafica y monitor)
const boolean debug = false;

// Calculo del numero de niveles de asistencia -1
const int num_niveles = (sizeof(niveles)/sizeof(niveles[0]))-1;

const int NIVEL_ZERO = 0;

//======= Variables interrupcion
volatile int p_pulsos = 0; // Variable donde se suman los pulsos del sensor PAS
volatile int p_frenadas = 0; // Variable donde se suman las pulsaciones del freno

//================== TONES ==================
// Frecuencias 4 octavas de
int c[5]={131,262,523,1046,2093};       // Do
int cs[5]={139,277,554,1108,2217};      // Do#
int d[5]={147,294,587,1175,2349};       // Re
int ds[5]={156,311,622,1244,2489};      // Re#
int e[5]={165,330,659,1319,2637};       // Mi
int f[5]={175,349,698,1397,2794};       // Fa
int fs[5]={185,370,740,1480,2960};      // Fa#
int g[5]={196,392,784,1568,3136};       // Sol
int gs[5]={208,415,831,1661,3322};      // Sol#
int a[5]={220,440,880,1760,3520};       // La
int as[5]={233,466,932,1866,3729};      // La#
int b[5]={247,494,988,1976,3951};       // Si

void nota(int frec, int ttime){
    tone(pin_piezo,frec);
    delay(ttime);
    noTone(pin_piezo);
}

void SOS_TONE(){
  nota(b[3],150);delay(40);
  nota(b[3],150);delay(40);
  nota(b[3],150);delay(70);
  nota(b[3],100);delay(40);
  nota(b[3],100);delay(40);
  nota(b[3],100);delay(70);
  nota(b[3],150);delay(40);
  nota(b[3],150);delay(40);
  nota(b[3],150);delay(100);
}

void ZERO_TONE(){
  nota(b[2],100);delay(40);
  nota(b[2],100);delay(40);
  nota(b[2],50);delay(40);
  nota(b[2],50);delay(70);
  
  nota(b[2],50);delay(70);
  
  nota(b[2],50);delay(40);
  nota(b[2],100);delay(40);
  nota(b[2],50);delay(70);

  nota(b[2],100);delay(40);
  nota(b[2],100);delay(40);
  nota(b[2],100);delay(300);
}

void repeatTones(boolean trigger, int steps, int frequency, int duration, int delayTime){
  if(trigger){
    int cont=steps;
    while(cont-- > 0){
      tone( pin_piezo, frequency, duration);
      if(delayTime>0)
        delay(delayTime);    
      //noTone(pin_piezo); 
    }
  }
}

//================== FUNCIONES ==================

//--- Funciones de interrupción

void pedal(){ p_pulsos++; }

void freno(){
  p_frenadas++;
  contador_retardo_inicio_progresivo = retardo_inicio_progresivo;
  bkp_contador_retardo_aceleracion = 0;
  cadencia=2;
}

//--- Funciones acelerador

void ayuda_arranque(){
  if(nivel > NIVEL_ZERO){ // Si no está en nivel Zero ajusta la ayuda de arranque.
    int contr = toques_salida_cuesta;
    int arranque_ca = 0;
    p_frenadas = 0;
    delay(350);
    
    while(contr -- > 0){
      if (p_frenadas > 0){
        p_frenadas = 0;
        arranque_ca++;
        repeatTones(tono_arranque, 1, 2100 + (200*arranque_ca), 50, 350);
      }
    }
    
    if(arranque_ca==toques_salida_cuesta){
      
      int tiempo = 6; // tiempo en cuartos de segundo 6 = 1,5 seg.
      
      while (digitalRead(pin_freno) == LOW) {delay(1); } // Retiene la ejecución hasta que no se suelta el freno.
    
      if (v_crucero <= 3.0) { // Aumentamos a 1 seg si hay nivel bajo
        tiempo = tiempo + (int) (1000 / tiempo_cadencia) * 1; 
      } else if (v_crucero <= 2.0) {// Aumentamos a 2 seg si hay nivel muy bajo
        tiempo = tiempo + (int) (1000/tiempo_cadencia) * 2;
      }  
      
      p_frenadas = 0;
    
      for (int ac = 0; ac < tiempo; ac++){
        if (p_frenadas > 0) { // Si pulsamos el freno, anulamos la asistencia
          contador_retardo_aceleracion = 0;
          establece_voltaje();
          repeatTones(tono_arranque, 1, 2800, 300, 0); // Cuarto tono, anulacion
          break;
        }
        contador_retardo_aceleracion++;
        establece_voltaje();
        //if (debug) {impresion_plotter();}
        delay(tiempo_cadencia);
      }
    }
  }
}
  
void establece_voltaje(){
  uint32_t valor;
  if (nivel <= NIVEL_ZERO && acelerador_nivel_zero_activo  && modo_acelerador_activo) { // Calcula el valor del DAC por medio del acelerador
    
    leer_Acelerador();
    nivel_aceleracion = v_acelerador * 5 / 1023;
    
    if (nivel_aceleracion < voltaje_minimo){
      nivel_aceleracion = voltaje_minimo;
    } else if (nivel_aceleracion > voltaje_maximo){
      nivel_aceleracion = voltaje_maximo;
    }
    
    valor = (4096 / 5) * nivel_aceleracion;
    
  } else { // Calcula el valor del DAC por medio del algoritmo autoprogresivo
    float incremento = ((v_crucero+0.3) - nivel_inicial_progresivo) / retardo_aceleracion;
    
    nivel_aceleracion = nivel_inicial_progresivo + (incremento * contador_retardo_aceleracion);
    
    if (nivel_aceleracion == nivel_inicial_progresivo){
      nivel_aceleracion = voltaje_minimo;
    } else if (nivel_aceleracion > v_crucero){
      nivel_aceleracion = v_crucero;
    }
    
    valor = (4096 / 5) * nivel_aceleracion;
  }
  dac.setVoltage(valor,false); // fija voltaje en DAC                               
}

  void leer_Acelerador(){
    
    // Tomamos multiples lecturas del acelerador para obtener su valor
    v_acelerador = capturaLecturaAcelerador();
   
    // Con la medida del acelerador detectamos si hay que salir del nivel Zero
    // Sale del nivel Zero si se está apretado el freno, no hay pedaladas, nos encontramos en nivel Zero y se ha detectado el corte repentino de acelerador.
    if(digitalRead(pin_freno) == LOW && pulsos == 0 && nivel==NIVEL_ZERO && (v_aceleradorprev > v_acelerador &&  v_acelerador <= a0_min_value+50.0 && v_aceleradorprev > a0_med_value-50.0)){ 
      nivel=nivel_por_defecto;
      v_crucero = niveles[nivel];
      repeatTones(tonos_cambia_nivel,1,1800,800,0);
    } 
    v_aceleradorprev=v_acelerador; // Almacena el valor de la medida anterior del acelerador para commparar los cambios bruscos y detectar la salida de nivel zero..  
     
    // Si no se está pedaleando se anula potencia del acelerador
    if(pulsos == 0){
      v_acelerador = a0_min_value;
    }
  
    /*
    // Si no se pedalea y se acelera, se aplica una potencia de referencia;
    if(pulsos == 0 && v_acelerador > a0_min_value - 20){ 
      float vf_acelerador = a0_med_value;
      v_acelerador = v_acelerador < vf_acelerador?v_acelerador:vf_acelerador;
    }
    */
  
    // Evita que los valores leidos se salgan del rango.
    nivelaAcelerador();
  
  }

  float capturaLecturaAcelerador(){
    float c_acelerador;
    // Leemos nivel de acelerador
    c_acelerador=v_acelerador;
    for (int f=1; f <= 30 ; f++){
      c_acelerador=c_acelerador+analogRead(pin_acelerador); // Tomamos 30 medidas para calcular la media.   
    }
    return c_acelerador/30;
  }
  
  void nivelaAcelerador(){
    // nivelamos los valores para que no salgan del rango de máximo/mínimo.
    if(v_acelerador <= a0_min_value) {
      v_acelerador = a0_min_value;
    }else if(v_acelerador >= a0_max_value) {
      v_acelerador = a0_max_value;
    }  
  }

void para_motor(){
  motor = -8;
  if (contador_retardo_aceleracion > 4){
    bkp_contador_retardo_aceleracion = contador_retardo_aceleracion/1.2;
    cadencia=3;
  }
  contador_retardo_aceleracion = 0;
  aviso = false;
  auto_progresivo = true;
}

//--- Funciones de niveles

void cambia_nivel(){
    if (nivel > NIVEL_ZERO || !acelerador_nivel_zero_activo) { // Si estamos fuera del nivel zero o si no está activo el control de acelerador, cambiamos de nivel
      int detector_direccion=0;
    
      // Almacena los pulsos de freno para detectar la dirección (0 Sube, >1 Baja)
      delay(50); // Espera 50ms para iniciar la detección del segundo pulso.
      p_frenadas = 0;
    
      int lvcont=45;
      while (lvcont-- > 0){ // Escaneamos durante 450ms una segunda pulsación para detectar la bajada de nivel (45 vueltas de 10ms)
        delay(10);
        if (p_frenadas > 0) {
         p_frenadas = 0;
         detector_direccion++;
         break;
        }  
      }
    
      if (detector_direccion > 0) { // Baja Nivel
        nivel--;
        if (nivel <= nivel_minimo) {
          delay(90);
          repeatTones(tonos_cambia_nivel, 2, 1800, 300, 50); // Tono largo nivel mínimo.
          nivel = nivel_minimo;
        } else {
          repeatTones(tonos_cambia_nivel, 1, 1800, 60, 0); // Segundo tono (baja de nivel)      
        }   
      } else { // Sube Nivel
        nivel++;
        if (nivel >= num_niveles) {
          repeatTones(tonos_cambia_nivel, 2, 2200, 300, 50); //Tono largo nivel maximo.
          nivel = num_niveles;
        } else {
            repeatTones(tonos_cambia_nivel, 1, 2200, 60, 0); // Primer tono (sube de nivel)    
        }
      }
      
      v_crucero = niveles[nivel];
      delay(200);
    }
    p_frenadas = 0;
}

//-- Funciones de depuración

void impresion_plotter() {
   //===== Impresion plotter serial para grafica
   Serial.print("Pul: ");
   Serial.print(pulsos);
   Serial.print("/");
   Serial.print(cadencia);
   Serial.print(" ,Mot: ");
   Serial.print(motor);
   Serial.print(" ,Fren: ");
   Serial.print(frenadas);
   Serial.print(" ,V cr: ");
   Serial.print(v_crucero);
   Serial.print(" ,Niv ac: ");
   Serial.print(nivel_aceleracion);
   Serial.print(" ,Cnt ac: ");
   Serial.print(contador_retardo_aceleracion);
   Serial.print("/");
   Serial.print(bkp_contador_retardo_aceleracion);
   Serial.print(" ,cnt paro: ");
   Serial.print(contador_retardo_paro_motor);
   Serial.print(" ,cnt prog: ");
   Serial.print(contador_retardo_inicio_progresivo);
   Serial.print(" ,Valor Acelerador: ");
   Serial.println(analogRead(pin_acelerador));
   //=============================================
}

// --- Tonos de SETUP (puedes ver ejemplos de los tonos en la carpeta 'doc/tonos error' del proyecto de github)
// 1 tono - comienza inicialización
// tono nivel zero - solo suena si se ha activado el novel zero o asistencia de arranque.
// 3 tonos - termina inicialización
// [1,2] tonos - confirma el modo seleccionado

 
void setup() {

  // Configura DAC
  dac.begin(dir_dac);
  dac.setVoltage(810, true); // fija voltaje inicial en Dac (0.85v)

  
  if (debug) {
    // Inicia serial:
    Serial.begin(19200);
    Serial.println("Version Con/Sin Acelerador + Dac 1.5.9.11");  
    // Configura plotter
    Serial.println("8,-8,0,0,0,0");
  }

  // Configura pines y prepara las interrupciones
  pinMode(pin_piezo, OUTPUT);
  pinMode(pin_freno, OUTPUT);
  digitalWrite(pin_freno, HIGH);
  pinMode(pin_pedal, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(pin_pedal),pedal,CHANGE); //interrupcion pedal
  attachInterrupt(digitalPinToInterrupt(pin_freno),freno,FALLING); //interrupcion freno

  // Inicializamos el valor mínimo del acelerador, calculando la media de las medidas si tiene acelerador, en caso de no tener acelerador, mantenemos valor por defecto.
  // Esto es útil para controlar el corecto funcionamiento del acelerador , si este está presente.
  if (modo_acelerador_activo){
    float min_acelerador;
    for (int f=1; f <= 30 ; f++){
      min_acelerador = min_acelerador + analogRead(pin_acelerador); // Tomamos 30 medidas para calcular la media.   
    }
    min_acelerador = min_acelerador / 30;
    
    // Si la medida no es correcta, emitimos un aviso sonoro SOS para poder localizar el error y desactivamos el acelerador.
    if ((min_acelerador < a0_min_value - 50) || (min_acelerador > a0_min_value + 50)){
        modo_acelerador_activo = false;
        SOS_TONE();
    } else {
      a0_min_value = min_acelerador;
    }
  }
  
  repeatTones(tono_inicial, 1, 3000, 90, 190); // Tono aviso de inicio a la espera de frenadas (al encender bici)
  if (tono_inicial) {noTone(pin_piezo);}
  
  // Si encendemos la bici con el freno pulsado activamos la ayuda salida en cuesta y el nivel zero.
  // Una vez oido el beep inicial ya se puede soltar el freno o dar toques para cambiar de modo 2 si es tu elección.
  if (digitalRead(pin_freno) == LOW) {

    // Activamos ayuda en cuesta
    ayuda_salida_cuesta = true;
    
    // Activamos el nivel ZERO si el acelerador está activo
    if (modo_acelerador_activo){
      acelerador_nivel_zero_activo=true;
      nivel_minimo = NIVEL_ZERO;
      ZERO_TONE();
    }
  }

  // Selector de modos en el encendido, hay 2 modos configurables (modo 1 por defecto, modo 2 dando 4 toques de freno durante 3 segundos).
  // Emitira un pitido para el modo 1 (tipo alternativo de crucero), y 2 para el modo 2 (velocidad por pedaleo).  
  p_frenadas = 0;
  p_pulsos = 0;

  delay(10);
  int mdcont=300;
    while (mdcont-- > 0){ // Escaneamos durante hasta 3000ms para detectar el cambio de modo.
      delay(10);
      if (p_frenadas > 8){
        // Modo 2, al pedalear aumenta velocidad y al parar pedales disminuye
        // Activar con mas de 4 toques de freno tras oir el pitido o encender la bici.
        modo_crucero = 2;
        retardo_aceleracion = 6;
        retardo_paro_motor = 1.25;
        desacelera_al_parar_pedal = true;
        p_frenadas = 0;
        break;
      } else if (mdcont==0){
        v_crucero = niveles[nivel];// Fijamos el nivel seleccionado al encender la bici si aranca en modo 1
      }
    }
    //repeatTones(tono_inicial, modo_crucero, 3000, 100, 150); // Tono verificacion inicialización de modo x

  // Ajusta contadores de tiempo de la configuracion 
  retardo_paro_motor = retardo_paro_motor * (1000 / tiempo_cadencia);
  retardo_aceleracion = retardo_aceleracion * (1000 / tiempo_cadencia);
  retardo_inicio_progresivo = retardo_inicio_progresivo * (1000 / tiempo_cadencia);
  
  // Anulamos el retardo por seguridad para que empiece progresivo al encender la bici
  contador_retardo_inicio_progresivo = retardo_inicio_progresivo;

  //delay(300);
  repeatTones(tono_inicial, 3, 3000, 90, 90); // Tono de finalización de setup.
  delay(100);
  repeatTones(tono_inicial, modo_crucero, 2500, 90, 150); // Tono verificacion inicialización de modo x.
}

// Bucle principal
void loop() {
  p_pulsos = 0;
  p_frenadas = 0;
  delay(tiempo_cadencia);
  pulsos = p_pulsos;
  frenadas = p_frenadas;
  
  // Si se pedalea despacio o se paran los pedales
  if (pulsos < cadencia) {
      contador_retardo_inicio_progresivo++;
      contador_retardo_paro_motor++;
      if (contador_retardo_paro_motor > retardo_paro_motor){
        para_motor();
      }
  }

  // Si se pedalea normal (por encima de la cadencia).
  if (pulsos >= cadencia) {
      if (contador_retardo_inicio_progresivo < retardo_inicio_progresivo && auto_progresivo){
        contador_retardo_aceleracion = bkp_contador_retardo_aceleracion;
        auto_progresivo = false;
        cadencia=2;
      } else { 
        auto_progresivo = false;  
      }
      contador_retardo_inicio_progresivo = 0;
      contador_retardo_paro_motor = 0;
      if (contador_retardo_aceleracion < retardo_aceleracion){
        contador_retardo_aceleracion++;
      }     
      motor = -5;
  }

  // Si estan los pedales parados
  if (pulsos == 0){
    // Desacelera al parar los pedales
    if (contador_retardo_aceleracion > 0 && desacelera_al_parar_pedal){
      contador_retardo_aceleracion = contador_retardo_aceleracion - 2;
      if (contador_retardo_aceleracion < 0){
        contador_retardo_aceleracion = 0;
      }
    }   
  }

  // Si se ha pulsado el freno
  if (frenadas > 0) {  
    // Modo 1 sube nivel de asistencia con un toque y baja con 2 toques seguidos
    if (modo_crucero == 1 && pulsos > 3 && contador_retardo_aceleracion != 0){
      cambia_nivel();
    // Salidas en cuesta 4 toques de freno consecutivos si estamos parados (el primero no suena)
    } else if (nivel_aceleracion == voltaje_minimo && contador_retardo_aceleracion == 0
      && contador_retardo_paro_motor >= retardo_paro_motor && ayuda_salida_cuesta){
      ayuda_arranque();
    // Para motor al frenar
    } else {
      motor = -8;
      contador_retardo_aceleracion = 0;
      aviso = false;
    }
  }

  // Aviso de final de progresivo, configurar arriba si no se desea
  if (tono_fin_progresivo){
    if (contador_retardo_aceleracion == retardo_aceleracion && !aviso) {
      repeatTones(true, 1, 2900, 500, 0); // Tono de Final de progresivo
      aviso = true;
    }
  }
  
  establece_voltaje();
  
  if (debug) {impresion_plotter();}
}
