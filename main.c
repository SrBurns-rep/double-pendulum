#include "raylib.h"
#include <math.h>

typedef struct {
	float r1;
	float r2;
	float theta1;
	float theta2;
	float m1;
	float m2;
	float g;
	float acc1;
	float acc2;
	float v1;
	float v2;
	Vector2 origin;
	Vector2 pos1;
	Vector2 pos2;
}Double_pendulum;

Double_pendulum init_double_pendulum(float x, float y, float r1, float r2, float m1, float m2, float g){
	Double_pendulum pen = {
		.r1 = r1,
		.r2 = r2,
		.theta1 = PI / 2,
		.theta2 = PI / 2,
		.m1 = m1,
		.m2 = m2,
		.g = g,
		.origin = {x, y}
	};
	
	return pen;
}

void update_double_pendulum(Double_pendulum *pen){

	float part1, part2, part3, part4, part5;

	part1 = -pen->g*(2 * pen->m1 + pen->m2)*sinf(pen->theta1);
	part2 = -pen->m2*pen->g*sinf(pen->theta1 - 2 * pen->theta2);
	part3 = -2*sinf(pen->theta1 - pen->theta2)*pen->m2;
	part4 = pen->v2 * pen->v2 * pen->r2 + pen->v1 * pen->v1 * pen->r1 * cosf(pen->theta1 - pen->theta2);
	part5 = pen->r1 * (2 * pen->m1 + pen->m2 - pen->m2 * cosf(2 * pen->theta1 - 2 * pen->theta2));

	pen->acc1 = (part1 + part2 + part3 * part4) / part5;

	part1 = 2 * sinf(pen->theta1 - pen->theta2);
	part2 = pen->v1 * pen->v1 * pen->r1 * (pen->m1 + pen->m2);
	part3 = pen->g * (pen->m1 + pen->m2) * cosf(pen->theta1);
	part4 = pen->v2 * pen->v2 * pen->r2 * pen->m2 * cosf(pen->theta1 - pen->theta2);
	part5 = pen->r2 * (2 * pen->m1 + pen->m2 - pen->m2 * cosf(2 * pen->theta1 - 2 * pen->theta2));

	pen->acc2 = part1 * (part2 + part3 + part4) / part5;

	pen->v1 += pen->acc1;
	pen->theta1 += pen->v1;

	pen->v2 += pen->acc2;
	pen->theta2 += pen->v2;

	pen->pos1 = (Vector2){pen->r1 * sinf(pen->theta1), pen->r1 * cosf(pen->theta1)};
	pen->pos2 = (Vector2){pen->r2 * sinf(pen->theta2), pen->r2 * cosf(pen->theta2)};

	pen->pos1.x += pen->origin.x;
	pen->pos1.y += pen->origin.y;

	pen->pos2.x += pen->pos1.x;
	pen->pos2.y += pen->pos1.y;
}

void draw_double_pendulum(Double_pendulum *pen, Color c){
	DrawLineEx(pen->origin, pen->pos1, 5.f, BLUE);
	DrawLineEx(pen->pos1, pen->pos2, 5.f, BLUE);
	DrawCircleV(pen->pos1, 2 * fabsf(pen->m1), c);
	DrawCircleV(pen->pos2, 2 * fabsf(pen->m2), c);
}

int main(void){
	const int screen_width = 800;
	const int screen_height = 600;

	SetConfigFlags(FLAG_MSAA_4X_HINT);

	Image canvas = GenImageColor(screen_width, screen_height, (Color){0, 0, 0, 0});

	InitWindow(screen_width, screen_height, "Double Pendulum");

	SetTargetFPS(60);

	Double_pendulum pen1 = init_double_pendulum(screen_width / 2.f, 200, 160, 200, 8, 3, 1);
	//Double_pendulum pen2 = init_double_pendulum(screen_width / 2, 200, 160, 200, 8, 3.0001, 1);


	while(!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(BLACK);
			
			update_double_pendulum(&pen1);
			//update_double_pendulum(&pen2);
			draw_double_pendulum(&pen1, RED);
			//draw_double_pendulum(&pen2, BLUE);
			
		EndDrawing();
	}
	UnloadImage(canvas);
	CloseWindow();
	return 0;
}
