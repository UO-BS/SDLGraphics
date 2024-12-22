/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include "MainWindow.h"
#include <cmath>
#include "GMatrix.h"
#include "GRectangleTexture.h"
#include "GGraphicalComponent.h"


MainWindow::MainWindow(std::string windowName)
{
	if( !init(windowName) )
	{
		printf( "Failed to initialize!\n" );
	} else {
		if(displayMedia("red.bmp" ) == -1 || displayMedia("blue.bmp" ) == -1 || displayMedia("green.bmp" ) == -1 || !displayMedia("clueless.bmp" ))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;


			float temp=1.0;
			float sign=1.0;

			float xKeyboard=0.0;
			float yKeyboard=0.0;
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN ){
						switch( e.key.keysym.sym ){
							case SDLK_UP:
								printf( "UP\n" );
								xKeyboard+=2;
								break;
							case SDLK_DOWN:
								printf( "Down\n" );
								xKeyboard-=2;
								break;
							case SDLK_LEFT:
								printf( "Left\n" );
								yKeyboard+=2;
								break;
							case SDLK_RIGHT:
								printf( "Right\n" );
								yKeyboard-=2;
								break;
						}

					}
				}

				//Clear screen
				SDL_RenderClear( gRenderer );

				//HERE: ISSUES WITH CAMERA ROTATION. COULD BE BECAUSE OF BOGUS SCALING (SEE *20 AFTER PROJECTION) ________________________________________________________________________________
				//https://stackoverflow.com/questions/60561455/how-do-i-rotate-my-camera-correctly-in-my-3d-world
				//https://www.youtube.com/watch?v=cd2dnF4eelg 
				//https://www.3dgep.com/understanding-the-view-matrix/  

				//To Do: 
				// - Fix the "blowing up" when textures go off screen
				// - change warpTextureBySubdivision name and variable names to reflect new purpose
				// - fix the rectangle image issue. By setting the correct dimensions in GRectangleTexture, it always gives a square... Entering square dimension: gives correct
				// - Fix the fact that changing the camera quaternion rotates the world and not the camera
				// - Fix the arbitrary *20 in warpTextureBySubdivision

				// Camera details
				GMatrix<4,4> cameraDetails = GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,200,1}};
				// Rotating the camera using a quaternion 4x4 matrix
				GQuaternion cameraQuat{0.0f+xKeyboard,0.0f+yKeyboard,0.0f};
				cameraDetails *= cameraQuat.get4RotMat();

				// Main image
				GGraphicalComponent mainObject = GGraphicalComponent{};
				mainObject.addTexturePiece(GRectangleTexture{676,983,3,2},GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}});
				for (size_t i=0;i<mainObject.textures.size();i++) {
					std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp = warpTextureBySubdivision(mainObject.getTextures()[i], mainObject.getTextureTruePosRot()[i], cameraDetails);
					SDL_RenderGeometry(gRenderer, gTextures[mainObject.textures[i].getTextureID()], warp.first.data(), (int)warp.first.size(), warp.second.data(), (int)warp.second.size());
				}

				// The 3 directional pointers
				GGraphicalComponent directionalPointers = GGraphicalComponent{};
				directionalPointers.addTexturePiece(GRectangleTexture{100,10,0,2}, GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1}});
				directionalPointers.addTexturePiece(GRectangleTexture{100,10,1,2}, GMatrix<4,4>{std::array<float,16>{0,0,-1,0,0,1,0,0,1,0,0,0,0,0,0,1}});
				directionalPointers.addTexturePiece(GRectangleTexture{100,10,2,2}, GMatrix<4,4>{std::array<float,16>{0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1}});
				for (size_t i=0;i<directionalPointers.textures.size();i++) {
					std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp = warpTextureBySubdivision(directionalPointers.getTextures()[i], directionalPointers.getTextureTruePosRot()[i], cameraDetails);
					SDL_RenderGeometry(gRenderer, gTextures[directionalPointers.textures[i].getTextureID()], warp.first.data(), (int)warp.first.size(), warp.second.data(), (int)warp.second.size());
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}
}


std::pair<std::vector<SDL_Vertex>,std::vector<int>> MainWindow::warpTextureBySubdivision(GRectangleTexture text,
																						GMatrix<4,4> position,
																						GMatrix<4,4> cameraDetails) {


	std::vector<SDL_Vertex> newVerticies(text.getSrcVertices().size());

	// Need to add third dimension z with value 0, and 4th dimension with value 1 to textures
	std::vector<GVector<4>> D4text;
	for (int i=0;i<text.getSrcVertices().size();i++) {
		D4text.push_back(text.getSrcVertices()[i].extendDimension(0).extendDimension(1));
	}

	// Now move the texture to its position/rotation
	for (int i=0;i<text.getSrcVertices().size();i++) {
		D4text[i] = position*D4text[i];
	}

	//Create projection matrix
	GMatrix<4,4> matProj{};
	float fovAngle = 60.0f;
	float fovRad = 1.0f / tan((fovAngle *0.5f) * (3.14159f/180.0f));
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fAspectRatio = (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH;

	matProj[0] = fAspectRatio*fovRad;
	matProj[5] = fovRad;
	matProj[10] = (fFar+fNear)/(fFar-fNear);
	matProj[11] = 1;
	matProj[14] = (2*fNear*fFar)/(fNear-fFar);
	matProj[15] = 0.0f; //Just in case

	//Apply projection operations
	for (int i=0;i<D4text.size();i++) {

		GVector<3> vec3{D4text[i][0],D4text[i][1],D4text[i][2]};

		// THIS HAS TO DO WITH CAMERA POSITION AND ROTATION
		// THIS ORDER ROTATES THE OBJECT, NOT THE CAMERA.
		// TO rotate camera: Rotate all objects about axis of camera position?

		//Set w to 1
		GVector<4> vec4 = vec3.extendDimension(1);

		//Rotate the shape to fit 'camera rotation'
		GVector<4> vec = cameraDetails*vec4;

		//Apply projection matrix
		vec = matProj*vec;
		
		if (vec[3]!=0.0f) {
			vec[0] = vec[0]/vec[3]*20; //CHANGE THIS FROM 20 TO SOMETHING ELSE
			vec[1] = vec[1]/vec[3]*20; //CHANGE THIS FROM 20 TO SOMETHING ELSE
			vec[2] = vec[2]/vec[3]*20; //CHANGE THIS FROM 20 TO SOMETHING ELSE
			vec[3] = vec[3]/vec[3]*20; //CHANGE THIS FROM 20 TO SOMETHING ELSE
		}

		//Move the shape so that the projection is centered
		// 0,0 is in the center of the viewing screen now, instead of top left
		vec[0] += SCREEN_WIDTH/2 ;
		vec[1] += SCREEN_HEIGHT/2 ;

		newVerticies[i] = {{vec[0],vec[1]},
						{ 0xff,0xff,0xff,0xff },
						{text.getSrcVertices()[i][0]/text.srcWidth,
							text.getSrcVertices()[i][1]/text.srcHeight
						}};
		
	}


	//Construct and return pair
	std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp;
	warp.first = newVerticies;

	std::vector<int> indexList;
	int colCount = 2+text.subdivisionCount;
	for (int c=0;c<colCount-1;c++) {
		for (int r=0;r<colCount-1;r++) {
			indexList.insert(indexList.end(), {r+(c*colCount), r+1+(c*colCount), r+colCount+(c*colCount)});
			indexList.insert(indexList.end(), {r+1+(c*colCount), r+colCount+(c*colCount), r+colCount+1+(c*colCount)});
		}
	}
	
	warp.second = indexList;

	return warp;
}

MainWindow::~MainWindow() {
	close();
}


bool MainWindow::init(std::string windowName)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}

	return success;
}

size_t MainWindow::displayMedia(std::string path)
{

	//Load new texture
	SDL_Texture* gTexture = loadTexture(path);
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		return -1;
	} else {
		gTextures.push_back(gTexture);
		return gTextures.size() - 1;
	}
}

void MainWindow::close()
{
	//Destroying renderer will free all textures
	SDL_DestroyRenderer( gRenderer );
	//Destroy window
	SDL_DestroyWindow( gWindow );

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Texture* MainWindow::loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void MainWindow::freeTexture(size_t index){
	SDL_DestroyTexture(gTextures[index]);
	gTextures.erase(std::next(gTextures.begin(),index));
}