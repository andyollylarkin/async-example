#include <iostream>
#include <uv.h>
#include <string.h>

uv_loop_t *loop;

void on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0)
	{
		std::cerr << "New connection error: " << uv_strerror(status) << std::endl;
		return;
	}

	// Создание клиента
	uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);

	// Принятие подключения
	if (uv_accept(server, (uv_stream_t *)client) == 0)
	{
		std::cout << "Accepted new connection!" << std::endl;
		// Дальше можно отправить/получить данные от клиента
	}
	else
	{
		uv_close((uv_handle_t *)client, nullptr);
	}
}

int main()
{
	loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 7000, &addr); // Прослушивание порта 7000

	auto cb = [](uv_stream_t *server, int status)
	{
		uv_tcp_t *client = new uv_tcp_t;

		// Инициализация клиента
		uv_tcp_init(loop, client);

		// Принятие подключения
		if (uv_accept(server, (uv_stream_t *)client) == 0)
		{
			std::cout << "Accepted new connection!" << std::endl;
			char *message = "Hello from server!\n";

			uv_write_t *req = new uv_write_t;

			const uv_buf_t buf = uv_buf_init(message, strlen(message));

			uv_write(req, (uv_stream_t *)client, &buf, 1, nullptr);
		}
		else
		{
			uv_close((uv_handle_t *)client, nullptr);
			delete client; // Освобождаем память
		}
	};

	uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
	int r = uv_listen((uv_stream_t *)&server, 128, cb);

	if (r)
	{
		std::cerr << "Listen failed: " << uv_strerror(r) << std::endl;
		return 1;
	}

	std::cout << "Server listening on port 7000..." << std::endl;

	return uv_run(loop, UV_RUN_DEFAULT);

	uv_loop_close(loop);
	return 0;
}