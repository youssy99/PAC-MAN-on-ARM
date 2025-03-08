#ifndef MUSIC_H
#define MUSIC_H


//Default: 1.65
#define SPEEDUP 2

#define TIMERSCALER 1
#define TIMERSCALEREMU 0.3

#define SECOND 0x17D7840 * TIMERSCALEREMU


typedef char BOOL;
#define TRUE 1
#define FALSE 0

typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;


typedef enum frequencies
{
	a2b = 5351,	// 103Hz	k=5351 a2b
	b2 = 4500,	// 123Hz	k=4500 b2
	c3b = 4370,	// 127Hz	k)4370 c3b
	c3 = 4240,	// 131Hz	k=4240 c3
	d3 = 3779,	// 147Hz	k=3779 d3
	e3 = 3367,	// 165Hz	k=3367 e3
	f3 = 3175,	// 175Hz	k=3175 f3
	g3 = 2834,	// 196Hz	k=2834 g3
	a3b = 2670, // 208Hz  k=2670 a4b
	a3 = 2525,	// 220Hz	k=2525 a3
	b3 = 2249,	// 247Hz	k=2249 b3
	c4 = 2120,	// 262Hz	k=2120 c4
	d4 = 1890,	// 294Hz	k=1890 d4
	e4 = 1684,	// 330Hz	k=1684 e4
	f4 = 1592,	// 349Hz	k=1592 f4
	g4 = 1417,	// 392Hz	k=1417 g4
	a4 = 1263,	// 440Hz	k=1263 a4
	b4 = 1125,	// 494Hz	k=1125 b4
	c5 = 1062, 	// 523Hz	k=1062 c5
	b5 = 562,		// 988Hz	k=562	 b5
	fs5 = 751,	// 740Hz	k=751	 fs5
	ds5 = 893,	// 622Hz	k=893	 ds5
	c6 = 530, 	// 1047Hz	k=530  c6
	g6 = 354,		// 1568Hz k=354	 g6
	e6 = 421,		// 1319Hz k=421  e6
	e5 = 843,		// 659Hz  k=843  e5
	f5 = 796,		// 698Hz  k=796	 f5
	g5 = 709,   // 784Hz  k=709  g5
	gs5= 668,		// 831Hz  k=668	 gs5
	a5 = 631,   // 880Hz  k=631  a5
	fs3= 3003, 
	ds3=3561,
	gs3=2671,
	b1=8960,
	fs2=5973,
	ds2=7122,
	c2=8418,
	e2=6693,
	f2=6242,
	g2=5669,
	gs2=5342,
	a2=5050,
	eb2=7122,
	ab2=4748,
	db2=7936,
	db3=3997,
	gb2=5973,
	bb2=4748,
	d2=7507,
	gb3=2670,
	db4=1786,
	fb4=1501,
	ab4=1192,
	cb4=2005,
	pause = 0		// DO NOT SOUND
} FREQUENCY;


typedef struct 
{
	volatile FREQUENCY freq;
	volatile NOTE_DURATION duration;
} NOTE;

void playNote(NOTE note);
BOOL isNotePlaying(void);
void stopNote();

#endif
/* EOF */
