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
def DEST(x: int) -> int:
    return (1<<(10+(x&3))) if (x>0 and x<4) else 0

def ProcessEvent (runners : list[int], event : str) -> int:
    p2 = re.split ('/', event)
    p3 = re.split ('\\.', event)
    p1 = p2[0] if len(p2[0]) < len(p3[0]) else p3[0]
    advs = [] if len(p3)==1 else re.split(';', p3[1])
    res = re.split(';', p1) + advs
    Outs = 1 +\
           (1 if runners[1]>0 else 0) +\
           (1 if runners[2]>0 else 0) +\
           (1 if runners[3]>0 else 0)
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
            case '64(1)3': runners[0:2] = [-1, -1]
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
    Outs -= (1 if runners[0]>-1 else 0) +\
            (1 if runners[1]>-1 else 0) +\
            (1 if runners[2]>-1 else 0) +\
            (1 if runners[3]>-1 else 0)
    return Outs

def GenSequence (Outs : int, Bases : list[str], Bs : list[int], pitches : str, event : str) -> list[int]:
    to = [(Outs<<14) |\
          (0 if Bs[3][0] == -1 else (1<<13)) |\
          (0 if Bs[2][0] == -1 else (1<<12)) |\
          (0 if Bs[1][0] == -1 else (1<<11)) |\
          (0 if Bs[0][0] == -1 else (1<<10))]
    runners = [Bs[0][0],\
               Bs[1][0],\
               Bs[2][0],\
               Bs[3][0]]
    '''
    to = [(Outs<<14) |\
          (0 if Bases[3] == "" else (1<<13)) |\
          (0 if Bases[2] == "" else (1<<12)) |\
          (0 if Bases[1] == "" else (1<<11)) |\
          (0 if Bases[0] == "" else (1<<10))]
    runners = [(-1 if Bases[0] == "" else 0),\
               (-1 if Bases[1] == "" else 1),\
               (-1 if Bases[2] == "" else 2),\
               (-1 if Bases[3] == "" else 3)]
    '''
    if ((to[-1] & (1<<10)) == 0): # no batter at home
        to.append (to[-1] | (1<<10))
    Strikes = 0
    Balls   = 0
    Fouls   = 0
    Pitch   = 0
    if pitches == "BCBFH":
        None
    for p in pitches:
        Pitch = 1
        match p:
            case 'F' | 'L' | 'O' | 'R' | 'T':                                     Fouls  +=1
            case 'A' | 'C' | 'K' | 'M' | 'Q' | 'S':                               Strikes+=1
            case 'B' | 'I' | 'P' | 'V':                                           Balls  +=1
            case 'H' | 'N' | 'U' | '+' | '*' | '1' | '2' | '3' | '>' | 'X' | 'Y': Pitch   =0
            case '.':                                                             Pitch   =0; to=[to[-1]]
            case _ : assert False

        if Pitch:
            assert Balls   <  8
            assert Fouls   < 32
            assert Strikes <  4
            to.append(to[-1] & 0xFC00)
            to[-1] |= (Balls   << 7)
            to[-1] |= (Fouls   << 2)
            to[-1] |= (Strikes << 0)
            #if ((to[-1] & 0x3FF) > (to[-2] & 0x3FF)):
                #to.append(to[-1] & 0xFC00)
            #else: Pitch = 0
    OutsOnPlay = ProcessEvent(runners, event)
    for i in (range(len(runners))):
        Bs[i][1] = runners[i]
    '''
    for i in reversed(range(len(runners))):
        if runners[i]!=i:
            if runners[i] in range(4):
                Bases[runners[i]] = Bases[i]
            Bases[i] = ""
    '''
    BatterEvent = runners[0] != 0
    if not BatterEvent:
        if (Pitch): # take into account event on the pitch
            to.pop(-1)
        to[-1] = 1<<10
    else: to.append(0)
    to[-1] |= ((Outs+OutsOnPlay)<<14)
    to[-1] |= DEST(runners[0])
    to[-1] |= DEST(runners[1])
    to[-1] |= DEST(runners[2])
    to[-1] |= DEST(runners[3])
    if (not BatterEvent):
        to[-1] |= (Balls   << 7)
        to[-1] |= (Fouls   << 2)
        to[-1] |= (Strikes << 0)
    EndGameFlag = False
    if (EndGameFlag or
        (Outs+OutsOnPlay == 3)):
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