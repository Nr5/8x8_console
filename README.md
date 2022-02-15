Handheld console for atmega328P (use a bigger controller to add more buttons)


		
for adding another program simply make another function, 
	add it to the games array and its name to the game_names array (only capital letters and numbers, '@' in place of ' ')


do not mess with timer0 and timer2 because they are used for rendering and sound
do not write to DDRA/B/C or Write to PORTA/B/C
read from port c bits 1-5 for button presses (1: A, 2: UP, 3: DOWN, 4: RIGHT, 5: LEFT)

[<img src="https://thumbs.odycdn.com/d026c3c0c47de6a6e1ad3d092b4cded3.jpeg" width="50%">](https://odysee.com/@beep_doop:e/handheld_video_game_console "Now in Android: 55")

