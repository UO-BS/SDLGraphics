
#include "GEngine/MainWindow.h"
#include "GEngine/Entity.h"
#include "GEngine/GShaderLoader.h"
#include "GEngine/GMeshBuilder.h"


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
	const GTexture* redTexture    = assetManager.loadTexture("assets/red.bmp");
    const GTexture* blueTexture   = assetManager.loadTexture("assets/blue.bmp");
    const GTexture* greenTexture  = assetManager.loadTexture("assets/green.bmp");
	const GTexture* gradientTexture  = assetManager.loadTexture("assets/gradient.bmp");
    const GTexture* cluelessTexture  = assetManager.loadTexture("assets/clueless.bmp");

	// Generate mesh vertices
    std::vector<GTextureVertex> cube1Vertices = GMeshBuilder::CreateCube(50.0f, 50.0f, 50.0f);
	std::vector<GTextureVertex> cube2Vertices = GMeshBuilder::CreateCube(600.0f, 600.0f, 600.0f);

    // Upload geometry vectors to VRAM and cache their pointers inside the manager
    const GMesh* cluelessMesh = assetManager.loadMesh("cube1_face", cube1Vertices, cluelessTexture);
	const GMesh* gradientMesh = assetManager.loadMesh("cube2_face", cube2Vertices, gradientTexture);

	// Create my Entity list
	std::vector<Entity> entities;

	// Create Camera
	Entity camera = Entity{nullptr};
	camera.pos.transform(GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,-250,1}});

	// Create Objects
	Entity mainObject = Entity{cluelessMesh};
	entities.push_back(mainObject);
	Entity backgroundObject = Entity{gradientMesh};
	entities.push_back(backgroundObject);
	

	// TODO: Allow fovAngle to change
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

	// Compile and link Vertex and Fragment shaders into a single GPU worker program
	shaderProgramID = GShaderLoader::CreateShaderProgram("assets/vertex.glsl", "assets/fragment.glsl");
	if (shaderProgramID == 0) {
		std::cerr << "CRITICAL ERROR: Failed to create the GPU Shader Program.\n";
		return;
	}

	// Query the GPU for the handles of the shader variables
	shaderMatrixLocation   = glGetUniformLocation(shaderProgramID, "modelMatrix");
	shaderViewProjLocation = glGetUniformLocation(shaderProgramID, "viewProjMatrix");

	//Event handler
	SDL_Event e;
	
	//Main loop flag
	bool quit = false;

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
			// Read individual key presses here (not continuous keypresses)
		}


		// Read the currently pressed down keys
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		// Camera Rotations (Arrow keys and QE)
		if (currentKeyStates[SDL_SCANCODE_UP])    	camera.pos.rotate(-2, 0, 0);
		if (currentKeyStates[SDL_SCANCODE_DOWN])  	camera.pos.rotate(2, 0, 0);
		if (currentKeyStates[SDL_SCANCODE_LEFT])  	camera.pos.rotate(0, -2, 0);
		if (currentKeyStates[SDL_SCANCODE_RIGHT]) 	camera.pos.rotate(0, 2, 0);
		if (currentKeyStates[SDL_SCANCODE_E])  		camera.pos.rotate(0, 0, -2);
		if (currentKeyStates[SDL_SCANCODE_Q]) 		camera.pos.rotate(0, 0, 2);
		// Camera Translations (WASD)
		if (currentKeyStates[SDL_SCANCODE_W]) camera.pos.translate(GVector<3>{0.0f, 0.0f, 5.0f}, false);
		if (currentKeyStates[SDL_SCANCODE_S]) camera.pos.translate(GVector<3>{0.0f, 0.0f, -5.0f}, false);
		if (currentKeyStates[SDL_SCANCODE_A]) camera.pos.translate(GVector<3>{-5.0f, 0.0f, 0.0f}, false);
		if (currentKeyStates[SDL_SCANCODE_D]) camera.pos.translate(GVector<3>{5.0f, 0.0f, 0.0f}, false);
		// Camera Elevation (Shift and Control)
		if (currentKeyStates[SDL_SCANCODE_LSHIFT]) camera.pos.translate(GVector<3>{0.0f, 5.0f, 0.0f}, true);
		if (currentKeyStates[SDL_SCANCODE_LCTRL])  camera.pos.translate(GVector<3>{0.0f, -5.0f, 0.0f}, true);


		// Clear the canvas
		glClearColor(0.12f, 0.12f, 0.14f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Load the graphics pipeline shaders
		glUseProgram(shaderProgramID);

        // Pass the inverted camera View and Projection matrix to the shaderViewProjLocation for this frame
        GMatrix<4,4> viewProjMatrix = matProj * (camera.pos.getWorldPosition().getInverse());
        glUniformMatrix4fv(shaderViewProjLocation, 1, GL_FALSE, viewProjMatrix.getRaw());

        // Iterate over entities - drawing them
        for (const Entity& entity : entities) {
			// Pass the address where we write the entities current 4x4 Matrix location, and ask it to draw
            entity.draw(shaderMatrixLocation);
        }

        // Swap the canvas layer and the actual screen (display what ive been drawing)
        SDL_GL_SwapWindow(gWindow);
	}
}

MainWindow::~MainWindow() {
	close();
}


bool MainWindow::init(std::string windowName)
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) // Failed to initialize
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Configure OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    // 24-bit depth buffer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double-buffering to stop screen tearing
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

	// Create window
	gWindow = SDL_CreateWindow( 
		windowName.c_str(), 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		SDL_WINDOW_OPENGL );
	
	if( gWindow == nullptr )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		close();
		return false;
	}


	// Create Rendering Context (physical rendering timeline session?)
    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == nullptr) 
    {
        printf("OpenGL Context could not be created! SDL Error: %s\n", SDL_GetError());
        close();
        return false;
    }

	// Start GLAD - We need to pass SDL OS function lookup tool to GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) 
    {
        printf("Failed to initialize GLAD function loaders!\n");
        close();
        return false;
    }

	// Enable Depth Testing
    glEnable(GL_DEPTH_TEST);
    
    // Define viewport boundaries
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		close();
		return false;
    }

	// Initialize the asset manager object
	assetManager.init();

	return true;
}

void MainWindow::close()
{
	this->initialized = false;

	// Clear assets
	assetManager.clearAll();
	//Destroying renderer will free all textures
	if (gContext) {
		SDL_GL_DeleteContext( gContext );
		gContext = nullptr;
	}
	//Destroy window
	if (gWindow) {
		SDL_DestroyWindow( gWindow );
		gWindow = nullptr;
	}

	// Quit SDL ttf
	TTF_Quit();
	//Quit SDL subsystems
	SDL_Quit();
}
