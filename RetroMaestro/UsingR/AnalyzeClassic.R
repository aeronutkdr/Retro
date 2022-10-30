# build PlayerUseArray
PlayerUseArray <- array(c(0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4))
# build PlayerUseArray for all number of Outs
AllPlayerUseArray <- array(c(PlayerUseArray,PlayerUseArray+1,PlayerUseArray+2,PlayerUseArray+3))
# build RunMatrix as outer difference of AllPlayerUseArray
RunMatrix = outer (AllPlayerUseArray, AllPlayerUseArray, "-")
RunMatrix[1:32*2-1,1:32*2] <- RunMatrix[((1:32)*2)-1,1:32*2]+1
source('fvalues.R')
setwd('C:/ProgramData/MySQL/MySQL Server 5.7/Uploads')
freqMatrix <- fvalues('2017Output.txt', '2017EndInnings.txt')
solve(freqMatrix,-rowSums(freqMatrix*cRunMatrix))
