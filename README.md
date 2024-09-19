## Tabla de estados

Una tabla de estados es una herramienta común en la programación que se usa en la implementación de **máquinas de estado**. Una *máquina de estado* es un modelo matemático que se usa para diseñar sistemas que cambian de estado en respuesta de ciertos eventos.
- Una tabla de estados es una matriz o un array que define las stransiciones entre diferentes estados de un sistema. Cada entrada en la tabla indica el próximo estado que se debe alcanzar dado el estado actual y una entrada específica.
- **Estados:** Representan diferentes condiciones o situaciones en las que el sistema puede encontrarse.
- **Entradas/Acciones:** Son eventos o inputs que provocan un cambio de estado.
- **Transiciones:** Definen cómo el sistema cambia de un estado a otra en función de las entradas.

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

* **sa_sigaction = ping_handler:** Aquí se asigna la función ping_handler como el manejador de señales avanzado. Esta función se invocará cuando el programa reciba una señal, y debido a la activación de la bandera **SA_SIGINFO**, el manejador podrá recibir información adicional sobre la señal a través de los parámetros **siginfo_t * info** y **void * context**.

	void ping_handler(int signum, siginfo_t * info, void * context): 
 
- **int signum:** Es el número de la señal recibidaa (como SIGUSR1 o SIGUSR2).
**siginfo_t * info:** contiene información adicional sobre la señal, como el PID del proceso que la envió.
- **void * context:** Provee el contexto aicional para la ejecución, pero en este caso no se utiliza.

- **info->si_pid == getpid()):** Se verifica si el proceso que recibió la señal es el mismo que la envió. Si esto ocurre, se imprime un mensaje de error y se termina el programa con **exit(1)**. Esto es para evitar que el proceso se envíe señales a sí mismo, lo cual probablemente sería un error.

- **if (info->si_pid != g_server.pid):** Comprueba si el **PID** del remitente de la señal no coincide con el **PID** del **server**. Si no coinciden se imprime un mensaje de error y se retorna sin hacer más nada, esto debido a que solo el **server** debe enviar las señales a este manejador.

- **if (signum == SERVER_READY):** Si el número de **signum** es igual a **SERVER_READY**, entonces el programa actualiza el estado de **g_server.is_ready** a **1** y se imprime un mensaje confirmado que el servidor ha enviado la señal de estar listo.

- **if (signum == SERVER_BUSY):** Si la señal recibida es **SERVER_BUSY**(probablemente **SIGUSR2**), se actualiza el estado de **g_server.is_ready** a 0 (indicando que el servidor no está listo o está ocupado), y se imprime un mensaje que refleja esta condición.

* **g_server.pid = pid:** Establece el **server PID** al que se va a enviar el **ping** en la variable global **g_server.pid**. Esto es importante porque **ping handler** usará esta variable para verificar si las señales recibidas provienen del **server** correcto.

**sigaction(SIGUSR1, &sa, NULL):**
**sigaction(SIGUSR2, &sa, NULL):**

- Se utilizan dos llamadas a **sigaction** para asociar las señales **SIGUSR1** y **SIGUSR2** con el manejador de señales **ping_handler**, que se configuró previamente en la estructura **sa**.
- La primera llamada establece que **SIGUSR1** debe ser manejada por **ping_hanndler**, y la segunda hace lo mismo para **SIGUSR2**.
- La tercera variable **NULL** indica que no se necesita recuperar la configuración anterior para estas señales.

**handle_timeouts(pid):** Esta función envía señales **SIGUSR1** al servidor repetidamente y espera una respuesta durante un tiempo determinado. Durante este procesos, el programa está esperando que el servidor responda con la señal adecuada (SIGUS1 O SIGUSR2), actualizando **g_server.is_ready**

**Glosario**

## Tabla de estados

Una tabla de estados es una herramienta común en la programación que se usa en la implementación de **máquinas de estado**. Una *máquina de estado* es un modelo matemático que se usa para diseñar sistemas que cambian de estado en respuesta de ciertos eventos.
- Una tabla de estados es una matriz o un array que define las stransiciones entre diferentes estados de un sistema. Cada entrada en la tabla indica el próximo estado que se debe alcanzar dado el estado actual y una entrada específica.
- **Estados:** Representan diferentes condiciones o situaciones en las que el sistema puede encontrarse.
- **Entradas/Acciones:** Son eventos o inputs que provocan un cambio de estado.
- **Transiciones:** Definen cómo el sistema cambia de un estado a otra en función de las entradas.

## SA_SIGINFO

* Es una bandera utilizada en el contexto de la función ***sigaction*** para especificar cómo debe manejarse unaa señal en un programa C. Es parte del **estándar POSIX***. 
* Su propósito es indicar que se desea usar una función manejadora de señales más avanzada que provea información adicional sobre la señal, en lugar de solo usar un simple manejador de señales (es decir, solo pasando el número de la señal).

  
      void handler(int signum, siginfo_t * info, void  * context)

Esta función recibe un **puntero** a la estructura **siginfo_t** que contiene información detallada sobre la **señal**, como el ***PID del proceso*** que envió la señal, **el tipo de señal**, y otra información relevante.
