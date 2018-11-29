#include <limits.h>

#define ANALOG_MAX 1023

#define BTN_PIN 7
#define LED_PIN 8
#define OK_PIN 9
#define POT_PIN A0
#define ANTENNA_PIN 2

//liste des mots
const char *mots[]={
	"shell",
	"halls",
	"slick",
	"trick",
	"boxes",
	"leaks",
	"strobe",
	"bistro",
	"flick",
	"bombs",
	"break",
	"brick",
	"steak",
	"sting",
	"vector",
	"beats",
};
#define WORDS_COUNT (sizeof(mots)/sizeof(*mots))

// Morse settings and alphabet
#define UNIT_LENGTH 200
#define SHORT 1
#define LONG 3
#define PARTS_DELAY 1
#define LETTERS_DELAY 3
#define WORDS_DELAY 7

const char morse_alpha[][4]={
	{SHORT,LONG,0,0}, 			// A
	{LONG,SHORT,SHORT,SHORT}, 	// B
	{LONG,SHORT,LONG,SHORT}, 	// C
	{LONG,SHORT,SHORT,0}, 		// D
	{SHORT,0,0,0}, 				// E
	{SHORT,SHORT,2,SHORT}, 		// F
	{LONG,LONG,SHORT,0}, 		// G
	{SHORT,SHORT,SHORT,SHORT}, 	// H
	{SHORT,SHORT,0,0}, 			// I
	{SHORT,LONG,LONG,LONG}, 	// J
	{LONG,SHORT,LONG,0}, 		// K
	{SHORT,LONG,SHORT,SHORT}, 	// L
	{LONG,LONG,0,0}, 			// M
	{LONG,SHORT,0,0}, 			// N
	{LONG,LONG,LONG,0}, 		// O
	{SHORT,LONG,LONG,SHORT}, 	// P
	{LONG,LONG,SHORT,LONG}, 	// Q
	{SHORT,LONG,SHORT,0}, 		// R
	{SHORT,SHORT,SHORT,0}, 		// S
	{LONG,0,0,0}, 				// T
	{SHORT,SHORT,LONG,0}, 		// U
	{SHORT,SHORT,SHORT,LONG}, 	// V
	{SHORT,LONG,LONG,0}, 		// W
	{LONG,SHORT,SHORT,LONG}, 	// X
	{LONG,SHORT,LONG,LONG}, 	// Y
	{LONG,LONG,SHORT,SHORT}, 	// Z
};

long numeromot;

// Sleep and check for input at each UNIT_LENGTH ms
bool sleep_check(char units)
{
	for(char i=0; i<units; i++)
	{
		delay(UNIT_LENGTH);
		if(digitalRead(BTN_PIN) == HIGH)
		{
			Serial.println("Button pressed");
			int valPot = analogRead(POT_PIN);
			return WORDS_COUNT * valPot / ANALOG_MAX == numeromot;
		}
	}
	return false;
}

// returns wether the right frequency has been picked
bool send_char(char c)
{
	const char* code = morse_alpha[c-'a'];
	for(char i=0; i<4 && code[i]; i++)
	{
		digitalWrite(LED_PIN, HIGH);
		if(sleep_check(code[i])) return true;
		digitalWrite(LED_PIN, LOW);
		return sleep_check(PARTS_DELAY);
	}
}

// returns wether the right frequency has been picked
bool send_word(const char* s)
{
	for(; *s; s++)
	{
		if(send_char(*s)) return true;
		char delay = (*s==' ' ? WORDS_DELAY : LETTERS_DELAY) - PARTS_DELAY;
		if(sleep_check(delay)) return true;
	}
	return false;
}

void setup() {
	// initialisation:
	Serial.begin(9600);
	//lumiere rouge
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	//lumiere verte
	pinMode(OK_PIN, OUTPUT);
	digitalWrite(OK_PIN, LOW);
	//bouton
	pinMode(BTN_PIN,INPUT);
	//choix du mot au hasard
	randomSeed(analogRead(ANTENNA_PIN));
	numeromot = random(WORDS_COUNT);
	//renvoi du mot pour vérification
	Serial.print("mot: ");
	Serial.println(mots[numeromot]);
}

void loop() {
	if(send_word(mots[numeromot]))
	{ // success, set LEDs and wait indefinitely
		Serial.println("Success");
		digitalWrite(OK_PIN, HIGH);
		digitalWrite(LED_PIN, LOW);
		for(;;) delay(ULONG_MAX);
	}
}
