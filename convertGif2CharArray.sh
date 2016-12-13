#!/bin/bash

FILE=$1
THRESHOLD=$2
OUTPUT_FILE=$3.h
NAME=`echo $3 | awk '{ print toupper($1) }'`

convert "$FILE" -coalesce -threshold $THRESHOLD"%" -background white -alpha remove -resize 128x64 -negate $NAME.xbm
#convert "$FILE" -coalesce -dither Riemersma -colors 2 -background white -alpha remove -resize 128x64 $NAME.xbm

COUNT=`ls -1 $NAME-*.xbm | wc -l`

echo "" > $OUTPUT_FILE
for FILE in `ls -1 $NAME-*.xbm | sort -n -t- -k2`; do
	cat "$FILE" >> $OUTPUT_FILE
	rm $FILE
done;

WIDTH=`cat $OUTPUT_FILE | grep 0_width | tail -n1 | awk '{print $3}'`
HEIGHT=`cat $OUTPUT_FILE | grep 0_height | tail -n1 | awk '{print $3}'`

# Move closing paranthesis into it's own line so the line containing it can be removed later
sed -i '' $'s/\};/\\\n};/g' $OUTPUT_FILE
cat $OUTPUT_FILE | grep -v define | grep -v static | grep -v '\};' > tmp.h

echo "const char $NAME""[] PROGMEM = {" > $OUTPUT_FILE
cat tmp.h >> $OUTPUT_FILE
rm tmp.h
echo "};" >> $OUTPUT_FILE

echo "#define $NAME""_FRAMES" $COUNT >> $OUTPUT_FILE
echo "#define $NAME""_WIDTH $WIDTH" >> $OUTPUT_FILE
echo "#define $NAME""_HEIGHT $HEIGHT" >> $OUTPUT_FILE
