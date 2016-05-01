TODO

- Populate /packages/WtDesigner/data with executable and dependencies
- Update "package.xml" with release version and date.
- Update "config.xml" with release version and date.
- Update About Dialog content.
- Update version in the binarycreator command

Generate installer with:

binarycreator --offline-only -c config/config.xml -p packages WtDesignerInstallerV.1.0.0.exe
binarycreator --offline-only -c config/config_nix.xml -p packages WtDesignerInstallerV.1.0.0.run
