import re
'''
https://www.retrosheet.org/datause.html */
FEDCBA9876543210 */
OO321HBBBFFFFFSS */
 3 outs    : 2 bit 15..14 */
 4 runners : 4 bit 13..10 */
 7 balls   : 3 bit  9.. 7 */
31 fouls   : 5 bit  6.. 2 */
 3 strikes : 2 bit  1.. 0 */
total      : 16 bit */
'''
def DEST(x):
    return (1<<(10+(x&3))) if (x>0 and x<4) else 0

def ProcessEvent (runners, event):
    p2 = re.split ('/', event)
    p3 = re.split ('\\.', event)
    p1 = p2[0] if len(p2[0]) < len(p3[0]) else p3[0]
    advs = [] if len(p3)==1 else re.split(';', p3[1])
    res = re.split(';', p1) + advs
    for r in res:
        match r:
            case 'K': runners[0] = -1
            case '3': runners[0] = -1
            case '7': runners[0] = -1
            case '8': runners[0] = -1
            case '31': runners[0] = -1
            case 'S16': runners[0] = 1
            case 'S7': runners[0] = 1
            case '36(1)': runners[1] = -1
            case 'B-1': runners[0] = 1
            case '1-H': runners[1] = 4
            case 'HR': runners[0] = 4
            case '1-2': runners[1] = 2
            case '1-3': runners[1] = 3
            case '1-H(UR)': runners[1] = 4
            case '13': runners[0] = -1
            case '2-3': runners[2] = 3
            case '2-H': runners[2] = 4
            case '3-H': runners[3] = 4
            case '3-H(UR)': runners[3] = 4
            case '4': runners[0] = -1
            case '43': runners[0] = -1
            case '5': runners[0] = -1
            case '53': runners[0] = -1
            case '54(1)': runners[1] = -1
            case '6': runners[0] = -1
            case '63': runners[0] = -1
            case '64(1)3':
                runners[0] = -1
                runners[1] = -1
            case '9': runners[0] = -1
            case 'D7': runners[0] = 2
            case 'DGR': runners[0] = 2
            case 'E1': None
            case 'HP': runners[0] = 1
            case 'IW': runners[0] = 1
            case 'NP': None
            case 'PO1(13)': runners[1] = -1
            case 'S3': runners[0] = 1
            case 'S8': runners[0] = 1
            case 'S9': runners[0] = 1
            case 'SB2': runners[1] = 2
            case 'SB3': runners[2] = 3
            case 'T9': runners[0] = 3
            case 'W': runners[0] = 1
            case 'WP': None
            case  _ : print (r)

def GenSequence (start, pitches, events):
    OutsStart = start >> 14
    OutsOnPlay = 0
    BatterEvent = True
    EndGameFlag = False
    to = [start]
    runners = [0,
               1 if start & 0x0800 else 0,\
               2 if start & 0x1000 else 0,\
               3 if start & 0x2000 else 0]

    if ((to[-1] & (1<<10)) == 0): # no batter at home
        to.append (to[-1] | (1<<10))
    Strikes = 0
    Balls   = 0
    Fouls   = 0
    for p in pitches:
        Pitch = 1
        match p:
            case 'F' | 'L' | 'O' | 'R' | 'T':                                           Fouls  +=1
            case 'A' | 'C' | 'K' | 'M' | 'Q' | 'S':                                     Strikes+=1
            case 'B' | 'I' | 'P' | 'V':                                                 Balls  +=1
            case 'H' | 'N' | 'U' | '+' | '*' | '.' | '1' | '2' | '3' | '>' | 'X' | 'Y': Pitch   =0
            case _ : assert False

        if Pitch:
            assert Balls   <  8
            assert Fouls   < 32
            assert Strikes <  4
            to.append(to[-1] & 0xFC00)
            to[-1] |= (Balls   << 7)
            to[-1] |= (Fouls   << 2)
            to[-1] |= (Strikes << 0)
            if ((to[-1] & 0x3FF) > (to[-2] & 0x3FF)):
                to.append(to[-1] & 0xFC00)
            else: Pitch = 0
    ProcessEvent(runners, events)
    if not BatterEvent:
        if (Pitch): # take into account event on the pitch
            to.pop(-1)
        to[-1] = 1<<10
    else: to[-1] = 0
    to[-1] |= ((OutsStart+OutsOnPlay)<<14)
    to[-1] |= DEST(runners[0])
    to[-1] |= DEST(runners[1])
    to[-1] |= DEST(runners[2])
    to[-1] |= DEST(runners[3])
    to[-1] |= (Balls   << 7)
    to[-1] |= (Fouls   << 2)
    to[-1] |= (Strikes << 0)
    if BatterEvent:
        to[-1] &= 0xFC00
    if (EndGameFlag or
        (OutsStart+OutsOnPlay == 3)):
        to.append(0xFFFF)
    return to

# Pitches - Fouls 
# 'F': foul
# 'L': foul bunt
# 'O': foul tip on bunt
# 'R': foul ball on pitchout
# 'T': foul tip
# Pitches - Strikes
# 'A': automatic strike, usually for pitch timer violation
# 'C': called strike
# 'K': strike (unknown type)
# 'M': missed bunt attempt
# 'Q': swinging on pitchout
# 'S': swinging strike
# Pitches - Balls
# 'B': ball
# 'I': intentional ball
# 'P': pitchout
# 'V': called ball because pitcher went to his mouth or automatic ball on intentional walk or pitch timer violation
# Non Pitches
# 'H': hit batter
# 'N': no pitch (on balks and interference calls)
# 'U': unknown or missed pitch
# '+': following pickoff throw by the catcher
# '*': indicates the following pitch was blocked by the catcher
# '.': marker for play not involving the batter
# '1': pickoff throw to first
# '2': pickoff throw to second
# '3': pickoff throw to third
# '>': Indicates a runner going on the pitch
# In Play
# 'X': ball put into play by batter
# 'Y': ball put into play on pitchout