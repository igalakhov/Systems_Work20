#include "pipe_networking.h"


/*=========================
  server_handshake
  =========================*/
int server_handshake(int from_client) {
  printf("[Subserver %d] Starting handshake...\n", getpid());
  remove(WKP_NAME);

  char pp[HANDSHAKE_BUFFER_SIZE];
  char buf[HANDSHAKE_BUFFER_SIZE];

  read(from_client, pp, HANDSHAKE_BUFFER_SIZE);
  printf("[Subserver %d] Private pipe name recieved from client\n", getpid());
  printf("[Subserver %d] Connecting to %s\n", getpid(), pp);

  int to_client = open(pp, O_WRONLY);
  write(to_client, ACK, strlen(ACK) + 1);
  read(from_client, buf, HANDSHAKE_BUFFER_SIZE);

  printf("[Subserver %d] Connection confirmed by client\n", getpid());
  printf("[Subserver %d] Handshake complete!\n", getpid());

  return to_client;
}


/*=========================
  client_handshake
  =========================*/
int client_handshake(int *to_server) {
  printf("[Client %d] Starting handshake...\n", getpid());

  char pp[HANDSHAKE_BUFFER_SIZE];
  char buf[HANDSHAKE_BUFFER_SIZE];

  sprintf(pp, "%d", getpid());
  mkfifo(pp, 0666);

  printf("[Client %d] Connecting to well known pipe\n", getpid());
  *to_server = open(WKP_NAME, O_WRONLY);

  printf("[Client %d] Sending private pipe name\n", getpid());
  write(*to_server, pp, strlen(pp) + 1);

  int from_server = open(pp, O_RDONLY);
  remove(pp);
  read(from_server, buf, HANDSHAKE_BUFFER_SIZE);
  printf("[Client %d] Recieved [%s] from server, responding...\n", getpid(), buf);
  write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
  printf("[Client %d] Handshake complete!\n", getpid());

  return from_server;
}
