
function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addElevatedOperation("Execute", "{0,3010}", "@TargetDir@/vcredist_x64.exe","/norestart", "/q");
        component.addOperation("CreateShortcut", "@TargetDir@/WtDesigner.exe", "@StartMenuDir@/WtDesigner.lnk",
            "workingDirectory=@TargetDir@",
            "iconId=2");
		component.addOperation("CreateShortcut", "@TargetDir@/WtDesigner.exe", "@DesktopDir@/WtDesigner.lnk",
            "workingDirectory=@TargetDir@",
            "iconId=2");
		component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/Uninstall.lnk",
            "workingDirectory=@TargetDir@",
            "iconId=2");
    }
    else
    {
        // fix path for run script and icon
        component.addOperation("Replace", "@TargetDir@/WtDesigner.desktop", "{UNKNOWN_PATH}", "@TargetDir@");
        component.addOperation("Copy","@TargetDir@/WtDesigner.desktop","/usr/share/applications/WtDesigner.desktop")
    }
}