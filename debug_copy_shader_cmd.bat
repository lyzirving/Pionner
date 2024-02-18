echo "remove dir"
rmdir /s/q .\out\build\x64-debug\engine\shader
pause
echo "copy shader assets"
xcopy .\engine\shader\*  .\out\build\x64-debug\engine\shader\ /s/f/k/y
pause