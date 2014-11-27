echo off
for i in `find . -type f -name "*AN*"`  
do
echo "$i" >> angry_face.txt
done