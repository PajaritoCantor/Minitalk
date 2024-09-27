# MINITALK

## Client

**t_global	g_server;**

Esta variable global **g_server** es utilizada en el cliente para almacenar información sobre el estado del servidor y para gestionar la conexión y comunicación con el servidor.

**g_server.pid:** Este campo almacena el PID del servidor al que el cliente se conecta. El cliente lo utiliza para enviar señales (por ejemplo, confirmaciones y mensajes) al servidor.

**g_server.is_ready:** Indica si el servidor está listo para recibir un mensaje del cliente. Este valor cambia según las señales que el servidor envía al cliente:

Cuando se recibe la señal SIGUSR1 (definida como SERVER_READY), esta variable se pone a 1, indicando que el servidor está listo para recibir el mensaje.
Cuando se recibe la señal SIGUSR2 (definida como SERVER_BUSY), esta variable


**1. Main**

		int	main(int argc, char **argv)
		{
			t_info	client;
			int		msg_len;
		
			(void)argc, (void)argv;
			ft_printfd(2, "argcs: %d\n",argc);
			parser(argc, argv);
			init_client_data(argv, &client);
			if (check_server_status(client.server_pid) == 0)
				return (0);
			msg_len = ft_strlen(argv[2]);
			ft_printf("MSG_LEN: [%d]\n", msg_len);
			send_message_bits(&msg_len, 32, &client);
			transmit_message_to_server(client.msg, &client);
			return (0);
		}
Esta función se encarga de gestionar la ejecución del programa. Primero, verifica los argumentos de entrada mediante **parser**. Luego, inicializa los datos del cliente usando **init_client_data**, y verifica si el servidor está disponible con **check_server_status**. Si el servidor está listo, se procede a enviar el tamaño del mensaje usando **send_message_bits**, seguido por la transmisión del mensaje completo con **transmit_message_to_server.**

**2. Parser**

	void	parser(int argc, char **argv)
	{
		if (argc != 3)
			ft_print_error(USAGE);
		evaluate(argv[1]);
	}

**Parser** valida que el número de argumentos sea exactamente **3** (el nombre del programa, el PID del servidor y el mensaje). Si no se cumple esta condición, se genera un error. Posteriormente, llama a la función **evaluate** para analizar el primer argumento (el PID).

**3. Evaluate**

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
			ft_print_error((char *)str);
	}
**Evaluate** recorre el primer argumento (el PID del servidor) carácter por carácter. Utiliza la función **choose_state** para verificar que sea un valor válido y, si el estado final no es adecuado, lanza un error.

**4. Choose_state**

		int	choose_state(int state, char c)
		{
			int	pos;
		
			pos = 0;
			if (ft_isdigit(c))
				pos = 1;
			state = get_state(state, pos);
			return (state);
		}
**Choose_state** decide cómo cambiar el estado de acuerdo al carácter que se está evaluando. Si el carácter es un **dígito**, se ajusta el valor de pos, que es usado por la función **get_state** para obtener el nuevo estado.

**5. Get_state**

	int	get_state(int x, int y)
	{
		const int	states[][2] = {\
		{1, 2}, \
		{1, 1}, \
		{1, 2}, \
		};
	
		return (states[x][y]);
	}

**Get_state** retorna el nuevo estado en base a los valores actuales de x y y. Utiliza una tabla predefinida que mapea estos valores a diferentes estados.

**6. Init_client_data**

	void	init_client_data(char **argv, t_info *data)
	{
		ft_memset(data, 0, sizeof(t_info));
		data->server_pid = ft_atoi_limits(argv[1]);
		data->client_pid = getpid();
		ft_printfd(2, "PID CLIENT %d\n", data->client_pid);
		data->msg = argv[2];
		if (data->server_pid == 0)
			ft_print_error(BAD_SIGNAL);
	}

**Init_client_data** inicializa la estructura t_info que contiene los datos del cliente. Esta estructura almacena el PID del servidor (convertido a entero desde el argumento), el PID del cliente, y el mensaje a enviar. Si el PID del servidor es inválido, se lanza un error.

**7. Función check_server_status**

	int	check_server_status(int pid)
	{
		struct sigaction	sa;
	
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

**Check_server_status** intenta conectarse al servidor enviando señales al PID proporcionado. Usa la función **sigaction** para asociar las señales **SIGUSR1** y **SIGUSR2** con el manejador **server_state_signal_handler**. Llama a **attempt_server_connection** para enviar señales de conexión al servidor y esperar su respuesta.

**8. Attempt_server_connection**

		void	attempt_server_connection(int pid)
		{
			int	i;
		
			i = -1;
			while (++i < RETRY_TIMES)
			{
				kill(pid, SIGUSR1);
				ft_printf("Waiting response from server\n");
				sleep(RETRY_TIME);
				if (g_server.is_ready == 1)
					break ;
			}
		}
**Attempt_server_connection** envía señales de solicitud **(SIGUSR1)** al servidor y espera una respuesta por un tiempo determinado, definido por las constantes **RETRY_TIMES** y **RETRY_TIME**. Si el servidor responde con la señal **SERVER_READY**, se detiene el ciclo.

**9. Manejador de Señales del Servidor: Server_state_signal_handler**

	void	server_state_signal_handler(int signum, siginfo_t *info, void *context)
	{
		(void)signum, (void)context, (void)info;
		if (info->si_pid == getpid())
		{
			ft_printfd(2, "Error: Own process\n");
			exit(1);
		}
		if (info->si_pid != g_server.pid)
		{
			ft_printfd(2, "Error: Unexpected pid in tic_handler\n");
			return ;
		}
		if (signum == SERVER_READY)
			g_server.is_ready = 1;
		if (signum == SERVER_BUSY)
			g_server.is_ready = 0;
	}
 
Este manejador de señales, **server_state_signal_handler**, responde a las señales recibidas del servidor. Si la señal es **SERVER_READY**, actualiza el estado de disponibilidad del servidor a **1**. Si es **SERVER_BUSY**, lo actualiza a **0**.

**10. Send_message_bits**

	void	send_message_bits(void *data, size_t bit_length, t_info *info)
	{
		unsigned long long	value;
		int					i;
	
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
 
**Send_message_bits** convierte el dato (el tamaño del mensaje o un carácter del mismo) en una secuencia de bits, que luego son enviados al servidor usando señales **CHAR_1** o **CHAR_0**. Cada bit se evalúa y se envía de acuerdo a su valor (1 o 0).

**11. Transmit_message_to_server**

	void	transmit_message_to_server(char *str, t_info *data)
	{
		struct sigaction	sa;
		int					i;
	
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = client_signal_handler;
		sigaction(SIGUSR2, &sa, NULL);
		i = 0;
		while (str[i])
			send_message_bits(&str[i++], 8, data);
	}
 
**Transmit_message_to_server** recorre el mensaje carácter por carácter y llama a **send_message_bits** para transmitir cada uno de los caracteres, enviando cada bit individualmente.

**12. Manejador de Señales del Cliente: Client_signal_handler**

	void	client_signal_handler(int signum, siginfo_t *info, void *context)
	{
		(void)signum, (void)context, (void)info;
	}

Este manejador de señales, aunque está vacío en la implementación actual, podría ser utilizado para manejar cualquier señal relevante que reciba el cliente durante la transmisión de datos.

**13. Función send_signal**

		void	send_signal(pid_t pid, int signal)
		{
			if (kill(pid, signal))
				ft_print_error("Signal sending failed.");
		}

**Send_signal** envía una señal al servidor usando la función **kill**. Si ocurre algún error al enviar la señal, se genera un mensaje de error.

3. void keep_server_up(void)
Esta función mantiene al servidor en funcionamiento de manera indefinida. Utiliza un bucle infinito que ejecuta el comando sleep(1) para pausar la ejecución durante 1 segundo repetidamente.


## Server
Variable global g_client (en el servidor)
La variable global g_client es utilizada en el servidor para almacenar información sobre el cliente que se conecta y el estado de los mensajes que recibe el servidor.

**t_global	g_client;**

g_client.client_pid: Este campo almacena el PID del cliente que envía señales al servidor. Es esencial para poder enviar respuestas al cliente, como confirmar que las señales fueron recibidas.

g_client.actual_pid: Almacena el PID del cliente con el que el servidor está actualmente interactuando. Esto asegura que el servidor no maneje señales de múltiples clientes a la vez. Cuando se recibe la primera señal de un cliente, este campo se llena con el PID del cliente activo.

g_client.getting_header: Esta variable se utiliza para determinar si el servidor está esperando recibir la cabecera del mensaje (es decir, el tamaño del mensaje). Cuando este valor es 1, el servidor interpreta las señales como parte de la cabecera del mensaje.

g_client.getting_msg: Una vez que se ha recibido la cabecera, esta variable se pone a 1 para indicar que el servidor está esperando recibir el mensaje en sí.

g_client.msg: Es una estructura dentro de g_client que contiene información sobre el mensaje:

msg.size_message: Tamaño del mensaje en bits.
msg.message: Almacena el contenido del mensaje que se recibe del cliente.


**1. Función main**

El servidor comienza ejecutándose en la función main, donde ocurren varios pasos iniciales:

	int main(void)
	{
		struct sigaction	sa;
		pid_t				server_pid;
	
		// Inicializa la estructura global g_client a 0
		ft_memset(&g_client, 0, sizeof(t_global));
	
		// Obtiene el PID del servidor y lo muestra
		server_pid = getpid();
		ft_printfd(1, "Server PID: %d\n", server_pid);
	
		// Configura el manejo de señales con sigaction
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = server_signal_handler;
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
	
		// Mantiene el servidor en ejecución indefinidamente
		keep_server_up();
		return (0);
	}

**Inicialización:** El servidor inicializa **g_client**, que es una estructura que contiene el estado del servidor y los datos del cliente, a ceros usando **ft_memset().**

**PID del servidor:** Obtiene el **PID** del proceso del servidor con **getpid()** y lo muestra en la consola para que los clientes sepan a qué proceso enviar las señales.

**Configuración de señales:** Configura el manejo de señales **(SIGUSR1 y SIGUSR2)** usando **sigaction()**. Se asocia la función **server_signal_handler** a las señales para gestionar la comunicación entre el cliente y el servidor.

**Mantener el servidor activo:** Finalmente, entra en un bucle infinito con **keep_server_up(),** lo que mantiene el servidor activo.

**2. Manejo de señales: server_signal_handler**
   
Cuando el servidor recibe una señal **(SIGUSR1 o SIGUSR2)**, la función **server_signal_handler** es llamada automáticamente:

	void server_signal_handler(int signum, siginfo_t *info, void *context)
	{
		static int	i;
	
		(void)context;
	
		// Verifica si el cliente envió una señal nula (perdió el PID)
		info->si_pid = lost_signal(info->si_pid, signum, &i, context);
	
		// Verifica si la señal fue enviada por el propio servidor (error)
		if (info->si_pid == getpid())
			ft_print_error("Own process");
	
		// Asigna el PID del cliente que está enviando señales
		g_client.client_pid = info->si_pid;
	
		// Si es la primera vez que recibe del cliente, llama a toc()
		if (g_client.actual_pid == 0)
		{
			toc(g_client.client_pid);
			return ;
		}
	
		// Ignora si está recibiendo señales de otro cliente
		if (g_client.actual_pid != g_client.client_pid)
			return ;
	
		// Lógica para procesar encabezado o mensaje según el estado
		if (g_client.getting_header == 1)
			header_handler(&i, signum);
		else if (g_client.getting_msg == 1)
			msg_handler(&i, signum);
	
		// Envía una señal de respuesta al cliente
		if (g_client.client_pid != 0 && (signum == SIGUSR1 || signum == SIGUSR2))
			kill(g_client.client_pid, SIGNAL_RECEIVED);
	}

**Flujo en server_signal_handler:**

**Verificar la señal:** Utiliza **lost_signal()** para verificar si el PID de la señal es inválido (cero), en cuyo caso recupera el PID del cliente almacenado globalmente en **g_client.actual_pid.**

**Evitar manejar señales del propio servidor:** Si la señal proviene del mismo proceso del servidor **(getpid())**, lanza un error y no continúa.

**Asignar el PID del cliente:** Asigna el PID del cliente que envió la señal a **g_client.client_pid.**

**Primera comunicación con el cliente:** Si es la primera vez que el servidor recibe una señal de ese cliente, llama a **check_server_status** para notificar al cliente que está listo para recibir el mensaje.

**Procesar el mensaje:** Dependiendo del estado del servidor, maneja el encabezado del mensaje (si está en getting_header == 1) llamando a header_handler(), o el contenido del mensaje (si está en getting_msg == 1) llamando a msg_handler().

**Enviar respuesta al cliente:** Al final, si recibió una señal válida, envía una señal de confirmación (SIGNAL_RECEIVED) al cliente.

**3. client_server_status(int pid)**
   
La función **check_client_status** se llama cuando el servidor establece la primera conexión con un cliente. Esta función envía una señal de "listo" al cliente para indicar que el servidor está preparado para recibir mensajes.

	int check_client_status(int pid)
	{
		// Envía la señal de que el servidor está listo
		kill(pid, SERVER_READY);
	
		// Actualiza el estado del servidor
		g_client.actual_pid = pid;
		g_client.getting_header = 1;
		return (EXIT_SUCCESS);
	}
**Enviar señal al cliente:** Usa kill() para enviar una señal SERVER_READY al cliente.

**Actualizar estado:** Cambia el estado del servidor para indicar que está listo para recibir el encabezado del mensaje.

**4. Procesamiento del encabezado: header_handler(int * i, int signum)**

Cuando el servidor está esperando el encabezado del mensaje, utiliza header_handler() para recibir los bits del tamaño del mensaje, bit a bit.

	void header_handler(int *i, int signum)
	{
		const int	bit_value = get_signal_bit(signum);
	
		// Reconstruye el tamaño del mensaje a partir de las señales
		if ((*i) < HEADER_SIZE)
		{
			g_client.msg.size_message |= (bit_value << (HEADER_SIZE - 1 - (*i)));
			(*i)++;
		}
	
		// Si ya recibió el tamaño completo, asigna memoria para el mensaje
		if ((*i) == HEADER_SIZE)
			allocate_memmory_for_message(i);
	}

**Flujo en header_handler:**
Obtener el bit de la señal: Usa get_signal_bit() para obtener un valor de bit (0 o 1) dependiendo de la señal recibida (SIGUSR1 o SIGUSR2).

**Construir el tamaño del mensaje:** Con cada bit recibido, va construyendo el tamaño del mensaje (almacenado en g_client.msg.size_message) desplazando y sumando bits.

**Asignar memoria:** Una vez que se han recibido suficientes bits para el encabezado (definido por HEADER_SIZE), llama a allocate_memmory_for_message() para asignar memoria suficiente para almacenar el mensaje completo.

**5. Asignar memoria: allocate_memmory_for_message(int * i)**
Esta función se llama después de que se haya recibido el encabezado completo para asignar la memoria necesaria para almacenar el mensaje completo.


	void allocate_memmory_for_message(int *i)
	{
		// Muestra el tamaño del mensaje que se ha recibido
		printf("SIZE_MSG: [%d]\n", g_client.msg.size_message);
	
		// Asigna memoria para el mensaje
		g_client.msg.message = ft_calloc((g_client.msg.size_message + 1), 1);
		if (g_client.msg.message == NULL)
			ft_print_error("Memory allocation failed");
	
		// Cambia el estado para empezar a recibir el mensaje
		g_client.getting_header = 0;
		g_client.getting_msg = 1;
		(*i) = 0;
	}

**6. Procesamiento del mensaje: msg_handler(int * i, int signum)**

Una vez que el servidor está listo para recibir el mensaje, msg_handler() procesa los bits de cada carácter del mensaje, uno por uno.

	void msg_handler(int *i, int signum)
	{
		const int	bit_value = get_signal_bit(signum);
		static int	char_value;
		static int	msg_pos;
	
		// Construye el valor del carácter bit a bit
		if (*i % 8 < 8)
		{
			char_value |= (bit_value << (7 - (*i % 8)));
			(*i)++;
		}
	
		// Si se completó un carácter, lo almacena en el mensaje
		if (*i % 8 == 0)
		{
			g_client.msg.message[msg_pos] = char_value;
			char_value = 0;
			msg_pos++;
		}
	
		// Si ya se recibió el mensaje completo, lo imprime y resetea
		if (*i / 8 == g_client.msg.size_message)
		{
			printf("message: [%s]\n", g_client.msg.message);
			free(g_client.msg.message);
			ft_bzero(&g_client, sizeof(g_client));
			g_client.getting_header = 1;
			(*i) = 0;
			msg_pos = 0;
		}
	}

**Flujo en msg_handler:**

Recibir bits del mensaje: Va recibiendo bits de la señal y construyendo los caracteres del mensaje, bit por bit.

Almacenar caracteres: Una vez que ha recibido todos los bits de un carácter, lo almacena en la posición correcta del mensaje.

Finalizar el mensaje: Cuando se ha recibido el mensaje completo (verificado comparando *i / 8 con el tamaño del mensaje), imprime el mensaje, libera la memoria y reinicia el estado del servidor para prepararse para un nuevo mensaje.







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
