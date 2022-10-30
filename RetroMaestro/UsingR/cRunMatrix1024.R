#setwd('C:/Users/Kevin/Documents/MySQL')
cPlayerUseArray256 <- array(c(array(data=0,dim=16),
                              array(data=1,dim=16),
                              array(data=1,dim=16),
                              array(data=2,dim=16),
                              array(data=1,dim=16),
                              array(data=2,dim=16),
                              array(data=2,dim=16),
                              array(data=3,dim=16),
                              array(data=1,dim=16),
                              array(data=2,dim=16),
                              array(data=2,dim=16),
                              array(data=3,dim=16),
                              array(data=2,dim=16),
                              array(data=3,dim=16),
                              array(data=3,dim=16),
                              array(data=4,dim=16)))
cAllPlayerUseArray1024 <- array(c(cPlayerUseArray256,
                                  cPlayerUseArray256+1,
                                  cPlayerUseArray256+2,
                                  cPlayerUseArray256+3))
cRunMatrix1024 = outer (cAllPlayerUseArray1024, cAllPlayerUseArray1024, "-")
# OO321hBBSS
# 9876543210
# add one to all the entries that has nobody at home that add a runner at home
#cRunMatrix1024[1:512*2-1,1:512*2] <- cRunMatrix1024[1:512*2-1,1:512*2]+1
for (i in 1:1024) {
    if (as.integer(i/16) %% 2 == 1) {
#       cat (i)
#       cat ("\n")
        cRunMatrix1024[i-1,i] <- cRunMatrix1024[i-1,i] + 1
    }
}