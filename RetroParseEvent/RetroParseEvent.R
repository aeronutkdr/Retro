rm(list=ls())
#dat <- matrix(data=scan(file='C:/Users/Kevin/source/repos/Retro/RetroParseEvent/test_Rdata_2018.txt', sep=","), ncol=65, byrow=TRUE)
dat <- matrix(data=scan(file='C:/Users/Kevin/source/repos/Retro/RetroParseEvent/test_Rdata_2021.txt', sep=","), ncol=65, byrow=TRUE)
mat <- dat[,1:64]
res <- dat[,65]
s <- solve(mat, res)
s