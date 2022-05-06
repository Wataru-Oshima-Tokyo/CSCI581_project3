./run $(cat tests/t01.cmd) > t01.my
./run $(cat tests/t02.cmd) > t02.my
./run $(cat tests/t03.cmd) > t03.my
./run $(cat tests/t04.cmd) > t04.my
./run $(cat tests/t05.cmd) > t05.my
./run $(cat tests/t06.cmd) > t06.my
./run $(cat tests/t07.cmd) > t07.my
./run $(cat tests/t08.cmd) > t08.my
./run $(cat tests/t09.cmd) > t09.my
./run $(cat tests/t10.cmd) > t10.my
./run $(cat tests/t11.cmd) > t11.my
echo "test 1"
diff t01.my tests/t01.out 
echo "test 2"
diff t02.my tests/t02.out 
echo "test 3"
diff t03.my tests/t03.out 
echo "test 4"
diff t04.my tests/t04.out 
echo "test 5"
diff t05.my tests/t05.out 
echo "test 6"
diff t06.my tests/t06.out 
echo "test 7"
diff t07.my tests/t07.out 
echo "test 8"
diff t08.my tests/t08.out 
echo "test 9"
diff t09.my tests/t09.out 
echo "test 10"
diff t10.my tests/t10.out 
echo "test 11"
diff t11.my tests/t11.out 