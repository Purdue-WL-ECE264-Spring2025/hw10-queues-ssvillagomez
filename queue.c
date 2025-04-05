#include "queue.h"
#include "tile_game.h"
#include <stdlib.h> 

#define MAX_STATES 100000

int is_solved(struct game_state state){ 
	int goal[4][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,0}}; 
	for(int i = 0; i < 4; i++){ 
		for (int j = 0; j < 4; j++){ 
			if(state.tiles[i][j] != goal[i][j]) return 0; 
		}
	}
	return 1; 
}

void enqueue(struct queue *q, struct game_state state) {
	size_t encoded = serialize(state); 
	insert_at_tail(&q->data, encoded); 
}

struct game_state dequeue(struct queue *q) { 
	if (!q->data.head) {
		fprintf(stderr, "Error: dequeue from empty queue\n");
  	exit(1);
 	}
	size_t serialized = remove_from_head(&q->data);
  return deserialize(serialized);
}

int number_of_moves(struct game_state start) {
	struct queue q = {0};
	enqueue(&q, start);

 	size_t seen[MAX_STATES];
 	int seen_count = 0;

 	while (q.data.head) {
  	struct game_state current = dequeue(&q);

   	size_t hash = serialize(current);

   	int skip = 0;
  	for (int i = 0; i < seen_count; i++) {
   		if (seen[i] == hash) {
   		  skip = 1;
  	    break;
	    }
  	 }

 		if (skip) continue;

  	if (seen_count < MAX_STATES) {
   		seen[seen_count++] = hash;
  	} else {
   		fprintf(stderr, "Exceeded max seen states — aborting.\n");
    	return -1;
    }

  	if (is_solved(current)) {
    	free_list(q.data);
     	return current.num_steps;
  	}
 	 	struct game_state next;

		if (current.empty_row > 0) {
    	next = current;
   		move_down(&next);
   		enqueue(&q, next);
  	}
 		if (current.empty_row < 3) {
  		next = current;
     	move_up(&next);
     	enqueue(&q, next);
   	}
  	if (current.empty_col > 0) {
  		next = current;
    	move_right(&next);
     	enqueue(&q, next);
  	}
  	if (current.empty_col < 3) {
  	 	next = current;
    	move_left(&next);
    	enqueue(&q, next);
  	}
	}
 	free_list(q.data);
 	return -1;
}

