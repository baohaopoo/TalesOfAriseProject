

xcopy	/y/s	.\Engine\Public\*.*				.\Reference\Headers\
xcopy	/y/s	.\Engine\Bin\*.lib				.\Reference\Librarys\
xcopy	/y/s	.\Engine\ThirdPartyLib\*.lib	.\Reference\Librarys\

xcopy	/y/s	.\Engine\Bin\*.dll				.\Client\Bin\
xcopy	/y/s	.\Engine\Bin\ShaderFiles\*.hlsl	.\Client\Bin\ShaderFiles\

xcopy	/y/s	.\Engine\Bin\*.dll				.\Converter\Bin\