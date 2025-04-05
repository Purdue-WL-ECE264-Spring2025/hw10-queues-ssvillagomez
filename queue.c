#include "queue.h"
#include "tile_game.h"

#define MAX_STATES 1000000
static char visited[MAX_STATES] = {0}; 

extern int is_solved(struct game_state state);
extern void generate_all_next_states(struct game_state state, struct game_state *next_states, size_t *num_next);

void enqueue(struct queue *q, struct game_state state) {
	size_t encoded = serialize(state); 
	insert_at_tail(&q->data, encoded); 
}

struct game_state dequeue(struct queue *q) { 
	size_t encoded = remove_from_head(&q->data); 
	return deserialize(encoded); 
}

int number_of_moves(struct game_state start) { 
	struct queue q = {0}; 
	uint64_t key = serialize(start); 
	visited[key % MAX_STATES] = 1; 
	enqueue(&q, start); 
	
	while(q.data.head != NULL){ 
		struct game_state current = dequeue(&q); 
		if(is_solved(current)){ 
			free_list(q.data); 
			return current.num_steps; 
		} 

		struct game_state next_states[4];
		size_t num_next = 0; 

		generate_all_next_states(current, next_states, &num_next); 
		
		for(size_t i = 0; i < num_next; i++){ 
			uint64_t next_key = serialize(next_states[i]); 
			if(!visited[next_key % MAX_STATES]){ 
				visited[next_key % MAX_STATES] = 1; 
				enqueue(&q, next_states[i]); 
			} 
		} 
	} 
	free_list(q.data); 
	return -1; 
}
