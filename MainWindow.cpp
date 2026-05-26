
#include "MainWindow.h"
#include <cmath>
#include "GMatrix.h"
#include "WorldObject.h"
#include <bitset>


MainWindow::MainWindow(std::string windowName)
{
	if( !init(windowName) )
	{
		printf( "Failed to initialize!\n" );
	} else {
		this->initialized = true;
	}
}


void MainWindow::run() {
	if (!initialized) {
		printf( "MainWindow not initialized! run failed. \n" );
		return;
	}

	// Load all textures on startup (since there are so few right now)
	if( textureSet.findOrLoadTexture("red.bmp") == -1 || 
		textureSet.findOrLoadTexture("blue.bmp") == -1 || 
		textureSet.findOrLoadTexture("green.bmp") == -1 || 
		textureSet.findOrLoadTexture("clueless.bmp") == -1 )
	{
		printf( "Failed to load media!\n" );
		return;
	}

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
						cameraDetails.pos.translate(GVector<3>{0,0,10.0f}, false);
						break;
					case SDLK_a:
						printf( "a\n" );
						cameraDetails.pos.translate(GVector<3>{-10.0f,0,0}, false);
						break;
					case SDLK_s:
						printf( "s\n" );
						cameraDetails.pos.translate(GVector<3>{0,0,-10.0f}, false);
						break;
					case SDLK_d:
						printf( "d\n" );
						cameraDetails.pos.translate(GVector<3>{10.0f,0,0}, false);
						break;
				}

			}
		}

		//Clear screen
		SDL_RenderClear( gRenderer );


		// TODO: Remove this from the render loop | Create the objects (should only be done once)
		std::vector<WorldObject> worldObjects;
		WorldObject mainObject = WorldObject{};
		mainObject.graphics.addPolygon(GTexturePolygon{100, 100, textureSet.findOrLoadTexture("clueless.bmp")}, GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}});
		worldObjects.push_back(mainObject);

		/*
			My Graphics Pipeline:

			Polygons
			Model Space (GGraphicalComponent Position/Rotation)
			World Space (WorldObject Position/Rotation)
			Camera Space (Camera Inverse)
			Clip Space (Projection matrix)

			--- Now we can apply clipping based on if the polygons are visible
			--- Now we can apply subdivision based on distance from camera
			
			Perspective Divide
			Screen Space (convert to pixel values)
			Render with SDL_RenderGeometry
		*/

		std::vector<GTexturePolygon> polygonCopies = entityToClipSpace(worldObjects, cameraDetails);

		// Perform Clipping
		clipTriangles(polygonCopies);

		// Perform subdivision based on distance from camera (affects all x y z w u v values)

		std::unordered_map<uint32_t, std::vector<SDL_Vertex>> drawBatches = clipSpaceToDrawBatch(polygonCopies);

		// Draw all objects using SDL_RenderGeometry
		draw(drawBatches);
		

		// TODO: Fix the text rendering code below
		// Render text to the screen ( THIS IS HORRIBLY INEFFICIENT - it creates a new texture every frame)
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

uint8_t MainWindow::computeOutCode(GVector<4> vertex) {
    uint8_t outCode{0};

	outCode |= (vertex[2] < -vertex[3]) * PlaneIndex::NEAR_PLANE; // Near plane
	outCode |= (vertex[0] < -vertex[3]) * PlaneIndex::LEFT_PLANE; // Left plane
	outCode |= (vertex[0] > vertex[3]) * PlaneIndex::RIGHT_PLANE; // Right plane
	outCode |= (vertex[1] > vertex[3]) * PlaneIndex::TOP_PLANE; // Top plane
	outCode |= (vertex[1] < -vertex[3]) * PlaneIndex::BOTTOM_PLANE; // Bottom plane
	outCode |= (vertex[2] > vertex[3]) * PlaneIndex::FAR_PLANE; // Far plane

    return outCode;
}


void MainWindow::clipTriangles(std::vector<GTexturePolygon>& polygons) {
	// Loop through each polygon
	for (size_t i=0;i<polygons.size();i++) {
		// We only need to loop through the original triangles
		size_t oldTriangleAmount = polygons[i].triangles.size();
		
		// Loop through each of the original triangles
		for (size_t j=0;j<oldTriangleAmount;) {

			// Outcode of each vertex
			std::vector<uint8_t> outCodes(3);
			outCodes[0] = computeOutCode(polygons[i].triangles[j].vertices[0].worldVertex);
			outCodes[1] = computeOutCode(polygons[i].triangles[j].vertices[1].worldVertex);
			outCodes[2] = computeOutCode(polygons[i].triangles[j].vertices[2].worldVertex);

			// outCodes[0] | outCodes[1] | outCodes[2] == 000000 -> Triangle is fully visible
			if ((outCodes[0] | outCodes[1] | outCodes[2]) == 000000) {
				j++;
				continue;
			}
			// outCodes[0] & outCodes[1] & outCodes[2] != 000000 -> Triangle is fully invisible, remove it
			if ((outCodes[0] & outCodes[1] & outCodes[2]) != 000000) {
				polygons[i].removeTriangle(j);
				oldTriangleAmount--;
				continue;
			}

			// By reaching this point, we know that the triangle needs to be clipped at least once
			std::vector<GTextureVertex> vertexBuffer1{std::begin(polygons[i].triangles[j].vertices), std::end(polygons[i].triangles[j].vertices)};
			std::vector<GTextureVertex> vertexBuffer2;
			std::vector<uint8_t> outCodesBuffer;

			// Begin clipping planes in the following order:
			// Near Plane (this is important to do first since vertices behind the near plane can cause issues with the other planes)
			// Left and Right (often users will look left and right more than up and down)
			// Top and bottom
			// Far

			// If any of the outcodes have the plane's bit set, we need to clip against that plane.
			for (size_t plane=0;plane<6;plane++) {
				uint8_t planeBit = 1 << plane;

				for (size_t vertexIndex=0;vertexIndex<vertexBuffer1.size();vertexIndex++) {
					// Note: since we are using size_t (unsigned) we have to be careful when doing vertexIndex-1... Only use it when vertexIndex > 0
					size_t previousVertexIndex = (vertexIndex == 0) ? (vertexBuffer1.size() - 1) : (vertexIndex - 1);

					if ((outCodes[vertexIndex] & planeBit) != (outCodes[previousVertexIndex] & planeBit)) {
						// One vertex is inside and the other is outside, so find the intersection point and add it to the buffer
						
						// Calculate and add intersection point
						float dPrev = vertexBuffer1[previousVertexIndex].worldVertex[3] + (signMap[plane] * vertexBuffer1[previousVertexIndex].worldVertex[planeComponentMap[plane]]);
						float dCurr = vertexBuffer1[vertexIndex].worldVertex[3] + (signMap[plane] * vertexBuffer1[vertexIndex].worldVertex[planeComponentMap[plane]]);
						float t = dPrev / (dPrev - dCurr);
						GTextureVertex intersectionVertex{
							vertexBuffer1[previousVertexIndex].worldVertex + ((vertexBuffer1[vertexIndex].worldVertex - vertexBuffer1[previousVertexIndex].worldVertex) * t),
							vertexBuffer1[previousVertexIndex].textureVertex + ((vertexBuffer1[vertexIndex].textureVertex - vertexBuffer1[previousVertexIndex].textureVertex) * t)
						};
						vertexBuffer2.push_back(intersectionVertex);
						outCodesBuffer.push_back(computeOutCode(intersectionVertex.worldVertex));
					}

					if ((outCodes[vertexIndex] & planeBit) == 0) {
						// Current vertex is inside the plane, so add it to the buffer

						// Copy the current vertex
						vertexBuffer2.push_back(vertexBuffer1[vertexIndex]);
						outCodesBuffer.push_back(outCodes[vertexIndex]);
					}
				}

				// Now we move onto the next plane, so we need to move and clear the buffers
				vertexBuffer1 = vertexBuffer2; // Move the new vertices to the main buffer
				outCodes = outCodesBuffer; // Move the new outcodes to the main buffer
				vertexBuffer2.clear(); // Clear output buffer
				outCodesBuffer.clear(); // Clear output buffer

				// Verify that we still have >= 3 vertices to form a triangle, otherwise we can stop immediately
				if (vertexBuffer1.size() < 3) {
					break;
				}
			}

			// We have our clipped polygon in vertexBuffer1 (unless we have <3 vertices)
			if (vertexBuffer1.size() < 3) {
				// Remove invisible triangle
				polygons[i].removeTriangle(j);
				oldTriangleAmount--;
				continue;
			} else {
				// Remove original triangle and add the clipped polygon
				polygons[i].removeTriangle(j);
				oldTriangleAmount--;
				polygons[i].appendConvexPolygon(vertexBuffer1);
				continue;
			}
		}

	}

}

void MainWindow::draw(std::unordered_map<uint32_t, std::vector<SDL_Vertex>> drawBatches) {
	for (const auto& [textureID, vertices] : drawBatches) {
		if (vertices.size() <= 0) {continue;}
		SDL_RenderGeometry(gRenderer, textureSet.getTexture(textureID)->texture, vertices.data(), vertices.size(), nullptr, 0);
	}
}

/*
	Perform perspective divide (affects x y z values)
	Perform final conversion to pixel values (x y affected)
	Create SDL_Vertex objects from x y u v values
*/
std::unordered_map<uint32_t, std::vector<SDL_Vertex>> MainWindow::clipSpaceToDrawBatch(std::vector<GTexturePolygon> clipSpacePolygons) {
	std::unordered_map<uint32_t, std::vector<SDL_Vertex>> textureBuckets;

	for (size_t i=0;i<clipSpacePolygons.size();i++) {
		for (size_t j=0;j<clipSpacePolygons[i].triangles.size();j++) {
			for (size_t k=0;k<3;k++) {
				// 1. Perform perspective divide (divide x y z by w)
				clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[0] /= clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[3];
				clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[1] /= clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[3];
				clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[2] /= clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[3];
				
				// 2. Convert to pixel values (0,0 is in the center of the screen right now, we need to move it to the top left and convert to actual pixel counts)
				float X = ((clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[0]+1) /2 * SCREEN_WIDTH);
				float Y = ((clipSpacePolygons[i].triangles[j].vertices[k].worldVertex[1]+1) /2 * SCREEN_HEIGHT);
				
				// 3. Create SDL_Vertex objects from x y u v values (useless color value is also needed)
				// 4. Pass the SDL_Vertex objects to the correct texture bucket
				textureBuckets[clipSpacePolygons[i].textureID]
					.push_back({{X,Y},
						{ 0xff,0xff,0xff,0xff },
						{clipSpacePolygons[i].triangles[j].vertices[k].textureVertex[0],
						clipSpacePolygons[i].triangles[j].vertices[k].textureVertex[1]
						}});
			}
		}
	}

	return textureBuckets;
}

std::vector<GTexturePolygon> MainWindow::entityToClipSpace(std::vector<WorldObject> worldObjects, WorldObject cameraDetails) {
	// Master polygon list
	std::vector<GTexturePolygon> polygonCopies;

	// Create Projection matrix (this will be the same for all objects, so we only need to create it once)
	// TODO: Allow fovAngle to change.
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

	// Create camera inverse matrix 
	GMatrix<4,4> camInverse = cameraDetails.pos.getWorldPosition().getInverse();

	// Create the start of the combined transformation matrix
	GMatrix<4,4> transformMatrix = matProj * camInverse;

	// Loop through each graphical component
	for (size_t i=0;i<worldObjects.size();i++) {
		transformMatrix = transformMatrix * worldObjects[i].pos.getWorldPosition();

		// Loop through each polygon in the graphical component
		for (size_t j=0;j<worldObjects[i].graphics.polygons.size();j++) {
			transformMatrix = transformMatrix * worldObjects[i].graphics.polygonPosRot[j];

			// Make a clip space copy of the polygon and add it to the master list
			polygonCopies.push_back(GTexturePolygon{worldObjects[i].graphics.polygons[j]}.applyTransformation(transformMatrix));
		}
	}
	
	return polygonCopies;
}

MainWindow::~MainWindow() {
	close();
}


bool MainWindow::init(std::string windowName)
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // Failed to initizalize
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	
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
		close();
		return false;
	}
	
	//Create renderer for window
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
	if( gRenderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		close();
		return false;
	}

	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		close();
		return false;
    }
	
	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Renderer successfully created; initialize the textureSet object
	textureSet.init(gRenderer);

	return true;
}

void MainWindow::close()
{
	this->initialized = false;

	// Clear texture manager
	textureSet.clear();
	//Destroying renderer will free all textures
	if (gRenderer) {SDL_DestroyRenderer( gRenderer );}
	//Destroy window
	if (gWindow) {SDL_DestroyWindow( gWindow );}

	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL ttf
	TTF_Quit();
	//Quit SDL subsystems
	SDL_Quit();
}
