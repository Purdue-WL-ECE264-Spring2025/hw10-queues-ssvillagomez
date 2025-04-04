#include "queue.h"
#include "tile_game.h"

#define MAX_STATES 1000000
static char visited[MAX_STATES] = {0}; 

static int is_solved(struct game_state state) {
  uint8_t expected = 1;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (row == 3 && col == 3) return state.tiles[row][col] == 0;
      if (state.tiles[row][col] != expected++) return 0;
    }
  }
  return 1;
}

static void generate_all_next_states(struct game_state state, struct game_state out[4], size_t *count) {
  *count = 0;
  if (state.empty_row > 0) {
    out[*count] = state;
    move_up(&out[*count]);
    out[*count].num_steps++;
    (*count)++;
  }
  if (state.empty_row < 3) {
    out[*count] = state;
    move_down(&out[*count]);
    out[*count].num_steps++;
    (*count)++;
  }
  if (state.empty_col > 0) {
    out[*count] = state;
    move_left(&out[*count]);
    out[*count].num_steps++;
    (*count)++;
  }
  if (state.empty_col < 3) {
    out[*count] = state;
    move_right(&out[*count]);
    out[*count].num_steps++;
    (*count)++;
  }
}

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
	size_t key = serialize(start); 
	visited[key] = 1; 
	enqueue(&q, start); 
	
	while(q.data.head != 0){ 
		struct game_state current = dequeue(&q); 
		if(is_solved(current)){ 
			free_list(q.data); 
			return current.num_steps; 
		} 

		struct game_state next_states[4];
		size_t num_next = 0; 
		generate_all_next_states(current, next_states, &num_next); 
		for(size_t i = 0; i < num_next; i++){ 
			size_t next_key = serialize(next_states[i]); 
			if(!visited[next_key]){ 
				visited[next_key] = 1; 
				enqueue(&q, next_states[i]); 
			} 
		} 
	} 
	free_list(q.data); 
	return -1; 
}
