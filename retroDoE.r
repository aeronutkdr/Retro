#https://www.r-tutor.com/elementary-statistics/analysis-variance/factorial-design
setwd(dir='C:/Users/Kevin/source/repos/Retro')
tab <- read.delim(file='./pyEvRead/evRead.out', header=TRUE, sep=' ')
tab$IDHex <- paste('0x',as.hexmode(tab$ID), sep='')
strtoi(x=tab$IDHex, base=16)
tab$Out <- as.integer(tab$ID/strtoi('0x4000', base=16))
tab$B3 <- bitwAnd(tab$ID, strtoi(x <- '0x2000', base=16)) != 0
tab$B2 <- bitwAnd(tab$ID, strtoi(x <- '0x1000', base=16)) != 0
tab$B1 <- bitwAnd(tab$ID, strtoi(x <- '0x0800', base=16)) != 0
av <- aov(tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
lm(tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^2)

#https://www.r-tutor.com/elementary-statistics/analysis-variance/factorial-design
rv <- c(+
0.553,0.949,1.191,1.58,1.445,1.871,2.077,2.437,+
0.295,0.567,0.72,0.982,0.996,1.246,1.448,1.662,+
0.114,0.243,0.341,0.459,0.384,0.537,0.623,0.794)

fO = c("0 Out", "1 Out", "2 Out") # 1st factor levels 
f123 = c(0, 1)       # 2nd, 3rd, 4th factor levels 
kO = length(fO)          # number of 1st factors 
k123 = length(f123)      # number of 2nd, 3rd, 4th factors 
nn = 1
tm1 = gl(k123, 1, nn*kO*k123^3, factor(f123))
tm2 = gl(k123, k123, nn*kO*k123^3, factor(f123))
tm3 = gl(k123, k123^2, nn*kO*k123^3, factor(f123))
tmO = gl(kO, k123^3, nn*kO*k123^3, factor(fO))
av = aov(rv ~ tm1 * tm2 * tm3 * tmO)  # include interaction
av = aov(rv ~ tm1 + tm2 + tm3 + tmO)  # include interaction
summary(av)

df3 = read.csv("C:\\Users\\Kevin\\Downloads\\fastfood-3.csv")
r = c(t(as.matrix(df3))) # response data
f1 = c("Item1", "Item2", "Item3") # 1st factor levels 
f2 = c("East", "West")            # 2nd factor levels 
k1 = length(f1)          # number of 1st factors 
k2 = length(f2)          # number of 2nd factors 
n = 4                    # observations per treatment
tm1 = gl(k1, 1, n*k1*k2, factor(f1))
tm2 = gl(k2, n*k1, n*k1*k2, factor(f2))
av = aov(r ~ tm1 * tm2)  # include interaction
summary(av)