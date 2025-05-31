echo "remove dir"
rmdir /s/q .\content
echo "copy objects assets"
xcopy ..\..\..\..\content\*  .\content\ /s/f/k/y