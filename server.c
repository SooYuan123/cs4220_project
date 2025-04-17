#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Creates and configures the SSL context for the server
SSL_CTX *create_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    // Use TLS for the server
    method = TLS_server_method();

    // Create a new SSL context
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }


    return ctx;
}

// Loads the server certificate and private key into the context
void configure_context(SSL_CTX *ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);    // Set ECDH automatically

    // Load server certificate
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Load server private key
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// Creates a TCP socket, binds to the specified port, and starts listening
int create_socket(int port) {
    int s;
    struct sockaddr_in addr;


    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);


    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }


    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }


    if (listen(s, 1) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }


    return s;
}

// Handles incoming client connection and responds
void handle_client(SSL *ssl) {
    char buf[1024];
    int bytes = SSL_read(ssl, buf, sizeof(buf));    // Read encrypted data from client
    buf[bytes] = 0;
    printf("Received: %s\n", buf);


    const char *response = "Hello from server!";
    SSL_write(ssl, response, strlen(response));    // Send encrypted response back
}


int main(int argc, char **argv) {
    (void)argc;
    (void)argv;


    SSL_CTX *ctx;
    int sock;

    // Initialize the SSL library
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    // Create and configure context
    ctx = create_context();
    configure_context(ctx);

    // Create and bind socket to port 4433
    sock = create_socket(4433);


    while (1) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        
        // Accept incoming TCP connection
        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        // Wrap the TCP socket with TLS
        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        // Perform TLS handshake
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            handle_client(ssl);    // Interact with client securely
        }

        // Shutdown and clean up
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }

    // Clean up server resources
    close(sock);
    SSL_CTX_free(ctx);
}
