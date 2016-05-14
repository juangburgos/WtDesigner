
All
- Update About Dialog content.
Windows
- Compile in Windows using Configuration Properties -> Linker -> Manifest File:
	> UAC Execution Level = requireAdministrator
All
- Populate "deploy/packages/WtDesigner/data" with executable and dependencies
- Update "deploy/packages/WtDesigner/meta/package.xml" with release version and date.
Windows
- Copy "vcredist_x64.exe" to "/packages/WtDesigner/data/vcredist_x64.exe"
- Update "deploy/config/config.xml" with release version.
Linux
- Copy "src/linux_extras/*.*" to "/packages/WtDesigner/data"
- Copy rec/WtDesigner_logo.xpm to /packages/WtDesigner/data/WtDesigner_logo.xpm
- Update "WtDesigner.desktop" with release version.
- Update "deploy/config/config_nix.xml" with release version.
All
- Update Readme.md with latest changes and download links
- Update version in the binarycreator command
- Upload Binaries to Sourceforge and Github Releases

Generate installer with:

binarycreator --offline-only -c config/config.xml -p packages WtDesignerInstallerV.1.0.2.exe

PATH=$PATH:/home/juangburgos/Qt/QtIFW2.0.1/bin/
binarycreator --offline-only -c config/config_nix.xml -p packages WtDesignerInstallerV.1.0.2.run
