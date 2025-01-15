# Aclaración
Este proyecto está **inspirado** y **emulado** de una versión del proyecto Minitalk desarrollado por **[@PepeSegura](https://github.com/PepeSegura),** a quien agradezco profundamente su ayuda y motivación en el desarrollo de este trabajo, fundamental para mi avance personal. 

**Emular suele implicar que lo que se imita se toma como un modelo positivo e incluso se puede querer superarlo**, **repitiendo procesos** con la intención de **comprenderlos**, **asimilarlos** y, eventualmente, **adaptarlos** a las propias necesidades. Este **enfoque autodidacta** aplicado en mis primeros proyectos, me ayudó a **fortalecer conceptos** y **lógicas de programación** que con anterioridad parecían inalcanzables, y que ahora puedo aplicar en mis actuales proyectos.

Sobre **Minitalk** he visto cómo muchos estudiantes de **mi campus** lo abordan de manera superficial, subestimando su importancia. Sin embargo, para alguien como yo, que proviene de una disciplina completamente distinta, **Minitalk** representó un desafío extraordinario, una prueba de **paciencia**, **dedicación** y capacidad de conectar ideas que en un principio parecían no tener sentido.

**Mi objetivo**, como en los demás proyectos, es compartir mi experiencia y manera de abordar este tipo de proyectos para que otros, que tal vez han estado en la misma situación, puedan encontrar **motivación y herramientas** útiles para seguir adelante. **El camino del aprendizaje no es lineal**, y **cada quien lo transita a su manera**. Este proyecto es una contribución y **recordatorio** de que, aunque sea difícil, siempre se puede aprender algo nuevo.

**PD:** Los aportes de glosario y las especificaciones propias de cada función son contribuciones personales, resultado de las lecturas que suelo hacer en la red. En proyectos futuros prometo citar fuentes para que podais acudir directamente a ellas, pero en este caso, fueron el resultado de un sinnúmero de páginas web que intenté resumir a **"simples conceptos y explicaciones clave"**.
## Glosario

* **Señales en UNIX**

Las señales en UNIX son un mecanismo que permite la comunicación entre procesos o entre el sistema operativo y un proceso. Las señales son interrupciones asíncronas que notifican a un proceso que ha ocurrido un evento determinado, como una operación específica del sistema o una acción del usuario. Cada señal está asociada con un número de código y puede ser capturada o ignorada por el proceso, o bien el proceso puede terminar su ejecución en respuesta a la señal.

**Características Claves:**

* **Asíncronas:** Las señales se pueden enviar en cualquier momento, interrumpiendo la ejecución normal de un proceso.
* **Predeterminadas o Personalizadas:** El sistema operativo puede enviar señales para eventos predefinidos (por ejemplo, SIGINT se envía cuando el usuario presiona Ctrl+C), pero los procesos también pueden enviar señales entre sí.
Manejo de Señales: Los procesos pueden definir manejadores personalizados para reaccionar de manera específica a las señales (por ejemplo, limpiando recursos antes de finalizar). Esto se gestiona comúnmente mediante la estructura sigaction.
Ejemplos de Señales Comunes:
* **SIGINT:** Interrupción desde el teclado (Ctrl+C).
* **SIGTERM:** Solicitud de finalización del proceso.
* **SIGKILL:** Finaliza el proceso inmediatamente (no puede ser capturada ni ignorada).
**SIGUSR1 y SIGUSR2:** Señales definidas por el usuario, utilizadas para la comunicación personalizada entre procesos.


* **Tabla de estados:** Es una matriz que define las transiciones entre estados según la entrada recibida. Cada fila corresponde a un estado actual, y cada columna indica la acción a tomar en función de una condición (en este caso, si la entrada es un dígito o no).

* **Estados:** Los diferentes "modos" en los que puede encontrarse la máquina de estados. En tu caso:

* **Estado 0: Inicio (Init).**
* **Estado 1: Error (ERR).**
* **Estado 2: Se encontró un dígito (Found Digit).**
* **Posición (pos): Determina si la entrada es un dígito o no. Si la entrada es un dígito (isdigit(c)), se establece pos = 1, de lo contrario pos = 0.**

* **Relación entre posición y estado:**

* Si el programa está en el Estado 0 (inicio) y encuentra un dígito (pos = 1), la tabla indica que debe transitar al Estado 2 (dígito encontrado).
  
* Si el programa está en el Estado 0 y no encuentra un dígito (pos = 0), transita al Estado 1 (error).
  
* El Estado 1 (error) siempre lleva a sí mismo, ya que un error no se corrige.
  
* El Estado 2 (dígito encontrado) transita nuevamente a sí mismo si encuentra más dígitos.

		const int states[][2] = {
		   {1, 2},  // Estado 0: [sin dígito -> 1 (ERR), dígito -> 2 (Found Digit)]
		   {1, 1},  // Estado 1: [sin dígito -> 1 (ERR), dígito -> 1 (ERR)]
		   {1, 2},  // Estado 2: [sin dígito -> 1 (ERR), dígito -> 2 (Found Digit)]
		};

* **[0][0] = 1:** Si estamos en el Estado 0 y la entrada no es un dígito (pos = 0), transita al Estado 1 (error).
* **[0][1] = 2:** Si estamos en el Estado 0 y la entrada es un dígito (pos = 1), transita al Estado 2 (dígito encontrado).
* **[1][0] y [1][1] = 1:** Si estamos en el Estado 1 (error), permanece en el Estado 1 (el error es irreversible).
* **[2][0] = 1:** Si estamos en el Estado 2 (dígito encontrado) y la entrada no es un dígito, transita al Estado 1 (error).
* **[2][1] = 2:** Si estamos en el Estado 2 y la entrada es un dígito, permanece en el Estado 2.

* **Sigaction**

La estructura sigaction es utilizada para definir cómo un programa debe manejar señales. Esta estructura es clave cuando necesitas tener control preciso sobre qué hacer cuando el sistema operativo envía una señal a tu programa.

**Componentes principales de la estructura:**

**sa_handler o sa_sigaction:** El campo sa_handler define la función que será llamada cuando ocurra una señal. Alternativamente, puedes usar sa_sigaction para proporcionar información adicional sobre la señal recibida.
**sa_flags:** Un conjunto de banderas que modifican cómo se maneja la señal.
**sa_mask:** Define qué señales deben bloquearse mientras el manejador de señales está en ejecución.
**sa_flags**
Este campo en la estructura sigaction se utiliza para controlar cómo se deben manejar las señales. Algunas de las banderas más comunes son:

**SA_SIGINFO:** Indica que quieres recibir información adicional sobre la señal, permitiéndote usar sa_sigaction en lugar de sa_handler.

**SA_RESTART:** Hace que ciertas llamadas al sistema (como read, write, etc.) se reinicien automáticamente si fueron interrumpidas por una señal.

**SA_NODEFER:** Permite que la señal que está siendo manejada no sea bloqueada mientras el manejador está activo. Por defecto, cuando una señal es recibida, el sistema la bloquea hasta que el manejador termine.

**SA_RESETHAND:** Restaura el comportamiento por defecto de la señal después de que se haya manejado una vez. Esto significa que, tras manejar la señal, volverá a su comportamiento original.

**SA_NOCLDSTOP:** Impide que se genere una señal SIGCHLD cuando un proceso hijo se detiene o reanuda su ejecución (solo relevante para SIGCHLD).

# MINITALK

Minitalk es un proyecto desarrollado en C que implementa un sistema de comunicación entre un cliente y un servidor utilizando señales de Unix (como SIGUSR1 y SIGUSR2). El objetivo es que el cliente envíe mensajes al servidor, y el servidor los procese y responda de manera eficiente. 

## Client

* **1. Main**


		int main(int argc, char **argv)
		{
		    t_info client;
		    int msg_len;
		
		    (void)argc, (void)argv;
		    ft_printfd(2, "argcs: %d\n", argc);
		    validate_arguments(argc, argv);
		    initialize_client_data(argv, &client);
		    if (check_server_status(client.server_pid) == 0)
		        return (0);
		    msg_len = ft_strlen(argv[2]);
		    ft_printf("MSG_LEN: [%d]\n", msg_len);
		    send_message_bits(&msg_len, 32, &client);
		    transmit_message_to_server(client.msg, &client);
		    return (0);
		}

La función main gestiona la ejecución del programa. Primero, valida los argumentos de entrada mediante **validate_arguments.** Luego, inicializa los datos del **cliente** usando **initialize_client_data** y verifica si el **servidor** está disponible con **check_server_status**. Si el **servidor** está listo, se procede a enviar **el tamaño del mensaje** usando **send_message_bits**, seguido por la transmisión del mensaje completo con **transmit_message_to_server.**

**2. Validate_arguments**

		void validate_arguments(int argc, char **argv)
		{
		    if (argc != 3)
		        ft_print_error(USAGE);
		    evaluate_pid(argv[1]);
		}

**validate_arguments** valida que el número de argumentos sea exactamente 3 (el nombre del programa, el PID del servidor y el mensaje). Si no se cumple esta condición, se genera un error. Posteriormente, llama a la función evaluate_pid para analizar el primer argumento (el PID).

* **3. Función evaluate_pid**

		void evaluate_pid(const char *str)
		{
		    int i;
		    int state;
		
		    i = 0;
		    state = 0;
		    while (str[i])
		    {
		        state = choose_state(state, str[i]);
		        i++;
		    }
		    if (state < 2)
		        ft_print_error((char *)str);
		}

* **Evaluate_pid** recorre el primer argumento **(el PID del servidor)** carácter por carácter. Utiliza la función **choose_state** para verificar que sea un valor válido y, si el estado final no es adecuado, lanza un error.

* **4. Función choose_state**

		int choose_state(int state, char c)
		{
		    int pos;
		
		    pos = 0;
		    if (ft_isdigit(c))
		        pos = 1;
		    state = get_state(state, pos);
		    return (state);
		}

* **choose_state** decide cómo cambiar el estado de acuerdo al carácter que se está evaluando. Si el carácter es un **dígito,** se ajusta el valor de **pos**, que es usado por la función **get_state** para obtener el nuevo estado.

* **5. Función get_state**

		int get_state(int x, int y)
		{
		    const int states[][2] = {\
		        {1, 2}, \
		        {1, 1}, \
		        {1, 2}, \
		    };
		
		    return (states[x][y]);
		}

* **get_state** retorna el nuevo estado en base a los valores actuales de **x** y **y**. Utiliza una tabla predefinida que mapea estos valores a diferentes estados.

* **6. Función initialize_client_data**

		void initialize_client_data(char **argv, t_info *data)
		{
		    ft_memset(data, 0, sizeof(t_info));
		    data->server_pid = ft_atoi_limits(argv[1]);
		    data->client_pid = getpid();
		    ft_printfd(2, "PID CLIENT %d\n", data->client_pid);
		    data->msg = argv[2];
		    if (data->server_pid == 0)
		        ft_print_error(BAD_SIGNAL);
		}

* **initialize_client_data** inicializa la estructura **t_info** que contiene los datos del cliente. Esta estructura almacena el PID del servidor (convertido a entero desde el argumento), el PID del cliente, y el mensaje a enviar. Si el PID del servidor es inválido, se lanza un error.

* **7. Función check_server_status**

		int check_server_status(int pid)
		{
		    struct sigaction sa;
		
		    sa.sa_flags = SA_SIGINFO;
		    sa.sa_sigaction = server_state_signal_handler;
		    g_server.pid = pid;
		    g_server.is_ready = 0;
		    sigaction(SIGUSR1, &sa, NULL);
		    sigaction(SIGUSR2, &sa, NULL);
		    attempt_server_connection(pid);
		    ft_printf("Server ready: %d\n", g_server.is_ready);
		    return (g_server.is_ready);
		}

* **check_server_status** intenta conectarse al servidor enviando señales al **PID** proporcionado.

* **Configuración del Manejador de Señales:** Se inicializa una estructura sigaction para configurar el comportamiento de las señales **SIGUSR1** y **SIGUSR2.**
- Ambas señales se asocian con la función manejadora **server_state_signal_handler**, que procesará las respuestas del **server**.

* **Inicialización de Variables Globales:** **g_server.pid** se establece en el pid del server para usarlo como referencia al validar señales entrantes.
- **g_server.is_ready** se inicializa en 0 para reflejar que, inicialmente, el servidor no está listo.

* **Intento de Conexión:** La función llama a **attempt_server_connection(pid)** para iniciar el proceso de comunicación. **(VER EXPLICACIÓN DE attempt_server_connection)**

* **Salida:** Imprime el estado de disponibilidad del servidor y devuelve **g_server.is_ready**. Si el servidor respondió correctamente, **g_server.is_ready** será 1; de lo contrario, permanecerá en 0.

* **8. Función attempt_server_connection**

		void attempt_server_connection(int pid)
		{
		    int i;
		
		    i = -1;
		    while (++i < RETRY_TIMES)
		    {
		        kill(pid, SIGUSR1);
		        ft_printf("Waiting response from server\n");
		        sleep(RETRY_TIME);
		        if (g_server.is_ready == 1)
		            break;
		    }
		}

* **attempt_server_connection** envía señales de solicitud **(SIGUSR1)** al **servidor** y espera una respuesta por un tiempo determinado, definido por las constantes **RETRY_TIMES** y **RETRY_TIME**. Si el servidor responde con la señal **SERVER_READY**, se detiene el ciclo.

**Ciclo de Intentos:**
- **Envío de Señal:** Se utiliza **kill(pid, SIGUSR1)** para enviar la señal **SIGUSR1** al **server**, indicando una solicitud de conexión.

- **Esperar Respuesta:**
El programa espera un intervalo de tiempo **(RETRY_TIME)** antes de verificar si el servidor respondió.

- **Verificación del Estado:**
Si **g_server.is_ready** es igual a 1 (indicación de que el servidor está listo), el ciclo se interrumpe.

* **Límite de Intentos:**
El ciclo repite el proceso hasta alcanzar el máximo definido por **RETRY_TIMES.**
Si el servidor no responde después de los intentos permitidos, el proceso termina sin éxito.

* **9. Manejador de Señales del Servidor server_state_signal_handler**

		void server_state_signal_handler(int signum, siginfo_t *info, void *context)
		{
		    (void)signum, (void)context, (void)info;
		    if (info->si_pid == getpid())
		    {
		        ft_printfd(2, "Error: Own process\n");
		        exit(1);
		    }
		    if (info->si_pid != g_server.pid)
		    {
		        ft_printfd(2, "Error: Unexpected pid in server_state_signal_handler\n");
		        return;
		    }
		    if (signum == SERVER_READY)
		        g_server.is_ready = 1;
		    if (signum == SERVER_BUSY)
		        g_server.is_ready = 0;
		}

Este manejador de señales, **server_state_signal_handler**, responde a las señales recibidas del **servidor**. Si la señal es **SERVER_READY,** actualiza el estado de disponibilidad del servidor a 1. Si es **SERVER_BUSY** lo actualiza a **0**.

* **Validaciones Iniciales:**
- **Proceso Propio:** Si info->si_pid coincide con el PID del proceso actual (getpid()), la señal proviene de un error interno, y el programa se detiene con un mensaje de error.
- **PID No Esperado:** Si info->si_pid no coincide con g_server.pid, la señal proviene de un proceso no esperado, y se descarta con un mensaje de error.
* **Procesamiento de Señales:**
**SERVER_READY:**
Si la señal recibida indica que el servidor está listo, se establece g_server.is_ready = 1.
**SERVER_BUSY:**
Si el servidor está ocupado, se establece g_server.is_ready = 0.

* **10. Función send_message_bits**

		void send_message_bits(void *data, size_t bit_length, t_info *info)
		{
		    unsigned long long value;
		    int i;
		
		    value = 0;
		    if (bit_length == 8)
		        value = *((unsigned char *)data);
		    else if (bit_length == 32)
		        value = *((unsigned int *)data);
		    i = bit_length - 1;
		    while (i >= 0)
		    {
		        if (value & (1ULL << i))
		            send_signal(info->server_pid, CHAR_1);
		        else
		            send_signal(info->server_pid, CHAR_0);
		        i--;
		        usleep(500 * 500);
		    }
		}

**send_message_bits** convierte el dato (el **tamaño del mensaje** o un carácter del mismo) en una secuencia de **bits**, que luego son enviados al servidor usando señales **CHAR_1** o **CHAR_0**. Cada bit se evalúa y se envía de acuerdo a su valor (1 o 0).

11. Función transmit_message_to_server

	void transmit_message_to_server(char *str, t_info *data)
	{
	    struct sigaction sa;
	    int i;
	
	    sa.sa_flags = SA_SIGINFO;
	    sa.sa_sigaction = client_signal_handler;
	    sigaction(SIGUSR2, &sa, NULL);
	    i = 0;
	    while (str[i])
	        send_message_bits(&str[i++], 8, data);
	}

**transmit_message_to_server** recorre el mensaje carácter por carácter y llama a send_message_bits para transmitir cada uno de los caracteres, enviando cada bit individualmente.

* **12. Manejador de Señales del Cliente client_signal_handler**

		void client_signal_handler(int signum, siginfo_t *info, void *context)
		{
		    (void)signum, (void)context, (void)info;
		}

Este **manejador de señales**, aunque está vacío en la implementación actual, podría ser utilizado para manejar cualquier señal relevante que reciba el cliente durante la transmisión de datos.

* **13. Función send_signal**

		void send_signal(pid_t pid, int signal)
		{
		    if (kill(pid, signal))
		        ft_print_error("Signal sending failed.");
		}

**send_signal** envía una señal al **PID** proporcionado. Si falla en el intento de enviar la señal, se lanza un error.
  
## Server

* **1. Función main**

		int main(void)
		{
		    struct sigaction sa;
		    pid_t server_pid;
		
		    ft_memset(&g_client, 0, sizeof(t_global));  // Inicializa la estructura global a cero.
		    server_pid = getpid();  // Obtiene el PID del servidor.
		    ft_printfd(1, "Server PID: %d\n", server_pid);  // Imprime el PID del servidor.
		
		    sa.sa_flags = SA_SIGINFO;  // Establece que se utilizarán funciones de manejo de señales con información adicional.
		    sa.sa_sigaction = server_signal_handler;  // Asigna el manejador de señales.
		
		    // Configura los manejadores de señales para SIGUSR1 y SIGUSR2.
		    sigaction(SIGUSR1, &sa, NULL);
		    sigaction(SIGUSR2, &sa, NULL);
		
		    keep_server_up();  // Llama a la función que mantiene al servidor en ejecución.
		    return (0);
		}

* **ft_memset(&g_client, 0, sizeof(t_global));:** Inicializa la estructura global g_client para evitar datos basura.
* **getpid();:** Obtiene el PID del proceso del servidor.
* **sigaction():** Configura los manejadores de señales para que **server_signal_handler** maneje **SIGUSR1** y **SIGUSR2.**
* **keep_server_up();:** Llama a la función que mantiene el servidor en ejecución.


* **2. Función keep_server_up**

		void keep_server_up(void)
		{
		    while (1)
		    {
		        sleep(1);  // Mantiene el servidor en un ciclo infinito, durmiendo por un segundo en cada iteración.
		    }
		}

**while (1):** Crea un bucle infinito que permite que el servidor siga funcionando.
**sleep(1);:** Hace que el servidor "duerma" un segundo en cada iteración, evitando el uso innecesario de CPU.
4. Función lost_signal


* **int lost_signal(int s_si_pid, int signum, int * i, void * context)**

		{
		    (void)context;  // Evita advertencias sobre variable no utilizada.
		    if (s_si_pid == 0 && (signum == SIGUSR1 || signum == SIGUSR2))
		    {
		        ft_printf("i: [%d] client: %d with signal: %d\n", (*i), s_si_pid, signum);
		        s_si_pid = g_client.actual_pid;  // Asigna el PID actual del cliente.
		    }
		    return (s_si_pid);
		}

* **lost_signal():** Esta función maneja la lógica para asignar el PID del cliente cuando se recibe una señal.
Imprime información sobre la señal y el índice actual (i) si el PID del cliente es 0.

* **5. Función server_signal_handler**

		void server_signal_handler(int signum, siginfo_t *info, void *context)
		{
		    static int i;  // Variable estática para llevar el conteo de señales recibidas.
		
		    (void)context;  // Evita advertencias sobre variable no utilizada.
		    info->si_pid = lost_signal(info->si_pid, signum, &i, context);  // Actualiza el PID del cliente.
		
		    if (info->si_pid == getpid())
		        ft_print_error("Own process");  // Verifica si se recibe señal de sí mismo.
		
		    g_client.client_pid = info->si_pid;  // Guarda el PID del cliente.
		
		    if (g_client.actual_pid == 0)
		    {
		        check_client_status(g_client.client_pid);  // Verifica el estado del cliente.
		        return;
		    }
		
		    if (g_client.actual_pid != g_client.client_pid)
		        return;  // Si el PID no coincide, termina la función.
		
		    if (g_client.getting_header == 1)
		        header_handler(&i, signum);  // Maneja el encabezado del mensaje.
		    else if (g_client.getting_msg == 1)
		        msg_handler(&i, signum);  // Maneja el cuerpo del mensaje.
		
		    if (g_client.client_pid != 0 && (signum == SIGUSR1 || signum == SIGUSR2))
		        kill(g_client.client_pid, SIGNAL_RECEIVED);  // Envía señal al cliente de que se recibió correctamente.
		}

* **server_signal_handler():** Maneja las señales recibidas del **cliente**. Utiliza el **PID del cliente** para actualizar el estado y decidir si se deben manejar encabezados o mensajes.
Las **señales recibidas **(SIGUSR1** y **SIGUSR2)** son procesadas y se verifican condiciones relacionadas con el estado del **cliente.**

* **6. Función check_client_status**

		int check_client_status(int pid)
		{
		    kill(pid, SERVER_READY);  // Envía señal de que el servidor está listo para recibir.
		    g_client.actual_pid = pid;  // Actualiza el PID actual.
		    g_client.getting_header = 1;  // Establece que se está recibiendo el encabezado.
		    return (EXIT_SUCCESS);
		}
		check_client_status(): Notifica al cliente que el servidor está listo para recibir mensajes, actualiza el estado y establece que se está recibiendo el encabezado.
		

* **7. Función get_signal_bit**

		int get_signal_bit(int signum)
		{
		    if (signum == SIGUSR1)
		        return (0);  // Devuelve 0 para SIGUSR1.
		    return (1);  // Devuelve 1 para SIGUSR2.
		}
		get_signal_bit(): Convierte la señal recibida en un valor de bit (0 o 1) para su procesamiento posterior.


* **8. Función allocate_memmory_for_message**

		void allocate_memmory_for_message(int *i)
		{
		    printf("SIZE_MSG: [%d]\n", g_client.msg.size_message);
		    g_client.msg.message = ft_calloc((g_client.msg.size_message + 1), 1);  // Reserva memoria para el mensaje.
		    if (g_client.msg.message == NULL)
		        ft_print_error("Memory allocation failed");  // Manejo de errores en caso de falla.
		    g_client.getting_header = 0;  // Cambia el estado.
		    g_client.getting_msg = 1;  // Indica que se está recibiendo el mensaje.
		    (*i) = 0;  // Reinicia el índice.
		}
		allocate_memmory_for_message(): Reserva memoria para el mensaje según el tamaño especificado. Maneja errores en caso de falla en la asignación de memoria.
		

* **9. Función header_handler**

		void header_handler(int *i, int signum)
		{
		    const int bit_value = get_signal_bit(signum);  // Obtiene el valor de bit de la señal.
		
		    if ((*i) < HEADER_SIZE)
		    {
		        g_client.msg.size_message |= (bit_value << (HEADER_SIZE - 1 - (*i)));  // Construye el tamaño del mensaje en bits.
		        (*i)++;
		    }
		    if ((*i) == HEADER_SIZE)
		        allocate_memmory_for_message(i);  // Llama a la función para asignar memoria si se ha recibido el encabezado completo.
		}

**header_handler():** Maneja la recepción del encabezado del mensaje. Si se ha recibido el tamaño completo, llama a la función que asigna memoria para el mensaje.


* **10. Función msg_handler**

		void msg_handler(int *i, int signum)
		{
		    const int bit_value = get_signal_bit(signum);  // Obtiene el valor de bit de la señal.
		    static int char_value;  // Almacena el carácter en construcción.
		    static int msg_pos;  // Posición actual en el mensaje.
		
		    if (*i % 8 < 8)
		    {
		        char_value |= (bit_value << (7 - (*i % 8)));  // Agrega el bit al carácter actual.
		        (*i)++;
		    }
		    if (*i % 8 == 0)
		    {
		        g_client.msg.message[msg_pos] = char_value;  // Almacena el carácter completo en el mensaje.
		        char_value = 0;  // Reinicia el valor del carácter.
		        msg_pos++;  // Avanza a la siguiente posición del mensaje.
		    }
		    if (*i / 8 == g_client.msg.size_message)
		    {
		        printf("message: [%s]\n", g_client.msg.message);  // Imprime el mensaje completo.
		       free(g_client.msg.message);  // Libera la memoria del mensaje.
	        g_client.msg.message = NULL;  // Reinicia el puntero.
	        g_client.getting_msg = 0;  // Cambia el estado a que no se está recibiendo un mensaje.
	        g_client.getting_header = 1;  // Indica que se puede recibir otro encabezado.
	        g_client.actual_pid = 0;  // Reinicia el PID actual.
	    }
	}
* **msg_handler():** Maneja la recepción del cuerpo del mensaje. Combina los bits recibidos en caracteres y los almacena en el mensaje. Cuando el mensaje completo se ha recibido, se imprime y se libera la memoria.


