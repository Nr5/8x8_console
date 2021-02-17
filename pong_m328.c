#define F_CPU 2000000 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
union {
	struct {
			uint8_t bright[8];
			uint8_t medium[8];
			uint8_t faint[8];
			}array;
	uint64_t number[3];
} gfx_buffer;
typedef struct note {
	uint8_t duration;
	uint16_t wavelength;
} note ;
note sound_buffer[0x100];
uint8_t sound_buffer_length;
uint8_t note_index;
uint8_t note_duration;
uint8_t numbers[] = {
//zero
   0b010,
   0b101,
   0b111,
   0b101,
   0b010,
//one
   0b110,
   0b010,
   0b010,
   0b010,
   0b111,
//two
   0b110,
   0b001,
   0b010,
   0b100,
   0b111,
//three
   0b110,
   0b001,
   0b110,
   0b001,
   0b110,
//four
   0b101,
   0b101,
   0b111,
   0b001,
   0b001,
//five
   0b111,
   0b100,
   0b110,
   0b001,
   0b110,
//six
   0b011,
   0b100,
   0b110,
   0b101,
   0b010,
//seven
   0b111,
   0b001,
   0b011,
   0b001,
   0b001,
//eight
   0b010,
   0b101,
   0b010,
   0b101,
   0b111,
//nine
   0b011,
   0b101,
   0b011,
   0b001,
   0b110,
// :
   0b111,
   0b111,
   0b010,
   0b000,
   0b010,
// ;
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// <
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// =
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// >
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// ?
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// @
   0b000,
   0b000,
   0b000,
   0b000,
   0b000,
// A
   0b010,
   0b101,
   0b111,
   0b101,
   0b101,
// B
   0b110,
   0b101,
   0b110,
   0b101,
   0b110,
// C
   0b011,
   0b100,
   0b100,
   0b100,
   0b011,
// D
   0b110,
   0b101,
   0b101,
   0b101,
   0b110,
// E
   0b111,
   0b100,
   0b111,
   0b100,
   0b111,
// F
   0b111,
   0b100,
   0b110,
   0b100,
   0b100,
// G
   0b011,
   0b100,
   0b100,
   0b101,
   0b011,
// H
   0b101,
   0b101,
   0b111,
   0b101,
   0b101,
// I
   0b111,
   0b010,
   0b010,
   0b010,
   0b111,
// J
   0b111,
   0b001,
   0b001,
   0b101,
   0b010,
// K
   0b101,
   0b101,
   0b110,
   0b101,
   0b101,
// L
   0b100,
   0b100,
   0b100,
   0b100,
   0b111,
// M
   0b101,
   0b111,
   0b111,
   0b101,
   0b101,
// N
   0b000,
   0b110,
   0b101,
   0b101,
   0b101,
// O
   0b111,
   0b101,
   0b101,
   0b101,
   0b111,
// P
   0b110,
   0b101,
   0b110,
   0b100,
   0b100,
// Q
   0b010,
   0b101,
   0b101,
   0b111,
   0b011,
// R
   0b110,
   0b101,
   0b101,
   0b110,
   0b101,
// S
   0b011,
   0b100,
   0b010,
   0b001,
   0b110,
// T
   0b111,
   0b010,
   0b010,
   0b010,
   0b010,
// U
   0b101,
   0b101,
   0b101,
   0b101,
   0b111,
// V
   0b101,
   0b101,
   0b101,
   0b101,
   0b010,
// W
   0b101,
   0b111,
   0b111,
   0b111,
   0b110,
// X
   0b101,
   0b101,
   0b010,
   0b101,
   0b101,
// Y
   0b101,
   0b101,
   0b010,
   0b010,
   0b010,
// Z
   0b111,
   0b001,
   0b010,
   0b100,
   0b111,



};
uint8_t * ascii_chars = numbers -48*5;


char* p_c;
uint8_t p_x;
char* p_string;
void print(char* str){
	gfx_buffer.number[1]=0;
	gfx_buffer.number[2]=0;
	gfx_buffer.array.bright[0]=0;
	
	gfx_buffer.array.bright[6]=0;
	gfx_buffer.array.bright[7]=0xff;
	p_string=str;
	p_c = p_string + 2;
	p_x=0;
}

void print_continue(){
	if(!*p_c)p_c=p_string;
	gfx_buffer.array.bright[1]= ascii_chars[*(p_c-2) * 5 +0] << (4+p_x) | 
								ascii_chars[*(p_c-1) * 5 +0] <<    p_x  |
								ascii_chars[*(p_c  ) * 5 +0] >> (4-p_x);
	gfx_buffer.array.bright[2]= ascii_chars[*(p_c-2)*5 +1] << (4+p_x) | 
								ascii_chars[*(p_c-1) * 5 +1] << p_x|
								ascii_chars[*(p_c) * 5 +1] >> (4-p_x);
	gfx_buffer.array.bright[3]= ascii_chars[*(p_c-2)*5 +2] << (4+p_x) |
								ascii_chars[*(p_c-1) * 5 +2] << p_x|
								ascii_chars[*(p_c) * 5 +2] >> (4-p_x);
	gfx_buffer.array.bright[4]= ascii_chars[*(p_c-2)*5 +3] << (4+p_x) |
								ascii_chars[*(p_c-1) * 5 +3] << p_x|
								ascii_chars[*(p_c) * 5 +3] >> (4-p_x);
	gfx_buffer.array.bright[5]= ascii_chars[*(p_c-2)*5 +4] << (4+p_x) |
								ascii_chars[*(p_c-1) * 5 +4] << p_x|
								ascii_chars[*(p_c) * 5 +4] >> (4-p_x);

	p_x= (p_x+1)%4;
	if (!p_x){
		p_c++;
	}

}
//uint8_t *gfx_buffer_faint = gfx_buffer + 8;
uint8_t x = 4;
uint8_t y = 4;
uint8_t counter = 0;

enum key{
	NONE=0,
	LEFT,
	RIGHT,
	DOWN,
	UP

};
enum key input_buffer[2]={NONE,NONE};
ISR(TIMER0_OVF_vect){ // gfx
	counter++;
	PORTB = 0;
	PORTD = ~(1 << (counter%8)) ;
	PORTB = gfx_buffer.array.faint[(counter % 8) ];
}

ISR(TIMER0_COMPA_vect){ // gfx
	PORTB = gfx_buffer.array.medium[(counter % 8) ];
}
ISR(TIMER0_COMPB_vect){ // gfx
	PORTB = gfx_buffer.array.bright[(counter % 8) ];
}

ISR(TIMER2_COMPA_vect){ // sfx
	PORTC ^= 1;
	note_duration ++;
	if(note_duration >= sound_buffer[note_index].duration){
		note_index++ ;
		OCR2A = sound_buffer[note_index].wavelength;
		note_duration = 0;
	}
	if (note_index >= sound_buffer_length)	{
		note_index = 0;
		TCCR2B &= ~0b111; // Start timer at Fcpu/64
		PORTC &= ~1;
	}
}
typedef struct point {
		uint8_t x;
		uint8_t y;

} point;

point  snake[64];
point* snake_end = snake + 2;

point fruit = (point) {2,2};


int8_t dir_x = 1;
int8_t dir_y = 0;


void game_snake(){
label:
	sound_buffer[0].duration=10;
	sound_buffer[0].wavelength=0x20;
	sound_buffer[1].duration=10;
	sound_buffer[1].wavelength=0x10;
	sound_buffer[2].duration=10;
	sound_buffer[2].wavelength=0x08;
	sound_buffer_length = 2;
	OCR2A   = sound_buffer[0].wavelength; // Set CTC compare value with a prescaler of 64 
	TCCR2B |= 0; // Start timer at Fcpu/64
	

	snake[0]=(point){5,4};
	snake[1]=(point){4,4};
	snake[2]=(point){3,4};
    point* snake_end = snake + 2;
	gfx_buffer.array.bright[fruit.y] = 128>>fruit.x;
	while (1){
		for (uint8_t i = 0 ; i<48 ;i++){	
		if (!(PINC & (1 << 5))  ) {
			//LEFT	
			if ( !input_buffer[0] ){
					input_buffer[0]=LEFT;
			}else
			for (uint8_t k = 0; k< 1 ;k++) {
				if (input_buffer[k] != RIGHT && input_buffer[k] != LEFT && !input_buffer[k+1]){
						input_buffer[k+1] = LEFT;
						break;
				}
			}
		}
		if (!(PINC & (1 << 4)) ) {
			//RIGHT
			if ( !input_buffer[0] ){
					input_buffer[0]=RIGHT;
			}else
			for (uint8_t k = 0; k< 1 ;k++) {
				if (input_buffer[k] != LEFT && input_buffer[k] != RIGHT && !input_buffer[k+1]){
						input_buffer[k+1] = RIGHT;
						break;
				}
			}
		}
		if (!(PINC & (1 << 3)) ) {
			//DOWN
			if ( !input_buffer[0] ){
					input_buffer[0]=DOWN;
			}else
			for (uint8_t k = 0; k< 1 ;k++) {
				if (input_buffer[k] != UP && input_buffer[k] != DOWN && !input_buffer[k+1]){
						input_buffer[k+1] = DOWN;
						break;
				}
			}
		}
		if (!(PINC & (1 << 2)) && dir_x ) {
			//UP
			if ( !input_buffer[0] ){
					input_buffer[0]=UP;
			}else
			for (uint8_t k = 0; k< 1 ;k++) {
				if (input_buffer[k] != DOWN && input_buffer[k] != UP && !input_buffer[k+1]){
						input_buffer[k+1] = UP;
						break;
				}
			}
		}
		_delay_ms(1);
		}
		gfx_buffer.array.faint[snake_end->y] &= ~(128 >> snake_end->x);
		

		memmove(snake+1,snake,((snake_end+1)-snake) * sizeof (point) );
		if (*input_buffer){
			if(!dir_x && (*input_buffer == RIGHT || *input_buffer == LEFT) ){
					dir_x = 	(*input_buffer == RIGHT) - (*input_buffer == LEFT);
					dir_y = 0;
			}
			else if(!dir_y && (*input_buffer == DOWN || *input_buffer == UP)){
					dir_x = 0;
					dir_y =(*input_buffer == DOWN) - (*input_buffer == UP);
			}
		}
		gfx_buffer.array.medium[snake[0].y] &= ~(128 >> snake[0].x);
		snake[0].x = (uint8_t)(snake[0].x + dir_x ) % 8;
		snake[0].y = (uint8_t)(snake[0].y + dir_y ) % 8;
		//memmove(input_buffer , input_buffer+1,3 );
	    input_buffer[0]= input_buffer[1];
	    input_buffer[1]= 0;
		gfx_buffer.array.faint[snake[0].y] |= (128 >> snake[0].x);
		gfx_buffer.array.medium[snake[0].y] |= (128 >> snake[0].x);
		
		if (snake[0].x == fruit.x && snake[0].y == fruit.y){
			gfx_buffer.array.bright[fruit.y] = 0;
			snake_end++;
			//gfx_buffer.array.bright[fruit.y] &= ~(128>>fruit.x);
			if ((snake_end-snake)+1 == 64){
					print("@@@YOU@WON:::@@@");
					while ( (PINC & 0b111110) == 0b111110){
						print_continue();
						if (!*p_c)print("@@@YOU@WON:::@@@");
						_delay_ms(30);
					}
					return;
			}
			uint8_t rand = counter % (64 - (snake_end+1-snake) ) ;
			for (uint8_t r = 0; r <= rand; r++){
				if (gfx_buffer.array.faint[r/8] & (128>>(r%8)) ) rand ++;
			}
			//gfx_buffer.number[0] |= (1<<rand);
			fruit.x = rand % 8;
			fruit.y = rand / 8;
			gfx_buffer.array.bright[fruit.y] |= (128>>fruit.x);
			TCCR2B |= (1 << CS22); // Start timer at Fcpu/64
			TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
			OCR2A   = sound_buffer[0].wavelength; // Set CTC compare value with a prescaler of 64 
		}
		for (point* tail = snake + 4; tail <= snake_end; tail ++ ){
			if (tail->x == snake->x && tail->y == snake->y){
				uint8_t points = (snake_end-snake)+1;
				uint8_t digit1 = points /10;
				uint8_t digit2 = points % 10;
				gfx_buffer.array.bright[0]=0;
				gfx_buffer.array.bright[1]=numbers[digit1*5 +0] <<4 | numbers[digit2*5 +0];
				gfx_buffer.array.bright[2]=numbers[digit1*5 +1] <<4 | numbers[digit2*5 +1];
				gfx_buffer.array.bright[3]=numbers[digit1*5 +2] <<4 | numbers[digit2*5 +2];
				gfx_buffer.array.bright[4]=numbers[digit1*5 +3] <<4 | numbers[digit2*5 +3];
				gfx_buffer.array.bright[5]=numbers[digit1*5 +4] <<4 | numbers[digit2*5 +4];
				gfx_buffer.array.bright[6]=0;
				gfx_buffer.array.bright[7]=0xff;
				
				gfx_buffer.number[1]=0;
				gfx_buffer.number[2]=0;
				
				sound_buffer[0].duration=160;
				sound_buffer[0].wavelength=0x40;
				
				sound_buffer[1].duration=80;
				sound_buffer[1].wavelength=0x80;
				

				sound_buffer_length = 3;
				OCR2A   = sound_buffer[0].wavelength; // Set CTC compare value with a prescaler of 64 
				TCCR2B |= (1 << CS22); // Start timer at Fcpu/64
				
				do {
				_delay_ms(100);
				} while ( (PINC & 0b111110) == 0b111110 );


				gfx_buffer.number[0]=0;
				
				goto label;
			}
		}

		_delay_ms(1);
	}

}


void game_paint(){
	uint8_t posx = 4;
	uint8_t posy = 4;

	while (1){
		gfx_buffer.array.bright[posy] = 0;

		posx =(uint8_t) (posx - ((~PINC >> 5) & 1) + ((~PINC >> 4) & 1) ) % 8;
		posy =(uint8_t) (posy - ((~PINC >> 2) & 1) + ((~PINC >> 3) & 1) ) % 8;
		
		gfx_buffer.array.bright[posy] = 128 >> posx	;
		
		_delay_ms(50);
		if (~PINC & (1<<1) )gfx_buffer.array.faint[posy] ^= 128 >> posx;
	}
}

void game_alphabet(){
	while(1)	print("ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789:0@@@");
}
#define NOTE_DISTANCE 1.0905077326652577 // 2 ** (1/8) 
void game_music(){
	//OCR2A   = sound_buffer[0].wavelength; // Set CTC compare value with a prescaler of 64 
	//TCCR2B |= (1 << CS22); // Start timer at Fcpu/64
	while (1){
		if ((~PINC & 0b111100) == 0){
			gfx_buffer.number[1]=0x00;
			gfx_buffer.array.bright[7]=0xff;
			sound_buffer[0].wavelength = 0;
			sound_buffer[0].duration = 0;
			sound_buffer_length = 0;
			TCCR2B = 0;
			continue;
		
		}
		sound_buffer_length = 1;
		sound_buffer[0].duration = 0xff;
		switch (~PINC & 0b111100){
			case (1 << (6-UP)):		            sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE;
							  
									  break;
			case (1 << (6-UP)) | (1 << (6-RIGHT)):  sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE;
									  break;
			case (1 << (6-RIGHT)): 				    sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE;
									  break;
			case (1 << (6-RIGHT)) | (1 << (6-DOWN)):sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE;
									  break;
			case (1 << (6-DOWN)): 				    sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE * NOTE_DISTANCE;
									  break;
			case (1 << (6-DOWN)) | (1 << (6-LEFT)): sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE * NOTE_DISTANCE;
									  break;
			case (1 << (6-LEFT)): 				    sound_buffer[0].wavelength = 63.f * NOTE_DISTANCE;
									  break;
			case (1 << (6-LEFT)) | (1 << (6-UP)): 	sound_buffer[0].wavelength = 63.f ;
									  break;
		}
		if (PINC & 0b10) sound_buffer[0].wavelength *= 2;
	    gfx_buffer.array.medium[2]=(1 << (sound_buffer[0].wavelength / 32 )) - 1;
		TCCR2B 	|= (3); // Start timer at Fcpu/64
		//TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
		OCR2A   = sound_buffer[0].wavelength; // Set CTC compare value with a prescaler of 64 
		_delay_us(1);
	}

}


void (*games[])() ={
	game_snake,
	game_paint,
	game_alphabet,
	game_music,
};
char* game_names[] = {
	"@@SNAKE@@",
	"@@PAINT@@",
	"@@ABC@@",
	"@@MUSIC@@",

};
int main(){
	
	DDRB = 0xff;
	DDRD = 0xff;
	PORTD = 0xfe;
	
	DDRC  = 0b0000001;
	PORTC = 0b1111110;

	
	OCR0A = 0x04;
	OCR0B = 0x40;
	TCCR0A |= 0 ; 
	TCCR0B |= (1 ) ; 
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B) | (1 << TOIE0); //set compare and overflow interrupt
	
	TCNT2   = 0;
	TCCR2A |= (1 << WGM21); // Configure timer 2 for CTC mode
	TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt

	sei();

	uint8_t game_selection = 0;
	while (1){
		game_selection =((sizeof(games)/ sizeof(void(*)()))+game_selection + ((~PINC >> 3) & 1) - ((~PINC >> 2) & 1) ) % (sizeof(games)/ sizeof(void(*)()));
		//game_selection = 2;
		/*
		uint8_t digit1 = game_selection / 10;
		uint8_t digit2 = game_selection % 10;
		
		gfx_buffer.array.bright[0]=0;
		gfx_buffer.array.bright[1]=numbers[digit1*5 +0] <<4 | numbers[digit2*5 +0];
		gfx_buffer.array.bright[2]=numbers[digit1*5 +1] <<4 | numbers[digit2*5 +1];
		gfx_buffer.array.bright[3]=numbers[digit1*5 +2] <<4 | numbers[digit2*5 +2];
		gfx_buffer.array.bright[4]=numbers[digit1*5 +3] <<4 | numbers[digit2*5 +3];
		gfx_buffer.array.bright[5]=numbers[digit1*5 +4] <<4 | numbers[digit2*5 +4];
		gfx_buffer.array.bright[6]=0;
		gfx_buffer.array.bright[7]=0xff;
		
		gfx_buffer.number[1]=0;
		gfx_buffer.number[2]=0;
		*/
		
		print(game_names[game_selection]+2);
		print_continue();
		_delay_ms(30);
		uint8_t i;
		while ( (PINC & 0b1100) == 0b1100 ){
			if (~PINC & (1 << 1) ){
				gfx_buffer.number[0]=0;
				games[game_selection]();
			}
			if (! (i++ % 5)){
				print_continue();
				if (!*p_c)print(game_names[game_selection]);
			}
			_delay_ms(10);
		}
	}


}

