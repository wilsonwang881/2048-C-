/*
 * 2048.cpp
 *
 *  Created on: 4 Feb 2017
 *      Author: Lei Wang (Wilson)
 *      CID: 01214043
 *      Imperial College, EEE
 *      Spring 2017 Assignment
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<string>
#include<math.h>
#include<ctime>

using namespace std;

int space_compute(int raw);

struct chessboard {
	//the 2048 chessboard is a 4x4 array
	int matrix[4][4];

	//fine the maximum value in this 4x4 array
	//in order to determining how much space should be reserved
	int max_value() {
		int max = 0;

		//go through every single element in the 4x4 array
		//to find the maximum
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (matrix[i][j] > max) {
					max = matrix[i][j];
				}
			}
		}
		return max;
	}

	//for displaying the 4x4 2048 chessboard
	string display() {
		//fine the maximum value
		int max = max_value();

		//compute the space reserved for the maximum number
		int space = space_compute(max);
		int num_space;
		stringstream chessboard;

		//calculate how much space each single element will occupy
		//as well as how much empty space should be reserved for individual element
		//return the stringstream object
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				chessboard << matrix[i][j];
				if (matrix[i][j] == 0) {
					num_space = space;
				} else {
					num_space = space - space_compute(matrix[i][j]) + 1;
				}
				for (int k = 0; k < num_space; k++) {
					chessboard << " ";
				}
			}
			chessboard << endl;
		}
		//return the stringstream as a string object
		return chessboard.str();
	}
};

//for checking whether the game has reached an end
bool end_check(const chessboard board);

//declare the function used to put a 2
//at random position in the 2048 chessboard
bool random_2(chessboard& board);

//declare the function that moves the 2048 matrix
bool move_add(chessboard& board, char movement);

//declare the function that moves the 2048
//without doing addition
bool move_void(chessboard& board, char movement);

int main() {
	string conf_name;
	ifstream conf_in;
	chessboard board;

	//prompts
	cout << "enter initial configuration file name:" << endl;
	cin >> conf_name;

	//open the configuration file
	conf_in.open(conf_name.c_str());

	//check availability
	//if the designated file is not found, using the default initial chessboard
	if (!conf_in.is_open()) {
		cout << "file not found, using default start configuration" << endl;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				board.matrix[i][j] = 0;
			}
		}
		board.matrix[3][3] = 2;
	} else {
		//if the file is found, read the file
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				conf_in >> board.matrix[i][j];
			}
		}
	}

	char usr;
	bool end = false;
	bool moved_adding, moved_void1, moved_void2, end_in_loop;
	end_in_loop = false;
	while (end == false && end_in_loop == false) {
		moved_void1 = false;
		moved_void2 = false;
		moved_adding = false;
		end_in_loop = false;
		cout << board.display() << endl;

		while (moved_void1 == false && moved_void2 == false
				&& moved_adding == false && end_in_loop==false) {


			cin >> usr;

			//move the chessboard without adding
			//if moved, return true
			moved_void1 = move_void(board, usr);

			//adding the corresponding numbers without moving
			//if added, return true
			moved_adding = move_add(board, usr);

			//move the chessboard without adding
			//if moved, return true
			moved_void2 = move_void(board, usr);

			//check whether if any other movements are possible
			//regardless of user' previous movement
			end_in_loop = end_check(board);
		}

		//put a random 2 onto the 2048 chessboard
		//if this failed, i.e. all spaces are filled
		//return true
		end = random_2(board);

		cout << endl;
	}

	cout << "game over" << endl;
	return 0;
}

//define the function used to compute the length of a number
int space_compute(int raw) {
	int interval;
	int exponent = 0;
	bool found = false;

	while (found == false) {
		//use the built-in function pow
		//to find the number that is both closest to and bigger
		//than the input number
		interval = pow(10, exponent);
		if (interval >= raw) {
			found = true;
		} else {
			exponent++;
		}
	}
	return exponent;
}

//define the function
//that checks whether the game is over
bool end_check(const chessboard board) {
	bool check = false;
	int check_count = 0;

	//define an array consisted of 4 operations
	char operation[4] = {'w','a','s','d'};

	//move the 2048 chessboard in 4 directions
	for (int j = 0; j < 4; j++){
		//reset the chessboard
		chessboard re_board = board;

		//check whether moving the chessboard in certain direction is possible
		check = move_void(re_board, operation[j]);

		//if one operation is possible
		//add 1 to the check_count
		if(check == true){
			check_count++;
		}
	}

	//check if there is any space that is still 0
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board.matrix[i][j] == 0) {
				return false;
			}
		}
	}

	//if any of the moving operations is possible
	//return false
	//if any of the moving operations is not possible
	//return true
	if(check_count>0){
		return false;
	}
	else{
		return true;
	}
}

//define the function used to put a 2
//at random position in the 2048 matrix
bool random_2(chessboard& board) {
	int position, row, column;
	bool gen_end = false;
	bool gen_succ = false;

	//generate the seed
	srand(time(NULL));

	while (gen_succ == false) {
		gen_end = end_check(board);

		if (gen_end == true) {
			return true;
		}
		position = rand() % 16;
		row = position / 4;
		column = position % 4;

		if (board.matrix[row][column] == 0) {
			board.matrix[row][column] = 2;
			gen_succ = true;
		} else {
			gen_succ = false;
		}
	}
	return false;
}

//define the function that move the 2048 matrix
bool move_add(chessboard& board, char movement) {
	//indicating whether a move has been made or not
	//default: false
	bool move_succ = false;

	if (movement == 'w' || movement == 'W') {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				if (board.matrix[j][i] == board.matrix[j+1][i]
															&& board.matrix[j][i] != 0) {
					board.matrix[j][i] = board.matrix[j][i]
														 + board.matrix[j + 1][i];
					board.matrix[j+1][i] = 0;
					move_succ = true;
				}
			}
		}
		return move_succ;
	} else if (movement == 'a' || movement == 'A') {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				if (board.matrix[i][j] == board.matrix[i][j+1]
														  && board.matrix[i][j] != 0) {
					board.matrix[i][j] = board.matrix[i][j]
														 + board.matrix[i][j+1];
					board.matrix[i][j+1] = 0;
					move_succ = true;
				}
			}
		}
		return move_succ;
	} else if (movement == 's' || movement == 'S') {
		for (int i = 0; i < 4; i++) {
			for (int j = 3; j > 0; j--) {
				if (board.matrix[j][i] == board.matrix[j - 1][i]
															  && board.matrix[j][i] != 0) {
					board.matrix[j][i] = board.matrix[j][i]
														 + board.matrix[j - 1][i];
					board.matrix[j - 1][i] = 0;
					move_succ = true;
				}
			}
		}
		return move_succ;
	} else if (movement == 'd' || movement == 'D') {
		for (int i = 0; i < 4; i++) {
			for (int j = 3; j > 0; j--) {
				if (board.matrix[i][j] == board.matrix[i][j-1]
														  && board.matrix[i][j] != 0) {
					board.matrix[i][j] = board.matrix[i][j]
														 + board.matrix[i][j-1];
					board.matrix[i][j-1] = 0;
					move_succ = true;
				}
			}
		}
		return move_succ;
	} else {
		return move_succ;
	}
}

//define the function that moves the 2048
//without doing addition
bool move_void(chessboard& board, char movement) {
	int i = 0;
	int fill_position, move_from;
	bool moved = false;
	bool moved_void = false;

	if (movement == 'w' || movement == 'W') {
		for (int j = 0; j < 4; j++) {
			i = 0;
			while (i < 4) {
				if (board.matrix[i][j] == 0) {
					fill_position = i;
					move_from = fill_position + 1;
					moved = false;
					while (move_from < 4 && moved == false) {
						if (board.matrix[move_from][j] != 0) {
							board.matrix[fill_position][j] =
									board.matrix[move_from][j];
							board.matrix[move_from][j] = 0;
							moved = true;
							moved_void = true;
						} else {
							move_from++;
						}
					}
					i++;
				} else {
					i++;
				}
			}
		}
		return moved_void;
	} else if (movement == 'a' || movement == 'A') {
		for (int j = 0; j < 4; j++) {
			i = 0;
			while (i < 4) {
				if (board.matrix[j][i] == 0) {
					fill_position = i;
					move_from = fill_position + 1;
					moved = false;
					while (move_from < 4 && moved == false) {
						if (board.matrix[j][move_from] != 0) {
							board.matrix[j][fill_position] =
									board.matrix[j][move_from];
							board.matrix[j][move_from] = 0;
							moved = true;
							moved_void = true;
						} else {
							move_from++;
						}
					}
					i++;
				} else {
					i++;
				}
			}
		}
		return moved_void;
	} else if (movement == 's' || movement == 'S') {

		for (int j = 0; j < 4; j++) {
			i = 3;
			while (i >= 0) {
				if (board.matrix[i][j] == 0) {
					fill_position = i;
					move_from = fill_position - 1;
					moved = false;
					while (move_from >= 0 && moved == false) {
						if (board.matrix[move_from][j] != 0) {
							board.matrix[fill_position][j] =
									board.matrix[move_from][j];
							board.matrix[move_from][j] = 0;
							moved = true;
							moved_void = true;
						} else {
							move_from--;
						}
					}
					i--;
				} else {
					i--;
				}
			}
		}
		return moved_void;
	} else if (movement == 'd' || movement == 'D') {
		for (int j = 0; j < 4; j++) {
			i = 3;
			while (i >= 0) {
				if (board.matrix[j][i] == 0) {
					fill_position = i;
					move_from = fill_position - 1;
					moved = false;
					while (move_from >= 0 && moved == false) {
						if (board.matrix[j][move_from] != 0) {
							board.matrix[j][fill_position] =
									board.matrix[j][move_from];
							board.matrix[j][move_from] = 0;
							moved = true;
							moved_void = true;
						} else {
							move_from--;
						}
					}
					i--;
				} else {
					i--;
				}
			}
		}
		return moved_void;
	}else{
		return moved_void;
	}
}
