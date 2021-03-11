#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "raylib.h"


typedef enum {
	DIRT = 0,
	STONE = 1,
	COAL = 2,
	RNDSTUFF = 3,
	EMPTY = 4,
	LADDER = 5,
	PROP = 6
} blockType;

typedef struct Block {
    blockType type;  // тут ещё должна быть прочность
} Block;

int rand_modulo(int min, int max) {
	return min + rand() % (max - min);
}

int rand_reduct(int max, int min) {
	return (rand() / (float)INT16_MAX) * (max + 1 - min) + min;
}

bool try_chance(int chance) {
	return rand_reduct(0, chance) == chance-1;
}

void updateCameraCenter(Camera2D *camera, Vector2 *playerPosition, int width, int height)
{
    camera->offset = (Vector2) { width/2, height/2 };
    camera->target = *playerPosition;
}

int main(void)
{
	time_t t;
	srand((unsigned) time(&t));
	
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib test");
	
	/* int */ float blockSideSize = 50;
	
    Vector2 blockSize = {
		blockSideSize,
		blockSideSize
	};
	
	// const float stepSize = blockSideSize;
	Color playerColor = DARKGRAY;
	
	/*
    Vector2 playerPosition = {
		(float)screenWidth /2 - blockSize.x/2,
		(float)screenHeight/2 - blockSize.y/2
	};
	*/
	int player_x = 16;
	int player_y = -1;
	Vector2 playerPosition = {
		(float)blockSize.x * player_x,
		(float)blockSize.y * player_y
	};
	
	int money = 0;
	
	Camera2D camera = { 0 };
    camera.target = playerPosition;
    camera.offset = (Vector2) { screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
	
	
	
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
	const int amount_of_blockTypes = 4;
	Color BlockColors[amount_of_blockTypes];
	BlockColors[DIRT] = BROWN;  // Color dirtBlockColor = BROWN;
	BlockColors[STONE] = GRAY;
	BlockColors[COAL] = BLACK;
	BlockColors[RNDSTUFF] = LIGHTGRAY;
	
	const int chanse_to_be_stone = 15;  // 1/15, 6,6(6)%
	const int chanse_to_be_resourse = 6;  // 1/7, 16,6(6)%
	const int chanse_to_be_stuff = 10;  // 1/10, 10%
	
	const int gameFieldWidth = 32;
	const int gameFieldHeight = 32;
	Block blocks[gameFieldWidth][gameFieldHeight];
	for(int x = 0; x < gameFieldWidth; ++x)
		for(int y = 0; y < gameFieldWidth; ++y)
		{
			// в идеале нужно сделать такой рандом, который для люого блока сразу определяет его тип,
			// например, с большим типом seed, т.е. будет брать первое значение от этого seed
			// (при реализации миров с разными seed, часть seed рандома будет равняться seed мира, другая идентификатору блока)
			// но если делать "втупую", то это будет просто одно из случайных распределений, а не интеллектуальное,
			// где блоки будут представлять взаимосвязанную структуру;
			// например, чтобы ресурс было тяжелее добывать, он был под камнем/рядом с лавой
			bool is_stone = try_chance(chanse_to_be_stone);
			blocks[x][y] = (Block) { 0 };
			if(is_stone)
				blocks[x][y].type = STONE;
			else if( y > 0 && ( blocks[x][y-1].type == STONE
				|| (x > 0 && x+1 < gameFieldWidth && (blocks[x-1][y-1].type == STONE && blocks[x+1][y-1].type == STONE)) )
				&& try_chance(chanse_to_be_resourse) ) {
					bool is_stuff = try_chance(chanse_to_be_stuff);
					if(is_stuff)
						blocks[x][y].type = RNDSTUFF;
					else
						blocks[x][y].type = COAL;
				}
			else
				blocks[x][y].type = DIRT;
		}
	
	// Block block1 = { 0 };
	/*
	block1.position = (Vector2) {
		(float)playerPosition.x,
		(float)playerPosition.y + blockSize.y
	};
	*/
	// block1.type = DIRT;
	
	Vector2 cursorPosition = { 0.0f, 0.0f };
	char buffer [50];
	
    // Main game loop
    while ( !WindowShouldClose() )    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
		if ( IsKeyPressed(KEY_RIGHT) ) {
			if(player_x+1 < gameFieldWidth) {
				if(player_y == -1)
					++player_x;
				else {
					blockType block_type = blocks[player_x+1][player_y].type;
					if(block_type != STONE) {
						++player_x;
						if(block_type == DIRT || block_type == COAL || block_type == RNDSTUFF) {
							blocks[player_x][player_y].type = EMPTY;
							if(block_type == DIRT)
								money += 1;
							else if(block_type == COAL)
								money += 3;
							else {
								
							}
						}
					}
				}
			}
		}
        if ( IsKeyPressed(KEY_LEFT ) ) {
			if(player_x > 0) {
				if(player_y == -1)
					--player_x;
				else {
					blockType block_type = blocks[player_x-1][player_y].type;
					if(block_type != STONE) {
						--player_x;
						if(block_type == DIRT || block_type == COAL || block_type == RNDSTUFF) {
							blocks[player_x][player_y].type = EMPTY;
							if(block_type == DIRT)
								money += 1;
							else if(block_type == COAL)
								money += 3;
							else {
								
							}
						}
					}
				}
			}
		}
        if ( IsKeyPressed(KEY_UP   ) ) {
			if(player_y > -1) {
				if(player_y) {  // != 0
					blockType block_type = blocks[player_x][player_y-1].type;
					if(block_type != STONE) {
						--player_y;
						if(block_type == DIRT || block_type == COAL || block_type == RNDSTUFF) {
							blocks[player_x][player_y].type = EMPTY;
							if(block_type == DIRT)
								money += 1;
							else if(block_type == COAL)
								money += 3;
							else {
								
							}
						}
					}
				}
				else
					--player_y;
				
				// if(player_y == -1) { // на поверхности }
			}
		}
        if ( IsKeyPressed(KEY_DOWN ) ) {
			if(player_y+1 < gameFieldHeight) {
				blockType block_type = blocks[player_x][player_y+1].type;
				if(block_type != STONE) {
					++player_y;
					if(block_type == DIRT || block_type == COAL || block_type == RNDSTUFF) {
						blocks[player_x][player_y].type = EMPTY;
						if(block_type == DIRT)
							money += 1;
						else if(block_type == COAL)
							money += 3;
						else {
							
						}
					}
				}
				
				// if(player_y > -1) { // под землёй }
			}
		}
        //----------------------------------------------------------------------------------
		
		if ( IsKeyPressed(KEY_KP_SUBTRACT) ) camera.zoom = 0.5f;  // blockSideSize = 25, 37, 50
		if ( IsKeyPressed(KEY_KP_ADD) ) camera.zoom = 1.0f;
		
		if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
			cursorPosition = GetMousePosition();
			// n = sprintf(buffer, "(%f, %f)", cursorPosition.x, cursorPosition.y); DrawText(buffer, 10, 10, 20, DARKGRAY);
			if(cursorPosition.x > 750.0f && cursorPosition.y < 50.0f) {
				// button clicked
			}
		}
		
		/*
		blockSize = (Vector2) {
			blockSideSize,
			blockSideSize
		};*/
		
		playerPosition = (Vector2) {
			blockSize.x * player_x,
			blockSize.y * player_y
		};
		
		// здесь оверинжиниринг, можно просто рассчитывать, какой блок находится в области видимости, и с них начинать отрисовку
		// Call update camera function by its pointer
		updateCameraCenter(&camera, &playerPosition, screenWidth, screenHeight);
		//----------------------------------------------------------------------------------
		
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
		{
            ClearBackground(RAYWHITE);
			
			BeginMode2D(camera);
			{
				for(int x = 0; x < gameFieldWidth; ++x)
					for(int y = 0; y < gameFieldWidth; ++y)
						if(blocks[x][y].type != EMPTY) {
							Vector2 position = (Vector2) {
								blockSize.x * x,
								blockSize.y * y
							};
							DrawRectangleV(position, blockSize, BlockColors[ blocks[x][y].type ]);
						}
				// DrawRectangleV(block1.position, blockSize, BlockColors[block1.type]);
				
				DrawRectangleV(playerPosition, blockSize, playerColor);
			}
			EndMode2D();
			
			// DrawRectangleV((Vector2){screenWidth/2, screenHeight/2}, blockSize, playerColor);
			
			DrawText("move with arrow keys", 10, 10, 20, DARKGRAY);
			
			// GUI
			DrawRectangle(750, 0, 50, 50, DARKGREEN);
			sprintf(buffer, "%d$", money);
			DrawText(buffer, 760, 60, 20, DARKGRAY);
		}
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}