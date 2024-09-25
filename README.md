# Client

    int main	(int argc, char **argv)
    {
    (void)argc, (void)argv;
    int msg_len;
    t_info data;
    
    printf("Número de argumentos recibidos: %d\n", argc);
    parser(argc, argv);
    init_data(argv, &data);
    if (ping(data.server_pid) == 0) {
        printf("El servidor no está listo.\n");
        return (0);
    }
    msg_len = ft_strlen(argv[2]);
    printf("MSG_LEN: [%d]\n", msg_len);
    send_signals(&msg_len, 32, &data);
    printf("Enviando mensaje...\n");
    send_message(data.message, &data);
    printf("Mensaje enviado correctamente\n");
    return (0);
    }

**(void)argc, (void)argv:** Se utiliza para evitar advertencias del compilador sobre los parámetros argc y argv si no se utilizan directamente en el código.

**int msg_len:** Esta variable almacenará la longitud del mensaje que se quiere enviar al servidor.

**t_info data:** Esta estructura de tipo t_info, definida en el proyecto, contiene información relevante para la comunicación entree el cliente y el servidor:

* El PID del server.
* El mensaje a enviar
* Otra información necesaria para manejar las señales y los mensajes.

**parser(argc, argv):** ( **void   parser(int argv, char ** argv):** Esta función valida los argumentos de entrada del programa, asegurándose de que el número de argumentos sea **argc** sea **3**  y que el **server_pid** sea válido. Esta función garantiza una correcta comunicación entre **client**y **server**, evitando errores que podrían afectar el **envío de señales** y la **reconstrucción del mensaje**.



    int	get_state(int x, int y)
    {
	    const int	states[][2] =
	    {
		    {1, 2},
		    {1, 1},
		    {1, 2},
	    };

	    return (states[x][y]);
        }

    int	choose_state(int state, char c)
    {
	    int	pos;

	    pos = 0;
	    if (ft_isdigit(c))
	    {
		    pos = 1;
	    }
	    state = get_state(state, pos);
	    return (state);
    }

    void	evaluate(const char *str)
    {
	    int	i;
	    int	state;

	    i = 0;
	    state = 0;
	    while (str[i])
	    {
		    state = choose_state(state, str[i]);
		    i++;
	    }
	    if (state < 2)
	    {
		    ft_print_error((char *)str);
	    }
    }

    void	parser(int argc, char **argv)
    {
	    if (argc != 3)
		    ft_print_error(USAGE);
	    evaluate(argv[1]);
    }

**if (argc != 3):** Esta línea verifica si la cantidad de argumentos recibido es exactamente 3:

* **argv[0]:** Nombre del Programa: **client_pid**)
* **argv[1]:** **server PID**
* **argv[2]:** **Message**

* Si **[argc]** es diferente de **3**, el programa **imprime** un **mensaje de error** y se detiene la ejecución.
* Si el número de argumentos es correcto, la función llama a **evaluate** (**const char * str)** **(argv[1])** que representa el **server PID** (**pid_t s_info::server_pid**) y evalúa si este es válido.

**int state** es el estado inicial de la evaluación. Se usa en conjunto con **choose_state** para moverse entre diferentes estadoss, lo que ayuda a verificar la válidez del PID.
- En cada iteración del bucle, **chose_state** se llama con el **estado actual** y el **carácter actual** de la cadena. Esta función verifica si el carácter es un dígito utilizando la función **ft_isdigit**. Si es así, se establece en **pos == 1**.

- **get_state** se llama con el **estado actual** y el **carácter actual**. Esto determina el nuevo estado del autómata bassándose en si el carácter es un dígito o no.

* **const int states[][2] = {...}:** Es una matriz de enteros de 2 filas y 2 columnas. Cada fila corresponde a un estado, y cada columna corresponde al valor de **pos**(0 o 1).
- **{1, 2}:** En el **Estado 0**, si el carácter no es un dígito **(pos = 0)** el autómata se mueve al **Estado 1**. Si es un dígito **(pos = 1)**, el autómata se mueve al **Estado 2**.
- **{1, 1}:** En el **Estado 1**, siempre se mueve al **Estado 1**, independiente de si el carácter es un dígito o no.
- **{1, 2}:** En el **Estado 2**, si el carácter no es un digito **(pos = 0)**, el aútomata se mueve al **Estado 1**. Si es un dígito, se mantiene el **Estado 2**.

**init_data(argv, &data):** Esta función inicializa la estructura **t_info** con los valores necesarios para comunicar a **client** y **server**, en este caso **Client PID**, **Server PID**, evaluando la validéz de este, confirmando la validez de este, y **Message**.  

	void	init_data(char **argv, t_info *data)
	{
		ft_memset(data, 0, sizeof(t_info));
		ft_printf("Inicializando datos...\n");
		data->client_pid = getpid();
		ft_printf("Obteniendo PID del client: %d\n", data->client_pid);
		data->server_pid = ft_atoi_limits(argv[1]);
		ft_printf("Convirtiendo PID del servidor: %s\n", argv[1]);
		data->message = argv[2];
		ft_printf("Mensaje asignado: %s\n", data->message);
		if (data->server_pid == 0)
		ft_print_error("PID del servidor es inválido.");
	}
- Se inicializa la memoria de la **struct** a cero ( **void * ft_memset(void * b, int c, size_t len** ). Esto es crucial para evitar comportamientos inesperados por causa de datos no inicializados y garantiza que todos los campos comiencen con valores predeterminados.
- Obtiene el **client PID**, así se puede identificar a **client** durante la comunicación con **server**.
- Convierte el **argv[1]**, en este caso **server PID**, de ASCI a Integer(**int ft_atoi limits(const char * s)**). Esto es esencial para poder enviar señales al **server** y asegurar que el **server PID** se maneje correctamente en formato de Integer.
- Asigna el **argv[2]** a **Message** (**char * s_info::message**).
- Verifica si el **server PID** es cero, y de ser así muestra un mensaje de error(**Invalid server PID**).

**if (ping(data.server_pid) == 0):** Esta función tiene la tarea de enviar señales al **server** y verificar si está listo para recibir **Message**.

	void	ping_handler(int signum, siginfo_t *info, void *context)
	{
		(void)signum, (void)context, (void)info;
		ft_printf("Señal recibida de PID: %d\n", info->si_pid);
		if (info->si_pid == getpid())
		{
			ft_printfd(2, "Error: Own process\n");
			exit(1);
		}
		if (info->si_pid != g_server.pid)
		{
			ft_printfd(2, "Error: Unexpected pid in ping_handler\n");
			return ;
		}
		if (signum == SERVER_READY)
		{
			g_server.is_ready = 1;
			ft_printf("Servidor listo (SIGUSR1 recibido)\n");
		}
		if (signum == SERVER_BUSY)
		{
			g_server.is_ready = 0;
			ft_printf("Servidor no listo (SIGUSR2 recibido)\n");
		}
	}

	void	handle_timeouts(int pid)
	{
		int	i;

		i = -1;
		while (++i < RETRY_TIMES)
		{
			kill(pid, SIGUSR1);
			ft_printf("Esperando respuesta del servidor\n");
			sleep(RETRY_TIME);
			if (g_server.is_ready == 1)
				break ;
		}
	}
	int ping	(int pid)
	{
    	    struct sigaction sa;

    	    printf("Configurando ping para el servidor con PID: %d\n", pid);
	    sa.sa_flags = SA_SIGINFO;
    	    sa.sa_sigaction = ping_handler;
    	    g_server.pid = pid;
    	    g_server.is_ready = 0;
   	    sigaction(SIGUSR1, &sa, NULL);
    	    sigaction(SIGUSR2, &sa, NULL);
    	    handle_timeouts(pid);
            printf("Server ready: %d\n", g_server.is_ready);
            return (g_server.is_ready);
       }
**int ping(int pid):** Esta función se encarga de configurar el entorno para verificar si el **server** está disponible. Devuelve un **integer** que indica el estado del **server** **(listo o no listo)**

* **struct sigaction sa:** Es una estructura definida por POSIX que se utiliza para especificar cómo un proceso debería manejar una señal específica. La estructura contiene varios campos que configuran el comportamiento del manejador de señales.

* **sa.sa_flags = SA_SIGINFO:** Esta línea establece la bandera **SA_SIGINFO** en el campo **sa_flags** de la **struct sigaction sa** en lugar de la más simple **sa_handler**

* **sa_sigaction = ping_handler:** Aquí se asigna la función **ping_handler** como el **manejador de señales** avanzado. Esta función se invocará cuando el programa reciba una **señal**, y debido a la activación de la bandera **SA_SIGINFO**, el manejador podrá recibir información adicional sobre la señal a través de los parámetros **siginfo_t * info** y **void * context**.

		void ping_handler(int signum, siginfo_t * info, void * context): 
 
- **int signum:** Es el número de la señal recibidaa (como SIGUSR1 o SIGUSR2).
**siginfo_t * info:** contiene información adicional sobre la señal, como el PID del proceso que la envió.
- **void * context:** Provee el contexto aicional para la ejecución, pero en este caso no se utiliza.

- **info->si_pid == getpid()):** Se verifica si el proceso que recibió la señal es el mismo que la envió. Si esto ocurre, se imprime un mensaje de error y se termina el programa con **exit(1)**. Esto es para evitar que el proceso se envíe señales a sí mismo, lo cual probablemente sería un error.

- **if (info->si_pid != g_server.pid):** Comprueba si el **PID** del remitente de la señal no coincide con el **PID** del **server**. Si no coinciden se imprime un mensaje de error y se retorna sin hacer más nada, esto debido a que solo el **server** debe enviar las señales a este manejador.

- **if (signum == SERVER_READY):** Si el número de **signum** es igual a **SERVER_READY**, entonces el programa actualiza el estado de **g_server.is_ready** a **1** y se imprime un mensaje confirmado que el servidor ha enviado la señal de estar listo.

- **if (signum == SERVER_BUSY):** Si la señal recibida es **SERVER_BUSY**(probablemente **SIGUSR2**), se actualiza el estado de **g_server.is_ready** a 0 (indicando que el servidor no está listo o está ocupado), y se imprime un mensaje que refleja esta condición.

* **g_server.pid = pid:** Establece el **server PID** al que se va a enviar el **ping** en la variable global **g_server.pid**. Esto es importante porque **ping handler** usará esta variable para verificar si las señales recibidas provienen del **server** correcto.

**g_server.is_ready = 0:** Se establece en **0**, lo que indica que el**server** no está listo inicialmente.

**sigaction(SIGUSR1, &sa, NULL):**
**sigaction(SIGUSR2, &sa, NULL):**

- Se utilizan dos llamadas a **sigaction** para asociar las señales **SIGUSR1** y **SIGUSR2** con el manejador de señales **ping_handler**, que se configuró previamente en la estructura **sa**.
- La primera llamada establece que **SIGUSR1** debe ser manejada por **ping_hanndler**, y la segunda hace lo mismo para **SIGUSR2**.
- La tercera variable **NULL** indica que no se necesita recuperar la configuración anterior para estas señales.

**handle_timeouts(pid):** Esta función envía señales **SIGUSR1** al **server** **RETRY_TIMES** y espera una respuesta durante un tiempo determinado. Durante este proceso, el programa está esperando que el **server** responda con la señal adecuada **(SIGUS1 O SIGUSR2)**, actualizando **g_server.is_ready**.

**msg_len = ft_strlen(argv[2]);** Aquí se calcula la longitud del **message** **argv[2]** que se quiere enviar al **server**.

**send_signals(&msg_len, 32, &data);** 

	void	send_signals(void *data, size_t bit_length, t_info *info)
	{
		int					i;
		unsigned long long	value;

		value = 0;
		if (bit_length == 8)
			value = *((unsigned char *)data);
		else if (bit_length == 32)
			value = *((unsigned int *)data);
		i = bit_length - 1;
		while (i >= 0)
		{
			if (value & (1ULL << i))
			{
			send_signal(info->server_pid, CHAR_1);
			}
			else
			{
			send_signal(info->server_pid, CHAR_0);
			}
			i--;
			usleep (100);
	}
	}

* Esta función toma el valor de **msg_len**, lo castea a **8** o **32** **bits** y lo envía **bit a bit** al **server**.
* **Envía los bits de un valor (ya sea **msg_len** o los **caracteres**) uno por uno, usando **send_signal()**. Dependiendo del valor del **bit**, envía **SIGUSR1** (para un **1**) o **SIGUSR2** (para un **0**).
- Usa **send_signal(pid_t pid, int signal)** para envíar **señales** al proceso del **server** utilizando **kill()**. Dependiendo del **bit**, envía **SIGUSR1** o **SIGUSR2**.
- Se usa **usleep()** para hacer una pequeña pausa entre el envío de cada **bit**, permitiendo que el **server** procese las señales.

		**send_message(data.message, &data);**

		void	send_message(char *str, t_info *data)
		{
			struct sigaction	sa;
			int					i;

		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = signal_handler;
		sigaction(SIGUSR2, &sa, NULL);
		i = 0;
		while (str[i])
			send_signals(&str[i++], 8, data);
		}

* **Después de enviar la longitud del mensaje, se envía la longitud del mensaje en sí:**
  - **send_message** toma cada carácter del **message**, lo convierte a **8 bits** y llama a **send_signals()** para enviar esos **bits** uno por uno.
  - La función también configura el **client_signal_handler** para manejar señales entrantes durante el proceso de envío.

		void	client_signal_handler(int signum, siginfo_t *info, void *context)
		{
			(void)signum;
			(void)info;
			(void)context;
		}

* Esta función aquí no tiene ninguna funcionalidad más allá de **recibir** una **señal** y **"silenciar"** los parámetros que se le pasan.
  

# Server

	int	main(void)
	{
		struct sigaction	sa;
		pid_t				server_pid;
	
		ft_memset(&g_client, 0, sizeof(t_global));
		server_pid = getpid();
		ft_printfd(1, "Server PID: %d\n", server_pid);
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = signal_handler;
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
		keep_server_up();
		return (0);
	}

Es una estructura definida por POSIX que se utiliza para especificar cómo un proceso debería manejar una señal específica. La estructura contiene varios campos que configuran el comportamiento del manejador de señales.

	**struct sigaction sa:**


Es un tipo de dato que se utiliza para representar un ID de proceso (PID) en sistemas Unix y Linux. Cada proceso en el sistemaa tiene un PID único que lo identifica.

	**pid_t server_pid;** **pid_t** 
  
Esta línea inicializa la estructura global **g_client** a cero (**ft_memset**).

	**ft_memset(&g_client, 0, sizeof(t_global));**
 
Se obtiene el **PID** del proceso actual(**server**) usando **get_pid** y almacenándolo en la variable **server_pid**.

	**server_pid = getpid();**
  
Esta línea establece la bandera **SA_SIGINFO** en el campo **sa_flags** de la **struct sigaction sa** en lugar de la más simple **sa_handler**

	**sa.sa_flags = SA_SIGINFO:**
 
Se asigna la función **server_signal_handler** como el manejador de señales que será invocado cuando se reciba una señal.

	**sa.sa_sigaction = server_signal_handler;**
	
 - **sa_sigaction** permite manejar señales con más información, por ejemplo, de qué proceso provino la señal. Es un puntero a la función **server_signal_handler** que se ejecutará cuando se reciba una señal. Esta es la función **manejadora de señales** del **server**.

			void	server_signal_handler(int signum, siginfo_t *info, void *context)
			{
				static int	i;
			
				(void)context;
				info->si_pid = lost_signal(info->si_pid, signum, &i, context);
				if (info->si_pid == getpid())
					ft_print_error("own process");
				g_client.client_pid = info->si_pid;
				if (g_client.actual_pid == 0)
				{
					pong(g_client.client_pid);
					return ;
				}
				if (g_client.actual_pid != g_client.client_pid)
					return ;
				if (g_client.getting_header == 1)
					handle_header(&i, signum);
				else if (g_client.getting_msg == 1)
					handle_msg(&i, signum);
				else if (g_client.client_pid != 0
					&& (signum == SIGUSR1 || signum == SIGUSR2))
				{
					kill(g_client.client_pid, SIGNAL_RECEIVED);
				}
			}	


  - **signum** es el número de la señal recibida (puede ser **SIGUSR1** o **SIGUSR2**).
  - **info** es un puntero a una estructura **siginfo_t** que contiene información adicional sobre la señal, como el **PID** del proceso que envió la **señal**.
  - **context** es información sobre el contexto del procesador cuando ocurrió la señal(no se utiliza aquí).

* **static int i;** Declara una **variable estática i**, que retiene su valor entre llamadas consecutivas a la función. Esta variable puede estar actuando como un contador o índice para seguir el progreso de la recepción de un **mensaje**.

Esta función se utiliza para recuperar el **PID** **(Process ID)** del proceso emisor de una señal.
	
 	**info->si_pid = lost_signal(info->si_pid, signum, &i, context);** 

 - Si el **PID** proporcionando por el sistema es **0**, la función asigna el **PID** almacenado previamente en **g_client.actual_pid** como el emisor, asegurando así que el servidor pueda continuar el proceso de comunicación con el **client** correcto.
  
	
			int	lost_signal(int sender_pid, int signum, int *i, void *context)
				{
					(void)context;
					if (sender_pid == 0 && (signum == SIGUSR1 || signum == SIGUSR2))
					{
						printf("i [%d] client: %d con señal: %d\n", (*i), sender_pid, signum);
						sender_pid = g_client.actual_pid;
					}
					return (sender_pid);
				}
* **Parámetros:**

  - * **sender_pid:** El **PID** del proceso que envió la **señal**.
    * **signum**: El número de la **señal** que se ha recibido (ya sea **SIGUSR1** o **SIGUSR2**.)
    * ***i** Es un puntero a un entero, contador de señales o índice para rastrear el progreso en el proceso de recepción de datos.
    * **context:** Un parámetro adicional que no se utiliza en esta función, por lo que se anua co **(void)context**.
      
* Si **(sender_pid == 0** y la **señal** recibida es **SIGUSR1** o **SIGUSR2**, se ejecutará el bloque de código dentro del **if**.
	- Que **sender_pid** sea igual a **0**, puede ocurrir si por alguna razón, el sistema no proporciona el **PID** del **proceso emisor de la señal**.

Si **sender_pid** es 0, se asigna **g_client.actual_pid** a **sender_pid**. Esto significa que si no se puede determinar el **PID** del proceso emisor, el **server** utiliza el **actual_pid** almacenado en la estructura **g_client** para identificar al client. 

	**sender_pid = g_client.actual_pid;**

La función devuelve el **sender_pid**, ya sea el que originalmente se proporcionó o el que se obtuvo de **g_client.actual_pid** si el original era **0**.

	**return (sender_pid):** 

 
Vuelve a **server_signal_handler**. 

Se verifica si el proceso que envió la señal es el mismo **server**.

	* **if (info->si_pid == getpid()):** 
 
- El **server** guarda el **PID** del **client** que envió la **señal** en la variable gobal **g_client.client_pid**. 
	- Esto asegura que el **server** recuerde quien está enviando las **señales** para el resto del procesammiento.

			* **g_client.client_pid = info->si_pid;** 

*  Si **g_client.actual_pid** es 0, significa que el **server** no ha establecido aún una conexión firme con el **client**.
	- En este caso, el **server** responde con **pong()** al **client**, que es una forma de **confirmmar que el cliente está conectado** y que el **server** está listo para recibir datos.

			**if (g_client.actual_pid == 0)** 
			{
				pong(g_client.client_pid);
				return ;
			}

La función **pong** es una forma de **confirmar la recepción de la señal del cliente por parte del servidor** y preparar al servidor para iniciar el proceso de comunicación con el **client**

- Envía la señal **SIGUSR1** al **client** para informarle que el **server** está listo para procesar el mensaje. 

  		**kill(pid, SERVER_READY):**

- Almacena el **pid** del **client** en la variable global **g_client.actual_pid**, que es parte de la estructura **g_client**. Esto le permite al servidor "recordar" con qué cliente está interactuando durante la sesión actual. 

		**g_client.actual_pid = pid:**

- Esto indica que el **server** ha comenzado a recibir el mensaje y está esperando la primera parte de él (**header**).
  	- Se utiliza para gestionar el estado del servidor y asegurarse de que los datos recibidos se procesen de manera ordenada (primero el **header**, luego el cuerpo de **message**)


* Esta condición verifica si el proceso **PID** del **client** actual es igual al **PID** del client que envió la señal. Si no coinciden, el **server** no hará nada y saldrá de la función. Esto asegura que el **server** solo procese señales del **client** con el que está interactuando activamente.

		**if (g_client.actual_pid != g_client.client_pid)
			return ;**
  
Aquí, el **server** está esperando recibir la cabecera del **mensaje** (**getting_header == 1**), llama a la función **handler_header** para procesar las señales que forman el encabezado del mensaje. 

	if (g_client.getting_header == 1)
	    handle_header(&i, signum);

**función void_handler**

	void	handle_header(int *i, int signum)
	{
		const int	bit_value = get_bit_value(signum);
	
		if ((*i) < HEADER_SIZE)
		{
			g_client.msg.size_msg |= (bit_value << (HEADER_SIZE - 1 - (*i)));
			(*i)++;
		}
		if ((*i) == HEADER_SIZE)
			reserve_memory_for_msg(i);
	}


- Esta función convierte la señal recibida (signum) en un valor binario (0 o 1). Dependiendo de si la señal es SIGUSR1 o SIGUSR2, interpretamos si el bit de la señal es 0 o 1.

		**get_bit_value(signum):**


- Aquí se está construyendo el tamaño del mensaje (almacenado en size_msg) bit por bit. HEADER_SIZE es una constante que indica el número total de bits en el encabezado (probablemente 32 o 64 bits, dependiendo del tamaño de los mensajes).

		**if ((* i) < HEADER_SIZE):** 

-  Esta línea inserta el bit recibido en la posición correcta de size_msg, moviendo el valor recibido al lugar adecuado mediante un desplazamiento a la izquierda.

		**g_client.msg.size_msg |= (bit_value << (HEADER_SIZE - 1 - ( *i))):** 

		**( * i)++:** 

Una vez que se han recibido todos los bits del encabezado (cuando el número de bits recibidos es igual a HEADER_SIZE), se llama a reserve_memory_for_msg para reservar la memoria necesaria para almacenar el mensaje.

	**if (( * i) == HEADER_SIZE):***

* Si el servidor ya ha procesado el encabezado y está esperando el mensaje (cuando **getting_msg == 1)**, llama a la función **handle_msg** para procesar las señales que contienen los bits del mensaje real.

		* else if (g_client.getting_msg == 1)
	    	handle_msg(&i, signum);










* **sigaction(SIGUSR1, &sa, NULL);** Esta línea registra el **manejador de señales** para la **señal** **SIGUSR1**, la función **server_signal_handler** se ejecutará. 
	- Es una llamada al sistema que modifica el comportamiento del proceso al recibir una señal. Aquí estamos diciendo que, para la señal **SIGUSR1**, se debe ejecutar la función **server_signal_handler** cuando la señal sea capturada.

* **sigaction(SIGUSR2, &sa, NULL);** Esta línea hace lo mismo que la anterior, pero para la señal **SIGUSR2**. Ahora el servidor está preparado para manejar tanto **SIGUSR1** como **SIGUSR2** con la misma función **server_signal_handler**.

* keep_server_up();







**Glosario**

## Tabla de estados

Una tabla de estados es una herramienta común en la programación que se usa en la implementación de **máquinas de estado**. Una *máquina de estado* es un modelo matemático que se usa para diseñar sistemas que cambian de estado en respuesta de ciertos eventos.
- Una tabla de estados es una matriz o un array que define las stransiciones entre diferentes estados de un sistema. Cada entrada en la tabla indica el próximo estado que se debe alcanzar dado el estado actual y una entrada específica.
- **Estados:** Representan diferentes condiciones o situaciones en las que el sistema puede encontrarse.
- **Entradas/Acciones:** Son eventos o inputs que provocan un cambio de estado.
- **Transiciones:** Definen cómo el sistema cambia de un estado a otra en función de las entradas.

## sigaction

La estructura sigaction es utilizada para definir cómo un proceso maneja señales específicas. Es una estructura que contiene información sobre cómo manejar una señal. Los miembros más importantes de esta estructura son:

**sa_handler o sa_sigaction:** Aquí puedes especificar una función que será llamada cuando una señal sea recibida.
**sa_flags:** Establece ciertas opciones sobre cómo se debe manejar la señal (por ejemplo, si se debe usar el manejador de señales extendido o no).
**sa_mask:** Define qué otras señales deben bloquearse mientras se está ejecutando el manejador de señales.

## sa_flags

El campo s**a_flags** en la estructura **sigaction** es un conjunto de opciones que modifican el comportamiento del **manejador de señales**. Algunas de las banderas más comunes que se pueden configurar en sa_flags incluyen:

### SA_SIGINFO

- Es una bandera utilizada en el contexto de la función ***sigaction*** para especificar cómo debe manejarse unaa señal en un programa C. Es parte del **estándar POSIX***.
  
- Su propósito es indicar que se desea usar una función manejadora de señales más avanzada que provea información adicional sobre la señal, en lugar de solo usar un simple manejador de señales (es decir, solo pasando el número de la señal).
  
**SA_RESTART:** Hace que algunas llamadas al sistema (como read, write, etc.) que son interrumpidas por la señal se reinicien automáticamente.

**SA_NODEFER:** Permite que la señal que está siendo manejada no sea bloqueada mientras el manejador está activo.

**SA_RESETHAND:** Restaura el comportamiento por defecto de la señal después de que esta haya sido manejada una vez, es decir, después de que se ejecute el manejador, la señal vuelve a su comportamiento original.

**SA_NOCLDSTOP:** Impide que se genere una señal SIGCHLD cuando un proceso hijo se detiene o continúa (solo relevante para SIGCHLD).
