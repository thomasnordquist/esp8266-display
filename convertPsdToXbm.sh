#!/bin/bash
PSD=$1
LABELS=`identify -verbose $PSD | grep label | cut -d":" -f2 | tr -d ' '`

rm $PSD.h 2> /dev/null
I=0;
for LABEL in $LABELS; do 
	NEWLABEL="IMG_"`awk '{ print toupper($0) }' <<< $LABEL | sed 's/-/_/g'`
	convert $PSD[$I] $NEWLABEL.xbm

	echo "// "$NEWLABEL >> $PSD.h
	cat $NEWLABEL.xbm >> $PSD.h
	rm $NEWLABEL.xbm

	I=$[ $I + 1 ];
done;
	sed -i ''  's/static/const/g' $PSD.h;
	cat $PSD.h;
