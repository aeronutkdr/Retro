rm(list=ls())
#
Reorder <- function(m) {
	order <- array (data = c(0x2, 0x6, 0x1, 0xA,
				       0x5, 0x0, 0x4, 0x9,
					 0xE, 0x8, 0xD, 0xC))
	out <- matrix(nrow=dim(m)[1], ncol=12)
	for (i in 1:dim(out)[1]) {
		for (j in 1:12) {
			out[i,j] = m[i,order[j]+1]
		}
	}
	out
}
#
Unzero <- function(m) {
    for (i in 1:dim(m)[1]) {
        found <- FALSE
        for (j in 1:dim(m)[2]) {
            found = found || m[i,j] != 0
        }
        if (!found) {
#           print (i)
            m[i,i] = -1
        }
    }
    m
}
#
Calculate <- function(sq, run, mask) {
    x <- Adjust(0x3FF, mask)
    #print(x)
    bits <- 0
    while (x>0)
    {
        bits = bits + bitwAnd(x,1)
        x <- bitwShiftR(x,1)
    }
    #print(bits)
    if (bits > 0)
    {
        bits = 10-bits
        square_adj <- matrix (data = 0,
                              nrow=bitwShiftR(1024,bits),
                              ncol=bitwShiftR(1024,bits))
        runs_adj   <- matrix (data = 0,
                              nrow=bitwShiftR(1024,bits),
                              ncol=1)
	  #print (dim(square_adj))
        for (i in 0:1023) {
            i_idx <- 1+Adjust(i, mask)
            #print(i_idx)
            for (j in 0:1023) {
                j_idx <- 1+Adjust(j, mask)
                #print(j_idx)
                square_adj[i_idx, j_idx] = square_adj[i_idx, j_idx] + sq[1+i,1+j]
            }
            runs_adj[i_idx,1] = runs_adj[i_idx,1] + run[1+i,2]
        }
        square_adj <- Unzero(square_adj)
        #print(runs_adj)
        solution <- solve(a=square_adj,b=runs_adj)
    }
    solution
}
#
Adjust <- function(v, m) {
    t <- 0
    for (i in 1:dim(m)[1]) {
        tt <- bitwAnd(v,m[i,1])
        tt <- bitwShiftR(tt,m[i,2])
        t <- t + tt
    }
    t
}
#
CalcPart <- function(sq, run, mask) {
    x <- Adjust(0x3FF, mask)
    #print(x)
    bits <- 0
    while (x>0)
    {
        bits = bits + bitwAnd(x,1)
        x <- bitwShiftR(x,1)
    }
    #print(bits)
    if (bits > 0)
    {
        bits = 10-bits
        square_adj <- matrix (data = 0,
                              nrow=bitwShiftR(1024,bits),
                              ncol=bitwShiftR(1024,bits))
        runs_adj   <- matrix (data = 0,
                              nrow=bitwShiftR(1024,bits),
                              ncol=1)
	  #print (dim(square_adj))
        for (i in 0:1023) {
            i_idx <- 1+Adjust(i, mask)
            #print(i_idx)
            for (j in 0:1023) {
                j_idx <- 1+Adjust(j, mask)
                #print(j_idx)
                square_adj[i_idx, j_idx] = square_adj[i_idx, j_idx] + sq[1+i,1+j]
            }
            runs_adj[i_idx,1] = runs_adj[i_idx,1] + run[1+i,2]
        }
        square_adj <- Unzero(square_adj)
        matrix(c(square_adj,runs_adj),ncol=dim(square_adj)[1]+1)
        #print(runs_adj)
        #solution <- solve(a=square_adj,b=runs_adj)
    }
    #solution
}
#setwd('C:/Users/10032877/Documents/Visual Studio 2012/Projects/RetroEventMaestro')
#setwd('C:/Users/Kevin/Documents/Visual Studio 2008/Projects/Retro/RetroEventMaestro')
setwd('C:\\Users\\Kevin\\source\\repos\\Retro\\RetroEventMaestro')
raw <- matrix(data = scan (file = "2018.out", what = integer(), sep = ","), ncol=3, byrow=TRUE)
#raw <- matrix(data = scan (file = "2021.out", what = integer(), sep = ","), ncol=3, byrow=TRUE)
freq <- matrix(data = raw[1:(dim(raw)[1]-1024),], ncol=3)
runs <- matrix(data = raw[(dim(raw)[1]-1023):dim(raw)[1],1:2], ncol=2)
square <- matrix (data = 0, nrow=1024,ncol=1024)
for (i in 1:dim(freq)[1]) {
    square[freq[i,1]+1,freq[i,2]+1] <- freq[i,3]
}
#OO321HCCCC <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3FF, 0), byrow=TRUE))
#	Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE))[1:48,]
#OO321CCCC  <- Calculate(square, runs, matrix(nrow=2, ncol=2, data=c(0x3EF, 1, 0x00F, 0), byrow=TRUE))
#	Reorder(matrix(OO321CCCC,ncol=16,byrow=TRUE))[1:24,]
#OO         <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x300, 8          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x300, 8          ), byrow=TRUE))
#OO321H     <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3F0, 4          ), byrow=TRUE))
#	matrix(OO321H    ,byrow=TRUE,ncol=16)
OO321      <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3E0, 5          ), byrow=TRUE))
	matrix(OO321     ,byrow=TRUE,ncol=8)
#these won't solve
#z321H      <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x0F0, 4          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x0F0, 4          ), byrow=TRUE))
#CCCC       <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x00F, 0          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x00F, 0          ), byrow=TRUE))
#z321HCCCC  <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x0FF, 0), byrow=TRUE))
#z321CCCC   <- Calculate(square, runs, matrix(nrow=2, ncol=2, data=c(0x0EF, 1, 0x00F, 0), byrow=TRUE))

# this looks like the best match to twitter:
#Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])
#Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,])
#Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])-Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,])
#(Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])+Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,]))/2
#zz <- file("ex.data", "w")
#write(runs, zz, 1024)
#write(square, zz, 1024)
#close(zz)
