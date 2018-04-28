// Example program:
// Using SDL2 to create an application window

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <time.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 1280
#define HEIGHT 960
#define CELLSIZE 16
#define NB_CIRCLES 8

#define TRESHOLD 1

typedef struct vector {
	float x;
	float y;
} vector;

typedef struct point {
	int x;
	int y;
} point;

typedef struct line {
	point p1;
	point p2;
} line;

typedef struct circle {
	vector center;
	float radius;
	vector direction;
	float speed;
} circle;

typedef struct data {
	int treshold;
	size_t width;
	size_t height;
	float* values;
	int* activation;
	circle* circles;
	size_t nb_circles;
} data;

typedef struct render_data {
	size_t nb_c_x;
	size_t nb_c_y;
	SDL_Rect* cells;
	size_t nb_lines;
	line* lines;
	data* v_data;
} render_data;

render_data* init_grid(int w, int h, int cell_size) {
	render_data *r_d = malloc(sizeof(render_data));
	r_d->nb_c_x = w/cell_size-1;
	r_d->nb_c_y = h/cell_size-1;

	r_d->cells = malloc((r_d->nb_c_x)*(r_d->nb_c_y)*sizeof(SDL_Rect));

	for(size_t j=0; j<r_d->nb_c_y; j++) {
		for(size_t i=0; i<r_d->nb_c_x; i++) {
			r_d->cells[i+j*r_d->nb_c_x].x = (i+1)*cell_size-2;
			r_d->cells[i+j*r_d->nb_c_x].y = (j+1)*cell_size-2;
			r_d->cells[i+j*r_d->nb_c_x].w = 5;
			r_d->cells[i+j*r_d->nb_c_x].h = 5;
		}
	}

	r_d->nb_lines = (r_d->nb_c_x) + (r_d->nb_c_y);
	r_d->lines = malloc(r_d->nb_lines * sizeof(line));

	for(size_t i=0; i<r_d->nb_c_x; i++) {
		r_d->lines[i].p1.x = (i+1)*cell_size;
		r_d->lines[i].p1.y = 0;
		r_d->lines[i].p2.x = (i+1)*cell_size;
		r_d->lines[i].p2.y = h;
	}

	for(size_t j=0; j<r_d->nb_c_y; j++) {
		r_d->lines[j+r_d->nb_c_x].p1.x = 0;
		r_d->lines[j+r_d->nb_c_x].p1.y = (j+1)*cell_size;
		r_d->lines[j+r_d->nb_c_x].p2.x = w;
		r_d->lines[j+r_d->nb_c_x].p2.y = (j+1)*cell_size;
	}

	r_d->v_data = NULL;

	return r_d;
}

float lerp(float v0, float v1, float t) {
	return v0 + t * (v1 - v0);
}

float lerp_t(float o, float t) {
	return (1 - o)/(t - o);
}

float get_value(data* d, int x, int y) {
	return d->values[x + y * d->width];
}

void render_marching_sq(const render_data* d, SDL_Renderer* renderer) {
	for(size_t x=0; x<d->v_data->width-1; x++) {
		for(size_t y=0; y<d->v_data->height-1; y++) {
			int NW = d->v_data->activation[x + y*d->v_data->width];
			int NE = d->v_data->activation[x+1 + y*d->v_data->width];
			int SW = d->v_data->activation[x + (y+1)*d->v_data->width];
			int SE = d->v_data->activation[x+1 + (y+1)*d->v_data->width];

			float d_x = ((float)x) + 1;
			float d_y = ((float)y) + 1;

			int A_X = d_x      ;     int A_Y = d_y      ;
			int B_X = (d_x + 1);     int B_Y = d_y      ;
			int C_X = (d_x + 1);     int C_Y = (d_y + 1);
			int D_X = d_x      ;     int D_Y = (d_y + 1);

			int treshold = 1;
			if (d->v_data->treshold != 0)
				treshold = d->v_data->treshold;

			float A_V = get_value(d->v_data, x, y)/treshold;
			float B_V = get_value(d->v_data, x + 1, y)/treshold;
			float C_V = get_value(d->v_data, x + 1, y + 1)/treshold;
			float D_V = get_value(d->v_data, x, y + 1)/treshold;

			float AB_X = lerp(A_X, B_X, lerp_t(A_V, B_V));
			float BC_Y = lerp(B_Y, C_Y, lerp_t(B_V, C_V));
			float AD_Y = lerp(A_Y, D_Y, lerp_t(A_V, D_V));
			float DC_X = lerp(D_X, C_X, lerp_t(D_V, C_V));

			if(NW == 0 && NE == 0 && SW == 0 && SE == 0) {
				//
			} else if(NW == 0 && NE == 0 && SW == 0 && SE == 1) {
				SDL_RenderDrawLine( renderer, B_X*CELLSIZE, BC_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);
			} else if(NW == 0 && NE == 0 && SW == 1 && SE == 0) {
				SDL_RenderDrawLine( renderer, A_X*CELLSIZE, AD_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 0 && NE == 0 && SW == 1 && SE == 1) {
				SDL_RenderDrawLine( renderer, A_X*CELLSIZE, AD_Y*CELLSIZE,
						B_X*CELLSIZE, BC_Y*CELLSIZE);

			} else if(NW == 0 && NE == 1 && SW == 0 && SE == 0) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						B_X*CELLSIZE, BC_Y*CELLSIZE);

			} else if(NW == 0 && NE == 1 && SW == 0 && SE == 1) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 0 && NE == 1 && SW == 1 && SE == 0) {
				// Point selle à gérer
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						A_X*CELLSIZE, AD_Y*CELLSIZE);
				SDL_RenderDrawLine( renderer, B_X*CELLSIZE, BC_Y*CELLSIZE,
						DC_X*CELLSIZE, C_Y*CELLSIZE);

			} else if(NW == 0 && NE == 1 && SW == 1 && SE == 1) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						A_X*CELLSIZE, AD_Y*CELLSIZE);

			} else if(NW == 1 && NE == 0 && SW == 0 && SE == 0) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						A_X*CELLSIZE, AD_Y*CELLSIZE);

			} else if(NW == 1 && NE == 0 && SW == 0 && SE == 1) {
				// Point selle à gérer
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						B_X*CELLSIZE, BC_Y*CELLSIZE);
				SDL_RenderDrawLine( renderer, A_X*CELLSIZE, AD_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 1 && NE == 0 && SW == 1 && SE == 0) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 1 && NE == 0 && SW == 1 && SE == 1) {
				SDL_RenderDrawLine( renderer, AB_X*CELLSIZE, A_Y*CELLSIZE,
						B_X*CELLSIZE, BC_Y*CELLSIZE);

			} else if(NW == 1 && NE == 1 && SW == 0 && SE == 0) {
				SDL_RenderDrawLine( renderer, A_X*CELLSIZE, AD_Y*CELLSIZE,
						B_X*CELLSIZE, BC_Y*CELLSIZE);

			} else if(NW == 1 && NE == 1 && SW == 0 && SE == 1) {
				SDL_RenderDrawLine( renderer, A_X*CELLSIZE, AD_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 1 && NE == 1 && SW == 1 && SE == 0) {
				SDL_RenderDrawLine( renderer, B_X*CELLSIZE, BC_Y*CELLSIZE,
						DC_X*CELLSIZE, D_Y*CELLSIZE);

			} else if(NW == 1 && NE == 1 && SW == 1 && SE == 1) {
				//
			}

		}
	}
}

void render_text(const char* text, TTF_Font* font, SDL_Renderer* renderer, int x, int y) {

	SDL_Color White = { 128, 128, 238};

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, White);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int text_width = surfaceMessage->w;
	int text_height = surfaceMessage->h;

	SDL_FreeSurface(surfaceMessage);

	SDL_Rect Message_Rect;
	Message_Rect.x = x*CELLSIZE - text_width/2;
	Message_Rect.y = y*CELLSIZE - text_height - 2;
	Message_Rect.w = text_width;
	Message_Rect.h = text_height;

	SDL_RenderCopy(renderer, Message, NULL, &Message_Rect);
	SDL_DestroyTexture(Message);
}


typedef struct r_args {
	const render_data* d;
	SDL_Renderer* renderer;
} r_args;
//***renderer

void render(const render_data* d, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

	// Clear winow
	SDL_RenderClear( renderer );


	SDL_SetRenderDrawColor( renderer, 128, 128, 128, 255 );

	//for(size_t i=0; i < d->nb_lines; i++) {
	//    // Render line
	//    line* l = d->lines + i;
	//    SDL_RenderDrawLine( renderer, l->p1.x, l->p1.y, l->p2.x, l->p2.y);
	//}

//	for(size_t i=0; i < d->nb_c_x * d->nb_c_y; i++) {
//		// Render rect
//		if(d->v_data->activation[i] == 1) {
//			SDL_SetRenderDrawColor( renderer, 128, 255, 128, 255 );
//		} else {
//			SDL_SetRenderDrawColor( renderer, 128, 128, 128, 255 );
//		}
//		SDL_RenderFillRect( renderer, d->cells + i );
//	}

	SDL_SetRenderDrawColor( renderer, 128, 255, 128, 255 );
	render_marching_sq(d, renderer);

//	TTF_Font* Sans = TTF_OpenFont("Hack-Regular.ttf", 8);
	//if(Sans == NULL) {
	//    const char * error = TTF_GetError();
	//    printf("%s \n", error);
	//}
//	for(size_t i=0; i < d->v_data->width * d->v_data->height; i++) {
//		int x = i % d->v_data->width; int y = i / d->v_data->width;
//		char buffer[20];
//		snprintf(buffer, 20, "%d", (int)d->v_data->values[i]);
//		render_text(buffer, Sans, renderer, x+1, y+1);
//	}

//	TTF_CloseFont(Sans);

	//SDL_SetRenderDrawColor( renderer, 128, 255, 128, 255 );
	//for(size_t i=0; i < d->v_data->nb_circles; i++) {
	//	circle* c = d->v_data->circles + i;
	//	SDL_RenderDrawLine( renderer, c->center.x-c->radius, c->center.y-c->radius, c->center.x-c->radius, c->center.y+c->radius);
	//	SDL_RenderDrawLine( renderer, c->center.x-c->radius, c->center.y+c->radius, c->center.x+c->radius, c->center.y+c->radius);
	//	SDL_RenderDrawLine( renderer, c->center.x+c->radius, c->center.y+c->radius, c->center.x+c->radius, c->center.y-c->radius);
	//	SDL_RenderDrawLine( renderer, c->center.x+c->radius, c->center.y-c->radius, c->center.x-c->radius, c->center.y-c->radius);
	//}

	SDL_RenderPresent(renderer);
}

void compute_data(data* v_data) {
	for(size_t x=0; x<v_data->width; x++) {
		for(size_t y=0; y<v_data->height; y++) {
			//size_t x_val = x - width/2;
			//size_t y_val = y - height/2;
			//v_data->values[x + width*y] = (float)(x_val*x_val) - (float)(y_val*y_val);
            float somme = 0;
            for(size_t i=0; i<v_data->nb_circles; i++) {
		        circle* c = v_data->circles + i;
                float v = (float) ((c->radius / CELLSIZE) * (c->radius / CELLSIZE))/((x - c->center.x / CELLSIZE)*(x - c->center.x / CELLSIZE) + (y - c->center.y / CELLSIZE)*(y - c->center.y / CELLSIZE));
                somme += v;
            }
			v_data->values[x + v_data->width*y] = somme;
			//v_data->activation[x + width*y] = v_data->values[x + width*y] < v_data->treshold;
			v_data->activation[x + v_data->width*y] = v_data->values[x + v_data->width*y] > 1;
		}
	}
}

Uint32 tick_update(Uint32 interval, void* d) {
	data* dat = (data*) d;
	for(size_t i=0; i<dat->nb_circles; i++) {
		circle* c = dat->circles + i;
		if(c->center.x <= c->radius || c->center.x >= WIDTH - c->radius - 2*CELLSIZE)
			c->direction.x *= -1;
		if(c->center.y <= c->radius || c->center.y >= HEIGHT - c->radius - 2*CELLSIZE)
			c->direction.y *= -1;
		c->center.x += c->direction.x*c->speed * interval;
		c->center.y += c->direction.y*c->speed * interval;
	}
	compute_data(dat);
	return interval;
}

Uint32 tick_render(Uint32 interval, void* data) {
	r_args* dat = (r_args*) data;
	const render_data* d = dat->d;
	SDL_Renderer* renderer = dat->renderer;

	render(d, renderer);
	return interval;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int width = (WIDTH - CELLSIZE)/CELLSIZE;
	int height = (HEIGHT - CELLSIZE)/CELLSIZE;
	float values[width*height];
	int activation[width*height];
	circle circles[NB_CIRCLES];

	for(size_t i=0; i<NB_CIRCLES; i++) {
		circles[i].center.x = WIDTH/2;
		circles[i].center.y = HEIGHT/2;
		circles[i].radius = (1*CELLSIZE + (rand() % (3*CELLSIZE)) )* WIDTH/(50*CELLSIZE);
		float x = 50-rand()%100; float y = 50-rand()%100;
		circles[i].direction.x = x / (sqrtf(x*x + y*y));
		circles[i].direction.y = y / (sqrtf(x*x + y*y));
		circles[i].speed = 0.1f* WIDTH/(50*CELLSIZE);
	}

	data v_data;
	v_data.width = width;
	v_data.height = height;
	v_data.values = values;
	v_data.activation = activation;
	v_data.treshold = TRESHOLD;
	v_data.circles = circles;
	v_data.nb_circles = NB_CIRCLES;

	compute_data(&v_data);

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);              // Initialize SDL2
	TTF_Init();

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
			"An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			WIDTH,                               // width, in pixels
			HEIGHT,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// The window is open: could enter program loop here (see SDL_PollEvent())

	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Setup renderer
	SDL_Renderer* renderer = NULL;
	renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

	render_data* d = init_grid(WIDTH,HEIGHT,CELLSIZE);

	d->v_data = &v_data;

	r_args a;
	a.d = d;
	a.renderer = renderer;

	SDL_TimerID timer = SDL_AddTimer(30, tick_render, &a);

	SDL_TimerID timer2 = SDL_AddTimer(30, tick_update, &v_data);

	SDL_Event event;
	int cont = 1;

	while (cont) {
		while (SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				cont = 0;
			}
			if(event.type == SDL_KEYUP) {
				switch(event.key.keysym.sym) {
					case SDLK_z:
						d->v_data->treshold++;
						//printf("%d\n", treshold);
						break;
					case SDLK_s:
						d->v_data->treshold--;
						//printf("%d\n", treshold);
						break;
					default:
						cont = 0;
						break;
				}
			}
		}
	}

	// Close and destroy the window
	SDL_RemoveTimer(timer);
	SDL_RemoveTimer(timer2);

	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}

