#!/usr/bin/env bash

if [ ! $PREFIX ]
then PREFIX=/
fi

tmpfile=$PREFIX/tmp/sillyvm_codefile-$$.h.tmp
touch $tmpfile

sed -n '1,/^\/\* INSTRUCTIONS START/p' < include/code.h >> $tmpfile
awk '
BEGIN {
  opcode = 0
  FS = "."
}

/^\w+(\.\w+)?$/ {
  len = 13 - length($1) - length($2)
  if ($2) kind = "_" toupper($2)
  else    ++len
  printf "#define SILLY_INSTR_%s%s", toupper($1), kind
  for (i = 0; i < len; ++i) printf " "
  printf "0x%02x\n", opcode++
}

/# / {
  print "//", substr($0, 3)
}

END {
  printf "#define COUNT_SILLY_INSTR         0x%03x\n", opcode
}
' < opc_list.txt >> $tmpfile
sed -n '/^\/\* INSTRUCTIONS END/,$p' < include/code.h >> $tmpfile

mv $tmpfile include/code.h
