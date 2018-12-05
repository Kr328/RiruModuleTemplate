package com.github.kr328.plugin.riru.conf;

public class GlobalConfig {
    public String buildPath;
    private static GlobalConfig instance;

    public static GlobalConfig get() {
        if ( instance == null ) {
            instance = new GlobalConfig();
            instance.buildPath = LocalProperties.get().getProperty("build.path" ,"build");
        }

        return instance;
    }
}
