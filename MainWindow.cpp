/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include "MainWindow.h"
#include <cmath>
#include "GMatrix.h"
#include "GRectangleTexture.h"
#include "WorldObject.h"


MainWindow::MainWindow(std::string windowName)
{
	if( !init(windowName) )
	{
		printf( "Failed to initialize!\n" );
	} else {
		// Load all textures (since there are so few right now)
		if( textureSet.findOrLoadTexture("red.bmp" ) == -1 || 
			textureSet.findOrLoadTexture("blue.bmp" ) == -1 || 
			textureSet.findOrLoadTexture("green.bmp" ) == -1 || 
			textureSet.findOrLoadTexture("clueless.bmp" ) == -1)
		{
			printf( "Failed to load media!\n" );
		}
		else // Successfully loaded textures
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			// Camera details
			WorldObject cameraDetails = WorldObject{GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,-800,1}}, GGraphicalComponent{}};

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
								printf( "Up\n" );
								cameraDetails.pos.rotate(-2,0,0);
								break;
							case SDLK_DOWN:
								printf( "Down\n" );
								cameraDetails.pos.rotate(2,0,0);
								break;

							case SDLK_LEFT:
								printf( "Left\n" );
								cameraDetails.pos.rotate(0,-2,0);
								break;
							case SDLK_RIGHT:
								printf( "Right\n" );
								cameraDetails.pos.rotate(0,2,0);
								break;

							case SDLK_w:
								printf( "w\n" );
								cameraDetails.pos.translate(GVector<3>{0,0,10}, false);
								break;
							case SDLK_a:
								printf( "a\n" );
								cameraDetails.pos.translate(GVector<3>{-10,0,0}, false);
								break;
							case SDLK_s:
								printf( "s\n" );
								cameraDetails.pos.translate(GVector<3>{0,0,-10}, false);
								break;
							case SDLK_d:
								printf( "d\n" );
								cameraDetails.pos.translate(GVector<3>{10,0,0}, false);
								break;
						}

					}
				}

				std::cout << cameraDetails.pos.getWorldPosition() << "\n";

				//Clear screen
				SDL_RenderClear( gRenderer );

				//HERE: ISSUES WITH CAMERA ROTATION.
				//https://stackoverflow.com/questions/60561455/how-do-i-rotate-my-camera-correctly-in-my-3d-world
				//https://www.3dgep.com/understanding-the-view-matrix/  
				//https://stackoverflow.com/questions/724219/how-to-convert-a-3d-point-into-2d-perspective-projection
				//https://gamedev.stackexchange.com/questions/178643/the-view-matrix-finally-explained

				//To Do: 
				// - Fix objects behind camera appearing anyway (but flipped) ... SDL clips by default when texture goes off screen, but not when behind?
				// - change warpTextureBySubdivision name and variable names to reflect new purpose
				// - fix the rectangle image issue. By setting the correct dimensions in GRectangleTexture, it always gives a square... Entering square dimension: gives correct
				// - Make a PI constnat


				// Main image
				WorldObject mainObject = WorldObject{};
				mainObject.graphics.addTexturePiece(GRectangleTexture{676,983,textureSet.findOrLoadTexture("clueless.bmp"),2},GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}});
				for (size_t i=0;i<mainObject.graphics.textures.size();i++) {
					std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp = warpTextureBySubdivision(mainObject.graphics.getTextures()[i], mainObject.getTextureTruePosRot()[i], cameraDetails.pos.getWorldPosition());
					SDL_RenderGeometry(gRenderer, 
										textureSet.getTexture(mainObject.graphics.textures[i].getTextureID()),
										warp.first.data(), 
										(int)warp.first.size(), 
										warp.second.data(), 
										(int)warp.second.size());
				}

				/*
				// The 3 directional pointers
				WorldObject directionalPointers = WorldObject{};
				directionalPointers.graphics.addTexturePiece(GRectangleTexture{100,10,textureSet.findOrLoadTexture("red.bmp"),2}, GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1}});
				directionalPointers.graphics.addTexturePiece(GRectangleTexture{100,10,textureSet.findOrLoadTexture("blue.bmp"),2}, GMatrix<4,4>{std::array<float,16>{0,0,-1,0,0,1,0,0,1,0,0,0,0,0,0,1}});
				directionalPointers.graphics.addTexturePiece(GRectangleTexture{100,10,textureSet.findOrLoadTexture("green.bmp"),2}, GMatrix<4,4>{std::array<float,16>{0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1}});
				for (size_t i=0;i<directionalPointers.graphics.textures.size();i++) {
					std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp = warpTextureBySubdivision(directionalPointers.graphics.getTextures()[i], directionalPointers.getTextureTruePosRot()[i], cameraDetails.pos.getWorldPosition());
					SDL_RenderGeometry(gRenderer, 
										textureSet.getTexture(directionalPointers.graphics.textures[i].getTextureID()), 
										warp.first.data(), 
										(int)warp.first.size(), 
										warp.second.data(), 
										(int)warp.second.size());
				}
				*/

				// Render text to the screen ( THIS IS HORRIBLY INNEFICIENT - it creates a new texture every frame)
				// Adapt this to the new TextureSet class
					// dynamicTextTexture class would not regenerate the texture unless you call .changeText()
				//alternate solution: 
				TTF_Font* Sans = TTF_OpenFont("OpenSans-Regular.ttf",24);
				SDL_Color Black = {1,1,1};
				SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "temp text HERE", Black);
				SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
				SDL_Rect message_rect;
				message_rect.x = 50;
				message_rect.y=50;
				message_rect.w=100;
				message_rect.h=100;
				SDL_RenderCopy(gRenderer, Message, NULL, &message_rect);
				SDL_FreeSurface(surfaceMessage);
				SDL_DestroyTexture(Message);
				TTF_CloseFont(Sans);

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

	// TBD: Allow fovAngle to change based on user choice. Example game mechanic: higher velocity -> wider fov to "feel" speed?
	//Create projection/clip matrix
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

	//Apply operations
	for (int i=0;i<D4text.size();i++) {

		GVector<3> vec3{D4text[i][0],D4text[i][1],D4text[i][2]};

		//Set w to 1
		GVector<4> vec4 = vec3.extendDimension(1);

		//VIEW TRANSFORMATION
		//Transform the shape to fit 'camera state'
		GVector<4> vec = cameraDetails.getInverse()*vec4; 
		//std::cout << vec4 << " - becomes " << vec << "\n";


		//PROJECTION TRANSFORMATION
		//Apply projection matrix
		vec = matProj*vec;

		if (vec[3]!=0.0f) {
			vec[0] = vec[0]/vec[3];
			vec[1] = vec[1]/vec[3];
			vec[2] = vec[2]/vec[3];
		}

		//Move  so that the projection is centered
		// 0,0 is in the center of the viewing screen now, instead of top left
		vec[0] = ((vec[0]+1) /2 * SCREEN_WIDTH);
		vec[1] = ((vec[1]+1) /2 * SCREEN_HEIGHT);


		newVerticies[i] = {{vec[0],vec[1]},
						{ 0xff,0xff,0xff,0xff },
						{text.getSrcVertices()[i][0]/text.srcWidth,
							text.getSrcVertices()[i][1]/text.srcHeight
						}};
		
	}

	// Prepare render triangles (SDL_rendergeometry uses a simple set of triangles formed from {x,y} coordinates to render textures)
	
	//First set the vertices (the {x,y} vertices)
	std::pair<std::vector<SDL_Vertex>,std::vector<int>> warp;
	warp.first = newVerticies;

	// Create vertices index list
	// This code takes each "sub-rectangle" and adds 2 triangles to the set.
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
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // Failed to initizalize
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

				//Renderer successfully created; initalize the textureSet object
				this->textureSet = GTextureSet();
				textureSet.init(gRenderer);

			}
		}
	}

	return success;
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
	TTF_Quit();
}
