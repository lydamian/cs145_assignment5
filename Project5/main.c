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
int jingle_bells[] = {4,4,4,-1,4,4,4,-1,4,6,2,3,4,-1,5,5,5,5,5,4,4,8,8,4,3,3,4,3,-2,6};		    
int jingle_size = sizeof(jingle_bells)/sizeof(jingle_bells[0]);
int mary_lambs[] = {4,3,2,3,4,4,4,3,3,3,4,6,6,4,3,2,3,4,4,4,4,3,3,4,3,2};
int mary_size = sizeof(mary_lambs)/sizeof(mary_lambs[0]);
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
	
	notes[0].freq = pitch_scalar*220; // A - 220
	notes[0].duration = duration;
	
	notes[1].freq = pitch_scalar*246; //B - 246.942
	notes[1].duration = duration;
	
	notes[2].freq = pitch_scalar*261; //C
	notes[2].duration = duration;
	
	notes[3].freq = pitch_scalar*293; //D
	notes[3].duration = duration;
	
	notes[4].freq = pitch_scalar*330;//E
	notes[4].duration = duration;
	
	notes[8].freq = pitch_scalar*330;//E FAST
	notes[8].duration = duration/2;
	
	notes[5].freq = pitch_scalar*349; //F
	notes[5].duration = duration;
	
	notes[6].freq = pitch_scalar*392; //G
	notes[6].duration = duration;
	
	notes[7].freq = 1; //REST
	notes[7].duration = 200;
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
	lcd_clr();
	char top_row[16];
	sprintf(top_row, "Now playing");
	lcd_pos(0,0);
	lcd_puts2(top_row);
	
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
	// after prompt_start_game = A = yes
	char top_row[16];
	char bot_row[16];
	lcd_pos(0,0);
	sprintf(top_row, "Jingle Bells:1");
	lcd_puts2(top_row);
	
	lcd_pos(1,0);
	sprintf(bot_row, "Mary Lamb:2");
	lcd_puts2(bot_row);
	
	//avr_wait(500);
	return ;
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
	 
	 lcd_clr();
	 char bot_row[16];
	 lcd_pos(0,0);
	 sprintf(bot_row, "Play? A=y,B=n");
	 lcd_puts2(bot_row);

	 return ;
 }
 
 
 
 // ===== END GAME =========================================
 // thanos dies

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
int chosen_song[30];
int chosen_length = 0;
void copy_song(int song[], int length) {
		for (int i = 0; i < length; i++) {
			chosen_song[i] = song[i];
		}
		chosen_length = length;
}

int n = 0;
void move_notes() {
	char top_row[16];
	sprintf(top_row, "C");
	lcd_pos(0,n);
	lcd_puts2(top_row);
	avr_wait(500);
	for (int n = 0; n < 16; n++) {
		lcd_clr();
		printf(top_row, "C");
		lcd_pos(0,n);
		lcd_puts2(top_row);
		avr_wait(500);
	}
}

enum state {init, pick_song, jingle_bell, mary_lamb, start, play, quit};
int main(void)
{
	//local variables
	int k;
	//unsigned short state = 0;
	unsigned short song_index;
	unsigned short note_index;
	
	// setting up
	//setup();
	
	// main logic
	enum state curr_state;
	curr_state = init; //initialize
    while (1) 
    {	
		//prompt_start_game();
		//play_song(jingle_bells, 29);
		
		//execute states
		switch(curr_state) {
			//this will direct states
			case init:
				setup();
				break;
			case pick_song:
				display_song_list();
				break;
			case jingle_bell:
				//load the jingle bells array
				copy_song(jingle_bells, jingle_size);
				break;
			case mary_lamb:
				//load mary lamb array
				copy_song(mary_lambs, mary_size);
				break;
			case start:
				prompt_start_game();
				break;
			case play:
				//need to implement notes on screen
				//play_song(chosen_song, chosen_length);
				move_notes();
				break;
		}
		//direct states
		switch(curr_state) {
			case init:
				curr_state = pick_song;
				break;
			case pick_song:
				while (!is_pressed(0,0) || !is_pressed(0,1)) { //have to choose either song
					k = get_key();
					if (k == 1) {
						curr_state = jingle_bell;
						break;
					}
					else if (k == 2){
						curr_state = mary_lamb; //NOT IMPLEMENTED YET
						break;
					}
				}
				break;
			case jingle_bell:
				curr_state = start;
				break;
			case mary_lamb:
				curr_state = start;
				break;
			case start:
				while (!is_pressed(0,3) || !is_pressed(1,3)) { //A = yes, B = no
					k = get_key();
					if (k == 4) { //A
						//start game
						//play_song(jingle_bells,30); //plays jingle bells for now
						curr_state = play;
						break;
					}
					else if (k == 8) { //B
						//restart
						curr_state = pick_song;
						break;
					}
				}
				break;
			case play:
				curr_state = init;
				break;	
		}
		
    }
}


/*
		switch(state){
			//lcd_clr();
			case 0: // prompt user to pick a song
				display_song_list();
				k = get_key();
				if(k == 0){ // first song: #1
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
				if(k == '15'){ //#
					state = 2;
					note_index = 0;
				}
				break;
			case 2: // play game
				lcd_pos(0,0);
				lcd_puts2("Starting Game");
				
		}
		*/
