@set YYYY=%1
@set EVPATH=Object
@set EV=evRead
@set ROOT=..\data
@set DATPATH=%ROOT%\%YYYY%eve
@set OUTPATH=%ROOT%
%EVPATH%\%EV%.exe %DATPATH% 1> %OUTPATH%\%EV%%YYYY%.stdout 2> %OUTPATH%\%EV%%YYYY%.stderr