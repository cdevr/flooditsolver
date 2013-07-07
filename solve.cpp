#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <assert.h>
#include <queue>

using namespace std;

struct state {
	int data[14][14];
	vector<int> moves;

	state(char * filename) {
		FILE *fh = fopen(filename, "r");

		char line[100];
		int y = 0;
		while (fgets(line, sizeof(line), fh) != NULL & y < 14) {
			int x = 0;
			int c = 0;

			char * step = &line[0];
			while (c < sizeof(line) && x < 14) {
				if (line[c] == ' ' || line[c] == '\n') { 
					data[y][x] = atoi(step);
					if (c+1 > sizeof(line)) break;
					step = &line[c+1];
					x++;
				}
				c++;
			}
			y++;
		}
		fclose(fh);

		area_diff = 0;
	}

	state(const state & s) {
		moves.assign(s.moves.begin(), s.moves.end());
		for (int y = 0; y < 14; y++)
			for (int x = 0; x < 14; x++)
				data[y][x] = s.data[y][x];
		area_diff = 0;
	}
	
	state() {
		for (int y = 0; y < 14; y++)
			for (int x = 0; x < 14; x++)
				data[y][x] = 0;
		area_diff = 0;
	}

	void print() {
		for (int y = 0; y < 14; y++) {
			for (int x = 0; x < 14; x++) { 
				printf("%d ", data[y][x]);
			}
			printf("\n");
		}
	}

	void colorprint(const char * line_prefix = "") {
		for (int y = 0; y < 14; y++) {
			printf("%s", line_prefix);
			for (int x = 0; x < 14; x++) { 
				switch(data[y][x]) {
					case 0:
						printf("  ");
						break;
					case 1:
						printf("[105m  [0m");
						break;
					case 2:
						printf("[104m  [0m");
						break;
					case 3:
						printf("[103m  [0m");
						break;
					case 4:
						printf("[101m  [0m");
						break;
					case 5:
						printf("[106m  [0m");
						break;
					case 6:
						printf("[42m  [0m");
						break;
				}
			}
			printf("\n");
		}
		printf("moves (%zd) : ", moves.size());
		for (vector<int>::iterator i = moves.begin(); i != moves.end(); i++) {
			printf("%d ", *i);
		}
		printf("\n");
	}

	int area_diff;
	state move(int fill) {
		int last_area = area();
		state result(*this);
		result.recfill(fill);
		result.moves.push_back(fill);
		result.area_diff = result.area() - last_area;
		return result;
	}

	const bool contains_color(int color) const {
		for (int y = 0; y < 14; y++) {
			for (int x = 0; x < 14; x++) {
				if (data[y][x] == color) return true;
			}
		}
		return false;
	}

	const int colors_left() const {
		int r = 0;
		for (int i = 1; i <= 6; i++) {
			if (contains_color(i)) r++;
		}
		return r;
	}

	const int steps_to_bottom_right() const {
		int r = 0;
		int l = data[0][0];
		// diagonal
		for (int x = 1; x < 14; x++) {
			if (data[x-1][x] != l) {
				r++;
				l = data[x][x];
			}
			if (data[x][x] != l) {
				r++;
				l = data[x][x];
			}
		}
		return r;
	}

	const int bfdistance() const {
		int mark[14][14];
		for (int y = 0; y < 14; y++) {
			for (int x = 0; x < 14; x++) {
				mark[y][x] = -1;
			}
		}
		bfdistance(mark, data[0][0], 0, 0);
		int res = 0;
		for (int y = 0; y < 14; y++) {
			for (int x = 0; x < 14; x++) { 
				//printf("%2.2d ", mark[y][x]);
				res += mark[y][x];
			}
			//printf("\n");
		}
		return res;
	}

	const int bfdistance(int (&mark)[14][14], int c, int x, int y) const {
		if (mark[y][x] == -1 && data[y][x] == c) {
			mark[y][x] = 0;
			int r = 1;
			if (y-1 > 0) r += bfdistance(mark, c, x, y-1);
			if (y+1 < 14) r += bfdistance(mark, c, x, y+1);
			if (x-1 > 0) r += bfdistance(mark, c, x-1, y);
			if (x+1 < 14) r += bfdistance(mark, c, x+1, y);
			return r;
		} else {
			if (mark[y][x] == 0) return 0; 
			// fill from this point, distance starts out as 1
			mark[y][x] = 1;
			if (y-1 > 0) filldist(mark, data[y][x], 1, x, y-1);
			if (y+1 < 14) filldist(mark, data[y][x], 1, x, y+1);
			if (x-1 > 0) filldist(mark, data[y][x], 1, x-1, y);
			if (x+1 < 14) filldist(mark, data[y][x], 1, x+1, y);
		}
		return 0;
	}

	const void inline filldist(int (&mark)[14][14], int c, int d, int x, int y) const {
		// calculate our distance, if the color is the same, it's d, otherwise d+1
		if (data[y][x] != c) d += 1;
		
		if (mark[y][x] == -1 || mark[y][x] > d) {
			mark[y][x] = d;
			if (y-1 > 0) filldist(mark, data[y][x], d, x, y-1);
			if (y+1 < 14) filldist(mark, data[y][x], d, x, y+1);
			if (x-1 > 0) filldist(mark, data[y][x], d, x-1, y);
			if (x+1 < 14) filldist(mark, data[y][x], d, x+1, y);
		}
	}

	const int area() const {
		int mark[14][14];
		for (int y = 0; y < 14; y++) {
			for (int x = 0; x < 14; x++) {
				mark[y][x] = 0;
			}
		}
		return rec_area(mark, data[0][0], 0, 0);
	}

	int rec_area(int (&mark)[14][14], int c, int x, int y) const {
		if (mark[y][x] == 0 && data[y][x] == c) {
			mark[y][x] = 1;
			int r = 1;
			if (y-1 > 0) r += rec_area(mark, c, x, y-1);
			if (y+1 < 14) r += rec_area(mark, c, x, y+1);
			if (x-1 > 0) r += rec_area(mark, c, x-1, y);
			if (x+1 < 14) r += rec_area(mark, c, x+1, y);
			return r;
		}
		return 0;
	}

	void recfill(int fill) {
		if (data[0][0] == fill)
			return;
		recfill(data[0][0], fill, 0, 0);
	}

	void recfill(int old, int nw, int x, int y) {
		assert(old != nw);
		if (data[y][x] == old) {
			data[y][x] = nw;
			if (y-1 > 0) recfill(old, nw, x, y-1);
			if (y+1 < 14) recfill(old, nw, x, y+1);
			if (x-1 > 0) recfill(old, nw, x-1, y);
			if (x+1 < 14) recfill(old, nw, x+1, y);
		}
	}

	const long rank() const {
		return 2 * area() + 100 * area_diff - steps_to_bottom_right() - 90 * colors_left() ;
	}

	const long rank2() const {
		return area();
	}

	const long rank3() const {
		return 2 * area() + 100 * area_diff - 90 * colors_left() ;
	}

	const long rank4() const {
		return -bfdistance();
	}
};

struct state_compare : public less<state> { 
	const bool operator() (const state & one, const state & two) {
		const int clFact = 0;
		const int mFact = 1000;
		// return one->area() - clFact * one->colors_left() - mFact * one->moves.size() < two->area() - clFact * two->colors_left() - mFact * two->moves.size();
		//return -100 * one.area_diff + one.steps_to_bottom_right() +900 * one.colors_left() + 5 * one.moves.size() > -100 * two.area_diff + two.steps_to_bottom_right() +900 * two.colors_left() - 5 * two.moves.size();
		return one.rank() < two.rank();
	}
};

priority_queue<state, vector<state>, state_compare> * states;
priority_queue<state, vector<state>, state_compare> * next_states;

int main(int argc, char ** argv) {
	states = new priority_queue<state, vector<state>, state_compare>();
	next_states = new priority_queue<state, vector<state>, state_compare>();

	int best_solution_moves = 999;
	state best_solution;

	if (argc > 1) {
		state start(argv[1]);
		//start.print();
		//start.colorprint();
		//printf("\nSearching ...\n\n");

		int max_tracked = 400;
		if (argc > 2) max_tracked = atoi(argv[2]);

		long i = 0;

		state start_ptr(start);
		states->push(start_ptr);

		bool found = false;
		while (! found) {	
			int cnt = 0;
			while (! states->empty() && cnt < max_tracked) {
				if (found) break;
				state s = states->top();
				states->pop();

				int cur_area = s.area();
				for (int m = 1; m <= 6; m++) {
					if (found) break;

					state ns = s.move(m);
					if (ns.area() == 196) {
						//printf("SOLUTION %zd moves !\n", ns.moves.size());
						// ns->colorprint("\t");
						
						if (ns.moves.size() < best_solution_moves) {
							best_solution_moves = ns.moves.size();
							best_solution = ns;
						}
						
						// all states have the same number of moves anyway, so stop
						found = true;
						break;
					} else {
						// if we have a solution, obviously any move will still generate a solution, so don't even try
						// we're trying to find better solutions
						if (ns.area() > cur_area) {
							next_states->push(ns);
						}
					}
				}
				cnt++;
			}
			
			if (cnt == 0) {
				break;
			}
			//printf("Flipping cnt=%d found=%d\n", cnt, found);
			delete states;
			states = next_states;
			next_states = new priority_queue<state, vector<state>, state_compare>();
		}
	}

	//printf("\n");
	printf("%d\n", best_solution_moves);
	//best_solution.colorprint();

	//printf("\n");
	for (vector<int>::iterator i = best_solution.moves.begin(); i != best_solution.moves.end(); i++) {
		printf("%d ", *i);
	}
	printf("\n");

//	if (argc > 1) {
//		printf("\nSolution moves detailed : \n");
//		state s(argv[1]);
//		s.colorprint();
//		for (vector<int>::iterator i = best_solution->moves.begin(); i != best_solution->moves.end(); i++) {
//			s = *s.move(*i);
//			printf("s.area() == %d\n", s.area());
//			s.colorprint("\t\t");
//		}
//		printf("\n");
//	}	

	return 0;
}
