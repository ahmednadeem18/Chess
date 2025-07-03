#include <iostream>
#include <fstream>
using namespace std;
const int sizes = 8;
char board[sizes][sizes]={ '\0' };

void initialize();
void display();
void makemove(int s_x, int s_y, int d_x, int d_y);
void changeturn(int& turn);
bool rookmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool bishopmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool queenmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool kingmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool horsemove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool WhitePawnmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool BlackPawnmove(int s_x, int s_y, int d_x, int d_y, bool turn);
bool isupper(char move);
bool islower(char move);
bool validdestination(bool turn, int s_x, int s_y, int d_x, int d_y);
bool validsource(bool turn, int s_x, int s_y);
bool ischeck(bool turn);
bool ischeckmate(bool turn);
void reverse(int s_x, int s_y, int d_x, int d_y, char piece);
bool ischeckaftermove(int s_x, int s_y, int d_x, int d_y, bool turn);
void filewriting();
void undomove();
void promotepawn(int x, int y, bool turn);
int main() 
{
	char s_x_temp = 0, d_x_temp = 0;
	char undo = 'n';
	int s_x = 0, s_y=0, d_x = 0, d_y = 0, turn = 0;
	initialize();
	while (true)
	{
		if (ischeckmate(turn))
		{
			if (turn == 0)
			{
				cout << "CheckMate! Nice Job \nPlayer 2 wins." << endl;
			}
			else
			{
				cout << "CheckMate! Nice Job \nPlayer 1 wins." << endl;
			}
			break;
		}
		cout << "You Want to Undo (y for yes/n for no)." << endl;
		cin >> undo;
		if (undo == 'y' || undo == 'Y')
		{
			undomove();
			changeturn(turn);
		}

		display();
		do {
			cout << "Enter the Source Position of player "<<turn+1 << endl;
			cin >> s_x_temp >> s_y;
			
			s_x = s_x_temp - 97;
			s_y--;
		} while (!validsource(turn, s_x, s_y));
		do {
			cout << "Enter the Destination Position of player " << turn + 1 << endl;
			cin >> d_x_temp >> d_y;

			d_x = d_x_temp - 97;
			d_y--;
		} while (!validdestination(turn, s_x, s_y,d_x,d_y)||ischeckaftermove(s_x, s_y, d_x, d_y,turn));
		filewriting();
		makemove(s_x, s_y, d_x, d_y);
		if ((turn == 0 && board[d_x][d_y] == 'p' && d_x == 7) || (turn == 1 && board[d_x][d_y] == 'P' && d_x == 0))
		{
			promotepawn(d_x, d_y, turn);
		}
		changeturn(turn);
		system("cls");



	} 
	return 0;
	
}

void initialize() {
	ifstream read("chess.txt");
	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			read >> board[i][j];
		}
	}
	read.close();
	ofstream write("previous.txt");
	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			write<< board[i][j];
		}
	}
	write.close();

}
void display() {
	char index = 'A';
	cout << "   ";
	for (int i = 0;i < sizes;i++)
	{
		cout <<"  " << i+1;
	}
	cout << endl;
	cout << "   ------------------------" << endl;
	for (int i = 0;i < sizes;i++)
	{
		cout << index<<" |";
		index++;
		for (int j = 0;j < sizes;j++)
		{
			cout << "  " << board[i][j];
		}
		cout << endl;
	}

	
}
void makemove(int s_x, int s_y, int d_x, int d_y)
{
	board[d_x][d_y] = board[s_x][s_y];
	board[s_x][s_y] = '.';
	
}
void changeturn(int& turn)
{
	if (turn == 0)
	{
		turn = 1;
	}
	else
	{
		turn = 0;
	}

}
bool islower(char move)
{
	if (move >96&&move<123)
	{
		return true;
	}
	return false;
}
bool isupper(char move)
{
	if (move > 64&&move<91)
	{
		return true;
	}
	return false;
}
bool validsource(bool turn, int s_x, int s_y)
{
	if (s_x < 0)
	{
		s_x = s_x + 32;
	}
	if (board[s_x][s_y] == '.')
	{
		return false;
	}
	else if (s_x > 7 || s_y > 7)
	{
		return false;
	}
	else if (turn == 0 && isupper(board[s_x][s_y]))
	{
		return false;

	}
	else if (turn == 1 && islower(board[s_x][s_y]))
	{
		return false;

	}
	for (int i = 0; i < sizes; i++) {
		for (int j = 0; j < sizes; j++) {
			if (validdestination(turn, s_x, s_y, i, j)&&!ischeckaftermove( s_x, s_y, i, j, turn))
			{
				return true;
			}
		}
	}

	return false;
}
bool validdestination(bool turn, int s_x, int s_y, int d_x,int d_y) {

	if (d_x < 0)
	{
		d_x = d_x + 32;
	}
	if (d_x > 7 || d_y > 7)
	{
		return false;
	}
	if (board[s_x][s_y] == 'R' || board[s_x][s_y] == 'r') {
		if (rookmove(s_x, s_y, d_x, d_y, turn)) {
			return true;
		}
	}
	else if (board[s_x][s_y] == 'B' || board[s_x][s_y] == 'b') {
		if (bishopmove(s_x, s_y, d_x, d_y,  turn)) {
			return true;

		}
	}
	//Same for Queen
	else if (board[s_x][s_y] == 'Q' || board[s_x][s_y] == 'q') {
		if (queenmove(s_x, s_y, d_x, d_y, turn)) {
			return true;

		}
	}
	//Same for King
	else if (board[s_x][s_y] == 'K' || board[s_x][s_y] == 'k') {
	if (kingmove(s_x, s_y, d_x, d_y, turn)) {
		return true;

	}
}
	//Same for Horse
	else if (board[s_x][s_y] == 'H' || board[s_x][s_y] == 'h') {
		if (horsemove(s_x, s_y, d_x, d_y, turn)) {
			return true;

		}
	}
	else if (board[s_x][s_y] == 'p') {
		if (WhitePawnmove(s_x, s_y, d_x, d_y, turn)) {
			return true;
		}
	}
	else if (board[s_x][s_y] == 'P') {
		if (BlackPawnmove(s_x, s_y, d_x, d_y, turn)) {
			return true;
		}
	}

	return false;

}
bool rookmove(int s_x, int s_y, int d_x, int d_y, bool turn) {
	// Step 1: Check if destination square does not contain a piece of the same player
	if ((turn == 0 && islower(board[d_x][d_y])) || (turn == 1 && isupper(board[d_x][d_y]))) {
			return false;
	}
	// Step 2: Check if the move is either horizontal or vertical
	if (s_x != d_x && s_y != d_y) {
		return false; // Not horizontal or vertical
	}
	// Step 3: Check for obstruction
	if (s_x == d_x) {
		// Case 1: Move is horizontal
		if (s_y < d_y) {
			// Moving right
			for (int col = s_y + 1; col < d_y; ++col) {
				if (board[s_x][col] != '.') {
					return false; // Obstruction found
				}
			}
		}
		else {
			// Moving left
			for (int col = s_y - 1; col > d_y; --col) {
				if (board[s_x][col] != '.') {
					return false; // Obstruction found
				}
			}
		}
	}
	else if (s_y == d_y) {
		// Case 2: Move is vertical
		if (s_x < d_x) {
			// Moving down
			//Write this code
			for (int row = s_x + 1; row < d_x; row++) {
				if (board[row][s_y] != '.') {
					return false; // Obstruction found
				}
			}
		}
		else {
			// Moving up
			// You have to write this code
			for (int row = s_x - 1; row > d_x; row--) {
				if (board[row][s_y] != '.') {
					return false; // Obstruction found
				}
			}

		}
	}
	// If all checks pass, the move is valid
	return true;
}
bool bishopmove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if ((turn == 0 && islower(board[d_x][d_y])) || (turn == 1 && isupper(board[d_x][d_y]))) {
		return false;
	}
	if (abs(s_x - d_x) != abs(s_y - d_y)) {
		return false; // Not a diagonal move
	}
	if (d_x > s_x && d_y > s_y) { // Case 1: Moving down-right
		for (int row = s_x + 1, col = s_y + 1; row < d_x && col < d_y;row++, col++) {

			if (board[row][col] != '.') {
				return false; // Obstruction found
			}
		}
	}
	else if (d_x < s_x && d_y < s_y) { 
		for (int row = s_x - 1, col = s_y - 1; row > d_x && col > d_y;row--, col--) {

			if (board[row][col] != '.') {
				return false; // Obstruction found
			}
		}
	}
	else if (d_x > s_x&& d_y < s_y) {
		for (int row = s_x + 1, col = s_y - 1; row < d_x && col > d_y;row++, col--) {

			if (board[row][col] != '.') {
				return false; // Obstruction found
			}
		}
	}
	else if (d_x < s_x&& d_y > s_y) {
		for (int row = s_x - 1, col = s_y + 1; row > d_x && col < d_y;row--, col++) {

			if (board[row][col] != '.') {
				return false; // Obstruction found
			}
		}
	}

}
bool horsemove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if ((turn == 0 && islower(board[d_x][d_y])) || (turn == 1 && isupper(board[d_x][d_y]))) {
		return false;
	}
	int dx = abs(d_x - s_x);
	int dy = abs(d_y - s_y);

	if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) {
		return false; // Not a valid Horse move
	}
	return true;
}
bool queenmove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if ((turn == 0 && islower(board[d_x][d_y])) || (turn == 1 && isupper(board[d_x][d_y]))) {
		return false;
	}
	if (rookmove(s_x, s_y, d_x, d_y, turn) || bishopmove(s_x, s_y, d_x, d_y, turn))
	{
		return true;
	}
	return false;
}
bool kingmove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if ((turn == 0 && islower(board[d_x][d_y])) || (turn == 1 && isupper(board[d_x][d_y]))) {
		return false;
	}
	if (abs(d_x - s_x) <= 1 && abs(d_y - s_y) <= 1)
	{
		return true;
	}
	return false;
}
bool WhitePawnmove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if (d_x - s_x != 1)
	{
		return false;
	}
	if (board[d_x][d_y]=='.'&&d_y==s_y)
	{
		return true;
	}
	if (abs(d_y - s_y)==1&& isupper(board[d_x][d_y]))
	{
		return true;
	}
	return false;

}
bool BlackPawnmove(int s_x, int s_y, int d_x, int d_y, bool turn)
{
	if (s_x - d_x != 1)
	{
		return false;
	}
	if (board[d_x][d_y] == '.' && d_y == s_y)
	{
		return true;
	}
	if (abs(d_y - s_y) == 1&& islower(board[d_x][d_y]))
	{
		return true;
	}
	return false;

}
bool emptyboard()
{

	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			if (board[i][j] != '.')
			{
				return true;
			}
		}
	}
	return false;
}
bool ischeck(bool turn) {
	int kingX = -1, kingY = -1;
	bool flag = false;

	for (int i = 0; i < sizes && !flag; i++) {
		for (int j = 0; j < sizes; j++) {
			if ((turn == 0 && board[i][j] == 'k') || (turn == 1 && board[i][j] == 'K')) {
				kingX = i;
				kingY = j;
				flag = true;
				break;
			}
		}
	}

	for (int i = 0; i < sizes; i++) {
		for (int j = 0; j < sizes; j++) {
			if ((turn == 0 && isupper(board[i][j])) || (turn == 1 && islower(board[i][j]))) {
				if (validdestination(!turn, i, j, kingX, kingY)) {
					return true;
				}
			}
		}
	}
	return false;
}
bool ischeckmate(bool turn) {
	if (!ischeck(turn)) {
		return false;
	}

	for (int i = 0; i < sizes; i++) {
		for (int j = 0; j < sizes; j++) {
			if ((turn == 1 && isupper(board[i][j])) || (turn == 0 && islower(board[i][j]))) {
				for (int x = 0; x < sizes; x++) {
					for (int y = 0; y < sizes; y++) {
						if (validdestination(turn, i, j, x, y)) {
							char capturedPiece = board[x][y];
							makemove(i, j, x, y);
							if (!ischeck(turn)) {
								reverse(i, j, x, y, capturedPiece);
								return false;
							}
							reverse(i, j, x, y, capturedPiece);
						}
					}
				}
			}
		}
	}
	return true;
}
void reverse(int s_x, int s_y, int d_x, int d_y, char piece) {
	board[s_x][s_y] = board[d_x][d_y];
	board[d_x][d_y] = piece;
}
bool ischeckaftermove(int s_x, int s_y, int d_x, int d_y, bool turn) {
	char piece = board[d_x][d_y];
	makemove(s_x, s_y, d_x, d_y);
	bool check = ischeck(turn);
	reverse(s_x, s_y, d_x, d_y, piece);
	return check;
}
void filewriting()
{
	char temp[sizes][sizes]{ '\0' };
	ifstream read("previous.txt");
	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			read >> temp[i][j];
		}
	}
	read.close();
	ofstream write("previous.txt");
	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			write << board[i][j];
		}
	}
	write.close();
	
}
void undomove()
{
	ifstream read("previous.txt");
	for (int i = 0;i < sizes;i++)
	{
		for (int j = 0;j < sizes;j++)
		{
			read >> board[i][j];
		}
	}
	read.close();
	
	cout << "Undo Done." << endl;
	
}
void promotepawn(int x, int y, bool turn) {
	char choice;
	cout << "Promote pawn to (Q, R, H, B): ";
	cin >> choice;

	if (choice == 'Q' || choice == 'q') {
		if (turn == 1) {
			board[x][y] = 'Q';
		}
		else {
			board[x][y] = 'q';
		}
	}
	else if (choice == 'R' || choice == 'r') {
		if (turn == 1) {
			board[x][y] = 'R';
		}
		else {
			board[x][y] = 'r';
		}
	}
	else if (choice == 'H' || choice == 'h') {
		if (turn == 1) {
			board[x][y] = 'H';
		}
		else {
			board[x][y] = 'h';
		}
	}
	else if (choice == 'B' || choice == 'b') {
		if (turn == 1) {
			board[x][y] = 'B';
		}
		else {
			board[x][y] = 'b';
		}
	}
	else {
		cout << "Invalid choice. Please select Q, R, H, or B." << endl;
		promotepawn(x, y, turn); 
	}
}