if %PLATFORM% equ "x86" (
    set RIDE_PYTHON=C:\Python39
) else (
    set RIDE_PYTHON=C:\Python39-x64
)

echo %PLATFORM%
echo %RIDE_PYTHON%
