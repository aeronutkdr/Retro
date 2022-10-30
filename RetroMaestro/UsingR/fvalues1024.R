# "1024" refers to the 1024 discrete states 3, 2, 1, H, 4B, 4S, 4Out
# g seems to be unused
fvalues1024 <- function(f,g) {
    fm_in <- matrix(scan(f, integer(), sep="\t"), ncol=3, byrow=TRUE)
#   end_in <- matrix(scan(g, integer(), sep="\t"), ncol=2, byrow=TRUE)
    fm <- matrix(0,1024,1024,TRUE)
# populate fm
for (i in 1:dim(fm_in)[1])
{
#    count1 <- fm_in[i,1] %% 16
#    count2 <- fm_in[i,2] %% 16
#    c1 <- 16*as.integer(fm_in[i,1]/32) + count1
#    c2 <- 16*as.integer(fm_in[i,2]/32) + count2
    fm[fm_in[i,1], fm_in[i,2]] = fm[fm_in[i,1], fm_in[i,2]] + fm_in[i,3]
}
# fix1: subtract the # of times in this state
for (i in 1:1024) {
#fm[i,i] = fm[i,i]-colSums(fm)[i]
fm[i,i] = fm[i,i]-rowSums(fm)[i]
if (fm[i,i] == 0) { fm[i,i] = 1 }
#fm[i,i+1] = fm[i,i+1] - fm[i,i]
}
## fix2
#for (i in 1:24*2) {
#fm[i,i] = fm[i,i] - rowSums(fm)[i]
#}
## fix3
#for (i in 49:64) {
#fm[i,i] = -1
#}
## fix4
#for (i in 1:dim(end_in)[1])
#{
#end_in[i,1] <- end_in[i,1]+1
##fm[end_in[i,1],end_in[i,1]] <- fm[end_in[i,1],end_in[i,1]] + end_in[i,2]
##fm[end_in[i,1],end_in[i,1]+1] <- fm[end_in[i,1],end_in[i,1]+1] - end_in[i,2]
#}
return (fm)
}
