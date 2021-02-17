#include "raylib.h"


typedef enum {DIRT, STONE} blockType;

typedef struct Block {
    Vector2 position;
    blockType type;
} Block;


void updateCameraCenter(Camera2D *camera, Vector2 *playerPosition, int width, int height)
{
    camera->offset = (Vector2){ width/2, height/2 };
    camera->target = *playerPosition;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");

    Vector2 blockSize = {
		(float)50,
		(float)50
	};

    Vector2 playerPosition = {
		(float)screenWidth /2 - blockSize.x/2,
		(float)screenHeight/2 - blockSize.y/2
	};
	
	Camera2D camera = { 0 };
    camera.target = playerPosition;
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
	
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    const float stepSize = 50;
	Color playerColor = DARKGRAY;
	Color dirtBlockColor = BROWN;
	Color stoneBlockColor = GRAY;
	
	Block block1 = {
		0
	};
	block1.position = (Vector2){
		(float)playerPosition.x,
		(float)playerPosition.y + blockSize.y
	};
	block1.type = DIRT;
	
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
		if ( IsKeyPressed(KEY_RIGHT) ) playerPosition.x += stepSize;
        if ( IsKeyPressed(KEY_LEFT ) ) playerPosition.x -= stepSize;
        if ( IsKeyPressed(KEY_UP   ) ) playerPosition.y -= stepSize;
        if ( IsKeyPressed(KEY_DOWN ) ) playerPosition.y += stepSize;
        //----------------------------------------------------------------------------------
		
		// Call update camera function by its pointer
		updateCameraCenter(&camera, &playerPosition, screenWidth, screenHeight);
		//----------------------------------------------------------------------------------
		
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
			
            ClearBackground(RAYWHITE);
			
            DrawText("move with arrow keys", 10, 10, 20, DARKGRAY);
			
			BeginMode2D(camera);
				
				if(block1.type == DIRT)
					DrawRectangleV(block1.position, blockSize, dirtBlockColor);
				else if(block1.type == STONE)
					DrawRectangleV(block1.position, blockSize, stoneBlockColor);
				
				DrawRectangleV(playerPosition, blockSize, playerColor);
				
			EndMode2D();
			
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}