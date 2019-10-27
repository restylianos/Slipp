#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>



typedef struct init_snake{
		int x;
		int y;
		int head;
		int tail;
		int tempy;
		int dir;
		int score;
	}snake_t;


typedef struct food{
	int x;
	int y;
	int exists;
}food_t;



void die(snake_t *sn);
void tailremove(int **map,snake_t *sn,int dim);
void movement(snake_t *sn,int **map,int dim,food_t *fd);
void initialize(snake_t *sn,int dim,int **map);
char move();
void food_generator(food_t *fd,int **map,int dim);
void print(int **map,int dim,snake_t *sn);



int main(int argc,char *argv[]){
	
	int i;
	int dim;

	snake_t snake;
	snake_t *sn;
	sn = &snake;

	food_t food;
	food_t *fd;
	fd=&food;


		//debug for user input
		if(argv[1]==NULL || argc != 2){
		printf("[-]You forgot to pass the arguments or the arguments where more than 1!\n[+]Usage ./snake [size of grid]\n");
		return 1;
		}
	
		dim = atoi(argv[1]); 
		if(dim < 15){
			printf("[-]Be careful your map is too small try again. Size must be over 15!\n");
			exit(0);
		}
		

		int **map=(int **)malloc(dim * sizeof(int*)); 
		for(i=0;i<dim;i++){
		map[i] = (int *)malloc(dim * sizeof(int));
		}

		fd -> exists = 0;
		snake.dir = 100; // assci char of d gia tin arxi
		sn->score=0;


		initialize(sn,dim,map);
		
		
		while(1){

			system("clear");
			food_generator(fd,map,dim);
			print(map,dim,sn);
			usleep(150000);
			movement(sn,map,dim,fd);
			tailremove(map,sn,dim);

		}
		free(map);
		return 0;

}


void movement(snake_t *sn,int **map,int dim,food_t *fd){ //logiki gia to pws kinite to fidaki
	char input;
	input = move();
	// debug tolower case
	if(input == 'D') input = 'd';
	else if(input == 'S') input = 's';
	else if(input == 'W') input = 'w';
	else if(input == 'A') input = 'a';

	if((( input  == 'd' || input == 'a') || (input ==  'w' || input == 's')) && (abs((sn->dir)-input)>5)) sn->dir = input; // debug gia na min girnaei stin aditheti kateuthinsi kai na trexei sinexeia me ascii times
																														   // ascii values: a=97, d=100, w=119, s=115
	if(sn->dir == 'd'){
		(sn -> y)++;
		if(sn -> y == dim-2){
			die(sn);
		}
		if(map[sn->x][sn->y]==-1){
			fd -> exists = 0;
			(sn -> tail)--;
			(sn -> score)++;
		}
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1) die(sn);
		(sn -> head)++;
		map[sn -> x][ sn -> y ] = (sn -> head);
	}

	else if(sn->dir == 'a'){
		(sn -> y)--;
		if(sn -> y == -1){
			die(sn);
		}
		if(map[sn->x][sn->y]==-1){ 
			fd -> exists = 0;
			(sn -> tail)--;
			(sn -> score)++;
		}
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1) die(sn);
		(sn -> head)++;
		map[sn -> x][ sn -> y ] = (sn -> head);
	}

	else if(sn->dir == 'w'){
		(sn -> x)--;
		if(sn -> x == -1){
			die(sn);
		}
		if(map[sn->x][sn->y]==-1){
			fd -> exists = 0;
			(sn -> tail)--;
			(sn -> score)++;
		}
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1) die(sn);
		(sn -> head)++;
		map[sn -> x][ sn -> y ] = (sn -> head);
	}

	else if(sn->dir == 's'){
		(sn -> x)++;
		if(sn -> x == dim-2){
			die(sn);
		}
		if(map[sn->x][sn->y]==-1){
			fd -> exists = 0;
			(sn -> tail)--;
			(sn -> score)++;
		}
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1) die(sn);
		(sn -> head)++;
		map[sn -> x][ sn -> y ] = (sn -> head);
	}
	
}

void tailremove(int **map,snake_t *sn,int dim){
	int i,j;
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if(map[i][j] == (sn -> tail)){
				map[i][j] = 0;
			}
		}
	}
	(sn -> tail)++;
}


void print(int **map,int dim,snake_t *sn){
	int i,j;
	printf("Score: %d\n",sn -> score);

	for(i=0;i<dim;i++){
			printf("%c",'-');
	}
	printf("\n");

	for(i=0;i<dim-2;i++){ 
		printf("%c",'|');
			for(j=0;j<dim-2;j++){
		 		if(map[i][j]==0) printf("%c",' ');
		 		if(map[i][j] > 0 && map[i][j] != (sn->head)) printf("%c",'*'); 
		 		if(map[i][j]==(sn->head)) printf("%c",'@');
		 		if(map[i][j]== -1) printf("%c",'X');
		 		if(j==dim-3) printf("%c\n",'|');
		    }
	} 
	
	for(i=0;i<dim;i++){
			printf("-");
	}	
	printf("\n");

}


void initialize(snake_t *sn,int dim,int **map){ //arxiki dimiourgia fidiou
	int i=0;
	sn -> x = dim/2;
	sn -> y = dim/2;
	sn -> head = 5;
	sn -> tail = 1; 
	sn -> tempy = (sn -> y);

	for (i=0;i<(sn -> head);i++){
		(sn -> tempy)++;
		map[sn -> x][(sn -> tempy) - (sn -> head)] = i+1; //vazei tis times stin oura apo 1 mexri head-1

	}
}


char move()
{
    int flags;
    char key[2];

    flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags|O_NONBLOCK);

    // To get the input from the terminal without pressing the enter button
    system ("/bin/stty raw");
    read(STDIN_FILENO, key, 2);
    // Reset the "system ("/bin/stty raw")" to display correctly the output
    system ("/bin/stty cooked");

    return key[0];
}


void food_generator(food_t *fd,int **map,int dim){ // symperifora tou food
	srand(time(NULL));

	int a = (rand() % (dim -3 - 1 + 1)) + 1;
	int b = (rand() % (dim -3 - 1 + 1)) + 1;

	fd -> x = a;
	fd -> y = b;

	while(map[fd -> x][fd -> y] !=0){
		int a = (rand() % (dim -3 - 1 + 1)) + 1;
		int b = (rand() % (dim -3 - 1 + 1)) + 1;
		fd -> x = a;
		fd -> y = b;
	}

	if((fd -> exists)==0){
		map[fd -> x][fd -> y] = -1;
		fd -> exists = 1;
	}
}

void die(snake_t *sn){ //end game
	printf("\nYou lost! Try again\n");
	printf("[+] Score = %d\n",sn -> score);
	exit(0);
}
