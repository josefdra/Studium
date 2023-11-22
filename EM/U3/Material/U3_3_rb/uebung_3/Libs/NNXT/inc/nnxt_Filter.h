#ifndef FLOATING_AVERAGE_H
#define FLOATING_AVERAGE_H

#include <inttypes.h>

// Ueber wieviele Werte soll der gleitende Mittelwert berechnet werden?
// Zulaessige Werte 1..255
#define SIZE_OF_AVG  8

// Datentyp, ueber den der gleitende Mittelwert berechnet werden soll.
typedef uint16_t tFloatAvgType;
// typedef float tFloatAvgType;

// Wird nur intern fuer die Durchschnittsberechnung benutzt.
// Muss Zahlen fassen koennen, die SIZE_OF_AVG mal groesser als tFloatAvgType sind.
typedef uint32_t tTempSumType;
// typedef float tTempSumType;

// Die Struktur, in der die Daten zwischengespeichert werden
typedef struct
 {
	tFloatAvgType aData[SIZE_OF_AVG];
	uint8_t IndexNextValue;
 } tFloatAvgFilter;


// Initialisiert das Filter mit einem Startwert.
void InitFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter,
		  tFloatAvgType i_DefaultValue);

// Schreibt einen neuen Wert in das Filter.
void AddToFloatAvg(tFloatAvgFilter * io_pFloatAvgFilter,
		   tFloatAvgType i_ui16NewValue);

// Berechnet den Durchschnitt aus den letzten SIZE_OF_AVG eingetragenen Werten.
tFloatAvgType GetOutputValue(tFloatAvgFilter * io_pFloatAvgFilter);

#endif
