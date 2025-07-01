#ifndef _EVPLAYEVENT_H_
#define _EVPLAYEVENT_H_

enum evPlayEventType
{
    //evUnknownEvent,
    evNoEvent,
    //evGenericOut,
    evStrikeout,
    evStolenBase,
    evDefensiveIndifference,
    evCaughtStealing,
    //evPickoffError,
    evPickoff,
    evWildPitch,
    evPassedBall,
    evBalk,
    evOtherAdvance,
    evFoulError,
    evWalk,
    evIntentionalWalk,
    evHitByPitch,
    evInterference,
    //evError,
    evFieldersChoice,
    evSingle,
    evDouble,
    evTriple,
    evHomeRun,
    //evMissingPlay,
    //evUnassist,
    //evAssisted,
    //evLinedDoublePlay,
    //evGroundDoublePlay,
    evGroundRuleDouble,
    evPickoffCaughtStealing,
    evNumEvents
};

#define evErrorInvolved (0x80)
struct evParseType
{
    unsigned char        mNumEv;
    enum evPlayEventType mEv[2];
    unsigned char        mNumRes;
    struct evResult
    {
        unsigned char mFrom;
        unsigned char mTo;
        unsigned char mNumVia;
        unsigned char mVia[9];
    } mResult[4];
    //unsigned char        mBaseRes [ 4]; /* B123 */
    //unsigned char        mInvolved[10]; /* [9] is always 0 */
};
char* evParse (char* str, struct evParseType *p);
char* evParseEvent (char* str, enum evPlayEventType* ev);
char* evString (enum evPlayEventType ev);
void evDump(struct evParseType *p);

#endif /* _EVPLAYEVENT_H_ */