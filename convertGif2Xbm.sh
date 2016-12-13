#!/bin/bash

FILE=$1
THRESHOLD=$2
OUTPUT_FILE=$3.h
NAME=`echo $3 | awk '{ print toupper($1) }'`

convert "$FILE" -coalesce -resize 128x64 -threshold $THRESHOLD"%" -background white -alpha remove $NAME.png
#convert "$FILE" -coalesce -dither Riemersma -colors 1 -background white -alpha remove -resize 128x64 $NAME.png

COUNT=`ls -1 $NAME-*.xbm | wc -l`

#asseble file
cat $NAME-*.xbm > $OUTPUT_FILE
rm $NAME-*.xbm

# Fix names
sed -i '' "s/-/_/g" $OUTPUT_FILE

# Save amount of frames
echo "#define "$NAME"_FRAMES" $COUNT >> $OUTPUT_FILE

# Create an array with all char arrays
IFS=$'\n'
ARRAY="const char* $NAME"'[] = {'
for LINE in `cat $OUTPUT_FILE | grep "static" | awk '{print $3}' | sort -n -t_ -k 2`; do
	ARRAY="$ARRAY `echo -n $LINE | cut -d[ -f1`,"
done;

ARRAY=$ARRAY" };";

echo $ARRAY >> $OUTPUT_FILE

WIDTH=`cat $OUTPUT_FILE | grep 0_width |tail -n1 | awk '{print $3}'`
HEIGHT=`cat $OUTPUT_FILE | grep 0_height | tail -n1 | awk '{print $3}'`

echo "#define $NAME""_WIDTH $WIDTH" >> $OUTPUT_FILE
echo "#define $NAME""_HEIGHT $HEIGHT" >> $OUTPUT_FILE
