#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>




SSL_CTX *create_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;


    method = TLS_client_method();


    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }


    return ctx;
}


void configure_context(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "client.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }


    if (SSL_CTX_use_PrivateKey_file(ctx, "client.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}


int create_socket(const char *host, int port) {
    int s;
    struct sockaddr_in addr;


    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);


    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }


    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }


    return s;
}


void communicate_with_server(SSL *ssl) {
    const char *request = "Hello from client!";
    SSL_write(ssl, request, strlen(request));


    char buf[1024];
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = 0;
    printf("Received: %s\n", buf);
}


int main(int argc, char **argv) {
    (void)argc;
    (void)argv;


    SSL_CTX *ctx;
    int sock;


    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();


    ctx = create_context();
    configure_context(ctx);


    sock = create_socket("127.0.0.1", 4433);


    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);


    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        communicate_with_server(ssl);
    }


    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
}
