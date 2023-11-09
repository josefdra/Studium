/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

/* PORTB2 = A1
   PORTB1 = A4
   PORTB0 = A7

   TODO: Einfügen der Schalter und Taster
*/

int main(void)
{
	// PORTD als Beispiel Port
	// Beispiel falls die Schalter auf PIND0, PIND1, PIND2 und die Taster auf 
	// PIND3, PIND4 liegen
	// Pull-ups der unteren 5 PINs setzen
	uint_8 PIND |= 0x1F
	uint_8 safe;
	uint_8 var;
	// Setzen der ersten 3 Pins in PORTB auf Output
	uint_8 DDRB = 0x07;
	uint_8 helper = PORTB;

    while(1)
    {
		// Sprung Punkt
		point:
		// Abfrage ob Taster A4 (Start) gedrückt wurde
		if (PIND & (1 << PIND4)){
			// Taster A4 wieder auf 0 setzen
			PIND &= ~(1 << PIND4)
			// Durch Initialisierung in der Schleife ist eine Anpassung des Startwerts auch
			// noch zu Laufzeiten möglich
			safe = (PIND & 0x07) + 1;
			var = safe;
			while(1){
				// Variable um 1 dekrementieren außer = 0, dann wieder auf Startwert
				if (var != 0)
				{
					var -= 1;
				}
				else
				{
					var = safe;
				}

				// Falls noch andere Bits in PORTB (helper) gesetzt sind diese entfernen
				helper &= 0xF8;
				helper |= var;
				// Bits in PORTB setzen
				PORTB = helper;

				// Abfrage 10 mal alle 100ms
				for (int i = 0; i < 10; i++){
					// Abfrage, ob Taster A3 gedrückt wurde
					if (PIND & (1 << PIND3)){
						// Taster A3 wieder auf 0 setzen
						PIND &= ~(1 << PIND3)
						// Wenn der Taster A3 am 3. Pin hängt
						// Die untersten 3 Bits von PORTB löschen und somit die LEDs deaktivieren
						PORTB &= 0xF8
						// Wieder zur Abfrage von Taster A4 (Start) springen
						goto point;
					}
					_delay_ms(100);
					// Taster A
				}
				// Werte von PORTB wieder helper zuweisen falls sich ein anderes Bit in PORTB 
				// verändert hat mit dem wir hier nicht arbeiten. Passiert hier logischerweise
				// nicht, wäre aber sonst sinnvoll
				helper = PORTB;
			}
		}
		_delay_ms(100);
    }
}
