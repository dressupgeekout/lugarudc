//
// lugarudc -- Dreamcast main routine
// Charlotte Koch <dressupgeekout@gmail.com>
//

#include <iostream>

#include <kos.h>
#include <GL/gl.h>
#include <GL/glkos.h>

//#include "Game.hpp"

int
main(void)
{
  maple_device_t *controller;
  cont_state_t *controllerState;

  std::cout << "This is the Dreamcast version" << std::endl;

  glKosInit();

  controller = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

  if (!controller) {
    // XXX no controller present.
    return 1;
  }

  bool done = false;

  while (!done) {
    controllerState = (cont_state_t*)maple_dev_status(controller);

    if (!controllerState) {
      // XXX error reading controller state
      continue;
    }

    if (controllerState->buttons & CONT_START) {
      // START button was pressed, quit nicely.
      done = true;
    }
  }

  //Game::newGame();
  //Game::deleteGame();
  return 0;
}
