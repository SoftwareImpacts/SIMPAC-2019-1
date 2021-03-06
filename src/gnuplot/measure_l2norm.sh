#!/bin/bash

#m -f measure_l2norm.txt

for i in `seq 1 30`;
do
echo "i = " $i

SY=`cat param.in |grep "SY"|awk '{print $2}'`
AMP=`cat param.in|grep "Amp_x" |awk '{print $2}'`
SY_OLD="SY "$SY
AMP_OLD="Amp_x "$AMP


SY_TOGO=`echo $SY $i |awk '{print $1*1.5}'`
AMP_TOGO=`echo $AMP $i |awk '{print $1/1.5}'`
SY_NEW="SY "$SY_TOGO
AMP_NEW="Amp_x "$AMP_TOGO

echo "SY OLD "$SY_OLD
echo "SY NEW "$SY_NEW
echo "AMP OLD "$AMP_OLD
echo "AMP NEW "$AMP_NEW

cat param.in | sed s/"$SY_OLD"/"$SY_NEW"/ > aaa
cp aaa param.in
cat param.in     | sed s/"$AMP_OLD"/"$AMP_NEW"/ > bbb
cp bbb param.in

gnuplot measure_l2norm.gnu

done 
