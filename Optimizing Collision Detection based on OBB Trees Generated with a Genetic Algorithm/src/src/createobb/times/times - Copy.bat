REM tibia genetic
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp1.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp2.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp3.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp4.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp5.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp6.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp7.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp8.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp9.out
createobbGenetic FinalTibiaSinTapa.off FinalTibiaSinTapaG.obb > temp10.out
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 FinalTibiaSinTapaG.txt
del *.out

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
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 sphereM.txt
del *.out

REM jacky genetic
createobbGenetic jacky.off jackyG.obb > temp1.out
createobbGenetic jacky.off jackyG.obb > temp2.out
createobbGenetic jacky.off jackyG.obb > temp3.out
createobbGenetic jacky.off jackyG.obb > temp4.out
createobbGenetic jacky.off jackyG.obb > temp5.out
createobbGenetic jacky.off jackyG.obb > temp6.out
createobbGenetic jacky.off jackyG.obb > temp7.out
createobbGenetic jacky.off jackyG.obb > temp8.out
createobbGenetic jacky.off jackyG.obb > temp9.out
createobbGenetic jacky.off jackyG.obb > temp10.out
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 sphereG.txt
del *.out

REM FinalProtesis Matrix
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp1.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp2.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp3.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp4.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp5.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp6.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp7.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp8.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp9.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > temp10.out
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 FinalProtesisM.txt
del *.out

REM FinalProtesis genetic
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp1.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp2.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp3.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp4.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp5.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp6.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp7.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp8.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp9.out
createobbGenetic FinalProtesis.off FinalProtesisG.obb > temp10.out
copy temp1.out + temp2.out + temp3.out + temp4.out + temp5.out + temp6.out + temp7.out + temp8.out + temp9.out + temp10 FinalProtesisG.txt
del *.out

REM borrar los .obb
del *.obb