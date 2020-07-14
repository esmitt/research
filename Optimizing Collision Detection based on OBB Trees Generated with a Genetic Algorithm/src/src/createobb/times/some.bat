REM jacky Matrix
createobbMatrix jacky.off jackyM.obb > temp1.out
createobbMatrix jacky.off jackyM.obb > temp2.out
createobbMatrix jacky.off jackyM.obb > temp3.out
createobbMatrix jacky.off jackyM.obb > temp4.out
createobbMatrix jacky.off jackyM.obb > temp5.out
createobbMatrix jacky.off jackyM.obb > temp6.out
createobbMatrix jacky.off jackyM.obb > temp7.out
createobbMatrix jacky.off jackyM.obb > temp8.out
createobbMatrix jacky.off jackyM.obb > temp9.out
createobbMatrix jacky.off jackyM.obb > temp10.out
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 jackyM.txt
del *.out

