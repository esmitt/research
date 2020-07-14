REM FinalProtesis Matrix
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg1.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg2.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg3.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg4.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg5.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg6.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg7.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg8.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg9.out
createobbMatrix FinalProtesis.off FinalProtesisM.obb > asg10.out
copy asg1.out + asg2.out + asg3.out + asg4.out + asg5.out + asg6.out + asg7.out + asg8.out + asg9.out + asg10 FinalProtesisM.txt
del *.out