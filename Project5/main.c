/*
 * Project5.c
 *
 * Created: 5/29/2019 5:33:50 PM
 * Author : lydam
 */ 

#include "avr.h"
#include "lcd.h"
#include <stdio.h>

//globals
int PITCH = 2;
int DURATION = 130;
int keypad_pressed[16];

// ==================== SETUP =========================
int setup()
{
	inst_Jingle(PITCH, DURATION);
	SET_BIT(DDRA, 0);
	CLR_BIT(PORTA, 0);
	lcd_init();
	lcd_clr();
	SET_BIT(DDRA, 0);
	
	/*
	for(int i = 0; i < 16; i++){
		keypad_pressed[i] = 0;
	}
	*/
	
	return 1;
}
// ================ END SETUP =========================


// ================ KEYPAD ===============================================
// determines if a key on the keypad is pressed
int is_pressed(int r, int c){
	// set all pins of c to N/C - weak 0.
	DDRC = 0;
	PORTC = 0;
	// set PORTC's # r GPIO to Weak 1
	SET_BIT(PORTC, r);
	// set PORTC's # c GPIO to strong 0
	SET_BIT(DDRC, c); // set direction to output
	CLR_BIT(PORTC, c); // strong 0
	if( GET_BIT(PINC, r) != 0){//check if r is not 0)
		// not pressed
		return 0;
	}
	else{
		// is pressed
		return 1;
	}
}

int get_key(){
	int r, c;
	for(r = 0; r < 4; r++){
		for(c = 0; c < 4; c++){
			if(is_pressed(r, c+4)){
				return (r*4 + c) + 1; // pin number
			}
		}
	}
	return 0;
}
// ====== END KEYPAD =================================================

// ======= SPEAKER/MUSIC FUNCTIONS ===================================
typedef struct Note{
	int freq;
	int duration;
} Note;

Note notes[10];
int song_length = 30;
int some_notes[] = {2,2,2,-1,2,2,2,-1,2,4,0,7,2,-1,3,3,3,3,3,2,2,6,6,2,1,1,2,1,-2,4};
//TODO: (You need to add 2 songs) and their lengths, use the value -5 to signal the end
// of the string so we dont have to keep track of the length?

void wait(int duration){
	CLR_BIT(PORTA, 0);
	avr_wait(duration);	
}

//contains all 5 notes required to play Jingle Bells
//instantiates the notes: frequency and duration
void inst_Jingle(int pitch_scalar, int duration){
	// TODO: IMPLEMENT THIS
	// FILL IN THE REST OF THE NOTES FREQUENCY AND DURATION
	
	// A - 220
	
	//A# / Bb - 233.082
	
	//B - 246.942
	
	notes[0].freq = pitch_scalar*261; //C
	notes[0].duration = duration;
	
	
	//C# - 277.183
	
	notes[1].freq = pitch_scalar*293; //D
	notes[1].duration = duration;
	
	// D# 311.127
	
	notes[2].freq = pitch_scalar*330;//E
	notes[2].duration = duration;
	notes[3].freq = pitch_scalar*349; //F
	notes[3].duration = duration;
	
	// F# 370
	notes[4].freq = pitch_scalar*392; //G
	notes[4].duration = duration;
	
	// G# 415.305
	
	notes[5].freq = 1;
	notes[5].duration = 200;
}

void play_note(Note myNote){
	int freq = myNote.freq;
	int duration = myNote.duration;
	
	int i,n;
	double period = 1.0/freq;
	n = duration;
	
	double high = (period/2)*100000;
	double low = (period/2)*100000;
	int TH = (int)high;
	int TL = (int)low;

    //1  cycle
	for (i = 0; i < n; i++) {
		SET_BIT(PORTA,0);
		avr_wait_u(TH);
		CLR_BIT(PORTA,0);
		avr_wait_u(TL);
	}
	//return;
}

void play_song(int song[], int length){
	for(int i = 0; i < length; i++){
		if(song[i] == -1){
			wait(500);
			continue;
		}
		else if (song[i] == -2){
			wait(600);
			continue;
		}
		play_note(notes[song[i]]);
	}
	return ;
}

// Description: This function displays a list of the song choices
// e.g. 1) Call me mabye 2)Big daddy
//		3) Dont leave me 3) Tell me you love me
void display_song_list(){
	// TODO:
}

 // ========== END SPEAKER/MUSIC FUNCTIONS =================
 
 
 // ====== SCORE ===========================================
 unsigned short CORRECT_POS = 5;
 
 typedef struct Score{
	int num_right;
	int total;	 
 } Score;
 

// displays the score in the format:
//	SCORE: num-right/total
 void display_score(Score score){
	  // TODO:
 }
 
 // displays whether the correct key is pressed
 int is_right(unsigned short pos, unsigned short note_index, int goal_note_index){
	//TODO:
	if(pos == CORRECT_POS &&  note_index == goal_note_index){
		return 1;	
	}
	return 0; 
 }
 
 // ===== END SCORE ========================================
 
 // ======= GAME ===========================================
 void prompt_start_game(){
	 // TODO: Prompt the user asking them if they want to start the game
 }
 
 // ===== END GAME =========================================

/*
	Description:
	FLOW OF EVENTS: User is prompted with a song selection list, when song is selected
	the user will then be prompted with the "words" press <x> to start. WHen the user presses x
	the game begins and the corresponding song is played. TO play the game the user
	must press the corresponding key when the note is under the arrow.
	the Note will slide from left to right on the second row.
	the first row will not be used and will only display
	an arrow where the user must press the corresponding key.
	if the user gets it wrong, update the score struct. the total is incremented, but the
	num_right doesnt change. If the user gets it right then the total and num_right is incremented.

*/

int main(void)
{
	//local variables
	int k;
	unsigned short state = 0;
	unsigned short song_index;
	unsigned short note_index;
	
	// setting up
	setup();
	
	// main logic
    while (1) 
    {	
		switch(state){
			case 0: // prompt user to pick a song
				display_song_list();
				k = get_key();
				if(k == 0){ // first song
					song_index = 0;
					state = 1;
				}
				else if(k == 1){ // second song
					song_index = 1;
					state = 1;
				}
				break;
			case 1: // prompt user to start game
				prompt_start_game();
				k = get_key();
				if(k == '15'){
					state = 2;
					note_index = 0;
				}
				break;
			case 2: // play game
				lcd_pos(0,0);
				lcd_puts2("Starting Game");
		}
    }
}



