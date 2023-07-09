#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

SDL_Surface* loadSurface( std::string path )
{
    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    return loadedSurface;
}

bool init() {
    bool success = true;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if ( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !(IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            } else {
                // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    std::string path = "../assets/hello_world.png";

    //Load splash image
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL)
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = false;
    } else {
        //Convert surface to screen format
        gHelloWorld = SDL_ConvertSurface( loaded_surface, gScreenSurface->format, 0 );
        if( gHelloWorld == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loaded_surface );
    }

    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[]) {
    //Start up SDL and create window
    if (!init())
    {
        printf( "Failed to initialize!\n" );
    } else {
        //Load media
        if (!loadMedia())
        {
            printf( "Failed to load media!\n" );
        }
        //Apply the image
        SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
        //Hack to get window to stay up
        SDL_Event e; 
        bool quit = false; 
        while ( quit == false ) {
            while ( SDL_PollEvent( &e ) ) {
                if ( e.type == SDL_QUIT ) quit = true; 
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;

};