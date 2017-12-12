#!/bin/bash

tmp=tmp.txt

file_names='j jal srav sra srlv srl sllv sll add addu and div nor divu mult multu or slt sltu addi addi_max addiu andi lw ori xori corner invalid_instruction jr beq bgez bgezal bgtz blez bltz bltzal' #Names of all non arithmetic filenames

passed=pass #Pass variable

failed=fail #Fail variable
test_directory=test_binaries/ #Path to test binaries
bin=.bin
sol=sol_
txt=.txt

read -p 'Author name: ' username
echo $result
counter=1
type=-1

echo TestId , Instruction , Status , Author, Message > results.csv

for name in $file_names
do
	export output_filename passed name username counter type command
	file_loc="$test_directory$name$bin"
	command="$1 $file_loc"
	$command temp.txt #Run test binary in simulator
	result=$? #Retrieve return value of simulator
	
	case $result in
	10) #Passed
		type=-1
		arguments="$passed $name $username $counter $type"
		;;
	0) #Passed
		type=-1
		arguments="$passed $name $username $counter $type"
		;;
	1) #Arithmetic overflow
		type=-10
		arguments="$failed $name $username $counter $type"
		;;
	2) #Memory exception
		type=-11
		arguments="$failed $name $username $counter $type"
		;;
	3) #Invalid instruction
		type=-12
		arguments="$failed $name $username $counter $type"
		;;
	4) #Internal error
		type=-20
		arguments="$failed $name $username $counter $type"
		;;
	*) #Check result of arithmetic instructions
		val=$(cat test_binaries_sol/$sol$name$txt) #Load expected solution and compare
		if [ $val -eq $result ]
		then 
			type=-1
			arguments="$passed $name $username $counter $type"
		else
			type=0
			arguments="$failed $name $username $counter $type"
		fi
		;;
	esac

	./bin/write_csv $arguments >> results.csv #Print result to csv file

	((counter++)) #Counter increments, acts as test reference
done