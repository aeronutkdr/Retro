fvalues <- function(f,g) {
    fm_in <- matrix(scan(f, integer(), sep="\t"), ncol=3, byrow=TRUE)
    #end_in <- matrix(scan(g, integer(), sep="\t"), ncol=2, byrow=TRUE)
    fm <- matrix(0,64,64,TRUE)
# populate fm
for (i in 1:dim(fm_in)[1])
{
    fm[fm_in[i,1]+1,fm_in[i,2]+1]=fm_in[i,3]
}
# fix1:
for (i in 1:32*2-1) {
fm[i,i]     = -colSums(fm)[i]
fm[i,i+1]   = fm[i,i+1] - fm[i,i]
fm[i+1,i+1] = fm[i+1,i+1] - rowSums(fm)[i+1]
}
for (i in 25:32*2) {
fm[i,i]     = -fm[i-1,i]
}
# fix2
#for (i in 1:24*2) {
#fm[i,i] = fm[i,i] - rowSums(fm)[i]
#}
# fix3
#for (i in 49:64) {
#fm[i,i] = -1
#}
# fix4
#for (i in 1:dim(end_in)[1])
#{
#end_in[i,1] <- end_in[i,1]+1
##fm[end_in[i,1],end_in[i,1]] <- fm[end_in[i,1],end_in[i,1]] + end_in[i,2]
#fm[end_in[i,1],end_in[i,1]+1] <- fm[end_in[i,1],end_in[i,1]+1] - end_in[i,2]
#}
return (fm)
}
