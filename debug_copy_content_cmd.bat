echo "remove dir"
rmdir /s/q .\out\build\x64-debug\engine\content
pause
echo "copy objects assets"
xcopy .\content\*  .\out\build\x64-debug\engine\content\ /s/f/k/y
pause