#!/usr/bin/env bash
# Unit tests for battnotify.
# Usage: ./test.sh /path/to/batt /path/to/ac

echo 14 > "$1"
echo 0  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '14%' ]]; then
	echo '1 OK'
else
	echo '1 NOT OK'
	exit 1
fi

echo 16 > "$1"
echo 0  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '2 OK'
else
	echo '2 NOT OK'
	exit 1
fi

echo 14 > "$1"
echo 1  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '3 OK'
else
	echo '3 NOT OK'
	exit 1
fi

echo 16 > "$1"
echo 1  > "$2"
_output=$(./battnotify)
if [[ "$_output" == '' ]]; then
	echo '4 OK'
else
	echo '4 NOT OK'
	exit 1
fi

rm -rf "$1" "$2"
