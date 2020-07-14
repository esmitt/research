

REM jacky Matrix
createobbMatrix jacky.off jackyM.obb > ujmh1.out
createobbMatrix jacky.off jackyM.obb > ujmh2.out
createobbMatrix jacky.off jackyM.obb > ujmh3.out
createobbMatrix jacky.off jackyM.obb > ujmh4.out
createobbMatrix jacky.off jackyM.obb > ujmh5.out
createobbMatrix jacky.off jackyM.obb > ujmh6.out
createobbMatrix jacky.off jackyM.obb > ujmh7.out
createobbMatrix jacky.off jackyM.obb > ujmh8.out
createobbMatrix jacky.off jackyM.obb > ujmh9.out
createobbMatrix jacky.off jackyM.obb > ujmh10.out
copy ujmh1.out + ujmh2.out + ujmh3.out + ujmh4.out + ujmh5.out + ujmh6.out + ujmh7.out + ujmh8.out + ujmh9.out + ujmh10 jackyM.txt
del *.out

