rm(list=ls())
setwd(dir='C:/Users/10032877/Documents/gitHub/Retro/pyEvRead')
#https://www.r-tutor.com/elementary-statistics/analysis-variance/factorial-design
tab <- read.delim(file='evRead.out', header=TRUE, sep=' ')
tab$IDHex <- paste('0x',as.hexmode(x=tab$ID), sep='')
tab$Out <- as.integer(x = tab$ID/strtoi(x = '0x4000', base=16))
tab$B3 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x2000', base=16)) != 0
tab$B2 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x1000', base=16)) != 0
tab$B1 <- bitwAnd(a = tab$ID, b = strtoi(x = '0x0800', base=16)) != 0
tab$NVal <- tab$N * tab$Value
av <- aov(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = av)
mdl <- lm(formula = tab$Value ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
summary(object = mdl)
#avdev <- aov(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
#summary(object = avdev)
#mdldev <- lm(formula = tab$stdev.Transitions. ~ (tab$Out + tab$B3 + tab$B2 + tab$B1)^4)
#summary(object = mdldev)
#qicharts2::paretochart(av$coefficients)
#av$coefficients[2:5]
#av$effects[1:5]
par(mfrow=c(2,2),bg=rgb(1,1,0.8))
boxplot(formula = tab$Value ~ tab$Out)
boxplot(formula = tab$Value ~ tab$B1)
boxplot(formula = tab$Value ~ tab$B2)
boxplot(formula = tab$Value ~ tab$B3)
par(mfrow=c(1,1),bg=rgb(1,1,0.8))
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
