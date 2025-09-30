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

def ProcessEvent (runners : list[int], event : str, flags) -> int:
    #p2 = re.split ('/', event)
    #p3 = re.split ('\\.', event)
    #p1 = p2[0] if len(p2[0]) < len(p3[0]) else p3[0]
    #advs = [] if len(p3)==1 else re.split(';', p3[1])
    #res = re.split(';', p1) + advs
    res = re.split('[/\\.;]', event)
    Outs = 1 +\
           (1 if runners[1]>0 else 0) +\
           (1 if runners[2]>0 else 0) +\
           (1 if runners[3]>0 else 0)
    for r in res:
        match r:
            case 'K':       runners[0] = -1;         flags['eventtype'] =  3; flags['playonbatter'] = '2'
            case '3':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '3'
            case '7':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 7; flags['playonbatter'] = '7'
            case '8':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 8; flags['playonbatter'] = '8'
            case '31':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '31'
            case 'S16':     runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
            case 'S7':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 7;                           flags['hitvalue'] = 1
            case '36(1)':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonrunner1'] = '36'
            case 'B-1':     runners[0] =  1
            case '1-H':     runners[1] =  4
            case 'HR':      runners[0] =  4;         flags['eventtype'] = 23;                                                   flags['hitvalue'] = 4
            case '1-2':     runners[1] =  2
            case '1-3':     runners[1] =  3
            case '1-H(UR)': runners[1] =  5
            case '13':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 1; flags['playonbatter'] = '13'
            case '2-3':     runners[2] =  3
            case '2-H':     runners[2] =  4
            case '3-H':     runners[3] =  4
            case '3-H(UR)': runners[3] =  5
            case '4':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '4'
            case '43':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'
            case '5':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '5'
            case '53':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '53'
            case '54(1)':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner1'] = '54'
            case '6':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '6'
            case '63':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '63'
            case '64(1)3':  runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '43'; flags['playonrunner1'] = '64'
            case '9':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 9; flags['playonbatter'] = '9'
            case 'D7':      runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 7;                           flags['hitvalue'] = 2
            case 'DGR':     runners[0] =  2;         flags['eventtype'] = 21;                                                   flags['hitvalue'] = 2
            case 'E1':      flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 1; flags['eventtype'] = 18; flags['fieldedby'] = 1; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
            case 'HP':      runners[0] =  1;         flags['eventtype'] = 16;                               flags['ab'] = False; flags['responsible'] = False
            case 'IW':      runners[0] =  1;         flags['eventtype'] = 15;                               flags['ab'] = False
            case 'NP':      None
            case 'PO1(13)': runners[1] = -1;         flags['eventtype'] =  8; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner1'] = '13'; flags['porunner1'] = True
            case 'S3':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 3;                           flags['hitvalue'] = 1
            case 'S8':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 8;                           flags['hitvalue'] = 1
            case 'S9':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 9;                           flags['hitvalue'] = 1
            case 'SB2':     runners[1] =  2;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner1'] = True
            case 'SB3':     runners[2] =  3;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner2'] = True
            case 'T9':      runners[0] =  3;         flags['eventtype'] = 22; flags['fieldedby'] = 9;                           flags['hitvalue'] = 3
            case 'W':       runners[0] =  1;         flags['eventtype'] = 14;                               flags['ab'] = False
            case 'WP':      None;                    flags['eventtype'] =  9; flags['batterevent'] = False; flags['ab'] = False; flags['wildpitch']=True
            case 'G3+':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '3'
            case 'L8':      flags['battedballtype'] = 'L'; flags['hitlocation'] = '8'
            case 'BG13':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '13'; flags['bunt'] = True
            case 'F3D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '3D'
            case 'F6D':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '6D'
            case 'F78S':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '78S'
            case 'F78XD':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '78XD'
            case 'F7D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7D'
            case 'F7LD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7LD'
            case 'F7S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7S'
            case 'F89XD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '89XD'
            case 'F8S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '8S'
            case 'F8XD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '8XD'
            case 'F9D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9D'
            case 'F9LD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LD'
            case 'F9LS':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LS'
            case 'FO':       flags['battedballtype'] = 'F'
            case 'G1S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
            case 'G1S-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
            case 'G34':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
            case 'G34+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
            case 'G34S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34S'
            case 'G4M+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '4M'
            case 'G5':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
            case 'G5+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
            case 'G56':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '56'
            case 'G56S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '56S'
            case 'G6':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
            case 'G6MS-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6MS'
            case 'G6S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6S'
            case 'GDP':      flags['battedballtype'] = 'G'; flags['doubleplay'] = True
            case 'L4':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '4'
            case 'L7':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7'
            case 'L78':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '78'
            case 'L78XD+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '78XD'
            case 'L7D':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
            case 'L7D+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
            case 'L89':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '89'
            case 'L8D':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '8D'
            case 'L8S':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '8S'
            case 'L9L':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
            case 'L9L+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
            case 'P3':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '3'
            case 'P34D':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '34D'
            case 'P56S':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '56S'
            case 'P6D':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '6D'
            case  _ : print (r)
    Outs -= (1 if runners[0]>-1 else 0) +\
            (1 if runners[1]>-1 else 0) +\
            (1 if runners[2]>-1 else 0) +\
            (1 if runners[3]>-1 else 0)
    flags['rbi'] = (1 if runners[0]>3 else 0) +\
                   (1 if runners[1]>3 else 0) +\
                   (1 if runners[2]>3 else 0) +\
                   (1 if runners[3]>3 else 0)
    return Outs

def GenSequence (Outs : int, runners : list[int], pitches : str, event : str, flags) -> list[int]:
    to = [(Outs<<14) |\
          (0 if runners[3] == -1 else (1<<13)) |\
          (0 if runners[2] == -1 else (1<<12)) |\
          (0 if runners[1] == -1 else (1<<11)) |\
          (0 if runners[0] == -1 else (1<<10))]
    if ((to[-1] & (1<<10)) == 0): # no batter at home
        to.append (to[-1] | (1<<10))
    Strikes = 0
    Balls   = 0
    Fouls   = 0
    Pitch   = 0
    #if pitches == "TBBFX":
    if event == "43/G34":
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
    OutsOnPlay = ProcessEvent(runners, event, flags)
    flags['outsonplay'] = OutsOnPlay
    assert OutsOnPlay in range(0,4)
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
    if (Outs+OutsOnPlay == 3):
        to.append(0xFFFF)
        #flags['endgame'] = True
    return to
'''
Pitches - Fouls 
'F': foul
'L': foul bunt
'O': foul tip on bunt
'R': foul ball on pitchout
'T': foul tip
Pitches - Strikes
'A': automatic strike, usually for pitch timer violation
'C': called strike
'K': strike (unknown type)
'M': missed bunt attempt
'Q': swinging on pitchout
'S': swinging strike
Pitches - Balls
'B': ball
'I': intentional ball
'P': pitchout
'V': called ball because pitcher went to his mouth or automatic ball on intentional walk or pitch timer violation
Non Pitches
'H': hit batter
'N': no pitch (on balks and interference calls)
'U': unknown or missed pitch
'+': following pickoff throw by the catcher
'*': indicates the following pitch was blocked by the catcher
'.': marker for play not involving the batter
'1': pickoff throw to first
'2': pickoff throw to second
'3': pickoff throw to third
'>': Indicates a runner going on the pitch
In Play
'X': ball put into play by batter
'Y': ball put into play on pitchout
'''