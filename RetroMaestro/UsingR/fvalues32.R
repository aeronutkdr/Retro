# "32"  refers to the 32 end states (3rd, 2nd, 1st, 4Out)
fvalues32 <- function(f,g) {
    fm_in <- matrix(scan(f, integer(), sep="\t"), ncol=3, byrow=TRUE)
    end_in <- matrix(scan(g, integer(), sep="\t"), ncol=2, byrow=TRUE)
    fm <- matrix(0,32,32,TRUE)
# populate fm
for (i in 1:dim(fm_in)[1])
{
    fm[fm_in[i,1]/2+1,fm_in[i,2]/2+1]=
        fm[fm_in[i,1]/2+1,fm_in[i,2]/2+1]+
        fm_in[i,3]
}
# fix1: subtract the # of times in this state
for (i in 1:32) {
#fm[i,i] = fm[i,i]-colSums(fm)[i]
fm[i,i] = fm[i,i]-rowSums(fm)[i]
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