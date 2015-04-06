/*
 * (c) 2014 Luc Hondareyte <hondareyte.luc@laposte.net>
 */

#ifndef __BREATH_H__
#define TABLE_SIZE	128

/* courbe lineaire */
static const int a_curve[TABLE_SIZE] = 
{
	0, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 13, 14, 15, 
	16, 17, 18, 19, 20, 21, 22, 23, 
	24, 25, 26, 27, 28, 29, 30, 31, 
	32, 33, 34, 35, 36, 37, 38, 39, 
	40, 41, 42, 43, 44, 45, 46, 47, 
	48, 49, 50, 51, 52, 53, 54, 55, 
	56, 57, 58, 59, 60, 61, 62, 63, 
	64, 65, 66, 67, 68, 69, 70, 71, 
	72, 73, 74, 75, 76, 77, 78, 79, 
	80, 81, 82, 83, 84, 85, 86, 87, 
	88, 89, 90, 91, 92, 93, 94, 95, 
	96, 97, 98, 99, 100, 101, 102, 103, 
	104, 105, 106, 107, 108, 109, 110, 111, 
	112, 113, 114, 115, 116, 117, 118, 119, 
	120, 121, 122, 123, 124, 125, 126, 127
};

/* courbe pseudo exponentielle */
static const int b_curve[TABLE_SIZE] = 
{
	1, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 13, 14, 15, 
	17, 19, 22, 27, 31, 35, 38, 41, 
	44, 46, 49, 51, 53, 55, 57, 59, 
	61, 63, 65, 66, 68, 69, 71, 72, 
	74, 75, 76, 78, 79, 80, 81, 83, 
	84, 85, 86, 87, 88, 89, 90, 91, 
	92, 93, 94, 95, 96, 96, 97, 98, 
	99, 100, 100, 101, 102, 103, 103, 104, 
	105, 105, 106, 107, 107, 108, 109, 109, 
	110, 110, 111, 111, 112, 112, 113, 113, 
	114, 114, 115, 115, 116, 116, 117, 117, 
	117, 118, 118, 119, 119, 119, 120, 120, 
	120, 121, 121, 121, 122, 122, 122, 122, 
	123, 123, 123, 123, 124, 124, 124, 124, 
	124, 125, 125, 125, 125, 126, 126, 127
};

/* Voir doc */

#if defined __WT11_COMPAT__
static const int c_curve[TABLE_SIZE] = 
{
	20, 20, 20, 20, 20, 20, 20, 20, 
	20, 20, 20, 20, 21, 21, 21, 21, 
	21, 22, 22, 22, 22, 23, 23, 23, 
	24, 24, 24, 25, 25, 25, 26, 26, 
	27, 27, 28, 28, 29, 29, 30, 30, 
	31, 31, 32, 32, 33, 34, 34, 35, 
	35, 36, 37, 37, 38, 39, 40, 40, 
	41, 42, 43, 43, 44, 45, 46, 47, 
	48, 48, 49, 50, 51, 52, 53, 54, 
	55, 56, 57, 58, 59, 60, 61, 62, 
	63, 64, 65, 66, 67, 69, 70, 71, 
	72, 73, 74, 76, 77, 78, 79, 81, 
	82, 83, 85, 86, 87, 89, 90, 91, 
	93, 94, 95, 97, 98, 100, 101, 103, 
	104, 106, 107, 109, 110, 112, 113, 115, 
	117, 118, 120, 122, 123, 125, 126, 127
};

static const int d_curve[TABLE_SIZE] = 
{
	25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 26, 26, 26,
	26, 26, 27, 27, 27, 27, 28, 28,
	28, 28, 29, 29, 29, 30, 30, 31,
	31, 31, 32, 32, 33, 33, 34, 34,
	35, 35, 36, 36, 37, 37, 38, 38,
	39, 40, 40, 41, 42, 42, 43, 44,
	44, 45, 46, 47, 47, 48, 49, 50,
	50, 51, 52, 53, 54, 55, 55, 56,
	57, 58, 59, 60, 61, 62, 63, 64,
	65, 66, 67, 68, 69, 70, 71, 72,
	73, 75, 76, 77, 78, 79, 80, 82,
	83, 84, 85, 86, 88, 89, 90, 92,
	93, 94, 96, 97, 98, 100, 101, 102,
	104, 105, 107, 108, 110, 111, 113, 114,
	116, 117, 119, 120, 122, 123, 125, 126 
};

static const int e_curve[TABLE_SIZE] = 
{
	30, 30, 30, 30, 30, 30, 30, 30, 
	30, 30, 30, 30, 30, 31, 31, 31, 
	31, 31, 31, 32, 32, 32, 32, 33, 
	33, 33, 34, 34, 34, 35, 35, 35, 
	36, 36, 36, 37, 37, 38, 38, 39, 
	39, 40, 40, 41, 41, 42, 42, 43, 
	43, 44, 45, 45, 46, 46, 47, 48, 
	48, 49, 50, 50, 51, 52, 53, 53, 
	54, 55, 56, 56, 57, 58, 59, 60, 
	61, 62, 62, 63, 64, 65, 66, 67, 
	68, 69, 70, 71, 72, 73, 74, 75, 
	76, 77, 78, 79, 80, 82, 83, 84, 
	85, 86, 87, 88, 90, 91, 92, 93, 
	95, 96, 97, 98, 100, 101, 102, 104, 
	105, 106, 108, 109, 110, 112, 113, 115, 
	116, 118, 119, 120, 122, 123, 125, 126 
};	

#else
static const int c_curve[TABLE_SIZE] = 
{
	0, 3, 6, 9, 12, 15, 18, 21, 
	24, 27, 30, 33, 36, 39, 42, 45, 
	48, 51, 54, 57, 60, 63, 66, 69, 
	72, 75, 78, 81, 84, 87, 90, 71, 
	72, 73, 74, 75, 76, 77, 78, 79, 
	80, 81, 82, 83, 84, 85, 86, 87, 
	88, 89, 90, 104, 104, 105, 105, 105, 
	106, 106, 106, 106, 107, 107, 107, 108, 
	108, 108, 108, 109, 109, 109, 110, 110, 
	110, 110, 111, 111, 111, 112, 112, 112, 
	112, 113, 113, 113, 114, 114, 114, 114, 
	115, 115, 115, 116, 116, 116, 116, 117, 
	117, 117, 118, 118, 118, 118, 119, 119, 
	119, 120, 120, 120, 120, 121, 121, 121, 
	122, 122, 122, 122, 123, 123, 123, 124, 
	124, 124, 124, 125, 125, 125, 126, 126
};

/* Voir doc */
static const int d_curve[TABLE_SIZE] = 
{
	0, 3, 6, 9, 12, 15, 18, 21, 
	24, 27, 30, 33, 36, 39, 42, 45, 
	48, 51, 54, 57, 60, 63, 66, 69, 
	72, 75, 78, 81, 84, 87, 90, 84, 
	84, 85, 85, 85, 86, 86, 87, 87, 
	88, 88, 89, 89, 90, 90, 90, 91, 
	91, 92, 92, 93, 93, 94, 94, 95, 
	95, 95, 96, 96, 97, 97, 98, 98, 
	99, 99, 100, 100, 100, 101, 101, 102, 
	102, 103, 103, 104, 104, 105, 105, 105, 
	106, 106, 107, 107, 108, 108, 109, 109, 
	110, 110, 110, 111, 111, 112, 112, 113, 
	113, 114, 114, 115, 115, 115, 116, 116, 
	117, 117, 118, 118, 119, 119, 120, 120, 
	120, 121, 121, 122, 122, 123, 123, 124, 
	124, 125, 125, 125, 126, 126, 127, 127
};

/* Voir doc */
static const int e_curve[TABLE_SIZE] = 
{
	0, 1, 1, 1, 1, 2, 2, 3, 
	3, 3, 4, 4, 5, 5, 6, 6, 
	6, 7, 7, 8, 8, 9, 9, 9, 
	10, 10, 11, 11, 12, 12, 12, 13, 
	13, 14, 14, 15, 15, 15, 16, 16, 
	17, 17, 18, 18, 18, 19, 19, 20, 
	20, 21, 21, 21, 22, 22, 23, 23, 
	24, 24, 24, 25, 25, 26, 26, 27, 
	27, 27, 28, 28, 29, 29, 30, 31, 
	32, 33, 34, 35, 36, 37, 38, 39, 
	40, 41, 42, 43, 44, 45, 46, 47, 
	48, 49, 50, 52, 54, 56, 58, 60, 
	62, 64, 66, 68, 70, 72, 74, 77, 
	79, 81, 83, 85, 87, 89, 91, 93, 
	95, 97, 99, 102, 104, 106, 108, 110, 
	112, 114, 116, 118, 120, 122, 124, 127 
};
#endif

static const int f_curve[TABLE_SIZE] = 
{
	0, 0, 0, 1, 1, 2, 2, 3, 
	3, 3, 4, 4, 5, 5, 6, 6, 
	6, 7, 7, 8, 8, 9, 9, 9, 
	10, 10, 11, 11, 12, 12, 12, 13, 
	13, 14, 14, 15, 15, 15, 16, 16, 
	17, 17, 18, 18, 18, 19, 19, 20, 
	20, 21, 21, 21, 22, 22, 23, 23, 
	24, 24, 24, 25, 25, 26, 26, 27, 
	27, 27, 28, 28, 29, 29, 30, 31, 
	33, 35, 36, 38, 40, 41, 43, 45, 
	47, 48, 50, 52, 53, 55, 57, 58, 
	60, 62, 64, 65, 67, 69, 70, 72, 
	74, 75, 77, 79, 81, 82, 84, 86, 
	87, 89, 91, 92, 94, 96, 98, 99, 
	101, 103, 104, 106, 108, 109, 111, 113, 
	115, 116, 118, 120, 121, 123, 125, 127 
};

#endif
