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

**if (argc != 3)**
