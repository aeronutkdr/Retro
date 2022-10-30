#pragma once

namespace Retro
{
    public ref class FileEvent
    {
    public:
        FileEvent(void);
        FileEvent(System::String^ e);
        virtual System::String^ ToString(System::Void) override;
        System::Byte From (System::Void);
        System::Byte To   (System::Void);

        System::String^ m_GameName;
        System::String^ m_VisitTeam;
        System::Byte    m_Inning;
        System::Boolean m_HomeTeamBatting;
        System::Byte    m_Outs;
        System::String^ m_BatterStart;
        //System::String^ m_BatterRes;
        System::String^ m_Runner1;
        System::String^ m_Runner2;
        System::String^ m_Runner3;
        System::Byte    m_OutsOnPlay;
        System::Byte    m_BatterDest;
        System::Byte    m_Runner1Dest;
        System::Byte    m_Runner2Dest;
        System::Byte    m_Runner3Dest;
    protected:
        static array<System::Char>^splitChars = {','};
        static array<System::Char>^trimChars = {'\"'};
        System::Boolean ZeroOneToBoolean (System::String ^ str);
    };
}