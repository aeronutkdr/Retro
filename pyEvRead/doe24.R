rm(list=ls())
setwd(dir='C:/Users/Kevin/source/repos/Retro/pyEvRead')
#https://www.r-tutor.com/elementary-statistics/analysis-variance/factorial-design
tab <- read.delim(file='evRead.out', header=TRUE, sep=' ')
tab$IDHex <- paste('0x',as.hexmode(x=tab$ID), sep='')
#strtoi(x=tab$IDHex, base=16)
tab$Out <- as.integer(x = tab$ID/strtoi(x = '0x4000', base=16))
tab$B3 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x2000', base=16)) != 0
tab$B2 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x1000', base=16)) != 0
tab$B1 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x0800', base=16)) != 0
#tab
av <- aov(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = av)
mdl <- lm(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = mdl)
avdev <- aov(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = avdev)
mdldev <- lm(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = mdldev)