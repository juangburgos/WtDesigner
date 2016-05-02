TODO

- Populate /packages/WtDesigner/data with executable and dependencies
- Copy src/linux_extras to /packages/WtDesigner/data
- Copy rec/WtDesigner_logo.png to /packages/WtDesigner/data/WtDesigner_logo.png
- Update "package.xml" with release version and date.
- Update "config.xml" with release version and date.
- Update "WtDesigner.desktop" with release version.
- Update About Dialog content.
- Update version in the binarycreator command

Generate installer with:

binarycreator --offline-only -c config/config.xml -p packages WtDesignerInstallerV.1.0.1.exe
binarycreator --offline-only -c config/config_nix.xml -p packages WtDesignerInstallerV.1.0.1.run
