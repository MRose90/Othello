/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"
#include <limits>
using std::vector;
struct successor {
	OthelloBoard * b;
	int col;
	int row;
	successor * next;
	int score;
};

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}
//return score of p1 - p2
int utility(OthelloBoard* b){
	return b->count_score(b->get_p1_symbol())-b->count_score(b->get_p2_symbol());
}

successor * get_successors(OthelloBoard* b,char symb){
	//if legal move for symb, add to list
	successor * head = (successor*)malloc(sizeof(successor));
	head = NULL;
	successor * curr = head;
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			if (b->is_legal_move(c, r, symb)) {
				OthelloBoard * nb = new OthelloBoard(*b);
				successor * s = (successor*)malloc(sizeof(successor));
				s->b = nb;
				s->col = c;
				s->row = r;
				s->score = 0;
				if(head == NULL){
					head = s;
					curr = s;
				} else{
				curr->next = s;
				curr = s;
				}
			}
		}
	}
	return head;
}

char flip_symb(char curr) {
	if (curr == 'O')
		return 'X';
	return 'O';
}

int minimax(OthelloBoard* b, char symb,successor * s){
	//p2 is always min
	if (s == NULL) {
		return utility(b);
	}
	int nodeVal = 0;
	int currWorst = 500;
	int currBest = -500;
	while (s != NULL) {
		OthelloBoard * nb = new OthelloBoard(*b);
		nb->play_move(s->col,s->row,symb);
		int childVal = minimax(nb, flip_symb(symb), get_successors(nb,symb));
		if(childVal > currBest)
			currBest = childVal;
		if(childVal < currWorst)
			currWorst = childVal;
		successor * old = s;
		s = s->next;
		free(old);
	}
	if(symb == b->get_p1_symbol())
		return currWorst;
	return currWorst;
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	successor *s = get_successors(b, b->get_p2_symbol());
	int currBest = -500;
	while (s != NULL) {
		int childVal = minimax((s->b), b->get_p2_symbol(), get_successors((s->b),b->get_p2_symbol()));
		if (childVal > currBest){
			currBest = childVal;
			col = s->col;
			row = s->row;
		}
		successor * old = s;
		s = s->next;
		free(old);
	}

}


MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
