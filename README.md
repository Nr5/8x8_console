Handheld console for atmega328P (use a bigger controller to add more buttons)


		
for adding another program simply make another function, 
	add it to the games array and its name to the game_names array (only capital letters and numbers, '@' in place of ' ')


do not mess with timer0 and timer2 because they are used for rendering and sound
do not write to DDRA/B/C or Write to PORTA/B/C
read from port c bits 1-5 for button presses (1: A, 2: UP, 3: DOWN, 4: RIGHT, 5: LEFT)

<iframe id="odysee-iframe" width="560" height="315" src="https://odysee.com/$/embed/handheld_video_game_console/93c6a94fc4224d37ae22d57128fd7d97daf6b8d2?r=8aMEEtYVzP8A67iUEjC6Eaojqysssbj2" allowfullscreen></iframe>
