REM jacky Matrix
createobbMatrix jacky.off jackyM.obb > gemp1.out
createobbMatrix jacky.off jackyM.obb > gemp2.out
createobbMatrix jacky.off jackyM.obb > gemp3.out
createobbMatrix jacky.off jackyM.obb > gemp4.out
createobbMatrix jacky.off jackyM.obb > gemp5.out
createobbMatrix jacky.off jackyM.obb > gemp6.out
createobbMatrix jacky.off jackyM.obb > gemp7.out
createobbMatrix jacky.off jackyM.obb > gemp8.out
createobbMatrix jacky.off jackyM.obb > gemp9.out
createobbMatrix jacky.off jackyM.obb > gemp10.out
copy gemp1.out + gemp2.out + gemp3.out + gemp4.out + gemp5.out + gemp6.out + gemp7.out + gemp8.out + gemp9.out + gemp10 sphereM.txt
REM del *.out

REM jacky genetic
createobbGenetic jacky.off jackyG.obb > gemp1.out
createobbGenetic jacky.off jackyG.obb > gemp2.out
createobbGenetic jacky.off jackyG.obb > gemp3.out
createobbGenetic jacky.off jackyG.obb > gemp4.out
createobbGenetic jacky.off jackyG.obb > gemp5.out
createobbGenetic jacky.off jackyG.obb > gemp6.out
createobbGenetic jacky.off jackyG.obb > gemp7.out
createobbGenetic jacky.off jackyG.obb > gemp8.out
createobbGenetic jacky.off jackyG.obb > gemp9.out
createobbGenetic jacky.off jackyG.obb > gemp10.out
copy gemp1.out + gemp2.out + gemp3.out + gemp4.out + gemp5.out + gemp6.out + gemp7.out + gemp8.out + gemp9.out + gemp10 sphereG.txt
REM del *.out