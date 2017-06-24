/* Harrison Nguyen and Dennis Han */
/* hnguye68@jhu.edu and dhan10@jhu.edu */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define Asz 5
#define Bsz 4
#define Csz 3
#define Dsz 2
#define size_x 10
#define size_y 10

int ourmap[size_x][size_y]; /* set to 0 for nothing */
int theirmap[size_x][size_y];

int order = 0;
int exits = 0;

int guesses = 0;
int hits = 0;
int sunksize = 0;
struct p {
	int x;
	int y;
} Point;

struct shipinfo {
	int x[5];
	int y[5];
	int size;
	int health;
};

struct fleetinfo{
       	struct shipinfo loc[10];
	int total;
	int Atot; /* aircraft carrier */
	int Btot; /* battleship */
	int Ctot; /* cruiser */
	int Dtot; /* destroyer */
};

struct fleetinfo ourfleet; 
struct fleetinfo theirfleet;

/* initialize maps for our guesses and their guesses */
/* place ships here? */
void start_maps(void)
{
	/* initialize aircraft carrier */
        ourfleet.total = 10;
        ourfleet.Atot = 1;
	ourfleet.Btot = 4;
	ourfleet.Ctot = 3; 
	ourfleet.Dtot = 2;

	for (int i = 0; i < 5; i++) {
		ourmap[0][i] = 3;
		ourfleet.loc[0].x[i] = 0;
		ourfleet.loc[0].y[i] = i;
	}
	ourfleet.loc[0].health = Asz;
	ourfleet.loc[0].size = Asz;

	/* initialize battleship 1 */
	for (int i = 6; i < 10; i++) {
		ourmap[0][i] = 3;
		ourfleet.loc[1].x[i - 6] = 0;
		ourfleet.loc[1].y[i - 6] = i;
	}
	ourfleet.loc[1].health = Bsz;
	ourfleet.loc[1].size = Bsz;
	
	/* initialize battleship 2 */
	for (int i = 0; i < 4; i++) {
		ourmap[1][i] = 3;
		ourfleet.loc[2].x[i] = 1;
		ourfleet.loc[2].y[i] = i;
	}
	ourfleet.loc[2].health = Bsz;
	ourfleet.loc[2].size = Bsz;

	/* initialize cruiser 1 */
	for (int i = 4; i < 7; i++) {
		ourmap[1][i] = 3;
		ourfleet.loc[3].x[i - 4] = 1;
		ourfleet.loc[3].y[i - 4] = i;
	}
	ourfleet.loc[3].health = Csz;
	ourfleet.loc[3].size = Csz;

	/* initialize cruiser 2 */
	for (int i = 7; i < 10; i++) {
		ourmap[1][i] = 3;
		ourfleet.loc[4].x[i - 7] = 1;
		ourfleet.loc[4].y[i - 7] = i;
	}
	ourfleet.loc[4].health = Csz;
	ourfleet.loc[4].size = Csz;
	
        /* initialize cruiser 3 */
	for (int i = 4; i < 7; i++) {
		ourmap[2][i] = 3;
		ourfleet.loc[5].x[i - 4] = 2;
		ourfleet.loc[5].y[i - 4] = i;
	}
	ourfleet.loc[5].health = Csz;
	ourfleet.loc[5].size = Csz;

	/* destroyer 1 */
	ourmap[6][1]=3;
	ourmap[7][1]=3;
	ourfleet.loc[6].x[0] = 6;
	ourfleet.loc[6].y[0] = 1;
	ourfleet.loc[6].x[1] = 7;
	ourfleet.loc[6].y[1] = 1;
	ourfleet.loc[6].health = Dsz;
	ourfleet.loc[6].size = Dsz;

	/* destroyer 2 */
	ourmap[5][4]=3;
	ourmap[5][5]=3;
	ourfleet.loc[7].x[0] = 5;
	ourfleet.loc[7].y[0] = 4;
	ourfleet.loc[7].x[1] = 5;
	ourfleet.loc[7].y[1] = 5;
	ourfleet.loc[7].health = Dsz;

	/* destroyer 3 */
	ourmap[7][7]=3;
	ourmap[8][7]=3;
	ourfleet.loc[8].x[0] = 7;
	ourfleet.loc[8].y[0] = 7;
	ourfleet.loc[8].x[1] = 8;
	ourfleet.loc[8].y[1] = 7;
	ourfleet.loc[8].health = Dsz;
	ourfleet.loc[8].size = Dsz;

	/* destroyer 4 */
	ourmap[9][2]=3;
	ourmap[9][3]=3;
	ourfleet.loc[9].x[0] = 9;
	ourfleet.loc[9].y[0] = 3;
	ourfleet.loc[9].x[1] = 9;
	ourfleet.loc[9].y[1] = 3;
	ourfleet.loc[9].health = Dsz;
	ourfleet.loc[9].size = Dsz;
	//puts("Fleet made\n");

        theirfleet.total = 10;
        theirfleet.Atot = 1;
	theirfleet.Btot = 4;
	theirfleet.Ctot = 3; 
	theirfleet.Dtot = 2;
}

/* interprets message code to print proper message to stdout */
/* 0, string for fire */
/* 1 for hit */
/* 2 for miss */
/* 3, shipsize for sunk */
/* 4 for concede */
/* 5 for cheat */
void message(int code)
{
        if (code == 0) {
		printf("FIRE %c %d\n", Point.x+65, Point.y + 1);
		fflush(stdout);
	} else if (code == 1) {
	        puts("HIT");	
	} else if (code == 2) {
		puts("MISS");
	} else if (code == 3) {
		printf("SUNK %d\n", sunksize);  //needs size
	} else if (code == 4) {
		puts("CONCEDE");
		order == 3;
		fflush(stdout);
	} else if (code == 5) {
	        puts("CHEAT");
		order == 3;
		fflush(stdout);
	}
	//fflush(stdout);
}

/* interpret incoming message as code */
int decode(char * str)
{
	if (strcmp(str, "FIRE") == 0) {
		return 0;
	} else if (strcmp(str, "HIT") == 0) {
		return 1;
	} else if (strcmp(str, "MISS") == 0) {
		return 2;
	} else if (strcmp(str, "SUNK") == 0) {
		return 3;
	} else if (strcmp(str, "CONCEDE") == 0) {
		return 4;
	} else if (strcmp(str, "CHEAT") == 0) {
       		return 5;
	}
	fprintf(stderr, "Opponent's response invalid.\n");
	return EXIT_FAILURE;
}

/* search grid and return status of the input location */
int search(int map[size_x][size_y], struct p coord) {
	//printf("searching %d %d\n",coord.x,coord.y);
	if (map[coord.x][coord.y] == 0) { //sea
		return 0;
	} else if (map[coord.x][coord.y] == 1) { // hit
		return 1;
	} else if (map[coord.x][coord.y] == 2) { // miss
		return 2;
	} else if (map[coord.x][coord.y] == 3) { //occupied
		return 3;
	} else {/* if (map[coord.x][coord.y] == 4) { //attempted*/
		return 4; //not useful
	}
}

/* function to make guesses. Returns coordinates */
int gen_guess(int map[size_x][size_y])
{
	if (guesses == 0) {
		Point.x = 0;
		Point.y = 0; 
        } else {
		/* lastptstatus = theirmap[Point.x][Point.y]; */
		/* if (lastptstatus == 1) { */
		/* 	Point.x; */
		/*        	Point.y; */
		/* } else if (lastptstatus == 2) { */
		/* 	Point.x; */
		/*        	Point.y; */
		/* } else { */
		/* 	puts("Last point status not updated"); /\* this no hap *\/ */
		/* } */
		if (Point.y < 9) {
			if (Point.x < 9) {
				Point.x++;
			} else if (Point.x == 9) {
				Point.x = 0;
				Point.y++;
			}
		}
	}
	if (search(map, Point) == 0) {
       	} else {
       		// choose another coordinate;
        }
	guesses++;
	return 0;
}

/* update ship stats. Return 1 on success, 0 on failure. */
void update_ourfleet(int x, int y, struct fleetinfo ally) {
	int shipno = 0;
	for (int i = 0; i < ally.total; i++) {
		for (int j = 0; j < 5; j++) {
			if (ally.loc[i].x[j] == x && ally.loc[i].y[j] == y) {
				ally.loc[i].health--;
			        shipno = i;
			}
		}
	}
	if (ally.loc[shipno].health == 0) {
		ally.total--;
		if (ally.total == 0) {
			message(4);
			exits = 1;
		}
	        sunksize = ally.loc[shipno].size;
		if (sunksize == 5) {
			ally.Atot--;
	        } else if (sunksize == 4) {
			ally.Btot--;
		} else if (sunksize == 3) {
			ally.Ctot--;
		} else if (sunksize == 2) {
			ally.Dtot--;
		}
		message(3);
	}
}

void update_ourmap(char x, int y)
{
	struct p enemyattempt;
	enemyattempt.x = x - 65;
	enemyattempt.y = y;
	//printf("evaluating %d %d\n",enemyattempt.x, enemyattempt.y);
	if (search(ourmap, enemyattempt) == 0) { // sea - miss
		ourmap[x - 65][y] = 2;
		message(2);
	} else if (search(ourmap, enemyattempt) == 1) { //already hit
	        message(1);
	} else if (search(ourmap, enemyattempt) == 2) { //already missed
		message(2);
	} else if (search(ourmap, enemyattempt) == 3) {
		ourmap[x - 65][y] = 1;
		update_ourfleet(enemyattempt.x, enemyattempt.y,ourfleet);
	        message(1);
	}
}

void update_theirfleet(int sunkshipsize)
{
	theirfleet.total--;
	if (sunkshipsize == 5) {
	       	theirfleet.Atot--;
        } else if (sunkshipsize == 4) {
		theirfleet.Btot--;
        } else if (sunkshipsize == 3) {
	       	theirfleet.Ctot--;
       	} else if (sunkshipsize == 2) {
       		theirfleet.Dtot--;
       	}
}

void update_theirmap(void)
{
        
	if (search(theirmap, Point) == 0) { // sea - miss
	        theirmap[Point.x][Point.y] = 2;
		//return 2;
	} /*else if (search(theirmap, Point) == 1) { //already hit
		//return 1;
	} else if (search(theirmap, Point) == 2) { //already missed
		//return 2;
       	} */ else if (search(theirmap, Point) == 3) {
		theirmap[Point.x][Point.y] = 1;
		//return 1;
	}
}

int main(int argc, char *argv[])
{
	if (order == 0) {
	start_maps(); /* make the maps by placing ships on your map. Opponents
		       map does not have anything on it*/
	}
	assert(argc >= 1);
	if (argc > 1) {
		if (strcmp(argv[1],"-f") == 0 || strcmp(argv[1],"--first") == 0)                {
			order = 1;	
		}
	} else {
		order = 2;
	}
	if (order == 1) {
       		int guessout = gen_guess(theirmap);
       	        message(guessout);
		//	order = 2; //just switches which client stops
	}
	char str1 [7];
       	char str2 [7];
       	char x;
       	int y;
       	int z;
	int code1;
	int code2;

	while (order != 3) {
	if (order == 1) {
		assert(scanf("%s %s %c %d", str1,str2, &x, &y) != 0);
		//printf("scoping out %d %d",x,y);
		code1 = decode(str1);
		code2 = decode(str2);
		//printf("The first code is %d\n",code1);
		y = y - 1;
	} else if( order == 2) {
		//assert(scanf("%s %c %d", str1, &x, &y) != 0);
		//code1 = decode(str1);
		assert(scanf("%s %c %d", str2, &x, &y) != 0);
		code2 = decode(str2);
		y = y - 1;
	} 
	if (code1 == 1 || code1 == 2 ) {
		//assert(scanf("%s %c %d", str2, &x, &y) != 0);
		//code2 = decode(str2);
		assert(code2 == 0);
		y = y - 1;
		hits++;
		update_theirmap();
	} else if (code1 == 3) {
		assert(scanf("%d %s %c %d", &z, str2, &x, &y) != 0); //broken
		code2 = decode(str2);
		assert(code2 == 0);
		y = y - 1;
		update_theirfleet(z);
		hits++;
	} else if (code1 == 4 || code1 == 5) {
		exits = 1;
	} else if (code1 == 0||code2 == 0) {
		update_ourmap(x, y);
		int guessout = gen_guess(theirmap);
		message(guessout);
	}
	/* if (code1 == 1) { */
	/* 	hits++; */
	/* 	update_theirmap(); */
	/* } */
	/* if (code1 == 0 || code2 == 0) { */
	/* 	update_ourmap(x, y); */
	/* 	int guessout = gen_guess(theirmap); */
	/* 	message(guessout); */
	/* } */
	
	if (guesses - hits > 70) {
		message(5);
	}
	if (guesses > 100) {
		message(5);
	}
	}
	return EXIT_SUCCESS;
}
