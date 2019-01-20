#!/bin/bash
#---------------------------------------------------------------
# script to search a string in files contained in a directory
# version: 200119
# Edits:
#---------------------------------------------------------------

# make sure search_string supplied as command line arg else exit
[ $# -lt 2 ] && { 
echo "Usage: $0 search_string dir_path";
echo "Please pass search_string and dir_path as the argument to the program";
exit 1; }

#scan the directory and search files one by one 
scanDirectory(){
    # check if the directory is valid
    if [[ ! -d $1 ]]; then
        echo "$1 is not a valid directory"
    fi
}

# search the string in a file
searchFile(){
    for entry in "$2"/*
    do 
        filename="$entry"
        regex="$1"
        #echo $regex
        filecontent=$( cat -s -n "${filename}" )
        #echo $filecontent
        #While loop to read line by line
        lineno=0
        numOfOccurences=0
        while IFS= read -r line; do
            lineno=`expr "$lineno" + 1`
            for word in $line
            do
                if [[ " $word " =~ $regex ]]
                then
                    numOfOccurences=`expr "$numOfOccurences" + 1`
                    echo "found in line $lineno of $filename"
                fi
            done
        done < "$filename"
        echo -e "\nTotal number of occurences in $filename is $numOfOccurences\n"
    done
}

# create regex using for searching case insensitive text in files

createRegex(){
    tmpStr=$1
    rex=''

    # get both cases of each character for case insensitivity
    for (( i=0; i<${#tmpStr}; i++ )); do
        chr=${tmpStr:$i:1}

        if [[ "$chr" =~ [a-z] ]]; then
            # lowercase to uppercase
            tmpChr=$(echo $chr | tr 'a-z' 'A-Z')
            #echo $chr
            #echo $tmpChr
            rex="${rex}[$chr$tmpChr]"

        elif [[ "$chr" =~ [A-Z] ]]; then
            # uppercase to lowercase
            tmpChr=$(echo $chr | tr 'A-Z' 'a-z')
            #echo $chr
            #echo $tmpChr
            rex="${rex}[$chr$tmpChr]"

        else
            # non-alphabetic characters in the string
            #echo $chr
            rex="${rex}$chr"
        fi
    done
    #echo $rex
    searchFile $rex $2
}

scanDirectory $2
createRegex $1 $2