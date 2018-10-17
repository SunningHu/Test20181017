echo 清理编译过程中产生的过程文件
echo 请等待。。。
cd Debug
del /f /q /s *.*
cd ..
cd Release
del /f /q /s *.*
pause