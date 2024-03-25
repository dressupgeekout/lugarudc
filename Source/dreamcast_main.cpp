//
// lugarudc -- Dreamcast main routine
// Charlotte Koch <dressupgeekout@gmail.com>
//

#include <kos.h>
KOS_INIT_FLAGS(INIT_DEFAULT);

#include <iostream>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <stb_image/stb_image.h>

#include <dirent.h>

int
main(void)
{
  maple_device_t *controller;
  cont_state_t *controllerState;

  dbglog_set_level(DBG_WARNING);

  std::cout << "This is the Dreamcast version" << std::endl;

  DIR *dir = opendir("/cd");
  struct dirent *entry = NULL;

  do {
    entry = readdir(dir);
    if (!entry) continue;
    std::cout << "- " << std::string(entry->d_name) << std::endl;
  } while (entry);
  closedir(dir);

  controller = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

  if (!controller) {
    // No controller present, exit right away.
    return 1;
  }

  // ========== GRAPHICS INIT ==========
  glKosInit();
  glEnable(GL_TEXTURE_2D);

  // Expect counter-clockwise vertex order
  glFrontFace(GL_CCW);

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // depth test not needed for 2d
  glDisable(GL_DEPTH_TEST);

  // Orthographic camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Screen/Camera coords: left, right, bottom, top, near, far
  //glOrtho(0.0f, (float)640, 0.0f, (float)480, -1.0f, 1.0f);
  glOrtho(0.0f, (float)640, (float)480, 0.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Generate a texture and link it to our ID
  GLuint bgimg;
  glGenTextures(1, &bgimg);
  glBindTexture(GL_TEXTURE_2D, bgimg);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Scale down filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Scale up filter

  // Load the image from disk with STB
  int width;
  int height;
  int nchannels;
  std::cout << "CHARLOTTE 1" << std::endl;
  unsigned char *data = stbi_load("/cd/Data/Textures/Eyelid.png", &width, &height, &nchannels, 0);
  std::cout << "CHARLOTTE 2" << std::endl;

  if (data) {
    std::cout << "Loaded Textures/Eyelid.png OK" << std::endl;
    printf("%dx%d nchannels=%d\n", width, height, nchannels);
  } else {
    std::cout << "COULD NOT LOAD TEXTURE" << std::endl;
    return 1;
  }

  // Image loaded OK, apply texture formats and assign the data to it
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // You have to do this with PNGs on Dreamcast for some reason:
  glGenerateMipmapEXT(GL_TEXTURE_2D);

  stbi_image_free(data);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "there was a OpenGL error" << std::endl;
  }

  // Vertex/UV data setup
  float uv[4][2] = {
    // X -- Y
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
  };

  float xyz[4][3] = {
    { -1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0 },
  };

  // vars to manipulate pos and rot of our quad
  float posx = ((float)width/2.0f);
  float posy = ((float)height/2.0f);
  float rot = 0.0f;
  float factor = 1.0f;
  
  bool done = false;

  while (!done) {
    // ========== READ CONTROLLER ==========
    controllerState = (cont_state_t *)maple_dev_status(controller);

    if (!controllerState) {
      // Error reading controller state, try again.
      continue;
    }

    if (controllerState->buttons & CONT_START) {
      // START button was pressed, quit nicely.
      done = true;
      continue;
    }

    if (controllerState->buttons & CONT_DPAD_UP) {
      posy -= 1.0f;
    }

    if (controllerState->buttons & CONT_DPAD_DOWN) {
      posy += 1.0f;
    }

    if (controllerState->buttons & CONT_DPAD_LEFT) {
      posx -= 1.0f;
    }

    if (controllerState->buttons & CONT_DPAD_RIGHT) {
      posx += 1.0f;
    }

    if (controllerState->buttons & (CONT_DPAD_UP|CONT_DPAD_DOWN|CONT_DPAD_LEFT|CONT_DPAD_RIGHT))
    {
      printf("pos = %0.2fx%0.2f\n", (double)posx, (double)posy);
    }

    if (controllerState->buttons & CONT_A) {
      factor += 0.01f;
    }

    if (controllerState->buttons & CONT_B) {
      factor -= 0.01f;
    }

    // ========== DRAW A FRAME ==========
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, bgimg);

    glPushMatrix();
      glLoadIdentity();
      glTranslatef(posx, posy, 0.0f);
      glRotatef(rot, 0.0f, 0.0f, 1.0f);
      //glScalef((float)640*factor, (float)480*factor, 0.0);
      glScalef(320.0f, 240.0f, 0.0);

      glBegin(GL_QUADS);
        // Bottom left
        glTexCoord2fv(&uv[0][0]);
        glVertex3f(xyz[0][0], xyz[0][1], xyz[0][2]);

        // Bottom right
        glTexCoord2fv(&uv[1][0]);
        glVertex3f(xyz[1][0], xyz[1][1], xyz[1][2]);

        // top right
        glTexCoord2fv(&uv[2][0]);
        glVertex3f(xyz[2][0], xyz[2][1], xyz[2][2]);

        // top left
        glTexCoord2fv(&uv[3][0]);
        glVertex3f(xyz[3][0], xyz[2][1], xyz[3][2]);
      glEnd();
    glPopMatrix();

    glKosSwapBuffers();
  }

  glDeleteTextures(1, &bgimg);

  return 0;
}


// =========================================================================
// =========================================================================
// =========================================================================
// =========================================================================

#if 0

void GL_Init(uint16_t w, uint16_t h) {
    // Set "background" color to light blue
    glClearColor(0.10f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    // Expect Counter Clockwise vertex order
    glFrontFace(GL_CCW);

    // Enable Transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Depth test not needed for 2D
    glDisable(GL_DEPTH_TEST);

    // Set Orthographic ( 2D ) Camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Screen / Camera coords
    //      Left   Right    Bottom  Top      Near   Far
    glOrtho(0.0f, (float)w, 0.0f,  (float)h, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    // Setup for Controller
    maple_device_t  *cont;
    cont_state_t    *state;
    cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

    // Texture ID
    GLuint texture;

    // Initialize KOS
    dbglog_set_level(DBG_WARNING);
    printf("\n..:: 2D Textured Quad Example - Start ::..\n");
    glKosInit();

    // Set screen size and init GLDC
    GL_Init(640, 480);

    // Generate a texture and link it to our ID
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters best suited for Pixel Art ( Hard edge no repeat for scaling )
    // Prevents blur on scale.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Scale down filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Scale up filter

    // Load PNG via stb_image
    int width, height, nr_channels;
    //stbi_set_flip_vertically_on_load(true); // Flip img onload due to GL expecting top left (0,0)
    unsigned char *data = stbi_load("/rd/crate.png", &width, &height, &nr_channels, 0);

    // texture load debug
    printf("..:: STB_IMAGE Data ::..\n");
    printf("nr_channels: %d\n", nr_channels);
    printf("width:       %d\n", width);
    printf("height:      %d\n", height);

    // If data is loaded, apply texture formats and assign the data
    if(data) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     data);

        // Currently REQUIRED for OpenGLdc w/PNGs on Dreamcast
        glGenerateMipmapEXT(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture!\n");
    }

    // Cleanup
    stbi_image_free(data);

    // ..:: Output any GL Errors
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("OpenGL error: %x\n", error);
    }
    // ..:: Vertex / UV Data setup
   float uv[4][2]  = {
                        // UVs are in reverse Y order to flip image
                        // GL expects 0,0 to be the bottom left corner of an image while
                        // real world coordinates are typically top left.
                        // alt- use: stbi_set_flip_vertically_on_load(true); before loading texture.
                        // X    // Y
                        {0.0f, 0.0f},
                        {1.0f, 0.0f},
                        {1.0f, 1.0f},
                        {0.0f, 1.0f}
                    };

    float xyz[4][3] = {
                        // Remember these are Counter-clockwise
                        /*  (-1,-1)-----( 0,-1)-----( 1,-1)
                               |           |           |
                               |           |           |
                               |           |           |
                            (-1, 0)-----( 0, 0)-----( 1, 0)
                               |           |           |
                               |           |           |
                               |           |           |
                            (-1, 1)-----( 0, 1)-----( 1, 1)
                        */
                        // X      Y      Z
                        { -1.0f,  1.0f,  1.0f },    // Bottom Left
                        {  1.0f,  1.0f,  1.0f },    // Bottom Right
                        {  1.0f, -1.0f,  1.0f },    // Top Right
                        { -1.0f, -1.0f,  1.0f }     // Top Left
                      };
    // ..:: Setup some variables to manipulate position and rotation of our quad
    float pos_x = 320.0f;
    float pos_y = 240.0f;
    float rot   = 0.0f;

    while(1) {
        //..:: Check Controller Input
        state = (cont_state_t *)maple_dev_status(cont);

        if(!state) {
            printf("Error reading controller\n");
            break;
        }

        // Close program on Start Button
        if(state->start)
            break;

        //..:: Rotation on Triggers
        // Rotate CCW
        if(state->ltrig >= 255) {
            rot += 4.0f;
        }

        // Rotate CW
        if(state->rtrig >= 255) {
            rot -= 4.0f;
        }

        //..:: Scale on Y / B
        // Scale up
        if(state->y) {
            width = height += 4.0f;
        }

        // Scale down
        if(state->b) {
            width = height -= 4.0f;
        }

        // ..:: Begin GL Drawing
        glClearColor(0.10f, 0.5f, 1.0f, 1.0f);              // Sets background Color
        glClear(GL_COLOR_BUFFER_BIT);                     // Clears screen to that color

        pos_x += state->joyx * 0.05;
        pos_y -= state->joyy * 0.05;

        // Apply texture data to all draw calls until next bind
        glBindTexture(GL_TEXTURE_2D, texture);

        glPushMatrix();

            glLoadIdentity();

            // Matrix transforms are applied in reverse order.
            // Order should ALWAYS be:
            // 1) Translate
            // 2) Rotate
            // 3) Scale
            // Scaling "First" ( last in code ) allows all other transforms to be scaled as well.

            // Move quad to screen position
            //           X      Y      Z
            glTranslatef(pos_x, pos_y, 0.0f);
            //        val  X-axis   Y-axis   Z-axis
            glRotatef(rot, 0.0f,    0.0f,    1.0f);   // rotate only on enabled Axis (1.0f)
            //      tex_w   tex_h   Z doesn't change
            glScalef(width, height, 0.0f );

            glBegin(GL_QUADS);
                // Remember these are Counter-clockwise
                // Bottom Left
                glTexCoord2fv(&uv[0][0]);
                glVertex3f(xyz[0][0], xyz[0][1], xyz[0][2]);

                // Bottom Right
                glTexCoord2fv(&uv[1][0]);
                glVertex3f(xyz[1][0], xyz[1][1], xyz[1][2]);

                // Top Right
                glTexCoord2fv(&uv[2][0]);
                glVertex3f(xyz[2][0], xyz[2][1], xyz[2][2]);

                // Top Left
                glTexCoord2fv(&uv[3][0]);
                glVertex3f(xyz[3][0], xyz[3][1], xyz[3][2]);

            glEnd();

        glPopMatrix();

        // Finish the frame and flush to screen
        glKosSwapBuffers();
    }

    // ..:: Cleanup Memory
    glDeleteTextures(1, &texture);

    return 0;
}

#endif // 0 
