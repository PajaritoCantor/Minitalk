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

**parser(argc, argv):** Esta función valida los argumentos de entrada del programa, asegurándose de que el número de argumentos sea correcto y que el formato del PID del servidor sea válido. Esta función garantiza una correcta comunicación entre el cliente y el servidor, evitando errores que podrían afectar el envío de señales y la reconstrucción del mensaje.



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

* argv[0]: el nombre del programa.
* argv[1]: el PID del servidor.
* argv[2]: el mensaje que se quiere enviar.

* Si el número de argumentos es diferente de 3, el programa muestra un error y se detiene la ejecución.
* Si el número de argumentos es correcto, la función llama a **evaluate** con **(artgv[1])** que representa el PID del servidor y se evalúa si el PID es válido.

**int state** es el estado inicial de la evaluación. Se usa en conjunto con **choose_state** para moverse entre diferentes estadoss, lo que ayuda a verificar la válides del PID.

- En cada iteración del bucle, **chose_state** se llama con el *estado actual* y el *carácter actual* de la cadena. Esta función verifica si el carácter es un dígito utilizando la función ft_isdigit. Si es así, se establece en 1.
- La función **get_state** se llama con el *estado actual* y el *valor de pos*. Esto determina el nuevo estado del autómata bassándose en sí el carácter era un dígito o no.
- **const int states[][2] = {...}:** Es una matriz de enteros de 2 filas y 2 columnas. Cada fila corresponde a un estado, y cada columna corresponde al valor de **pos**(0 o 1).
- **La tabla de estados:**
- **{1, 2}:** En el estado 0, si el carácter no es un dígito (pos = 0), el autómata se mueve al estado 1. Si es un dígito (pos = 1), se mueve al estado 2.
- **{1, 1}:** En el estado 1, siempre se mueve al estado 1, independiente de si el carácter es un dígito o no.
- **{1, 2}:** En el estado 2, si el carácter no es un digito (pos = 0), el aútomata se mueve al estado 1. Si es un dígito, se mantiene el estado 2.
