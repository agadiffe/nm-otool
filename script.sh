#!/bin/bash

test_dir=$1
ft_nm_otool_dir=$2
file=0

if [ -n "$test_dir" ] && [ -n "$ft_nm_otool_dir" ]
then
	for entry in "$test_dir"/*
	do
		nm "$entry" &> nm_"$file"
		#otool -t "$entry" &> otool_"$file"
		"$ft_nm_otool_dir"/ft_nm "$entry" &> ftnm_"$file"
		#"$ft_nm_otool_dir"/ft_otool "$entry" &> ftotool_"$file"
		diff nm_"$file" ftnm_"$file" &> diff_nm_"$file"
		#diff otool_"$file" ftotool_"$file" &> diff_otool_"$file"
		echo "$file"
		(( file++ ))
	done
fi
