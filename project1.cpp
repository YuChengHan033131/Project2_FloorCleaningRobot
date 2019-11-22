#include <iostream>
#include <fstream>


using namespace std;
void put_blocks(bool matrix[45+5][15], int row, int col, int begin_col, int type);
void check_line(bool matrix[45+5][15], int row, int col, int start_row, int height);
bool is_game_over(bool matrix[45+5][15], int row, int col);

int main(){
	ifstream inFile("Tetris.data");
	if(!inFile) cout << "fail to open Tetris.data \n";
	bool matrix[45+5][15] = {0};
	int row, col;
	inFile >> row >> col;
	string block_type;
	int start_col;
	while(inFile >> block_type){
		if(block_type == "End") break;
		inFile >> start_col;
		if(block_type == "T1")      put_blocks(matrix, row, col, start_col, 0 );
		else if(block_type == "T2") put_blocks(matrix, row, col, start_col, 1 );
		else if(block_type == "T3") put_blocks(matrix, row, col, start_col, 2 );
		else if(block_type == "T4") put_blocks(matrix, row, col, start_col, 3 );
		else if(block_type == "L1") put_blocks(matrix, row, col, start_col, 4 );
		else if(block_type == "L2") put_blocks(matrix, row, col, start_col, 5 );
		else if(block_type == "L3") put_blocks(matrix, row, col, start_col, 6 );
		else if(block_type == "L4") put_blocks(matrix, row, col, start_col, 7 );
		else if(block_type == "J1") put_blocks(matrix, row, col, start_col, 8 );
		else if(block_type == "J2") put_blocks(matrix, row, col, start_col, 9 );
		else if(block_type == "J3") put_blocks(matrix, row, col, start_col, 10);
		else if(block_type == "J4") put_blocks(matrix, row, col, start_col, 11);
		else if(block_type == "S1") put_blocks(matrix, row, col, start_col, 12);
		else if(block_type == "S2") put_blocks(matrix, row, col, start_col, 13);
		else if(block_type == "Z1") put_blocks(matrix, row, col, start_col, 14);
		else if(block_type == "Z2") put_blocks(matrix, row, col, start_col, 15);
		else if(block_type == "I1") put_blocks(matrix, row, col, start_col, 16);
		else if(block_type == "I2") put_blocks(matrix, row, col, start_col, 17);
		else if(block_type == "O")  put_blocks(matrix, row, col, start_col, 18);
		else break;

		if(is_game_over(matrix, row, col)) break;
	}
	inFile.close();
	fstream outFile;
	outFile.open("Tetris.final", ios::out);
	for(int i = row-1; i >= 0; --i){
		for(int j = 0; j < col; ++j){
			outFile << matrix[i][j];
		}
		outFile << '\n';
	}
	outFile.close();
	return 0;
}

int pos_r[19][5] = {{0, 1, 1, 1, 2}, {0, 1, 1, 2, 3}, {0, 0, 0, 1, 2}, {0, 1, 1, 2, 3},
				    {0, 0, 1, 2, 3}, {0, 1, 1, 1, 2}, {0, 1, 2, 2, 3}, {0, 0, 0, 1, 2},
				    {0, 0, 1, 2, 3}, {0, 0, 0, 1, 2}, {0, 1, 2, 2, 3}, {0, 1, 1, 1, 3},
				    {0, 0, 1, 1, 2}, {0, 1, 1, 2, 3}, {0, 0, 1, 1, 2}, {0, 1, 1, 2, 3},
				    {0, 1, 2, 3, 4}, {0, 0, 0, 0, 1}, {0, 0, 1, 1, 2}					};

int pos_c[19][4] = {{1, 0, 1, 2}, {1, 0, 1, 1}, {0, 1, 2, 1}, {0, 0, 1, 0},
					{0, 1, 0, 0}, {0, 0, 1, 2}, {1, 1, 0, 1}, {0, 1, 2, 2},
					{0, 1, 1, 1}, {0, 1, 2, 0}, {0, 0, 0, 1}, {2, 0, 1, 2},
					{0, 1, 1, 2}, {1, 0, 1, 0}, {1, 2, 0, 1}, {0, 0, 1, 1},
					{0, 0, 0, 0}, {0, 1, 2, 3}, {0, 1, 0, 1}				};

void put_blocks(bool matrix[45+5][15], int row, int col, int begin_col, int type){
	int colb = begin_col - 1;
	int rowb = row;
	for(int i = row-1; i >= 0; --i){
		bool ok_to_fall = true;
		for(int k = 0; k < 4; ++k){
			int x = i + pos_r[type][k];
			int y = colb + pos_c[type][k];
			if(matrix[x][y] == 1){
				ok_to_fall = false;
				break;
			}
		}
		if(ok_to_fall) rowb = i;
		else break;
	}
	for(int i = 0; i < 4; ++i){
		int x = rowb + pos_r[type][i];
		int y = colb + pos_c[type][i];
		matrix[x][y] = 1;
	}
	if(rowb == row) return;
	check_line(matrix, row, col, rowb, pos_r[type][4]);
}

void check_line(bool matrix[45+5][15], int row, int col, int start_row, int height){
	int row_check = start_row;
	for(int i = 0; i < height && row_check < row; ++i){
		bool rst = true; //whole line reset to zero
		for(int j = 0; j < col; ++j){
			if(matrix[row_check][j] == 0){
				rst = false;
				break;
			}
		}
		if(rst){
			for(int p = row_check; p < row+3; ++p)
				for(int q = 0; q < col; ++q)
					matrix[p][q] = matrix[p+1][q];
		}else{
			row_check++;
		}
	}
}

bool is_game_over(bool matrix[45+5][15], int row, int col){
	for(int i = 0; i < col; ++i){
		if(matrix[row][i] == true) return true;
	}
	return false;
}
