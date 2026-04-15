#!/usr/bin/env bash

echo 14 > "$1"
echo 0  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '14%' ]]; then
	echo '1 OK'
fi

echo 16 > "$1"
echo 0  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '2 OK'
fi

echo 14 > "$1"
echo 1  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '3 OK'
fi

echo 16 > "$1"
echo 1  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '4 OK'
fi
