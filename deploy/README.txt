TODO

- Compile in Windows using Configuration Properties -> Linker -> Manifest File:
	> UAC Execution Level = requireAdministrator
- Populate "deploy/packages/WtDesigner/data" with executable and dependencies
- Copy "vcredist_x64.exe" to "/packages/WtDesigner/data/vcredist_x64.exe"
- Copy "src/linux_extras/*.*" to "/packages/WtDesigner/data"
- Copy rec/WtDesigner_logo.xpm to /packages/WtDesigner/data/WtDesigner_logo.xpm
- Update "deploy/packages/WtDesigner/metapackage.xml" with release version and date.
- Update "deploy/config/config.xml" and "deploy/config/config_nix.xml" with release version and date.
- Update "WtDesigner.desktop" with release version.
- Update About Dialog content.
- Update version in the binarycreator command

- Upload Binaries to Sourceforge and Github Releases

- Update Readme.md with latest changes and download links


Generate installer with:

binarycreator --offline-only -c config/config.xml -p packages WtDesignerInstallerV.1.0.1.exe

PATH=$PATH:/home/juangburgos/Qt/QtIFW2.0.1/bin/
binarycreator --offline-only -c config/config_nix.xml -p packages WtDesignerInstallerV.1.0.1.run
