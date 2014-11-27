echo off

for i in `find . -type f -name "*HA*"`  
do
echo "$i" >> get_files.txt 
done