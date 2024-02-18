echo "remove dir"
rmdir /s/q .\out\build\x64-debug\engine\assets
pause
echo "copy objects assets"
xcopy .\engine\assets\*  .\out\build\x64-debug\engine\assets\ /s/f/k/y
pause