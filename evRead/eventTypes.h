#ifndef EVENTTYPES_H
#define EVENTTYPES_H
enum EventType
{
    ET_UnknownEvent          =  0,
    ET_NoEvent               =  1,
    ET_Genericout            =  2,
    ET_Strikeout             =  3,
    ET_StolenBase            =  4,
    ET_DefensiveIndifference =  5,
    ET_Caughtstealing        =  6,
    ET_Pickofferror          =  7,
    ET_Pickoff               =  8,
    ET_WildPitch             =  9,
    ET_PassedBall            = 10,
    ET_Balk                  = 11,
    ET_OtherAdvance          = 12,
    ET_FoulError             = 13,
    ET_Walk                  = 14,
    ET_IntentionalWalk       = 15,
    ET_HitbyPitch            = 16,
    ET_Interference          = 17,
    ET_Error                 = 18,
    ET_FieldersChoice        = 19,
    ET_Single                = 20,
    ET_Double                = 21,
    ET_Triple                = 22,
    ET_Homerun               = 23,
    ET_MissingPlay           = 24
};

#endif /* EVENTTYPES_H */