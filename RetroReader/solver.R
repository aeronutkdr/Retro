rm(list=ls())
#
#Reorder <- function(m) {
#	order <- array (data = c(0x2, 0x6, 0x1, 0xA,
#				       0x5, 0x0, 0x4, 0x9,
#					 0xE, 0x8, 0xD, 0xC))
#	out <- matrix(nrow=dim(m)[1], ncol=12)
#	for (i in 1:dim(out)[1]) {
#		for (j in 1:12) {
#			out[i,j] = m[i,order[j]+1]
#		}
#	}
#	out
#}
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
#Calculate <- function(sq, run, mask) {
#    #print (NumRows)
#    x <- Adjust((NumRows-1), mask)
#   #print(x)
#    bits <- 0
#    while (x>0)
#    {
#        bits = bits + bitwAnd(x,1)
#        x <- bitwShiftR(x,1)
#    }
#    #print(bits)
#    if (bits > 0)
#    {
#        bits = NUMBITS-bits
#        square_adj <- matrix (data = 0,
#                              nrow=bitwShiftR(NumRows,bits),
#                              ncol=bitwShiftR(NumRows,bits))
#        runs_adj   <- matrix (data = 0,
#                              nrow=bitwShiftR(NumRows,bits),
#                              ncol=1)
#	  #print (dim(square_adj))
#	  #print (dim(sq))
#        #print (NumRows)
#        for (i in 0:(NumRows-1)) {
#            print(i)
#            i_idx <- 1+Adjust(i, mask)
#            #print(i_idx)
#            for (j in 0:(NumRows-1)) {
#                #print(j)
#                j_idx <- 1+Adjust(j, mask)
#                #print(j_idx)
#                #print (square_adj[i_idx, j_idx])
#		    #print (dim(sq))
#		    #print ("here")
#                #print (sq)
#		    #print (i)
#		    #print (j)
#                #print (sq[1+i,1+j])
#		    #print(i_idx)
#                #print(j_idx)
#                square_adj[i_idx, j_idx] = square_adj[i_idx, j_idx] + sq[1+i,1+j]
#            }
#            runs_adj[i_idx,1] = runs_adj[i_idx,1] + run[1+i,2]
#        }
#        square_adj <- Unzero(square_adj)
#        #print(runs_adj)
#       solution <- solve(a=square_adj,b=runs_adj)
#    }
#    solution
#}
#
#Adjust <- function(v, m) {
#    t <- 0
#    for (i in 1:dim(m)[1]) {
#        tt <- bitwAnd(v,m[i,1])
#        tt <- bitwShiftR(tt,m[i,2])
#        t <- t + tt
#    }
#    t
#}
#
#CalcPart <- function(sq, run, mask) {
#    x <- Adjust((NumRows-1), mask)
#    #print(x)
#    bits <- 0
#    while (x>0)
#    {
#        bits = bits + bitwAnd(x,1)
#        x <- bitwShiftR(x,1)
#    }
#    #print(bits)
#    if (bits > 0)
#    {
#        bits = NUMBITS-bits
#        square_adj <- matrix (data = 0,
#                              nrow=bitwShiftR(NumRows,bits),
#                              ncol=bitwShiftR(NumRows,bits))
#        runs_adj   <- matrix (data = 0,
#                              nrow=bitwShiftR(NumRows,bits),
#                              ncol=1)
#	  #print (dim(square_adj))
#        print (NumRows)
#        for (i in 0:(NumRows-1)) {
#            i_idx <- 1+Adjust(i, mask)
#            #print(i_idx)
#            for (j in 0:(NumRows-1)) {
#                j_idx <- 1+Adjust(j, mask)
#                #print(j_idx)
#                square_adj[i_idx, j_idx] = square_adj[i_idx, j_idx] + sq[1+i,1+j]
#            }
#            runs_adj[i_idx,1] = runs_adj[i_idx,1] + run[1+i,2]
#        }
#        square_adj <- Unzero(square_adj)
#        matrix(c(square_adj,runs_adj),ncol=dim(square_adj)[1]+1)
#        #print(runs_adj)
#        #solution <- solve(a=square_adj,b=runs_adj)
#    }
#    #solution
#}
setwd('C:\\Users\\Kevin\\source\\repos\\Retro\\RetroReader')
#setwd('C:/Users/10032877/Desktop/Reading/retrosheet')
#NUMBITS <- 14
#NumRows <- 2^NUMBITS
#raw <- matrix(data = scan (file = "tmp.txt", what = integer(), sep = ","), ncol=3, byrow=TRUE)
#freq <- matrix(data = raw[1:(dim(raw)[1]-NumRows),], ncol=3)
#runs <- matrix(data = raw[(dim(raw)[1]-NumRows+1):dim(raw)[1],1:2], ncol=2)
#square <- matrix (data = 0, nrow=NumRows,ncol=NumRows)
#for (i in 1:dim(freq)[1]) {
#    square[freq[i,1]+1,freq[i,2]+1] <- freq[i,3]
#}
#raw <- matrix(data = scan (file = "kdr.txt", what = integer(), sep = ","), ncol=3, byrow=TRUE); dim(raw)
#raw <- matrix(data = scan (file = "2000kdr.txt", what = integer(), sep = ","), ncol=3, byrow=TRUE); dim(raw)
#raw <- matrix(data = scan (file = "2018.txt", what = integer(), sep = ","), ncol=3, byrow=TRUE); dim(raw)
raw <- matrix(data = scan (file = "2021.txt", what = integer(), sep = ","), ncol=3, byrow=TRUE); dim(raw)
NumStates=raw[1,1]; NumStates
NUMBITS=ceiling(log(NumStates,2)); NUMBITS
runs <- matrix(data=0, nrow=2^NUMBITS, ncol=1); dim(runs)
for (i in 1:NumStates) {
    runs[i,1]=raw[1+i,2]
}; runs[1:10,1]
square <- matrix(data=0, nrow=2^NUMBITS, ncol=2^NUMBITS); dim(square)
for (i in (NumStates+2):dim(raw)[1]) {
    square[raw[i,1]+1,raw[i,2]+1] <- raw[i,3]
}; square[1,1:10]
rm(i)
NumRows=2^NUMBITS; NumRows
#dim(square)
#dim(runs)
#ABCdEFG   | Z  solve for D  (upper=+, lower=-)
#ABC EFG z | D
result <- solve(Unzero(square), -runs)[1:raw[1,1]]
resframe <- data.frame(state<-sprintf("%04X",raw[1+(1:raw[1,1])]),
                       value<-result,
                       intstate<-raw[1+(1:raw[1,1])],
                       balls<-bitwShiftR(bitwAnd(raw[1+(1:raw[1,1])],192),6),
                       strikes<-bitwAnd(raw[1+(1:raw[1,1])],63)); resframe[1:10,]
#                       row.names<-c("state","value","intstate","balls","strikes")); resframe[1:10,]
#resframe$state
#length(resframe$state)
#length(resframe)
plot.new()
plot.window(xlim<-c(0,13), ylim<-c(0.2,1.1),log="",par(xaxs="r"),par(yaxs="r"))
#plot.window(xlim<-c(0,17), ylim<-c(0.2,1.1),log="",par(xaxs="r"),par(yaxs="r"))
#plot.window(xlim<-c(0,15), ylim<-c(0.0,2),log="",par(xaxs="r"),par(yaxs="r"))
axis(1)
axis(2)
LAST=42
text (x<-(resframe$balls + resframe$strikes)[2:LAST],
      y<-resframe$value[2:LAST],
      labels<-sprintf("%d-%d %1.2f",
                      resframe$balls[2:LAST],
                      resframe$strikes[2:LAST],
                      resframe$value[2:LAST]))
#as.hex(resframe$state[LAST-1])
resframe$state[LAST-1]
resframe$state[LAST]
resframe$strikes[LAST]
resframe$state[LAST+1]
resframe$state[2]
#text (x<-(resframe$balls + resframe$strikes)[2:LAST],
#      y<-resframe$value[2:LAST],
#      labels<-sprintf("%d-%d",
#                      resframe$balls[2:LAST],
#                      resframe$strikes[2:LAST]))
for (i in 2:(LAST-1)) { #length(resframe$state-1)) {
   for (j in (i+1):LAST) { #length(resframe$state)) {
      if ((resframe$balls[i] == resframe$balls[j] &&
           resframe$strikes[i] + 1 == resframe$strikes[j]) ||
          (resframe$strikes[i] == resframe$strikes[j] &&
           resframe$balls[i] + 1 == resframe$balls[j])) {
#          plot (x <- c(resframe$balls[i] + resframe$strikes[i],
#                       resframe$balls[j] + resframe$strikes[j]),
#                y <- c(resframe$value[i], resframe$value[j]),
#                type <- "l")
#          lines (x <- c(resframe$balls[i] + resframe$strikes[i],
#                        resframe$balls[j] + resframe$strikes[j]),
#                 y <- c(resframe$value[i], resframe$value[j]))
          segments (x0 <- resframe$balls[i] + resframe$strikes[i],
                    y0 <- resframe$value[i],
                    x1 <- resframe$balls[j] + resframe$strikes[j],
                    y1 <- resframe$value[j])
      }
   }
}

#OO321HCCCCCCCC <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3FFF, 0), byrow=TRUE))
#OO321HCCxxxxCC <- Calculate(square, runs, matrix(nrow=2, ncol=2, data=c(0x3FC0, 4, 0x3, 0), byrow=TRUE))
#OO321Hxxxxxxxx <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3F00, 8), byrow=TRUE))
#	Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE))[1:48,]
#OO321CCCC  <- Calculate(square, runs, matrix(nrow=2, ncol=2, data=c(0x3E0, 1, 0x00F, 0), byrow=TRUE))
#	Reorder(matrix(OO321CCCC,ncol=16,byrow=TRUE))[1:24,]
#OO         <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x300, 8          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x300, 8          ), byrow=TRUE))
#OO321H     <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3F0, 4          ), byrow=TRUE))
#	matrix(OO321H    ,byrow=TRUE,ncol=16)
#OO321      <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x3E0, 5          ), byrow=TRUE))
#	matrix(OO321     ,byrow=TRUE,ncol=8)
#these won't solve
#z321H      <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x0F0, 4          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x0F0, 4          ), byrow=TRUE))
#CCCC       <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x00F, 0          ), byrow=TRUE))
#	CalcPart(square, runs, matrix(nrow=1, ncol=2, data=c(0x00F, 0          ), byrow=TRUE))
#z321HCCCC  <- Calculate(square, runs, matrix(nrow=1, ncol=2, data=c(0x0FF, 0), byrow=TRUE))
#z321CCCC   <- Calculate(square, runs, matrix(nrow=2, ncol=2, data=c(0x0E0, 1, 0x00F, 0), byrow=TRUE))

# this looks like the best match to twitter:
#Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])
#Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,])
#Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])-Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,])
#(Reorder(matrix(OO321HCCCC,ncol=16,byrow=TRUE)[1:24*2,])+Reorder(matrix(OO321CCCC ,ncol=16,byrow=TRUE)[1:24,]))/2
