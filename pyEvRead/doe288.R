rm(list=ls())
# __OO_321__BB__SS
# x(o,3,2,1,B,S) = 96o+48(3)+24(2)+12(1)+3B+S
# x(0,0,0,0,0,0) =  0
# x(0,0,0,0,0,1) =  1
# x(0,0,0,0,1,1) =  4
# x(0,0,0,0,2,1) =  7
# x(0,0,0,0,3,2) = 11
# x(2,1,1,1,3,2) = 192 + 48 + 24 + 12 + 9 + 2 = 287
GenK <- function(i) {
  Strikes = i %% 3
  Balls = (i%/%3) %% 4
  B1 = (i%/%12) %% 2
  B2 = (i%/%24) %% 2
  B3 = (i%/%48) %% 2
  Out = (i%/%96)
  K = c(1,             # (Intercept)
        Out,           # tab$Out
        B3,            # tab$B3TRUE 
        B2,            # tab$B2TRUE
        B1,            # tab$B1TRUE
        Balls,         # tab$Balls
        Strikes,       # tab$Strikes
        Out*B3,        # tab$Out:tab$B3TRUE
        Out*B2,        # tab$Out:tab$B2TRUE 
        Out*B1,        # tab$Out:tab$B1TRUE
        Out*Balls,     # tab$Out:tab$Balls
        Out*Strikes,   # tab$Out:tab$Strikes
        B3*B2,         # tab$B3TRUE:tab$B2TRUE
        B3*B1,         # tab$B3TRUE:tab$B1TRUE
        B3*Balls,      # tab$B3TRUE:tab$Balls
        B3*Strikes,    # tab$B3TRUE:tab$Strikes
        B2*B1,         # tab$B2TRUE:tab$B1TRUE
        B2*Balls,      # tab$B2TRUE:tab$Balls
        B2*Strikes,    # tab$B2TRUE:tab$Strikes
        B1*Balls,      # tab$B1TRUE:tab$Balls
        B2*Strikes,    # tab$B1TRUE:tab$Strikes
        Balls*Strikes) # tab$Balls:tab$Strikes
  return (K)
}
#setwd(dir='C:/Users/10032877/Documents/gitHub/Retro/pyEvRead')
setwd(dir='C:/Users/Kevin/source/repos/Retro/pyEvRead')
#https://www.r-tutor.com/elementary-statistics/analysis-variance/factorial-design
tab <- read.delim(file='evRead288.out', header=TRUE, sep=' ')
tab$IDHex <- paste('0x',as.hexmode(x=tab$ID), sep='')
tab$Out <- as.integer(x = tab$ID/strtoi(x = '0x1000', base=16))
tab$B3 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x0400', base=16)) != 0
tab$B2 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x0200', base=16)) != 0
tab$B1 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x0100', base=16)) != 0
tab$Balls <- bitwAnd(a = tab$ID, b = strtoi(x = '0x00F0', base=16)) / 16
tab$Strikes <- bitwAnd(a = tab$ID, b = strtoi(x = '0x000F', base=16))
tab$NVal <- tab$N * tab$Value
#av <- aov(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1 + tab$Balls + tab$Strikes))
av <- aov(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1 + tab$Balls + tab$Strikes)^2)
summary(object = av)
sum(GenK(50)*av$coefficients)
#av$coefficients
#mdl <- lm(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1 + tab$Balls + tab$Strikes))
#mdl <- lm(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1 + tab$Balls + tab$Strikes)^1)
#summary(object = mdl)
#avdev <- aov(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
#summary(object = avdev)
#mdldev <- lm(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
#summary(object = mdldev)
#qicharts2::paretochart(av$coefficients)
#av$coefficients[2:5]
#av$effects[1:5]
par(mfrow=c(2,3),bg=rgb(1,1,0.8))
boxplot(formula = tab$Value ~ tab$Out)
boxplot(formula = tab$Value ~ tab$B1)
boxplot(formula = tab$Value ~ tab$B2)
boxplot(formula = tab$Value ~ tab$B3)
boxplot(formula = tab$Value ~ tab$Balls)
boxplot(formula = tab$Value ~ tab$Strikes)
par(mfrow=c(1,1),bg=rgb(1,1,0.8))
plot(av$residuals)
par(mfrow=c(1,1),bg=rgb(1,1,0.8))
plot(av$fitted.values)
points(tab$Value)
#hist(av$residuals, main="Histogram", xlab="Residual")
v <- rep (tab$Value, round(tab$N/min(tab$N)))
# to match stdev:
#v <- c(rep (tab$Value-tab$stdev.Transitions., round(tab$N/min(tab$N))/2),
#       rep (tab$Value+tab$stdev.Transitions., round(tab$N/min(tab$N))/2))
hist(v)
qicharts2::paretochart(v)
df <- data.frame(tab$Out, tab$B3, tab$B2, tab$B1, tab$Value)
ggplot2::ggplot(data = df, mapping = ggplot2::aes(x = tab.Out, y=tab.Value, color=tab.Out, group=tab.Out)) +
  ggplot2::geom_line() +
  ggplot2::geom_point() +
  ggplot2::labs(title = "Interaction Plot", x = "Out", y = "Value")
ggplot2::ggplot(data = df, mapping = ggplot2::aes(x = tab.B1, y=tab.Value, color=tab.B1, group=tab.B1)) +
  ggplot2::geom_line() +
  ggplot2::geom_point() +
  ggplot2::labs(title = "Interaction Plot", x = "Out", y = "Value")
ggplot2::ggplot(data = df, mapping = ggplot2::aes(x = tab.B2, y=tab.Value, color=tab.B2, group=tab.B2)) +
  ggplot2::geom_line() +
  ggplot2::geom_point() +
  ggplot2::labs(title = "Interaction Plot", x = "Out", y = "Value")
ggplot2::ggplot(data = df, mapping = ggplot2::aes(x = tab.B3, y=tab.Value, color=tab.B3, group=tab.B3)) +
  ggplot2::geom_line() +
  ggplot2::geom_point() +
  ggplot2::labs(title = "Interaction Plot", x = "Out", y = "Value")