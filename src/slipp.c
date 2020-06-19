#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> 



typedef struct init_snake{
		int x;
		int y;
		int head;
		int tail;
		int tempy;
		int dir;
		int score;
		int is_alive;
	}snake_t;


typedef struct food{
	int x;
	int y;
	int exists;
}food_t;


void redprint_slow(char *message_to_print);
void die(snake_t *sn);
void tailremove(int **map,snake_t *sn,int dim);
void movement(snake_t *sn,int **map,int dim,food_t *fd);
void initialize(snake_t *sn,int dim,int **map);
char move();
void food_generator(food_t *fd,int **map,int dim);
void print(int **map,int dim,snake_t *sn);



int main(int argc,char *argv[]){
	srand(time(NULL));
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
		printf("\033[0;31m[-] Usage ./snake [size of grid]\n");
		return 1;
		}
	
		dim = atoi(argv[1]); 
		if(dim < 15){
			printf("\033[0;33m[-] Map size should be bigger than 15\n\033[0;0m");
			exit(0);
		}
		

		int **map=(int **)calloc(dim , sizeof(int*)); 
		for(i=0;i<dim;i++){
		map[i] = (int *)calloc(dim , sizeof(int));
		}

		fd -> exists = 0;
		snake.dir = 100;
		sn->score=0;


		initialize(sn,dim,map);
		
		
		while(1){

			system("clear");
			food_generator(fd,map,dim);
			print(map,dim,sn);
			usleep(150000);
			movement(sn,map,dim,fd);
			tailremove(map,sn,dim);
			if(!sn->is_alive){
				sn->is_alive = 1;
				system("clear");
				snake.dir = 100;
				sn->score=0;
				fd -> exists = 0;
				initialize(sn,dim,map);
				free(map);
				map = NULL;
				map=(int **)calloc(dim , sizeof(int*)); 
				for(i=0;i<dim;i++){
					map[i] = (int *)calloc(dim , sizeof(int));
				}
				continue; //fast fix for game restart
			}
		}
		
		return 0;

}


void movement(snake_t *sn,int **map,int dim,food_t *fd){ 
	char input;
	input = move();
	if(input == 'D') input = 'd';
	else if(input == 'S') input = 's';
	else if(input == 'W') input = 'w';
	else if(input == 'A') input = 'a';

	if((( input  == 'd' || input == 'a') || (input ==  'w' || input == 's')) && (abs((sn->dir)-input)>5)) sn->dir = input; 
																														   
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
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1){
			die(sn);
		} 
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
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1){
			die(sn);
		}
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
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1){
			die(sn);
		} 
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
		if(map[sn->x][sn->y]!=0 &&  map[sn->x][sn->y]!=-1){
			die(sn);
		} 
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
	

	for(i=0;i<dim;i++){
			printf("%c",'-');
	}
	printf("\n");

	for(i=0;i<dim-2;i++){ 
		printf("%c",'|');
			for(j=0;j<dim-2;j++){
		 		if(map[i][j]==0) printf("%c",' ');
		 		if(map[i][j] > 0 && map[i][j] != (sn->head)) printf("\033[0;33m%c\033[0;0m",'*'); 
		 		if(map[i][j]==(sn->head)) printf("%c",'@');
		 		if(map[i][j]== -1) printf("\033[0;32m%c\033[0;0m",'X');
		 		if(j==dim-3) printf("%c\n",'|');
		    }
	} 
	
	for(i=0;i<dim;i++){
			printf("-");
	}	
	printf("\n");

	printf("\033[0;32mScore: %d\033[0;0m\n",sn -> score);
}


void initialize(snake_t *sn,int dim,int **map){ 
	int i=0;
	sn -> x = dim/2;
	sn -> y = dim/2;
	sn -> head = 5;
	sn -> tail = 1; 
	sn -> tempy = (sn -> y);

	for (i=0;i<(sn -> head);i++){
		(sn -> tempy)++;
		map[sn -> x][(sn -> tempy) - (sn -> head)] = i+1;

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


void food_generator(food_t *fd,int **map,int dim){

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

void die(snake_t *sn){
	sn->is_alive = 0;
	int i;
    char *lost_msg = "[-] You lost! Try again\nFinal Score:";
	char *restart_msg = "Restarting in: 4 secs\n";
	int final_score = sn -> score; 

	redprint_slow(lost_msg);
	printf(" %d\n",final_score);
	redprint_slow(restart_msg);
	printf("->");

	for(i=0;i<4;i++){
		printf(" %d   ",4-i);
		fflush(stdout);
		sleep(1);
	}
}

void redprint_slow(char *message_to_print)
{
    int i = 0;
    printf("\033[0;31m"); //set color to red
    while (message_to_print[i] != '\0')
    {

        printf("%c", message_to_print[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m"); // color reset
    // free(message_to_print);
}
