#ifndef common_callbacks_h
#define common_callbacks_h

#ifdef __cplusplus
extern "C" {
#endif

//Returns wheither the game is running(for callbacks)
int running();

//Setups up callbacks for exiting the program properly
int setupCallbacks(void);

#ifdef __cplusplus
}
#endif

#endif
