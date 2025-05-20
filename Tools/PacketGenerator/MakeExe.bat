pushd %~dp0
pyinstaller --onefile PacketHandlerGenerator.py
MOVE .\dist\PacketHandlerGenerator.exe .\
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\PacketHandlerGenerator.spec
PAUSE