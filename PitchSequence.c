#include <string.h>
int PitchSequence(char* pitches, int* Count, int CountSize)
{
    int c = 0;
    char *pptr;
    memset(Count, 0, CountSize*sizeof(int));
    int thisCount = 0;
    for (pptr=pitches; pptr && c < CountSize; pptr++)
    {
        switch (*pptr)
        {
        /* https://www.retrosheet.org/eventfile.htm#3 */
        /* The fifth field, pitches, is a string of variable length and contains all pitches to this batter in this plate appearance. Most Retrosheet games do not have pitch data and consequently this field is blank for such games. */
        case '+': /*  following pickoff throw by the catcher*/
        case '*': /*  indicates the following pitch was blocked by the catcher*/
        case '.': /*  marker for play not involving the batter*/
        case '1': /*  pickoff throw to first*/
        case '2': /*  pickoff throw to second*/
        case '3': /*  pickoff throw to third*/
        case '>': /*  Indicates a runner going on the pitch*/
            break;
        case 'A': /*  automatic strike, usually for pitch timer violation*/
        case 'C': /*  called strike*/
        case 'F': /*  foul*/
        case 'K': /*  strike (unknown type)*/
        case 'L': /*  foul bunt*/
        case 'M': /*  missed bunt attempt*/
        case 'O': /*  foul tip on bunt*/
        case 'Q': /*  swinging on pitchout*/
        case 'R': /*  foul ball on pitchout*/
        case 'S': /*  swinging strike*/
        case 'T': /*  foul tip*/
        case 'X': /*  ball put into play by batter*/
        case 'Y': /*  ball put into play on pitchout*/
            thisCount++;
            Count[c++] = thisCount;
            break;
        case 'B': /*  ball*/
        case 'I': /*  intentional ball*/
        case 'P': /*  pitchout*/
        case 'V': /*  called ball because pitcher went to his mouth or automatic ball on intentional walk or pitch timer violation*/
        case 'H': /*  hit batter*/
            thisCount+= 0x100;
            Count[c++] = thisCount;
            break;
        case 'N': /*  no pitch (on balks and interference calls)*/
        case 'U': /*  unknown or missed pitch*/
        default:
            break;
        }
    }
    return c;
}