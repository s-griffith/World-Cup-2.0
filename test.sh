#!/bin/bash

TESTS_TO_RUN=10001
EXECUTABLE=./FileTester

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

shopt -s nullglob
declare -i FAILED_TESTS=0
declare -i VALGRIND_FAILED=0
declare -i DIFF_FAILED=0
VALGRINED_FAILED_NUMS=""

for i in inFiles/test*.in
do
	if [ ${i//[^0-9]/} -gt $TESTS_TO_RUN ]; then
		continue
	fi
	
	printf "test $i >>>  "
	$EXECUTABLE < $i > outFiles/test${i//[^0-9]/}.result
	diff outFilesDesired/output${i//[^0-9]/}.out outFiles/test${i//[^0-9]/}.result

	if [ $? -eq 0 ]
	then
		printf "Test Run: ${GREEN}pass${NC},   "
	else
		printf "Test Run: ${RED}fail${NC},   "
		FAILED_TESTS+=1
        DIFF_FAILED+=1
	fi
	valgrind --log-file=$i.valgrind_log --leak-check=full $EXECUTABLE < $i 1>/dev/null 2>/dev/null
	if [ -f $i.valgrind_log ]
	then
		cat $i.valgrind_log | grep "ERROR SUMMARY: 0" > /dev/null
		if [ $? -eq 0 ]
		then
			printf "Leak: ${GREEN}pass${NC}\n"
		else
			printf "Leak: ${RED}fail${NC}\n"
			cat $i.valgrind_log
			FAILED_TESTS+=1
            VALGRIND_FAILED+=1
			VALGRINED_FAILED_NUMS+="/"
			VALGRINED_FAILED_NUMS+=$i
		fi
	else
		printf "Leak: ${RED}couldnt get valgrind file${NC}\n"
		FAILED_TESTS+=1
	fi
	rm $i.valgrind_log
done

if [ ${FAILED_TESTS} -eq 0 ]; then
	printf "\n${GREEN} All tests passed :)${NC}\n\n"
else
	printf "\n${RED} Failed ${FAILED_TESTS}${NC} tests.\n\n"
	printf "\n${RED} Valgrind Failed ${VALGRIND_FAILED}${NC} tests.\n\n"
	printf "\n${RED} Valgrind Failed on: ${VALGRIND_FAILED_NUMS}${NC} tests.\n\n"
	printf "\n${RED} Diff Failed ${DIFF_FAILED}${NC} tests.\n\n"
fi
