rm(list=ls())
path <- 'C:/Users/Kevin/source/repos/Retro/RetroParseEvent/'
#file <- 'test_Rdata_2018.txt'
file <- 'test_Rdata_2021.txt'
dat <- matrix(data=scan(file=paste0(path, file), sep=","), ncol=65, byrow=TRUE)
mat <- dat[,1:64]
res <- dat[,65]
s <- solve(mat, res)
s[2*(1:32)]