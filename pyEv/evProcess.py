import re
import sys
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

def strval (s: str, v: int) -> str:
    return s + str(v)

def ProcessEvent (runners : list[int], event : str, flags : dict) -> int:
    #
    #p2 = re.split ('/', event)
    #p3 = re.split ('\\.', event)
    #p1 = p2[0] if len(p2[0]) < len(p3[0]) else p3[0]
    #advs = [] if len(p3)==1 else re.split(';', p3[1])
    #res = re.split(';', p1) + advs
    #event == "SB2.1-3(E2/TH)":
    #res = re.split('[/\\.;]', event) # break at charcters: "/.;"
    # https://regex101.com/
    #res = re.match('^([^\/\.]*)(?:\/?([^\.]*))?(?:\.(.*))?$', event)
    res = re.match('^([^/.]*)(?:/?([^.]*))?(?:.(.*))?$', event)
    advs = [res.group(1)]
    if (res.group(2) != None): advs += re.split('/', res.group(2))
    if (res.group(3) != None): advs += re.split(';', res.group(3))
    Outs = 1 +\
           (1 if runners[1]>0 else 0) +\
           (1 if runners[2]>0 else 0) +\
           (1 if runners[3]>0 else 0)
    assist = 0
    putout = 0
    for r in advs:
        match r:
            case None: None
            case ''  : None
            case '1-2':     runners[1] =  2
            case '1-3':     runners[1] =  3
            case '1-3(E2/TH)': runners[1] =  3; flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 2; flags['error' + str(flags['numerrors']) + 'type'] = 'T' #flags['eventtype'] = 18; flags['fieldedby'] = 2
            case '1-H':     runners[1] =  4
            case '1-H(UR)': runners[1] =  5
            case '13':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 1; flags['playonbatter'] = '13'; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 1
            case '2-3':     runners[2] =  3
            case '2-H':     runners[2] =  4
            case '3':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '3'; putout += 1; flags[strval('putout', putout)] = 3
            case '3-H':     runners[3] =  4
            case '3-H(UR)': runners[3] =  5
            case '31':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonbatter'] = '31'; putout += 1; flags[strval('putout', putout)] = 1; assist += 1; flags[strval('assist', assist)] = 3
            case '36(1)':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 3; flags['playonrunner1'] = '36'; putout += 1; flags[strval('putout', putout)] = 6; assist += 1; flags[strval('assist', assist)] = 3
            case '4':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '4'; putout += 1; flags[strval('putout', putout)] = 4
            case '4(1)3':   runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'; flags['playonrunner1'] = '4'; putout += 1; flags[strval('putout', putout)] = 4; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 4
            case '43':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonbatter'] = '43'; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 4
            case '46(1)':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '46'; putout += 1; flags[strval('putout', putout)] = 6; assist += 1; flags[strval('assist', assist)] = 4;
            case '486(1)':  runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 4; flags['playonrunner1'] = '486'; putout += 1; flags[strval('putout', putout)] = 6; assist += 1; flags[strval('assist', assist)] = 4; assist += 1; flags[strval('assist', assist)] = 8
            case '5':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '5'; putout += 1; flags[strval('putout', putout)] = 5
            case '53':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonbatter'] = '53'; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 5
            case '54(1)':   runners[1] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 5; flags['playonrunner1'] = '54'; putout += 1; flags[strval('putout', putout)] = 4; assist += 1; flags[strval('assist', assist)] = 5
            case '6':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '6'; putout += 1; flags[strval('putout', putout)] = 6
            case '63':      runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '63'; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 6
            case '64(1)':  runners[1] = -1; flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonrunner1'] = '64'; putout += 1; flags[strval('putout', putout)] = 4; assist += 1; flags[strval('assist', assist)] = 6
            case '64(1)3':  runners[0:2] = [-1, -1]; flags['eventtype'] =  2; flags['fieldedby'] = 6; flags['playonbatter'] = '43'; flags['playonrunner1'] = '64'; putout += 1; flags[strval('putout', putout)] = 4; assist += 1; flags[strval('assist', assist)] = 6; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 4
            case '7':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 7; flags['playonbatter'] = '7'; putout += 1; flags[strval('putout', putout)] = 7
            case '8':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 8; flags['playonbatter'] = '8'; putout += 1;flags[strval('putout', putout)] = 8
            case '9':       runners[0] = -1;         flags['eventtype'] =  2; flags['fieldedby'] = 9; flags['playonbatter'] = '9'; putout+=1; flags[strval('putout', putout)] = 9
            case 'B-1':     runners[0] =  1
            case 'CSH(242536)': runners[3] = -1;         flags['eventtype'] =  6; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner3'] = '242536'; flags['csrunner3'] = True; putout+=1; flags[strval('putout', putout)] = 6; assist += 1; flags[strval('assist', assist)] = 2; assist += 1; flags[strval('assist', assist)] = 4; assist += 1; flags[strval('assist', assist)] = 5; assist += 1; flags[strval('assist', assist)] = 3
            case 'D7':      runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 7;                           flags['hitvalue'] = 2
            case 'D8':      runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 8;                           flags['hitvalue'] = 2
            case 'D9':      runners[0] =  2;         flags['eventtype'] = 21; flags['fieldedby'] = 9;                           flags['hitvalue'] = 2
            case 'DGR':     runners[0] =  2;         flags['eventtype'] = 21;                                                   flags['hitvalue'] = 2
            case 'E1':      flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 1; flags['eventtype'] = 18; flags['fieldedby'] = 1; flags['error' + str(flags['numerrors']) + 'type'] = 'F'
            case 'E6':      flags['numerrors'] += 1; flags['error' + str(flags['numerrors']) + 'player'] = 6; flags['eventtype'] = 18; flags['fieldedby'] = 6; flags['error' + str(flags['numerrors']) + 'type'] = 'T'
            case 'HP':      runners[0] =  1;         flags['eventtype'] = 16;                               flags['ab'] = False; flags['responsible'] = False
            case 'HR':      runners[0] =  4;         flags['eventtype'] = 23;                                                   flags['hitvalue'] = 4
            case 'IW':      runners[0] =  1;         flags['eventtype'] = 15;                               flags['ab'] = False
            case 'K':       runners[0] = -1;         flags['eventtype'] =  3; flags['playonbatter'] = '2'; putout+=1; flags[strval('putout', putout)] = 2
            case 'PO1(13)': runners[1] = -1;         flags['eventtype'] =  8; flags['batterevent'] = False; flags['ab'] = False; flags['playonrunner1'] = '13'; flags['porunner1'] = True; putout += 1; flags[strval('putout', putout)] = 3; assist += 1; flags[strval('assist', assist)] = 1
            case 'S1':     runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
            case 'S16':     runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 1;                           flags['hitvalue'] = 1
            case 'S3':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 3;                           flags['hitvalue'] = 1
            case 'S7':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 7;                           flags['hitvalue'] = 1
            case 'S8':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 8;                           flags['hitvalue'] = 1
            case 'S9':      runners[0] =  1;         flags['eventtype'] = 20; flags['fieldedby'] = 9;                           flags['hitvalue'] = 1
            case 'SB2':     runners[1] =  2;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner1'] = True
            case 'SB3':     runners[2] =  3;         flags['eventtype'] =  4; flags['batterevent'] = False; flags['ab'] = False; flags['sbrunner2'] = True
            case 'T9':      runners[0] =  3;         flags['eventtype'] = 22; flags['fieldedby'] = 9;                           flags['hitvalue'] = 3
            case 'W':       runners[0] =  1;         flags['eventtype'] = 14;                               flags['ab'] = False
            case 'WP':                               flags['eventtype'] =  9; flags['batterevent'] = False; flags['ab'] = False; flags['wildpitch']=True
            case 'BG13':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '13'; flags['bunt'] = True
            case 'BG23':    flags['battedballtype'] = 'G'; flags['hitlocation'] = '23'; flags['bunt'] = True
            case 'F3D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '3D'
            case 'F6D':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '6D'
            case 'F78S':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '78S'
            case 'F78XD':        flags['battedballtype'] = 'F'; flags['hitlocation'] = '78XD'
            case 'F7D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7D'
            case 'F7LD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7LD'
            case 'F7S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '7S'
            case 'F8':   flags['battedballtype'] = 'F'; flags['hitlocation'] = '8'
            case 'F89':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '89'
            case 'F89S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '89S'
            case 'F89XD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '89XD'
            case 'F8S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '8S'
            case 'F8XD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '8XD'
            case 'F9D':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9D'
            case 'F9L':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9L'
            case 'F9LD':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LD'
            case 'F9LD+':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LD'
            case 'F9LS':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9LS'
            case 'F9S':       flags['battedballtype'] = 'F'; flags['hitlocation'] = '9S'
            case 'FO':       flags['battedballtype'] = 'F'
            case 'G1S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
            case 'G1S-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '1S'
            case 'G23-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '23'
            case 'G3+':     flags['battedballtype'] = 'G'; flags['hitlocation'] = '3'
            case 'G34':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
            case 'G34+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34'
            case 'G34S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '34S'
            case 'G3S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '3S'
            case 'G4':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '4'
            case 'G4M':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '4M'
            case 'G4M+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '4M'
            case 'G5':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
            case 'G5+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '5'
            case 'G56':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '56'
            case 'G56+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '56'
            case 'G56S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '56S'
            case 'G6':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
            case 'G6+':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6'
            case 'G6M':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6M'
            case 'G6M-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6M'
            case 'G6MS-':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6MS'
            case 'G6S':      flags['battedballtype'] = 'G'; flags['hitlocation'] = '6S'
            case 'GDP':      flags['battedballtype'] = 'G'; flags['doubleplay'] = True
            case 'L4':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '4'
            case 'L7':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7'
            case 'L7+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7'
            case 'L78':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '78'
            case 'L78D+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '78D'
            case 'L78XD+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '78XD'
            case 'L7D':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
            case 'L7D+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '7D'
            case 'L8':      flags['battedballtype'] = 'L'; flags['hitlocation'] = '8'
            case 'L8XD+':        flags['battedballtype'] = 'L'; flags['hitlocation'] = '8XD'
            case 'L89':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '89'
            case 'L89XD+':        flags['battedballtype'] = 'L'; flags['hitlocation'] = '89XD'
            case 'L8D':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '8D'
            case 'L8S':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '8S'
            case 'L9':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '9'
            case 'L9L':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
            case 'L9L+':       flags['battedballtype'] = 'L'; flags['hitlocation'] = '9L'
            case 'NP':      None
            case 'P3':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '3'
            case 'P34D':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '34D'
            case 'P56':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '56'
            case 'P56S':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '56S'
            case 'P6D':        flags['battedballtype'] = 'P'; flags['hitlocation'] = '6D'
            case  _ : print (r, file=sys.stderr)
    Outs -= (1 if runners[0]>-1 else 0) +\
            (1 if runners[1]>-1 else 0) +\
            (1 if runners[2]>-1 else 0) +\
            (1 if runners[3]>-1 else 0)
    flags['rbi'] = (1 if runners[0]>3 else 0) +\
                   (1 if runners[1]>3 else 0) +\
                   (1 if runners[2]>3 else 0) +\
                   (1 if runners[3]>3 else 0)
    return Outs

def GenSequence (Outs : int, runners : list[int], pitches : str, event : str, flags : dict) -> list[int]:
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
    if event == "HR/F78XD.1-H":
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