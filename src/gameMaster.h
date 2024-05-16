#ifndef GM
#define GM
#include "mensagem.h"
#include <lua.h>

void registrarFlecha(Archer*, Mapa*, Mensagem*);
void registrarFlechaLocal(Archer*, Mapa*);
void escreverTutorial(); // Deveria estar aqui?
int start_server(lua_State*); 
int run_client(lua_State*);
int run_server(lua_State*);
void* client_thread(void*);
void* server_thread(void*);
#endif
