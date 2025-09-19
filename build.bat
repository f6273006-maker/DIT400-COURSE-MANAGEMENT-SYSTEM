@echo off
REM Build script for DIT400 Course Management System (Java)

REM Compile Main.java
javac Main.java
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    exit /b %ERRORLEVEL%
)

REM Package into executable JAR
jar cfe cms_java.jar Main Main.class
IF %ERRORLEVEL% NEQ 0 (
    echo JAR creation failed!
    exit /b %ERRORLEVEL%
)

echo Build successful! Run with:
echo   java -jar cms_java.jar
