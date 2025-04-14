# cs4220_project1
# TLS Secure Client-Server Communication System

# Name: Juan Salazar & Soo Yuan Kong
# Institution: University of Colorado Colorado Springs (UCCS)
# Course: CS 4220 001
# Group: networkNinja3

## Project Description
This project implements a secure client-server communication channel using TLS 1.2/1.3 encryption through OpenSSL. The system demonstrates:
- Secure socket communication with RSA key exchange
- AES-256 encrypted data transmission
- Mutual authentication using X.509 certificates
- Industry-standard TLS implementation

Developed for the CS 4220 project to showcase practical application of cryptographic protocols in network security.

## Prerequisites
- Linux/macOS environment
- OpenSSL 1.1.1 or newer
- GCC compiler

## Encryption Details
- Key Algorithm: RSA (2048-bit by default)
- Protocol: TLS via OpenSSL
- Symmetric Cipher: Negotiated (e.g., AES-256)
- Certificates: Self-signed, PEM format

## Functionality Overview
- The server initializes a TLS context, loads its certificate/key, listens for client connections, and sends a response message upon receiving client input.
- The client connects securely to the server, sends a message, and displays the server’s response.
- Both sides authenticate with certificates and encrypt all communication using TLS.

## Special Notes
- Certificates must be present in the same directory as the compiled executables.
- Ensure that server is started before the client.
- This setup is designed for local testing and uses localhost (127.0.0.1) on port 4433.
- You can modify the IP and port in the source code for remote testing if needed.
- All cryptographic operations follow standard OpenSSL practices.

## Step 1: Install OpenSSL development libraries:
sudo apt-get update
sudo apt-get install libssl-dev

## Step 2: Create working directory and move into it:
mkdir temp1
cd temp1

## Step 3: Generate RSA keys and certificates
## Server:
openssl genpkey -algorithm RSA -out server.key
openssl req -new -key server.key -out server.csr

When filling out the CSR prompts, you can use:
- Country Name (2 letter code) [XX]:US
- State or Province Name (full name) []:Colorado
- Locality Name (eg, city) [Default City]:Colorado Springs
- Organization Name (eg, company) [Default Company Ltd]:UCCS
- Organizational Unit Name (eg, section) []:Computer Science Department
- Common Name (eg, your name or your server's hostname) []:redcloud.uccs.edu
- Email Address []:jsalaza8@redcloud.uccs.edu
- A challenge password []:jfsc123454321!
- An optional company name []:

openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

## Client:
openssl genpkey -algorithm RSA -out client.key
openssl req -new -key client.key -out client.csr

When filling out the CSR prompts, you can use:
- Country Name (2 letter code) [XX]:US
- State or Province Name (full name) []:Colorado
- Locality Name (eg, city) [Default City]:Colorado Springs
- Organization Name (eg, company) [Default Company Ltd]:UCCS
- Organizational Unit Name (eg, section) []:Computer Science Department
- Common Name (eg, your name or your server's hostname) []:redcloud.uccs.edu
- Email Address []:jsalaza8@redcloud.uccs.edu
- A challenge password []:jfsc123454321!
- An optional company name []:

openssl x509 -req -days 365 -in client.csr -signkey client.key -out client.crt

## Step 4: Create files
touch client.c server.c Makefile

## Step 5: File Structure
Use the "ls" command to make sure "client.c  client.crt  client.csr  client.key  Makefile  server.c  server.crt  server.csr  server.key" are included in the temp1 directory.
temp1:
- client.c           # Client program
- client.crt         # Client certificate
- client.csr         # Client certificate request
- client.key         # Client private key
- server.c           # Server program
- server.crt         # Server certificate
- server.csr         # Server certificate request
- server.key         # Server private key
- Makefile           # Compilation instructions

## Step 6: Edit and code for files
nano server.c
nano client.c
nano Makefile

Note: Code for server.c, client.c, and Makefile are included in the Github repository. Please go back to the main branch for reference.

## Step 7: Compilation and Execution
Compile the code using the Makefile: 
- make
  
Run the server: 
- ./server
  
In a separate terminal, run the client: 
- ./client

make clean


