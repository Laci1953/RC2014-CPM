Copy files with querry

Use: copyq filename.ext d:
copy files to destination disk (d:), with a querry for each file
( and an extra querry to allow overwriting files )
( ambiguous file references may be used, e.g. *.c or test?.asm )
( up to 512 files can be copied )

Example:

c>dir *.1
C: Y        1   : X        1
c>dir d:*.1
D: Y        1
c>copyq *.1 d:

Copy file Y.1 ? (Y/y = yes) :y
Overwrite destination file ? (Y/y = yes) :y
Copy file X.1 ? (Y/y = yes) :y
c>dir d:*.1
D: Y        1   : X        1
c>
